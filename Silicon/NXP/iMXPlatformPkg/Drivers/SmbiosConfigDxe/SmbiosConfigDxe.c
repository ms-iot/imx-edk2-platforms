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

/**

  The SmbiosConfigDxe driver functionality requires a couple Pcd settings
  to be set prior to use.
    PcdSmbiosOverrideEnable
    PcdSmbiosOverrideDevicePath

  This Smbios Config Dxe driver expects the Smbios.ini file to be in a
  "simplified" INI format with only Key=Value pairs and no [Section] headers.
  The file is expected to be in ASCII text with either LF or CRLF line endings.

  Example:

  Signature=SmbiosOverrideConfigurationTable
  Revision=1
  System Manufacturer=Contoso
  System Product Name=Contoso Product
  System SKU=Contoso SKU
  System Family=Contoso Family
  Baseboard Product=Contoso Baseboard

  Note: The last line must end with a newline character to ensure the last
  key-value pair is recorded during INI parsing.

**/

#define MAX_SMBIOS_KEY_REV_1 5
STATIC CONST CHAR16* mSmbiosKeyRev1[MAX_SMBIOS_KEY_REV_1] = {
                                            IMX_VARIABLE_SMBIOS_MANUFACTURER,
                                            IMX_VARIABLE_SMBIOS_PRODUCT_NAME,
                                            IMX_VARIABLE_SMBIOS_SKU,
                                            IMX_VARIABLE_SMBIOS_FAMILY,
                                            IMX_VARIABLE_SMBIOS_BASEBOARD_PRODUCT};

STATIC SMBIOS_OVERRIDE_NODE *mSmbiosOverrideListHead = NULL;

/**

  Checks if the input buffer is in valid "simplified" INI format

  @param[in]  Buffer      Input buffer to be validated
  @param[in]  BufferSize  Input buffer size

  @retval EFI_SUCCESS     Buffer is a valid "simplified" INI format
  @retval EFI_NOT_FOUND   Buffer is not a valid "simplified" INI format

**/
EFI_STATUS
EFIAPI
ValidateIniFormat (
  IN  CHAR8   *Buffer,
  IN  UINTN   BufferSize
  )
{
  UINTN       CurrentOffset;
  UINTN       NumEquals;
  EFI_STATUS  Status;

  NumEquals = 0;
  CurrentOffset = 0;
  while (CurrentOffset < BufferSize) {
    Status = EFI_NOT_FOUND;
    if (Buffer[CurrentOffset] == '=') {
      NumEquals++;
    }
    if (Buffer[CurrentOffset] == '\n') {
      if (NumEquals == 0) {
        Status = EFI_NOT_FOUND;
        goto Exit;
      }
      NumEquals = 0;
      Status = EFI_SUCCESS;
    }
    CurrentOffset++;
  }

Exit:
  return Status;
}

