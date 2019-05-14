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

#ifndef _VIRTUAL_GOP_DXE_H_
#define _VIRTUAL_GOP_DXE_H_

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DmaLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/GraphicsOutput.h>

#include <iMXDisplay.h>

typedef struct {
  VENDOR_DEVICE_PATH Mmc;
  EFI_DEVICE_PATH End;
} VID_DEVICE_PATH;

typedef struct {
  UINT32 HActive;
  UINT32 VActive;
} DISPLAY_TIMING;

#endif  /* _VIRTUAL_GOP_DXE_H_ */

