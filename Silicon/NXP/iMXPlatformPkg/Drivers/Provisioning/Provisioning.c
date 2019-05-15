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
  UINT32                     AuthorizationSize;
  EFI_STATUS                 Status;
  EFI_TCG2_PROTOCOL          *Tcg2Protocol;

  UINT8  TpmOut[TPMOUT_SZ];
  TPM2_CREATE_PRIMARY_COMMAND CreatePrimary = {0};
  UINT8 DefaultPolicySecret[POLICY_SECRET_SZ] = {0x83,0x71,0x97,0x67,0x44,0x84,
                                                 0xb3,0xf8,0x1a,0x90,0xcc,0x8d,
                                                 0x46,0xa5,0xd7,0x24,0xfd,0x52,
                                                 0xd7,0x6e,0x06,0x52,0x0b,0x64,
                                                 0xf2,0xa1,0xda,0x1b,0x33,0x14,
                                                 0x69,0xaa};
  UINT8 SensitiveUserData[SENSITIVE_USER_SZ] = {0x40,0x00,0x00,0x09,0x00,0x00,
                                                0x00,0x00,0x14,0x00,0x00,0x00,
                                                0x00,0x00,0x00,0x00,0x00,0x00,
                                                0x00,0x00,0x00,0x00,0x00,0x00,
                                                0x00,0x00,0x00,0x00,0x00};
  UINT8 SensitiveData[SENSITIVE_DATA_SZ] = {0x00,0x00,0x00,0x00};
  TPM2_EVICT_CONTROL_COMMAND EvictControl;

  // Grab a handle to the TCG2 TPM driver.
  Status = gBS->LocateProtocol (&gEfiTcg2ProtocolGuid, NULL,
                                (VOID **) &Tcg2Protocol);
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR, "%a: failed to locate protocol. Status: 0x%x\n",
           __FUNCTION__, Status));
    return Status;
  }

#if 1
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
  } else if (SwapBytes32 (RecvBuffer.Header.responseCode) == TPM_RC_SUCCESS) {
    goto SendEKCert;
  }
#endif

  UINT8 DataBuf [1000] = {0};
  UINT8 *DataPtr;

  DEBUG ((DEBUG_ERROR, "%a: TPM Generating EK Certificate!\n", __FUNCTION__));

  CreatePrimary.Header.tag = SwapBytes16 (TPM_ST_SESSIONS);
  CreatePrimary.Header.commandCode = SwapBytes32 (TPM_CC_CreatePrimary);
//CreatePrimary.Header.paramSize - Placeholder until end of marshalling
  CreatePrimary.Hierarchy = SwapBytes32 (TPM_RH_ENDORSEMENT);
  CreatePrimary.Sensitive.size = SwapBytes16 (0);
  CreatePrimary.Sensitive.sensitive.userAuth.size = SwapBytes16 (SENSITIVE_USER_SZ);
//CreatePrimary.Sensitive.sensitive.userAuth.buffer = SensitiveUserData[];
  CreatePrimary.Sensitive.sensitive.data.size = SwapBytes16 (SENSITIVE_DATA_SZ);
//CreatePrimary.Sensitive.sensitive.data.buffer = SensitiveData[];

  TPMA_OBJECT objattrib = {0};
  // Constructing a Default RSA EK Public Area Template
  // As per TCG EK Credential Profile spec Revision 14.
  CreatePrimary.Public.size = SwapBytes16 (RSA_TEMPLATE_SZ);
  CreatePrimary.Public.publicArea.type = SwapBytes16 (TPM_ALG_RSA);
  CreatePrimary.Public.publicArea.nameAlg = SwapBytes16 (TPM_ALG_SHA256);
  objattrib.fixedTPM = 1;
  objattrib.stClear = 0;
  objattrib.fixedParent = 1;
  objattrib.sensitiveDataOrigin = 1;
  objattrib.userWithAuth = 0;
  objattrib.adminWithPolicy = 1;
  objattrib.noDA = 0;
  objattrib.encryptedDuplication = 0;
  objattrib.restricted = 1;
  objattrib.decrypt = 1;
  objattrib.sign = 0;
  *((UINT32*)&CreatePrimary.Public.publicArea.objectAttributes) = SwapBytes32 (*((UINT32*)&objattrib));

  CreatePrimary.Public.publicArea.authPolicy.size = SwapBytes16 (POLICY_SECRET_SZ);
