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
#include "IoMux.h"

EFI_STATUS
SetupDisplayMux (
  IN  DISPLAY_CONTEXT   *DisplayContextPtr
  )
{
  DISPLAY_INTERFACE_TYPE              *pDisplayInterfaceType;
  volatile IMX_IOMUXC_GPR_REGISTERS   *pIomuxcGprReg;
  UINT32                              DisplayInterfaceIndex;
  DISPLAY_MODE                        DisplayMode;
  UINT32                              Gpr3Reg;
  UINT32                              SourceMask;
  UINT32                              SourceValue;
  EFI_STATUS                          Status;

  pIomuxcGprReg = DisplayContextPtr->IoMuxMmioBasePtr;
  DisplayMode = DisplayContextPtr->DisplayConfig.DisplayMode;
  pDisplayInterfaceType = DisplayContextPtr->DisplayConfig.DiOrder;
  Status = EFI_SUCCESS;

  Gpr3Reg = MmioRead32 ((UINT32)&pIomuxcGprReg->GPR3);
  Gpr3Reg &= ~(HDMI_MUX_CTL_MASK | MIPI_MUX_CTL_MASK |
               LVDS0_MUX_CTL_MASK | LVDS1_MUX_CTL_MASK);
  MmioWrite32 ((UINT32)&pIomuxcGprReg->GPR3, Gpr3Reg);

  for (DisplayInterfaceIndex = 0; DisplayInterfaceIndex < (UINT32)DisplayMode; ++DisplayInterfaceIndex) {
    Gpr3Reg = MmioRead32 ((UINT32)&pIomuxcGprReg->GPR3);
    switch (pDisplayInterfaceType[DisplayInterfaceIndex]) {
    case HdmiDisplay:
      SourceMask = HDMI_MUX_CTL_MASK;
      SourceValue = DisplayInterfaceIndex << HDMI_MUX_CTL_OFFSET;
      break;
    case MipiDisplay:
      SourceMask = MIPI_MUX_CTL_MASK;
      SourceValue = DisplayInterfaceIndex << MIPI_MUX_CTL_OFFSET;
      break;
    case Lvds0Display:
      SourceMask = LVDS0_MUX_CTL_MASK;
      SourceValue = DisplayInterfaceIndex << LVDS0_MUX_CTL_OFFSET;
      break;
    case Lvds1Display:
      SourceMask = LVDS1_MUX_CTL_MASK;
      SourceValue = DisplayInterfaceIndex << LVDS1_MUX_CTL_OFFSET;
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
    }
    if (EFI_ERROR (Status)) {
      break;
    }

    Gpr3Reg &= ~SourceMask;
    Gpr3Reg |= SourceValue;
    MmioWrite32 ((UINT32)&pIomuxcGprReg->GPR3, Gpr3Reg);
  }

  return Status;
}
