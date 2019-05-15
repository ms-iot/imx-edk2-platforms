/** @file
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

#include "SmbiosConfigDxe.h"

EFI_STATUS
EFIAPI
GetSmbiosOverrideData (
  )
{
  VOID *Buffer;
  UINTN BufferSize;
  CONST CHAR16 *DevicePathText;
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;
  EFI_FILE_HANDLE File;
  UINTN FileInfoSize;
  EFI_FILE_INFO *FileInfo;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *Fs;
  EFI_DEVICE_PATH_PROTOCOL *IntermediateDevicePath;
  EFI_HANDLE MediaHandle;
  EFI_FILE_HANDLE RootVolume;
  EFI_STATUS Status;
 
  File = NULL;
  RootVolume = NULL;
  Buffer = NULL;
  DevicePath = NULL;
  MediaHandle = NULL;
  Fs = NULL;

  DevicePathText = (CONST CHAR16 *) FixedPcdGetPtr(PcdSmbiosOverrideDevicePath);
  if ((DevicePathText == NULL) || (*DevicePathText == L'\0')) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_ERROR, "%a: PcdStorageMediaPartition is unspecified\n", __FUNCTION__));
    goto Exit;
  }

  DevicePath = ConvertTextToDevicePath (DevicePathText);
  if (!DevicePath) {
    DEBUG ((DEBUG_ERROR, "%a: Could not convert DevicePathText\n", __FUNCTION__));
    return FALSE;
  }

  IntermediateDevicePath = DevicePath;
  Status = gBS->LocateDevicePath(
    &gEfiSimpleFileSystemProtocolGuid,
    &IntermediateDevicePath,
    &MediaHandle);

  if (Status == EFI_NOT_FOUND) {
    Status = EFI_SUCCESS;
    DEBUG ((DEBUG_INFO,"%a: %s FAT partition is not ready yet\n", __FUNCTION__, DevicePathText));
    goto Exit;
  }

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: gBS->LocateDevicePath() failed. (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  Status = gBS->HandleProtocol (
    MediaHandle,
    &gEfiSimpleFileSystemProtocolGuid,
    (VOID**)&Fs
    );

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: gBS->HandleProtocol() failed. (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  Status = Fs->OpenVolume(Fs, &RootVolume);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fs->OpenVolume() failed. (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  Status = RootVolume->Open(
    RootVolume,
    &File,
    L"Smbios.cfg",
    EFI_FILE_MODE_READ,
    0);

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: RootVolume->Open() failed. (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  //
  // Get File Info
  // 
  FileInfoSize = sizeof(EFI_FILE_INFO) + 1024;
  FileInfo = AllocateZeroPool (FileInfoSize);
  if (FileInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: FileInfo allocation failed\n", __FUNCTION__));
    goto Exit;
  }

  Status = File->GetInfo (
                  File,
                  &gEfiFileInfoGuid,
                  &FileInfoSize,
                  FileInfo
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to get File Info (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  BufferSize = (UINTN) FileInfo->FileSize + sizeof(CHAR16);
  Buffer = AllocateZeroPool (BufferSize);
  if (Buffer == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Buffer allocation failed \n", __FUNCTION__));
    goto Exit;
  }

  Status = File->Read (
                File,
                &BufferSize,
                Buffer
                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: File Read failed (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "%a: Buffer contains %s\n", __FUNCTION__, Buffer));

Exit:

  if (Buffer != NULL) {
    gBS->FreePool (Buffer);
    Buffer = NULL;
  }

  if (FileInfo != NULL) {
    gBS->FreePool (FileInfo);
    FileInfo = NULL;
  }

  if (File != NULL) {
    File->Close(File);
    File = NULL;
  }

  if (RootVolume != NULL) {
    RootVolume->Close(RootVolume);
    RootVolume = NULL;
  }

  if (DevicePath != NULL) {
    FreePool(DevicePath);
  }

  return Status;

}

BOOLEAN
CheckSmbiosOverridePresent (
  )
{
  return FALSE;
}

EFI_STATUS
EFIAPI
SetSmbiosOverridePresent (
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SmbiosConfigDxeInitialize (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  BOOLEAN Locked;
  EFI_STATUS Status;

  DEBUG ((DEBUG_ERROR, "%a: Enter\n", __FUNCTION__));

  // Check if an Smbios override config was previously set
  Locked = CheckSmbiosOverridePresent();
  if (Locked == TRUE) {
    Status = EFI_SUCCESS;
    goto Exit;
  }

  // Lock down config no matter the result
  SetSmbiosOverridePresent();

  // Read Smbios Override file from filesystem
  Status = GetSmbiosOverrideData();

  // Validate Smbios override data

  // Store override data in NV variable

Exit:

  return Status;
}