//CreatePrimary.Public.publicArea.authPolicy.buffer = DefaultPolicySecret[];
  CreatePrimary.Public.publicArea.parameters.rsaDetail.symmetric.algorithm = SwapBytes16 (TPM_ALG_AES);
  CreatePrimary.Public.publicArea.parameters.rsaDetail.symmetric.keyBits.aes = SwapBytes16 (128);
  CreatePrimary.Public.publicArea.parameters.rsaDetail.symmetric.mode.aes = SwapBytes16 (TPM_ALG_CFB);
//CreatePrimary.Public.publicArea.parameters.rsaDetail.symmetric.details = NULL;
  CreatePrimary.Public.publicArea.parameters.rsaDetail.scheme.scheme = SwapBytes16 (TPM_ALG_NULL);
//CreatePrimary.Public.publicArea.parameters.rsaDetail.scheme.details = NULL;
  CreatePrimary.Public.publicArea.parameters.rsaDetail.keyBits = SwapBytes16 (2048);
  CreatePrimary.Public.publicArea.parameters.rsaDetail.exponent = SwapBytes32 (0);

  CreatePrimary.Public.publicArea.unique.rsa.size = SwapBytes16 (256);

  CreatePrimary.Data.size = SwapBytes16 (0);

  CreatePrimary.PcrSelection.count = SwapBytes32 (0);


  DataPtr = DataBuf;
  FILLBUFSTRUCT(DataPtr, CreatePrimary.Header);
  FILLBUFSTRUCT(DataPtr, CreatePrimary.Hierarchy);
  FILLBUFSTRUCT(DataPtr, CreatePrimary.Sensitive.size);

  FILLBUFSTRUCT(DataPtr, CreatePrimary.Sensitive.sensitive.userAuth.size);
  FILLBUF(DataPtr, SensitiveUserData, SwapBytes16(CreatePrimary.Sensitive.sensitive.userAuth.size));

  FILLBUFSTRUCT(DataPtr, CreatePrimary.Sensitive.sensitive.data.size);
  FILLBUF(DataPtr, SensitiveData, SwapBytes16(CreatePrimary.Sensitive.sensitive.data.size));

  FILLBUFSTRUCT(DataPtr, CreatePrimary.Public.size);
  FILLBUFSTRUCT(DataPtr, CreatePrimary.Public.publicArea.type);
  FILLBUFSTRUCT(DataPtr, CreatePrimary.Public.publicArea.nameAlg);
  FILLBUFSTRUCT(DataPtr, CreatePrimary.Public.publicArea.objectAttributes);

  FILLBUFSTRUCT(DataPtr, CreatePrimary.Public.publicArea.authPolicy.size);
  FILLBUF(DataPtr, DefaultPolicySecret, SwapBytes16(CreatePrimary.Public.publicArea.authPolicy.size));

  FILLBUFSTRUCT(DataPtr, CreatePrimary.Public.publicArea.parameters.rsaDetail.symmetric);
  FILLBUFSTRUCT(DataPtr, CreatePrimary.Public.publicArea.parameters.rsaDetail.scheme.scheme);
  FILLBUFSTRUCT(DataPtr, CreatePrimary.Public.publicArea.parameters.rsaDetail.keyBits);
  FILLBUFSTRUCT(DataPtr, CreatePrimary.Public.publicArea.parameters.rsaDetail.exponent);

  FILLBUFSTRUCT(DataPtr, CreatePrimary.Public.publicArea.unique.rsa.size);
  SetMem(DataPtr, 256, 0);
  DataPtr += 256;

  FILLBUFSTRUCT(DataPtr, CreatePrimary.Data.size);
  FILLBUFSTRUCT(DataPtr, CreatePrimary.PcrSelection.count);


  // Populate the actual size of the command
  SendBufferSize = DataPtr - DataBuf;
  DataPtr = DataBuf;
  CreatePrimary.Header.paramSize = SwapBytes32 (SendBufferSize);
  FILLBUFSTRUCT(DataPtr, CreatePrimary.Header);

  Status = Tcg2Protocol->SubmitCommand (
                          Tcg2Protocol,
                          SendBufferSize,
                          DataBuf,
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


  // Prepare the EvictControl command
  EvictControl.Header.tag = SwapBytes16 (TPM_ST_SESSIONS);
  EvictControl.Header.commandCode = SwapBytes32 (TPM_CC_EvictControl);
  EvictControl.AuthHandle = SwapBytes32 (TPM_RH_OWNER);
  CopyMem ((void*)&EvictControl.SequenceHandle, (void*)(TpmOut+sizeof (TPM2_RESPONSE_HEADER)), 4);
  AuthorizationSize = SwapBytes32 (SENSITIVE_USER_SZ);
  EvictControl.PersistentHandle = SwapBytes32 (0x81010001);


  DataPtr = DataBuf;
  FILLBUFSTRUCT(DataPtr, EvictControl.Header);
  FILLBUFSTRUCT(DataPtr, EvictControl.AuthHandle);
  FILLBUFSTRUCT(DataPtr, EvictControl.SequenceHandle);
  FILLBUFSTRUCT(DataPtr, AuthorizationSize);
  FILLBUF(DataPtr, SensitiveUserData, SwapBytes32(AuthorizationSize));
  FILLBUFSTRUCT(DataPtr, EvictControl.PersistentHandle);

  // Populate the actual size of the command
  SendBufferSize = DataPtr - DataBuf;
  DataPtr = DataBuf;
  EvictControl.Header.paramSize = SwapBytes32 (SendBufferSize);
  FILLBUFSTRUCT(DataPtr, EvictControl.Header);

  SetMem(TpmOut, TPMOUT_SZ, 0);

  Status = Tcg2Protocol->SubmitCommand (
                          Tcg2Protocol,
                          SendBufferSize,
                          DataBuf,
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

#if 1
SendEKCert:
#endif
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

  UINT8 SensitiveUserData[SENSITIVE_USER_SZ] = {0x40,0x00,0x00,0x09,0x00,0x00,
                                                0x00,0x00,0x14,0x00,0x00,0x00,
                                                0x00,0x00,0x00,0x00,0x00,0x00,
                                                0x00,0x00,0x00,0x00,0x00,0x00,
                                                0x00,0x00,0x00,0x00,0x00};

  Status = ReceiveBuffer ("MFG:devicecert\r\n", &CertPtr, &CertLen);
  if (EFI_ERROR (Status)) {
    goto cleanup;
  }

  TPMI_RH_PROVISION AuthHandle;
  TPM2B_AUTH Auth;
  TPM2B_NV_PUBLIC NvPublic = {0};

  AuthHandle = TPM_RH_OWNER;
  Auth.size = 0;
  CopyMem(Auth.buffer, SensitiveUserData, SENSITIVE_USER_SZ);

  NvPublic.nvPublic.nvIndex = EK_CERT_NVINDEX; // Endorsement certificate
  NvPublic.nvPublic.nameAlg = TPM_ALG_SHA256;
  NvPublic.nvPublic.attributes.TPMA_NV_PPWRITE = 1;
  NvPublic.nvPublic.attributes.TPMA_NV_OWNERWRITE = 1;
  NvPublic.nvPublic.attributes.TPMA_NV_AUTHWRITE = 1;
  NvPublic.nvPublic.attributes.TPMA_NV_POLICYWRITE = 1;
  NvPublic.nvPublic.attributes.TPMA_NV_COUNTER = 0;
  NvPublic.nvPublic.attributes.TPMA_NV_BITS = 0;
  NvPublic.nvPublic.attributes.TPMA_NV_EXTEND = 0;
  NvPublic.nvPublic.attributes.TPMA_NV_POLICY_DELETE = 0;
  NvPublic.nvPublic.attributes.TPMA_NV_WRITELOCKED = 0;
  NvPublic.nvPublic.attributes.TPMA_NV_WRITEALL = 1;
// Allow TPM2_NV_WriteLock to prevent further writes.
  NvPublic.nvPublic.attributes.TPMA_NV_WRITEDEFINE = 1;
  NvPublic.nvPublic.attributes.TPMA_NV_WRITE_STCLEAR = 0;
  NvPublic.nvPublic.attributes.TPMA_NV_GLOBALLOCK = 0;
// Let anyone authorized read it
  NvPublic.nvPublic.attributes.TPMA_NV_PPREAD = 1;
  NvPublic.nvPublic.attributes.TPMA_NV_OWNERREAD = 1;
  NvPublic.nvPublic.attributes.TPMA_NV_PPREAD = 1;
  NvPublic.nvPublic.attributes.TPMA_NV_OWNERREAD = 1;
// Disable lockout logic for failed auths
  NvPublic.nvPublic.attributes.TPMA_NV_NO_DA = 1;
// Write must be persisted by write command return. No lazy storage.
  NvPublic.nvPublic.attributes.TPMA_NV_ORDERLY = 0;
// Don't reset TPMA_NV_WRITTEN on a TPM reset or TPM restart.
  NvPublic.nvPublic.attributes.TPMA_NV_CLEAR_STCLEAR = 0;
// Not readlocked
  NvPublic.nvPublic.attributes.TPMA_NV_READLOCKED = 0;
// Not written
  NvPublic.nvPublic.attributes.TPMA_NV_WRITTEN = 0;
// Index may be undefined using owner authorization
  NvPublic.nvPublic.attributes.TPMA_NV_PLATFORMCREATE = 0;
// Unable to turn on readlock
  NvPublic.nvPublic.attributes.TPMA_NV_READ_STCLEAR = 0;


  NvPublic.nvPublic.authPolicy.size = 0;
  CopyMem(NvPublic.nvPublic.authPolicy.buffer, SensitiveUserData, SENSITIVE_USER_SZ);

  NvPublic.nvPublic.dataSize = CertLen;
//  NvPublic.nvPublic.dataSize = 0x100;


  NvPublic.size = sizeof(TPMI_RH_NV_INDEX) + sizeof(TPMI_ALG_HASH) + sizeof(TPMA_NV) + sizeof(UINT16) + sizeof(UINT16) + NvPublic.nvPublic.authPolicy.size;


  Status = Tpm2NvDefineSpace(AuthHandle, NULL, &Auth, &NvPublic);

  if (EFI_ERROR (Status)) {
    if (Status == EFI_ALREADY_STARTED) {
      DEBUG ((DEBUG_ERROR,"NV index for certificate already defined\n"));
    } else {
      DEBUG ((DEBUG_ERROR,"Tpm2NvDefineSpace failed!\n"));
      goto cleanup;
    }
  }

  TPMI_RH_NV_INDEX NvIndex = EK_CERT_NVINDEX;
  TPM2B_MAX_BUFFER InData;
  TPM2B_MAX_BUFFER OutData;

  DEBUG ((DEBUG_ERROR,"About to Tpm2NvWrite. Certlen is %d\n", CertLen));

  InData.size = CertLen;
  CopyMem(InData.buffer, CertPtr, CertLen);

  Status = Tpm2NvWrite(AuthHandle, NvIndex, NULL, &InData, 0);

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "Provisioning: Failed Tpm2NvWrite, Status = %x\n",
      Status
      ));
  }

  DEBUG ((DEBUG_ERROR,"About to Tpm2NvRead\n"));

  Status = Tpm2NvRead(AuthHandle, NvIndex, NULL, CertLen, 0, &OutData);

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "Provisioning: Failed Tpm2NvRead, Status = %x\n",
      Status
      ));
  }

/*
  Status = gRT->SetVariable (
                  (CHAR16 *) mDeviceCertVariableName,
                  &giMXPlatformProvisioningGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS |
                  EFI_VARIABLE_RUNTIME_ACCESS,
                  CertLen,
                  (VOID *) CertPtr
                  );
*/

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "Provisioning: Failed to save %s variable, Status = %x\n",
      mDeviceCertVariableName,
      Status
      ));
  }

  DEBUG ((DEBUG_ERROR,"Certificate store was successful\n"));

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
  volatile int x = 0;
  while(x);
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
