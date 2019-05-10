/** @file
*  This file contains routines used to provision a device during manufacturing.
*
*  Copyright (c) 2019 Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#include <Uefi.h>

#include <IndustryStandard/Tpm20.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimeBaseLib.h>
#include <Library/Tpm2CommandLib.h>
#include <Library/Tpm2DeviceLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Protocol/TcgService.h>
#include <Protocol/Tcg2Protocol.h>

#include "Provisioning.h"

STATIC CONST CHAR16 mDeviceProvisioned[] = L"DeviceProvisioned";
STATIC CONST CHAR16 mDeviceCertVariableName[] = L"ManufacturerDeviceCert";
STATIC CONST CHAR16 mSmbiosSystemSerialNumberName[] = L"SmbiosSystemSerialNumber";

/**
  Send a request string to the provisioning host and receive a response buffer.

  Send an Ascii string to a remote host, then await 4 bytes for a buffer length,
  length bytes of a buffer, then 4 bytes of a buffer checksum.

  @param[in] RequestString    Pointer to the Ascii string to send to the host.

  @param[out] Buffer          Double pointer to return the allocated buffer of
                              data received from the host.
  @param[out] Length          Pointer to the length of the received buffer.

  @retval  EFI_SUCCESS            The host sent a buffer successfully.
  @retval  EFI_NO_RESPONSE        Host did not respond within 5 seconds.
  @retval  EFI_OUT_OF_RESOURCES   Unable to allocate space for the buffer.
  @retval  EFI_CRC_ERROR          Checksum mismatch between device and host.
  @retval  EFI_STATUS             Return value from gRT->GetTime()

**/
EFI_STATUS
EFIAPI
ReceiveBuffer (
  CHAR8   *RequestString,
  UINT8   **Buffer,
  UINT32  *Length
  )
{
  UINTN       BytesRead;
  UINT32      Checksum;
  UINTN       CurrentEpoch;
  UINTN       i;
  UINT32      ReceivedChecksum;
  UINTN       StartEpoch;
  EFI_STATUS  Status;
  UINT8       *TempPtr;
  EFI_TIME    Time;

  *Buffer = NULL;
  *Length = 0;

  SEND_REQUEST_TO_HOST (RequestString);

  Status = gRT->GetTime (&Time, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  StartEpoch = EfiTimeToEpoch (&Time);

  // Timeout the poll in 5 seconds
  while (SerialPortPoll () == FALSE) {
    Status = gRT->GetTime (&Time, NULL);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    CurrentEpoch = EfiTimeToEpoch (&Time);
    if (CurrentEpoch - StartEpoch > 5) {
      return EFI_NO_RESPONSE;
    }
  }

  // Receive the 4-byte length of the buffer
  TempPtr = (UINT8*) Length;
  for (i = 4, BytesRead = 0; i > 0;) {
    BytesRead = SerialPortRead (TempPtr, i);
    i -= BytesRead;
    TempPtr += BytesRead;
  }

  // Allocate space for the buffer.
  *Buffer = AllocateZeroPool (*Length);
  if (*Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Receive the buffer from the host.
  for (i = *Length, TempPtr = *Buffer; i > 0;) {
    BytesRead = SerialPortRead (TempPtr, i);
    i -= BytesRead;
    TempPtr += BytesRead;
  }

  // Receive the 4-byte checksum from the host.
  TempPtr = (UINT8*) &ReceivedChecksum;
  for (i = 4, BytesRead = 0; i > 0;) {
    BytesRead = SerialPortRead (TempPtr, i);
    i -= BytesRead;
    TempPtr += BytesRead;
  }

  // Compute the checksum for the information we received.
  for (i = 0, Checksum = 0; i < *Length; i++) {
    Checksum += (*Buffer)[i];
  }

  if (ReceivedChecksum != Checksum) {
    DEBUG ((DEBUG_ERROR, "Checksum mismatch!\n Expected 0x%x\n Computed 0x%x\n",
           ReceivedChecksum, Checksum));
    return EFI_CRC_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Check UEFI variables for the DeviceProvisioned variable

  @retval  EFI_SUCCESS           DeviceProvisioned variable is present.
  @retval  EFI_NOT_READY         DeviceProvisioned variable is not present.

**/
EFI_STATUS
EFIAPI
ProvisionedCheck ()
{
  UINTN       DataSize;
  EFI_STATUS  Status;

  DataSize = 0;
  Status = gRT->GetVariable (
                  (CHAR16*) mDeviceProvisioned,
                  &giMXPlatformProvisioningGuid,
                  NULL,
                  &DataSize,
                  NULL
                  );

  if (Status == EFI_BUFFER_TOO_SMALL) {
      return EFI_SUCCESS;
  }

  return EFI_NOT_READY;
}

/**
  Set the DeviceProvisioned variable in UEFI variables

  @retval  EFI_SUCCESS           DeviceProvisioned variable was set.
  @retval  EFI_STATUS            Return the status of gRT->SetVariable ().

**/
EFI_STATUS
EFIAPI
ProvisionedSet ()
{

  UINT8       Data;
  EFI_STATUS  Status;

  Data = 1;
  Status = gRT->SetVariable (
                  (CHAR16 *) mDeviceProvisioned,
                  &giMXPlatformProvisioningGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS |
                  EFI_VARIABLE_RUNTIME_ACCESS,
                  1,
                  (VOID *) &Data
                  );

  return Status;
}

/**
  Check over serial that a provisioning host is present.

  Challenge the remote host with MFG:hostcheck and await a response of "MFGH".
  Fail if the host does not respond within 5 secs or the response is incorrect.

  @retval  EFI_SUCCESS           The remote host is present and responsive.
  @retval  EFI_NO_RESPONSE       No response from the remote host.
  @retval  EFI_UNSUPPORTED       Incorrect response from the remote host.

**/
EFI_STATUS
EFIAPI
RemoteHostExists ()
{
  UINTN       BytesRead;
  UINTN       CurrentEpoch;
  UINT32      HostResponse;
  UINTN       i;
  UINTN       StartEpoch;
  EFI_STATUS  Status;
  UINT8       *TempPtr;
  EFI_TIME    Time;


  SEND_REQUEST_TO_HOST ("MFG:hostcheck\r\n");

  Status = gRT->GetTime (&Time, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  StartEpoch = EfiTimeToEpoch (&Time);

  // Timeout the poll in 5 seconds to allow a device in the field to continue.
  while (SerialPortPoll () == FALSE) {
    Status = gRT->GetTime (&Time, NULL);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    CurrentEpoch = EfiTimeToEpoch (&Time);
    if (CurrentEpoch - StartEpoch > 5) {
      return EFI_NO_RESPONSE;
    }
  }

  // Receive the 4-byte flag from the host.
  TempPtr = (UINT8*) &HostResponse;
  for (i = 4, BytesRead = 0; i > 0;) {
    BytesRead = SerialPortRead (TempPtr, i);
    i -= BytesRead;
    TempPtr += BytesRead;
  }

  if (HostResponse != 0x4D464748) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  Retrieve the Endorsement Key Certificate from the TPM and send it to the
  provisioning host.

  Use the Tcg2Protocol to submit the EK Cert read command to the TPM.
  If the EK Cert was not persisted to handle 0x81010001, run CreatePrimary
  and EvictControl to generate an Endorsement Key based on EPS and save it into
  non-volatile storage. Run another ReadPublic command to pull it back out.

  Notify the provisioning host to receive the ekcert by sending MFG:ekcert.

  @retval  EFI_SUCCESS           Successfully sent the EK Certificate.
  @retval  EFI_NOT_FOUND         Unable to retrieve the EK Certificate from TPM
  @retval  EFI_STATUS            Return the status of gBS->LocateProtocol ().

**/
EFI_STATUS
EFIAPI
TransmitEKCertificate ()
{
  UINT32                     i;
  UINT32                     Length;
  TPM2_READ_PUBLIC_RESPONSE  RecvBuffer;
  UINT32                     RecvBufferSize;
  TPM2_READ_PUBLIC_COMMAND   SendBuffer;
  UINT32                     SendBufferSize;
  UINT32                     SendUint32;
  EFI_STATUS                 Status;
  EFI_TCG2_PROTOCOL          *Tcg2Protocol;
  UINT8  TpmCreatePrimary[TPMCREATEPRIMARY_SZ] = { 0x80,0x02, // TPM_ST_SESSIONS
           0x00,0x00,0x01,0x77,  // Command size is 0x177
           0x00,0x00,0x01,0x31,  // TPM_CC_CreatePrimary
           0x40,0x00,0x00,0x0b,  // TPM_RH_ENDORSEMENT
           0x00,0x00,
           0x00,0x1d,0x40,0x00,0x00,0x09,0x00,0x00,
           0x00,0x00,0x14,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x04,0x00,0x00,0x00,0x00,0x01,0x3a,0x00,
           0x01,0x00,0x0b,0x00,0x03,0x00,0xb2,0x00,
           0x20,0x83,0x71,0x97,0x67,0x44,0x84,0xb3,
           0xf8,0x1a,0x90,0xcc,0x8d,0x46,0xa5,0xd7,
           0x24,0xfd,0x52,0xd7,0x6e,0x06,0x52,0x0b,
           0x64,0xf2,0xa1,0xda,0x1b,0x33,0x14,0x69,
           0xaa,0x00,0x06,0x00,0x80,0x00,0x43,0x00,
           0x10,0x08,0x00,0x00,0x00,0x00,0x00,0x01,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  UINT8  TpmEvictEK[TPMEVICTEK_SZ] = { 0x80,0x02,        // TPM_ST_SESSIONS
           0x00,0x00,0x00,0x37,  // Length 55 bytes
           0x00,0x00,0x01,0x20,  // TPM_CC_EvictControl
           0x40,0x00,0x00,0x01,  // TPM_RH_OWNER
           0x80,0xFF,0xFF,0xFF,  // Handle (Fill later!)
           0x00,0x00,0x00,0x1d,
           0x40,0x00,0x00,0x09,
           0x00,0x00,0x00,0x00,
           0x14,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,
           0x00,0x00,0x00,0x00,
           0x00,
           0x81,0x01,0x00,0x01}; // Persistent Handle
  UINT8  TpmOut[TPMOUT_SZ];

  // Grab a handle to the TCG2 TPM driver.
  Status = gBS->LocateProtocol (&gEfiTcg2ProtocolGuid, NULL,
                                (VOID **) &Tcg2Protocol);
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR, "%a: failed to locate protocol. Status: 0x%x\n",
           __FUNCTION__, Status));
    return Status;
  }

  //
  // Send a TPM_CC_ReadPublic directly to the TPM instead of through the TPM
  //  library because we need the unmarshalled buffer back for EK Public.
  //
  SendBuffer.Header.tag = SwapBytes16 (TPM_ST_NO_SESSIONS);
  SendBuffer.Header.commandCode = SwapBytes32 (TPM_CC_ReadPublic);
  SendBuffer.ObjectHandle = SwapBytes32 (0x81010001);
  SendBufferSize = (UINT32) sizeof (SendBuffer);
  SendBuffer.Header.paramSize = SwapBytes32 (SendBufferSize);
  RecvBufferSize = sizeof (RecvBuffer);

  Status = Tcg2Protocol->SubmitCommand (
                          Tcg2Protocol,
                          SendBufferSize,
                          (UINT8 *)&SendBuffer,
                          RecvBufferSize,
                          (UINT8 *)&RecvBuffer
                          );
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR, "%a: failed to submit command to TPM. Status: 0x%x\n",
           __FUNCTION__, Status));
    return Status;
  }
  if (SwapBytes32 (RecvBuffer.Header.responseCode) == TPM_RC_SUCCESS) {
    goto SendEKCert;
  }

  DEBUG ((DEBUG_ERROR, "%a: TPM Generating EK Certificate!\n", __FUNCTION__));
  Status = Tcg2Protocol->SubmitCommand (
                          Tcg2Protocol,
                          TPMCREATEPRIMARY_SZ,
                          TpmCreatePrimary,
                          TPMOUT_SZ,
                          TpmOut
                          );
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR, "%a: failed to submit command to TPM. Status: 0x%x\n",
           __FUNCTION__, Status));
    return Status;
  }

  if (SwapBytes32 (((TPM2_RESPONSE_HEADER*)TpmOut)->responseCode) != TPM_RC_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Failed to createprimary in TPM!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  // Copy the handle for the EK generated by CreatePrimary into the evict command.
  CopyMem ((void*)&TpmEvictEK[14], (void*)(TpmOut+sizeof (TPM2_RESPONSE_HEADER)), 4);

  Status = Tcg2Protocol->SubmitCommand (
                          Tcg2Protocol,
                          TPMEVICTEK_SZ,
                          TpmEvictEK,
                          TPMOUT_SZ,
                          TpmOut
                          );
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR, "%a: failed to submit command to TPM. Status: 0x%x\n",
           __FUNCTION__, Status));
    return Status;
  }

  if (SwapBytes32 (((TPM2_RESPONSE_HEADER*)TpmOut)->responseCode) != TPM_RC_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Failed to Evict EK into TPM NV Storage!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  //
  // Send a TPM_CC_ReadPublic directly to the TPM instead of through the TPM
  //  library because we need the unmarshalled buffer back for EK Public.
  //
  SendBuffer.Header.tag = SwapBytes16 (TPM_ST_NO_SESSIONS);
  SendBuffer.Header.commandCode = SwapBytes32 (TPM_CC_ReadPublic);
  SendBuffer.ObjectHandle = SwapBytes32 (0x81010001);
  SendBufferSize = (UINT32) sizeof (SendBuffer);
  SendBuffer.Header.paramSize = SwapBytes32 (SendBufferSize);
  RecvBufferSize = sizeof (RecvBuffer);

  Status = Tcg2Protocol->SubmitCommand (
                          Tcg2Protocol,
                          SendBufferSize,
                          (UINT8 *)&SendBuffer,
                          RecvBufferSize,
                          (UINT8 *)&RecvBuffer
                          );
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR, "%a: failed to submit command to TPM. Status: 0x%x\n",
           __FUNCTION__, Status));
    return Status;
  }

  if (SwapBytes32 (RecvBuffer.Header.responseCode) != TPM_RC_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to retrieve EK Cert from TPM!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

SendEKCert:
  Length = SwapBytes16 (RecvBuffer.OutPublic.size);
  Length += 2; // Add a Uint16 to send the OutPublic size too

  SEND_REQUEST_TO_HOST ("MFG:ekcert\r\n");
  SerialPortWrite ((UINT8*)&Length, 4);
  SerialPortWrite ((UINT8*)&RecvBuffer.OutPublic, Length);
  for (i = 0, SendUint32 = 0; i < Length; i++) {
    SendUint32 += ((UINT8*)&RecvBuffer.OutPublic)[i];
  }
  SerialPortWrite ((UINT8*)&SendUint32, 4);

  return EFI_SUCCESS;
}

