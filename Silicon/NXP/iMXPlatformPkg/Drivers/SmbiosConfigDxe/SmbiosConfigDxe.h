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
#ifndef _SMBIOS_CONFIG_DXE_H_
#define _SMBIOS_CONFIG_DXE_H_

#include <Guid/FileInfo.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Protocol/SimpleFileSystem.h>

#include <iMXVarStore.h>

#define SMBIOS_CONFIG_OVERRIDE_PRESENT  L"SmbiosOverridePresent"
#define SMBIOS_CONFIG_FILE_NAME         L"Smbios.ini"

// Expected Smbios override file size is <1 KB so limit max file sizes to 10 KB
#define SMBIOS_CONFIG_MAX_FILE_SIZE     1024 * 10

typedef enum IniParserState {
  StateReadKey,
  StateReadValue,
  StateReadWindowsNewLine,
  StateValueDone,
  StateError,
} INI_PARSER_STATE;

typedef struct SmbiosOverrideNode {
  CHAR16 *Key;
  CHAR16 *Value;
  struct SmbiosOverrideNode *Next;
} SMBIOS_OVERRIDE_NODE;

#endif  // _SMBIOS_CONFIG_DXE_H_
