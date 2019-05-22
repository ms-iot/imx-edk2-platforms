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

typedef enum JsonParserState {
  StateFindKey,
  StateReadKey,
  StateKeyDone,
  StateFindValue,
  StateReadValue,
  StateValueDone,  
} JSON_PARSER_STATE;

#define MAX_VARIABLE_SIZE 100

typedef struct SmbiosOverrideNode {
  CHAR8 *Key;
  CHAR8 *Value;
  struct SmbiosOverrideNode *Next;
} SMBIOS_OVERRIDE_NODE;

STATIC CONST CHAR16 mSmbiosOverridePresent[] = L"SmbiosOverridePresent";

STATIC SMBIOS_OVERRIDE_NODE *mSmbiosOverrideListHead;

/**

  Finds and retrieves the next Key/Value pair from the buffer starting at the
  given offset.

  This routine scans through the provided buffer starting at the given offset.
  If finds the first '"' character and records this location as the start of the Key.
  Then it contineus scanning to find the next '"' instance, which denotes the KeyLength.

  We repeat these steps to find the Value position and length.

  At any point if a failure is detected, the algorithm bails.

  If we successfully find both Key and Value positions and lengths, then allocate/copy
  these to their respective buffers and return to the caller.

**/
EFI_STATUS
EFIAPI
GetNextKeyValuePair (
  CHAR8 *Buffer,
  UINTN *Offset,
  UINTN BufferSize,
  CHAR8 **Key,
  CHAR8 **Value
  )
{
  EFI_STATUS Status;
  UINTN CurrentOffset;
  UINTN KeyLength;
  UINTN ValueLength;
  JSON_PARSER_STATE State;
  CHAR8 *KeyStart;
  CHAR8 *ValueStart;
  CHAR8 *TempKey;
  CHAR8 *TempValue;

  CurrentOffset = *Offset;
  KeyLength = 0;
  ValueLength = 0;
  Status = EFI_NOT_FOUND;
  State = StateFindKey;
  KeyStart = NULL;
  ValueStart = NULL;
  TempKey = NULL;
  TempValue = NULL;

  while (CurrentOffset < BufferSize) {
    if (State == StateValueDone) {
      Status = EFI_SUCCESS;
      break;
    }

    switch (State) {
      case StateFindKey:
        if (Buffer[CurrentOffset] == '"') {
          State = StateReadKey;
        }
      break;
      case StateReadKey:
        if (KeyStart == NULL) {
          KeyStart = &Buffer[CurrentOffset];
        }
        if (Buffer[CurrentOffset] == '"') {
          State = StateKeyDone;
        } else {
          KeyLength++;
        }
      break;
      case StateKeyDone:
        if (Buffer[CurrentOffset] == ':') {
          State = StateFindValue;
        }
      break;
      case StateFindValue:
        if (Buffer[CurrentOffset] == '"') {
          State = StateReadValue;
        }
      break;
      case StateReadValue:
        if (ValueStart == NULL) {
          ValueStart = &Buffer[CurrentOffset];
        }
        if (Buffer[CurrentOffset] == '"') {
          State = StateValueDone;
        } else {
          ValueLength++;
        }
      break;
      default:
        DEBUG ((DEBUG_ERROR, "%a: Reached an unknown state %d\n", __FUNCTION__, State));
        Status = EFI_NOT_FOUND;
        goto Exit;
    }
    CurrentOffset++;
  }

  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  // Bounds check on Key and Value
  if ((KeyLength == 0) || (ValueLength == 0)) {
    Status = EFI_NOT_FOUND;
    goto Exit;
  }
  if ((KeyLength > MAX_VARIABLE_SIZE) || (ValueLength > MAX_VARIABLE_SIZE)) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  
  TempKey = AllocatePool(KeyLength + 1);
  if (TempKey == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  CopyMem(TempKey, KeyStart, KeyLength);
  TempKey[KeyLength] = '\0';
  KeyLength++;

  TempValue = AllocatePool (ValueLength + 1);
  if (TempValue == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  CopyMem (TempValue, ValueStart, ValueLength);
  TempValue[ValueLength] = '\0';
  ValueLength++;

  *Key = TempKey;
  *Value = TempValue;
  *Offset = CurrentOffset;

Exit:
  return Status;
}

#define MAX_SMBIOS_KEY_REV_1 5
CHAR8* mSmbiosKeyRev1[MAX_SMBIOS_KEY_REV_1] = {"System Manufacturer",
                                               "System Product Name",
                                               "System SKU",
                                               "System Family",
                                               "Baseboard Product"};

EFI_STATUS
EFIAPI
GetNextJsonKeyword (
  CHAR8 *Buffer,
  UINTN *Offset,
  UINTN BufferSize,
  CHAR8 *OutChar
  )
{
  UINTN CurrentOffset;
  EFI_STATUS Status;

  Status = EFI_NOT_FOUND;
  CurrentOffset = *Offset;

  while (CurrentOffset < BufferSize) {
    if ((Buffer[CurrentOffset] == '{') ||
        (Buffer[CurrentOffset] == '}') ||
        (Buffer[CurrentOffset] == '"') ||
        (Buffer[CurrentOffset] == ',')) {
      *OutChar = Buffer[CurrentOffset];
      Status = EFI_SUCCESS;
      break;
    }
    CurrentOffset++;
  }
  *Offset = CurrentOffset;

  return Status;
}

EFI_STATUS
EFIAPI
GetSmbiosOverride (
  CHAR8 *Key,
  CHAR8 **Value
  )
{
  SMBIOS_OVERRIDE_NODE *Node;
  EFI_STATUS Status;
  INTN CompareResult;
  CHAR16 UnicodeDebugString[MAX_VARIABLE_SIZE];
  
  Node = mSmbiosOverrideListHead;
  Status = EFI_NOT_FOUND;
  while (Node != NULL) {
    CompareResult = AsciiStrnCmp (Node->Key, Key, AsciiStrnSizeS(Key, MAX_VARIABLE_SIZE));
    if (CompareResult == 0) {
      AsciiStrToUnicodeStrS (Node->Key, UnicodeDebugString, MAX_VARIABLE_SIZE);
      DEBUG ((DEBUG_INFO, "%a: Found Smbios Override Key = %s\n", __FUNCTION__, UnicodeDebugString));
      AsciiStrToUnicodeStrS (Node->Value, UnicodeDebugString, MAX_VARIABLE_SIZE);
      DEBUG ((DEBUG_INFO, "%a: Smbios Override Value = %s\n", __FUNCTION__, UnicodeDebugString));
      *Value = Node->Value;
      Status = EFI_SUCCESS;
      goto Exit;
    }
    Node = Node->Next;
  }

Exit:
  return Status;
}

EFI_STATUS
EFIAPI
ValidateRevision1 (
  UINTN Count
  )
{
  CHAR8 *Value;
  EFI_STATUS Status;
  INTN i;
  CHAR16 UnicodeDebugString[MAX_VARIABLE_SIZE];

  Status = EFI_NOT_FOUND;

  // Remove Signature and Revision
  Count -= 2;
  if (Count != MAX_SMBIOS_KEY_REV_1) {
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  for (i = 0; i < MAX_SMBIOS_KEY_REV_1; i++) {
    AsciiStrToUnicodeStrS (mSmbiosKeyRev1[i], UnicodeDebugString, MAX_VARIABLE_SIZE);
    Status = GetSmbiosOverride(mSmbiosKeyRev1[i], &Value);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Failed to find Smbios Key %s\n", __FUNCTION__, UnicodeDebugString));
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "%a: Found %s\n", __FUNCTION__, UnicodeDebugString));
  }
  Status = EFI_SUCCESS;
Exit:
  return Status;
}

/**

  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER   File does not conform to our expected Json Format
  @retval EFI_NOT_FOUND           All expected Json keys could not be found
**/

EFI_STATUS
EFIAPI
ValidateBufferAndStoreInList (
  CHAR8* Buffer,
  UINTN BufferSize
  )
{
  EFI_STATUS Status;
  CHAR8 *Key;
  CHAR8 *Value;
  UINTN Offset;
  UINTN Count;
  CHAR8 NextChar;
  CONST STATIC CHAR8 Signature[] = "SmbiosOverrideConfigurationTable";
  INTN CompareResult;
  SMBIOS_OVERRIDE_NODE *NewNode;
  UINT64 Revision;

  Key = NULL;
  Value = NULL;
  // Fill Linked List with File data
  Offset = 0;
  Count = 0;
  Status = GetNextJsonKeyword(Buffer, &Offset, BufferSize, &NextChar);
  if (EFI_ERROR (Status)) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if (NextChar != '{') {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  while (TRUE) {
    Status = GetNextJsonKeyword (Buffer, &Offset, BufferSize, &NextChar);
    if (EFI_ERROR (Status)) {
      Status = EFI_INVALID_PARAMETER;
      goto Exit;
    }
    if (NextChar == '}') {
      Status = EFI_SUCCESS;
      break;
    }
    Status = GetNextKeyValuePair (Buffer, &Offset, BufferSize, &Key, &Value);
    if (EFI_ERROR (Status)) {
      Status = EFI_INVALID_PARAMETER;
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "%a: Found new Key/Value pair\n", __FUNCTION__));
    NewNode = AllocatePool (sizeof (SMBIOS_OVERRIDE_NODE));
    if (NewNode == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit; 
    }
    NewNode->Key = Key;
    NewNode->Value = Value;
    NewNode->Next = mSmbiosOverrideListHead;
    mSmbiosOverrideListHead = NewNode;
    Count++;
  }

  // Check Signature
  Status = GetSmbiosOverride("Signature", &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Smbios Override signature entry not found\n", __FUNCTION__));
    goto Exit;
  }
  
  CompareResult = AsciiStrnCmp (Signature, Value, AsciiStrnSizeS (Signature, MAX_VARIABLE_SIZE));
  if (CompareResult != 0) {
    DEBUG ((DEBUG_ERROR, "%a: Smbios Override signature incorrect \n", __FUNCTION__));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  // Check revision
  Status = GetSmbiosOverride ("Revision", &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Revision entry not found\n", __FUNCTION__));
    goto Exit;
  }
  
  Revision = AsciiStrDecimalToUintn (Value);
  switch (Revision) {
    case 1:
    Status = ValidateRevision1(Count);
    break;
    default:
    DEBUG ((DEBUG_ERROR, "%a: Revision not supported\n", __FUNCTION__));
    Status = EFI_NOT_FOUND;
    break;
  }

Exit:
  return Status;
}

VOID
DumpListInfo (
  )
{
  SMBIOS_OVERRIDE_NODE *Node;
  UINTN Count;
  UINT16 UnicodeKey[MAX_VARIABLE_SIZE];
  UINT16 UnicodeValue[MAX_VARIABLE_SIZE];

  Count = 0;
  Node = mSmbiosOverrideListHead;

  while (Node != NULL) {
    AsciiStrToUnicodeStr (Node->Key, UnicodeKey);
    AsciiStrToUnicodeStr (Node->Value, UnicodeValue);
    DEBUG ((DEBUG_INFO, "%a: Node[%d] = (%s, %s)\n", __FUNCTION__, Count, UnicodeKey, UnicodeValue));
    Count++;
    Node = Node->Next;
  }
}

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

  Status = ValidateBufferAndStoreInList (Buffer, BufferSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Buffer Validation failed (Status=%r)\n", __FUNCTION__, Status));
  }

  DumpListInfo();

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

EFI_STATUS
EFIAPI
CheckSmbiosOverridePresent (
  )
{
  UINTN DataSize;
  EFI_STATUS Status;

  DataSize = 0;
  Status = gRT->GetVariable ((CHAR16 *)mSmbiosOverridePresent,
                  &giMXPlatformSmbiosOverrideGuid,
                  NULL,
                  &DataSize,
                  NULL);

  if (Status == EFI_BUFFER_TOO_SMALL) {
    return EFI_SUCCESS;
  }

  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
SetSmbiosOverridePresent (
  )
{
  UINT8 Data;
  EFI_STATUS Status;

  Data = 1;
  Status = gRT->SetVariable ((CHAR16 *)mSmbiosOverridePresent,
                  &giMXPlatformSmbiosOverrideGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS |
                  EFI_VARIABLE_RUNTIME_ACCESS,
                  1,
                  (VOID *)&Data);
        
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
StoreSmbiosOverrideVariable (
  CHAR16* Key,
  CHAR16* Value
  )
{
  EFI_STATUS Status;

  Status = gRT->SetVariable (Key,
                &giMXPlatformSmbiosOverrideGuid,
                EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS |
                EFI_VARIABLE_RUNTIME_ACCESS,
                StrnSizeS (Value, MAX_VARIABLE_SIZE),
                (VOID *) Value);

  return Status;
}

EFI_STATUS
EFIAPI
StoreAllSmbiosOverrideVariables (
  )
{
  CHAR16 UnicodeKey[MAX_VARIABLE_SIZE];
  CHAR16 UnicodeValue[MAX_VARIABLE_SIZE];
  EFI_STATUS Status;
  SMBIOS_OVERRIDE_NODE *Node;

  Node = mSmbiosOverrideListHead;
  Status = EFI_NOT_READY;

  while (Node != NULL) {
    AsciiStrToUnicodeStrS (Node->Key, UnicodeKey, MAX_VARIABLE_SIZE);
    AsciiStrToUnicodeStrS (Node->Value, UnicodeValue, MAX_VARIABLE_SIZE);
    Status = StoreSmbiosOverrideVariable (UnicodeKey, UnicodeValue);
    if (EFI_ERROR (Status)) {
      goto Exit;
    }
    Node = Node->Next;
  }
  Status = EFI_SUCCESS;
Exit:
  return Status;
}

VOID
FreeSmbiosOverrideList(
  )
{
  SMBIOS_OVERRIDE_NODE *Node;
  SMBIOS_OVERRIDE_NODE *NodeToFree;

  Node = mSmbiosOverrideListHead;
  mSmbiosOverrideListHead = NULL;

  while (Node != NULL) {
    NodeToFree = Node;
    Node = Node->Next;

    FreePool (NodeToFree->Key);
    FreePool (NodeToFree->Value);
    FreePool (NodeToFree);
  }  
}

EFI_STATUS
EFIAPI
SmbiosConfigDxeInitialize (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS Status;

  // Check if an Smbios override config was previously set
  Status = CheckSmbiosOverridePresent();
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "%a: Smbios Override Locked\n", __FUNCTION__));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "%a: Smbios Override Unlocked\n", __FUNCTION__));

  // Lock down config no matter the result
  Status = SetSmbiosOverridePresent();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to set SmbiosOverridePresent (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  // Read Smbios Override file from filesystem
  Status = GetSmbiosOverrideData();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to read Smbios Override Data (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;  
  }

  // Store override data in NV variable
  Status = StoreAllSmbiosOverrideVariables();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed store to NV (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

Exit:
  FreeSmbiosOverrideList();

  return Status;
}