/**
  Receive a cross-signed device certificate from the provisioning host.

  Store the cross-signed device certificate in UEFI variables.

  @retval  EFI_SUCCESS           Successfully stored the device certificate.
  @retval  EFI_STATUS            Return the status of ReceiveBuffer ().
  @retval  EFI_STATUS            Return the status of gRT->SetVariable ().

**/
EFI_STATUS
EFIAPI
ReceiveCrossSignedCert ()
{
  UINT32      CertLen;
  UINT8*      CertPtr;
  EFI_STATUS  Status;

  Status = ReceiveBuffer ("MFG:devicecert\r\n", &CertPtr, &CertLen);
  if (EFI_ERROR (Status)) {
    goto cleanup;
  }

  Status = gRT->SetVariable (
                  (CHAR16 *) mDeviceCertVariableName,
                  &giMXPlatformProvisioningGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS |
                  EFI_VARIABLE_RUNTIME_ACCESS,
                  CertLen,
                  (VOID *) CertPtr
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "Provisioning: Failed to save %s variable, Status = %r\n",
      mDeviceCertVariableName,
      Status
      ));
  }

cleanup:
  if (CertPtr != NULL) {
    FreePool (CertPtr);
  }

  return Status;
}

/**
  Receive a SMBIOS values from the provisioning host.

  Store the device-specific SMBIOS values in UEFI variables.

  @retval  EFI_SUCCESS           Successfully stored the smbios values.
  @retval  EFI_STATUS            Return the status of ReceiveBuffer ().
  @retval  EFI_STATUS            Return the status of gRT->SetVariable ().

**/
EFI_STATUS
EFIAPI
ReceiveSmbiosValues ()
{
  UINT32      SmbiosLen;
  UINT8*      SmbiosPtr;
  EFI_STATUS  Status;

  Status = ReceiveBuffer ("MFG:smbiossystemserial\r\n", &SmbiosPtr, &SmbiosLen);
  if (EFI_ERROR (Status)) {
    goto cleanup;
  }

  Status = gRT->SetVariable (
                  (CHAR16 *) mSmbiosSystemSerialNumberName,
                  &giMXPlatformProvisioningGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS |
                  EFI_VARIABLE_RUNTIME_ACCESS,
                  SmbiosLen,
                  (VOID *) SmbiosPtr
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "Provisioning: Failed to save %s variable, Status = %r\n",
      mSmbiosSystemSerialNumberName,
      Status
      ));
  }

