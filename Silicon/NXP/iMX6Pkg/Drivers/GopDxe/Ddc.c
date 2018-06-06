/** @file
*
*  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
*  Copyright 2018 NXP
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

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <iMX6.h>
#include <iMX6ClkPwr.h>
#include <iMXDisplay.h>

#include "Display.h"
#include "Edid.h"
#include "Ddc.h"
#include "Hdmi.h"
#include "Lvds.h"

EFI_STATUS
Imx6DdcRead (
  IN  DISPLAY_CONTEXT     *DisplayContextPtr,
  IN  DISPLAY_INTERFACE_TYPE   DisplayInterface,
  IN  UINT8               SlaveAddress,
  IN  UINT8               RegisterAddress,
  IN  UINT32              ReadSize,
  IN  UINT8               *DataReadPtr
  )
{
  EFI_STATUS Status;

  switch (DisplayInterface) {
  case HdmiDisplay:
    Status = HdmiDdcRead (
               &DisplayContextPtr->DiContext[HdmiDisplay],
               SlaveAddress,
               RegisterAddress,
               ReadSize,
               HDMI_DDC_STANDARD_MODE,
               DataReadPtr
             );
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "%a: HdmiDdcRead failed\n", __FUNCTION__));
    }
    break;
  case MipiDisplay:
  case Lvds0Display:
  case Lvds1Display:
  default:
    Status = EFI_UNSUPPORTED;
    break;
  }

  return Status;
}
