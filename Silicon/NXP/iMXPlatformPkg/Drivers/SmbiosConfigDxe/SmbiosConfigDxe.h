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

typedef enum IniParserState {
  StateReadKey,
  StateReadValue,
  StateReadWindowsNewLine,
  StateValueDone,
  StateError,
} INI_PARSER_STATE;

typedef struct SmbiosOverrideNode {
  CHAR8 *Key;
  CHAR8 *Value;
  struct SmbiosOverrideNode *Next;
} SMBIOS_OVERRIDE_NODE;

#endif  // _SMBIOS_CONFIG_DXE_H_