cleanup:
  if (SmbiosPtr != NULL) {
    FreePool (SmbiosPtr);
  }

  return Status;
}

/**
  Run through the device provisioning flow if necessary.

  1) Exit immediately if UEFI variables says the device is already provisioned.
  2) Stall for 5 seconds then exit if there's no provisioning host.
  3) Send the EK Certificate to a provisioning host.
  4) Receive a device certificate from the provisioning host.
  5) Receive a SMBIOS values from the provisioning host.
  6) Store a UEFI variable indicating the device is provisioned.

  @retval  EFI_SUCCESS           Successfully provisioned the device.
  @retval  EFI_STATUS            Return the status of the failing sub-function.

**/
EFI_STATUS
EFIAPI
ProvisioningInitialize (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = ProvisionedCheck ();
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Device already provisioned!\n"));
    return Status;
  }

  DEBUG ((DEBUG_ERROR, "Device unprovisioned, checking for host!\n"));
  Status = RemoteHostExists ();
  if (EFI_ERROR (Status)) {
    SEND_REQUEST_TO_HOST ("MFGF:remotehost\r\n");
    DEBUG ((DEBUG_ERROR, "RemoteHostExists failed. 0x%x\n", Status));
    return Status;
  }

  Status = TransmitEKCertificate ();
  if (EFI_ERROR (Status)) {
    SEND_REQUEST_TO_HOST ("MFGF:ekcert\r\n");
    DEBUG ((DEBUG_ERROR, "TransmitEKCertificate failed. 0x%x\n", Status));
    return Status;
  }

  Status = ReceiveCrossSignedCert ();
  if (EFI_ERROR (Status)) {
    SEND_REQUEST_TO_HOST ("MFGF:devicecert\r\n");
    DEBUG ((DEBUG_ERROR, "ReceiveCrossSignedCert failed. 0x%x\n", Status));
    return Status;
  }

  Status = ReceiveSmbiosValues ();
  if (EFI_ERROR (Status)) {
    SEND_REQUEST_TO_HOST ("MFGF:smbios\r\n");
    DEBUG ((DEBUG_ERROR, "ReceiveSmbiosValues failed. 0x%x\n", Status));
    return Status;
  }

  Status = ProvisionedSet ();
  if (EFI_ERROR (Status)) {
    SEND_REQUEST_TO_HOST ("MFGF:provisioned\r\n");
    DEBUG ((DEBUG_ERROR, "ProvisionedSet failed. 0x%x\n", Status));
    return Status;
  }

  SEND_REQUEST_TO_HOST ("MFG:success\r\n");

  return Status;
}
