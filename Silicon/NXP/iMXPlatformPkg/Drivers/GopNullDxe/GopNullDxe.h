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

#ifndef _GOP_NULL_DXE_H_
#define _GOP_NULL_DXE_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/DmaLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/GraphicsOutput.h>

#define PIXEL_BYTES 4

typedef struct {
  VENDOR_DEVICE_PATH Mmc;
  EFI_DEVICE_PATH End;
} VID_DEVICE_PATH;

typedef struct {
  UINT32 HActive;
  UINT32 VActive;
  UINT32 Bpp;
} DISPLAY_TIMING;

#endif  /* _GOP_NULL_DXE_H_ */
