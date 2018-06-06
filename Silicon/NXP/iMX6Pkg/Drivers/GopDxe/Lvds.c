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
#include "Lvds.h"
#include "Edid.h"

EFI_STATUS
InitLvds (
  IN  DISPLAY_CONTEXT   *DisplayContextPtr
  )
{
  DISPLAY_INTERFACE_CONTEXT   *pLvdsDisplayContext;
  EFI_STATUS                  Status;
  LDB_CTRL_REG                LdbCtrlReg;

  Status = EFI_SUCCESS;
  pLvdsDisplayContext = &DisplayContextPtr->DiContext[Lvds0Display];
  ZeroMem (pLvdsDisplayContext, sizeof (*pLvdsDisplayContext));

  pLvdsDisplayContext->MmioBasePtr = (VOID *)LDB_BASE;
  if (pLvdsDisplayContext->MmioBasePtr == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to map LDB register\n", __FUNCTION__));
    goto Exit;
  }

  // LVDS CH1 enabled, routed to DisplayInterface0; ipu_di01_vsync_active_low
  LdbCtrlReg.Reg = MmioRead32 ((UINT32)pLvdsDisplayContext->MmioBasePtr + LDB_CTRL);
  LdbCtrlReg.ch0_mode = 1;
  LdbCtrlReg.ch1_mode = 1;
  LdbCtrlReg.di0_vs_polarity = 1;
  LdbCtrlReg.di1_vs_polarity = 1;
  MmioWrite32 ((UINT32)pLvdsDisplayContext->MmioBasePtr + LDB_CTRL, LdbCtrlReg.Reg);

  // No EDID available
  pLvdsDisplayContext->EdidDataSize = 0;

  Status = GetPreferredTiming (
             pLvdsDisplayContext->EdidData,
             pLvdsDisplayContext->EdidDataSize,
             &pLvdsDisplayContext->PreferredTiming
           );
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to retrieve LVDS preferred timing\n",
      __FUNCTION__));
    goto Exit;
  }

Exit:
  return Status;
}

EFI_STATUS
SetLvdsPower (
  IN  DISPLAY_INTERFACE_CONTEXT   *HdmiDisplayContextPtr,
  IN  BOOLEAN                     PowerState
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
SetLvdsDisplay (
  IN  DISPLAY_INTERFACE_CONTEXT   *pLvdsDisplayContext,
  IN  IMX_DISPLAY_TIMING          *Timings
  )
{
  return EFI_UNSUPPORTED;
}