/**

  Retrieves the next Key/Value pair from the given INI buffer.

  This routine scans through the provided buffer starting at the given offset.
  The first character is expected to be the start of the Key. The routine
  iterates through the buffer until it finds "=" character, which signifies
  the end of the Key. The process repeats for the Value portion and ends
  when a newline is found.

  At any point if a failure is detected, the routine bails with an error.
  If Key and Value are successfully found, allocate and return Unicode strings
  containing Key/Value and return these to the caller.

  Note: the caller is responsible for freeing these strings.

  @param[in]      Buffer        Input INI buffer with ASCII characters.
  @param[in out]  Offset        Pointer to starting offset into buffer. When
                                routine is finished, this pointer is updated
                                to hold the starting offset of the next INI
                                key.
  @param[in]      BufferSize    Total size of the input buffer in bytes.
  @param[out]     Key           Pointer to Unicode string containing Key
  @param[out]     Value         Pointer to Unicode string containing Value

  @retval EFI_SUCCESS             Successfully obtained the next Key/Value
                                  from the INI buffer.
  @retval EFI_NOT_FOUND           No more valid Key/Value pairs remaining
                                  in the buffer.
  @retval EFI_OUT_OF_RESOURCES    Error allocating memory for new Key
                                  or Value.
  @retval EFI_INVALID_PARAMETER   Bad parameter passed into this function.

**/
EFI_STATUS
EFIAPI
GetNextIniKeyValuePair (
  IN  CHAR8       *Buffer,
  IN OUT UINTN    *Offset,
  IN  UINTN       BufferSize,
  OUT CHAR16      **Key,
  OUT CHAR16      **Value
  )
{
  UINTN             CurrentOffset;
  UINTN             KeyLength;
  CHAR8             *KeyStart;
  INI_PARSER_STATE  State;
  EFI_STATUS        Status;
  CHAR8             *TempAsciiKey;
  CHAR8             *TempAsciiValue;
  CHAR16            *TempUnicodeKey;
  CHAR16            *TempUnicodeValue;
  UINTN             ValueLength;
  CHAR8             *ValueStart;

  if ((Buffer == NULL) || (Offset == NULL) ||
      (Key == NULL) || (Value == NULL)) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  CurrentOffset = *Offset;
  KeyLength = 0;
  ValueLength = 0;
  Status = EFI_NOT_FOUND;
  State = StateReadKey;
  KeyStart = NULL;
  ValueStart = NULL;
  TempAsciiKey = NULL;
  TempAsciiValue = NULL;
  TempUnicodeKey = NULL;
  TempUnicodeValue = NULL;

  //
  // Loop until end of file or we find a valid key/value pair
  //
  while (CurrentOffset < BufferSize) {
    switch (State) {
      case StateReadKey:
        if (KeyStart == NULL) {
          KeyStart = &Buffer[CurrentOffset];
        }
        if (Buffer[CurrentOffset] == '=') {
          State = StateReadValue;
        } else {
          KeyLength++;
        }
      break;

      case StateReadValue:
        if (ValueStart == NULL) {
          ValueStart = &Buffer[CurrentOffset];
        }

        //
        // Need to handle both Unix LF (\n) and Windows CRLF (\r\n) line endings
        //
        if (Buffer[CurrentOffset] == '\r') {
          State = StateReadWindowsNewLine;
        } else if (Buffer[CurrentOffset] == '\n') {
          State = StateValueDone;
        } else {
          ValueLength++;
        }
      break;

      case StateReadWindowsNewLine:
        if (Buffer[CurrentOffset] == '\n') {
          State = StateValueDone;
        } else {
          State = StateError;
        }
      break;

      case StateError:
      default:
        DEBUG ((DEBUG_ERROR, "%a: Reached an unknown/bad state %d\n", __FUNCTION__, State));
        Status = EFI_NOT_FOUND;
        goto Exit;
    }
    CurrentOffset++;

    if (State == StateValueDone) {
      Status = EFI_SUCCESS;
      break;
    }
  }

  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  // Bounds check on Key and Value
  if ((KeyLength == 0) || (ValueLength == 0)) {
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  if (KeyLength > (MAX_VARIABLE_SIZE - 1)) {
    KeyLength = MAX_VARIABLE_SIZE - 1;
  }

  if (ValueLength > (MAX_VARIABLE_SIZE - 1)) {
    ValueLength = MAX_VARIABLE_SIZE - 1;
  }

  //
  // Make Key entry. Two part process. First allocate and create an ASCII copy.
  // Then allocate and create a Unicode version. The Unicode version will be
  // returned to the caller.
  //
  TempAsciiKey = AllocatePool (KeyLength + 1);
  if (TempAsciiKey == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  CopyMem (TempAsciiKey, KeyStart, KeyLength);
  TempAsciiKey[KeyLength] = '\0';
  KeyLength++;

  TempUnicodeKey = AllocatePool (KeyLength * sizeof (CHAR16));
  if (TempUnicodeKey == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  Status = AsciiStrToUnicodeStrS (TempAsciiKey, TempUnicodeKey, MAX_VARIABLE_SIZE);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Make Value entry. Two part process. First allocate and create an ASCII copy.
  // Then allocate and create a Unicode version. The Unicode version will be
  // returned to the caller.
  //
  TempAsciiValue = AllocatePool (ValueLength + 1);
  if (TempAsciiValue == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  CopyMem (TempAsciiValue, ValueStart, ValueLength);
  TempAsciiValue[ValueLength] = '\0';
  ValueLength++;

  TempUnicodeValue = AllocatePool (ValueLength * sizeof (CHAR16));
  if (TempUnicodeValue == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  Status = AsciiStrToUnicodeStrS (TempAsciiValue, TempUnicodeValue, MAX_VARIABLE_SIZE);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  *Key = TempUnicodeKey;
  *Value = TempUnicodeValue;
  *Offset = CurrentOffset;

Exit:
  //
  // Always free the temporary Ascii buffers
  //
  if (TempAsciiKey != NULL) {
    FreePool (TempAsciiKey);
  }
  if (TempAsciiValue != NULL) {
    FreePool (TempAsciiValue);
  }

  //
  // Only free the Unicode buffers on error.
  // Caller's responsibility to free these on success.
  //
  if (EFI_ERROR (Status)) {
    if (TempUnicodeKey != NULL) {
      FreePool (TempUnicodeKey);
    }
    if (TempUnicodeValue != NULL) {
      FreePool (TempUnicodeValue);
    }
  }
  return Status;
}

/**

  Search Smbios Override Linked List for Key and retrieve corresponding value.

  This function walks through the Smbios Override Linked List and compares
  the stored Key with the input Key. If a match is found, return the corresponding
  value.

  @param[in] Key      Unicode String of the input Key element.
  @param[Out] Value   Pointer to a Unicode String that contains the Value
                      element in the linked list corresponding with the
                      provided Key element.

  @retval EFI_SUCCESS     Successfully found the Key/Value pair in the Linked
                          List.
  @retval EFI_NOT_FOUND   Input Key not found in the Linked List.

**/
EFI_STATUS
EFIAPI
GetSmbiosOverride (
  IN  CONST CHAR16  *Key,
  OUT CHAR16        **Value
  )
{
  INTN                  CompareResult;
  SMBIOS_OVERRIDE_NODE  *Node;
  EFI_STATUS            Status;
  
  Node = mSmbiosOverrideListHead;
  Status = EFI_NOT_FOUND;
  while (Node != NULL) {
    CompareResult = StrnCmp (Node->Key, Key, StrnSizeS (Key, MAX_VARIABLE_SIZE));
    if (CompareResult == 0) {
      DEBUG ((DEBUG_INFO, "%a: Found Smbios Override Key = %s\n", __FUNCTION__, Node->Key));
      DEBUG ((DEBUG_INFO, "%a: Smbios Override Value = %s\n", __FUNCTION__, Node->Value));
      *Value = Node->Value;
      Status = EFI_SUCCESS;
      goto Exit;
    }
    Node = Node->Next;
  }

Exit:
  return Status;
}

/**

  Prints out the current state of the Smbios Override Linked List to the debug log

**/
VOID
DumpListInfo (
  )
{
  UINTN                 Count;
  SMBIOS_OVERRIDE_NODE  *Node;

  Count = 0;
  Node = mSmbiosOverrideListHead;
  while (Node != NULL) {
    if ((Node->Key == NULL) || (Node->Value == NULL)) {
      DEBUG ((DEBUG_INFO, "%a: Node[%d] Key or Value is NULL. Skip \n", __FUNCTION__, Count));
    } else {
      DEBUG ((DEBUG_INFO, "%a: Node[%d] = (%s, %s)\n", __FUNCTION__, Count, Node->Key, Node->Value));
    }
    Count++;
    Node = Node->Next;
  }
}

/**

  Populates the Smbios Override Linked List.

  This function extracts every key-value pair from the input INI buffer
  and creates new nodes into the Smbios Override Linked List.

  @param[in] Buffer       Pointer to input Ascii characters that are in INI
                          format.
  @param[in] BufferSize   Size of the input buffer in bytes.

  @retval EFI_SUCCESS             Successfully finished populating the
                                  linked list.
  @retval EFI_OUT_OF_RESOURCES    Out of memory error.
  @retval others                  Error occurred when parsing the input INI
                                  buffer.

**/
EFI_STATUS
EFIAPI
PopulateSmbiosOverrideList (
  IN CHAR8    *Buffer,
  IN UINTN    BufferSize
  )
{
  CHAR16                *Key;
  SMBIOS_OVERRIDE_NODE  *NewNode;
  UINTN                 Offset;
  EFI_STATUS            Status;
  CHAR16                *Value;

  Key = NULL;
  Value = NULL;
  Offset = 0;
  while (TRUE) {
    Status = GetNextIniKeyValuePair (Buffer, &Offset, BufferSize, &Key, &Value);
    if (Status == EFI_NOT_FOUND) {
      // No new key/value pairs
      Status = EFI_SUCCESS;
      goto Exit;
    }
    if (EFI_ERROR (Status)) {
      goto Exit;
    }

    DEBUG ((DEBUG_INFO, "%a: Found new Key/Value pair \n", __FUNCTION__));
    NewNode = AllocatePool (sizeof (SMBIOS_OVERRIDE_NODE));
    if (NewNode == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit; 
    }

    NewNode->Key = Key;
    NewNode->Value = Value;
    NewNode->Next = mSmbiosOverrideListHead;
    mSmbiosOverrideListHead = NewNode;
  }

Exit:
  return Status;
}

/**

  Checks the Smbios Override linked list's signature and revision

  @param[out] OutRevision     Pointer to UINTN that contains the revision of
                              the config file

  @retval EFI_SUCCESS         Valid Smbios Override signature and revision
  @retval EFI_NOT_FOUND       Config file missing signature or revision fields

**/
EFI_STATUS
EFIAPI
ValidateSmbiosOverrideList (
  OUT UINTN *OutRevision
  )
{
  INTN        CompareResult;
  UINTN       Revision;
  EFI_STATUS  Status;
  CHAR16      *Value;

  Status = GetSmbiosOverride (IMX_VARIABLE_SMBIOS_SIGNATURE, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Smbios signature entry not found\n", __FUNCTION__));
    goto Exit;
  }

  CompareResult = StrnCmp (IMX_VARIABLE_SMBIOS_SIGNATURE_VALUE,
                           Value,
                           StrnSizeS (IMX_VARIABLE_SMBIOS_SIGNATURE_VALUE,
                                      MAX_VARIABLE_SIZE)
                          );
  if (CompareResult != 0) {
    DEBUG ((DEBUG_ERROR, "%a: Smbios signature incorrect\n", __FUNCTION__));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  Status = GetSmbiosOverride (IMX_VARIABLE_SMBIOS_REVISION, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Revision entry not found\n", __FUNCTION__));
    goto Exit;
  }

  Revision = StrDecimalToUintn (Value);
  if (Revision > IMX_VARIABLE_SMBIOS_MAX_REVISION) {
    DEBUG ((DEBUG_ERROR, "%a: Revision not supported\n", __FUNCTION__));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  *OutRevision = Revision;

Exit:
  return Status;
}

/**

  Open configuration file from disk and read the data

  This function locates the disk using the PcdSmbiosOverrideDevicePath and
  attempts to open the Smbios.ini file. If successful, the whole file is read into
  a buffer.

  The contents of the file are expected to be in INI format with ASCII characters and
  either LF or CRLF line endings.
  
  The function scans through the buffer and extracts out the Key/Value pairs from the
  INI file and adds those pairs to linked list (mSmbiosOverrideListHead) for quick
  retrieval by other functions.

  Once this is complete, the file can be closed.

  @param[out] Revision     Pointer to UINTN that contains the Revision value

  @retval EFI_SUCCESS             Override file found and data successfully retrieved
  @retval EFI_INVALID_PARAMETER   Pcd Device path was malformed.
  @retval Other                   Some error occurred when opening the file or parsing
                                  the INI contents.

**/
EFI_STATUS
EFIAPI
GetSmbiosOverrideData (
  OUT UINTN *Revision
  )
{
  VOID                              *Buffer;
  UINTN                             BufferSize;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath;
  CONST CHAR16                      *DevicePathText;
  EFI_FILE_HANDLE                   File;
  EFI_FILE_INFO                     *FileInfo;
  UINTN                             FileInfoSize;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   *Fs;
  EFI_DEVICE_PATH_PROTOCOL          *IntermediateDevicePath;
  EFI_HANDLE                        MediaHandle;
  EFI_FILE_HANDLE                   RootVolume;
  EFI_STATUS                        Status;
 
  File = NULL;
  RootVolume = NULL;
  Buffer = NULL;
  DevicePath = NULL;
  MediaHandle = NULL;
  Fs = NULL;

  //
  // Find Smbios.ini file starting with Pcd device path
  //
  DevicePathText = (CONST CHAR16 *) FixedPcdGetPtr (PcdSmbiosOverrideDevicePath);
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
  Status = gBS->LocateDevicePath (
    &gEfiSimpleFileSystemProtocolGuid,
    &IntermediateDevicePath,
    &MediaHandle);

  if (Status == EFI_NOT_FOUND) {
    DEBUG ((DEBUG_INFO,"%a: %s FAT partition is not ready yet\n", __FUNCTION__, DevicePathText));
    goto Exit;
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: gBS->LocateDevicePath() failed. (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  Status = gBS->HandleProtocol (
    MediaHandle,
    &gEfiSimpleFileSystemProtocolGuid,
    (VOID**)&Fs
    );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: gBS->HandleProtocol() failed. (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  Status = Fs->OpenVolume (Fs, &RootVolume);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fs->OpenVolume() failed. (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  Status = RootVolume->Open (
    RootVolume,
    &File,
    SMBIOS_CONFIG_FILE_NAME,
    EFI_FILE_MODE_READ,
    0);

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: RootVolume->Open() failed. (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  //
  // Read File Info
  // 
  FileInfoSize = sizeof (EFI_FILE_INFO) + 1024;
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

  if (FileInfo->FileSize > SMBIOS_CONFIG_MAX_FILE_SIZE) {
    Status = EFI_NOT_FOUND;
    DEBUG ((DEBUG_ERROR,
            "%a: File size is larger than expected (%d vs %d).\n",
            __FUNCTION__,
            FileInfo->FileSize,
            SMBIOS_CONFIG_MAX_FILE_SIZE));
    goto Exit;
  }

  BufferSize = (UINTN) FileInfo->FileSize + sizeof (CHAR16);
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

  //
  // Scan through the INI file and add its contents into the SmbiosOverride linked list
  //
  Status = ValidateIniFormat (Buffer, BufferSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: INI Format Validation failed (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  Status = PopulateSmbiosOverrideList (Buffer, BufferSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to populate override list (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  Status = ValidateSmbiosOverrideList (Revision);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Buffer Validation failed (Status=%r)\n", __FUNCTION__, Status));
  }

  DumpListInfo ();

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
    File->Close (File);
    File = NULL;
  }

  if (RootVolume != NULL) {
    RootVolume->Close (RootVolume);
    RootVolume = NULL;
  }

  if (DevicePath != NULL) {
    FreePool (DevicePath);
  }
  return Status;
}

/**

  Check whether the SmbiosOverridePresent variable is inside our NV store

  @retval EFI_SUCCESS     SmbiosOverridePresent variable present in NV store
  @retval EFI_NOT_READY   SmbiosOverridePresent variable does not exist in NV store

**/
EFI_STATUS
EFIAPI
CheckSmbiosOverridePresent (
  )
{
  UINTN       DataSize;
  EFI_STATUS  Status;

  DataSize = 0;
  Status = gRT->GetVariable ((CHAR16 *)SMBIOS_CONFIG_OVERRIDE_PRESENT,
                  &giMXPlatformSmbiosOverrideGuid,
                  NULL,
                  &DataSize,
                  NULL);

  if (Status == EFI_BUFFER_TOO_SMALL) {
    return EFI_SUCCESS;
  }

  return EFI_NOT_READY;
}

/**

  Store the SmbiosOverridePresent variable into NV store

  @retval EFI_SUCCESS   Variable successfully stored
  @retval Other         Some error occurred when attempting to store a variable

**/
EFI_STATUS
EFIAPI
SetSmbiosOverridePresent (
  )
{
  UINT8       Data;
  EFI_STATUS  Status;

  Data = 1;
  Status = gRT->SetVariable ((CHAR16 *)SMBIOS_CONFIG_OVERRIDE_PRESENT,
                  &giMXPlatformSmbiosOverrideGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS |
                  EFI_VARIABLE_RUNTIME_ACCESS,
                  1,
                  (VOID *)&Data);

  return Status;
}

/**

  Store a single Smbios Override variable into NV store

  @param[in]  Key     Unicode string of key to store into NV store
  @param[in]  Value   Unicode string of value to store into NV variable

  @retval EFI_SUCCESS   Override variable successfully stored
  @retval Other         Some error occurred when attempting to store a variable

**/
EFI_STATUS
EFIAPI
StoreSmbiosOverrideVariable (
  IN CONST CHAR16   *Key,
  IN CHAR16         *Value
  )
{
  EFI_STATUS Status;

  Status = gRT->SetVariable ((CHAR16 *)Key,
                &giMXPlatformSmbiosOverrideGuid,
                EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                StrnSizeS (Value, MAX_VARIABLE_SIZE),
                (VOID *) Value);

  return Status;
}

/**

  Store the Smbios Override variables into NV store based on Revision version

  @param[in]  Revision  Revision number to use to decide which override
                        variables to search for

  @retval EFI_SUCCESS   Override variables successfully stored into NV variables
  @retval Other         Some error occurred when attempting to store a variable

**/
EFI_STATUS
EFIAPI
StoreAllSmbiosOverrideVariables (
  IN UINTN Revision
  )
{
  INTN          i;
  CONST CHAR16  **OverrideArray;
  INTN          OverrideCount;
  EFI_STATUS    Status;
  CHAR16        *Value;

  switch (Revision) {
    case 1:
      OverrideCount = MAX_SMBIOS_KEY_REV_1;
      OverrideArray = mSmbiosKeyRev1;
    break;
    default:
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "%a: Revision not supported\n", __FUNCTION__));
      goto Exit;
    break;
  }

  for (i = 0; i < OverrideCount; i++) {
    Status = GetSmbiosOverride (OverrideArray[i], &Value);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a: Failed to find Smbios Key %s\n", __FUNCTION__, OverrideArray[i]));
      continue;
    }
    Status = StoreSmbiosOverrideVariable (OverrideArray[i], Value);
    if (EFI_ERROR (Status)) {
      goto Exit;
    }
  }

  Status = EFI_SUCCESS;

Exit:
  return Status;
}

/**

  Free nodes in the module's Smbios Override Linked List

**/
VOID
FreeSmbiosOverrideList (
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
  UINTN       SmbiosOverrideRevision;
  EFI_STATUS  Status;

  //
  // Check if SmbiosOverride is enabled in PCD
  //
  if (FixedPcdGet32 (PcdSmbiosOverrideEnable) == FALSE) {
    DEBUG ((DEBUG_INFO, "%a: PcdSmbiosOverride Not Enabled\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  //
  // Check if an Smbios override config was previously set
  //
  Status = CheckSmbiosOverridePresent ();
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "%a: Smbios Override Locked\n", __FUNCTION__));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "%a: Smbios Override Unlocked\n", __FUNCTION__));

  //
  // Lock down config no matter the result
  //
  Status = SetSmbiosOverridePresent ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to set SmbiosOverridePresent (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

  //
  // Read Smbios Override file from filesystem
  //
  Status = GetSmbiosOverrideData (&SmbiosOverrideRevision);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to read Smbios Override Data (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;  
  }

  //
  // Store override data in NV variable
  //
  Status = StoreAllSmbiosOverrideVariables (SmbiosOverrideRevision);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed store to NV (Status=%r)\n", __FUNCTION__, Status));
    goto Exit;
  }

Exit:
  FreeSmbiosOverrideList ();
  return Status;
}
