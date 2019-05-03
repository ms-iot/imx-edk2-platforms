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

// Display muxing register
#include "iMX6.h"

#include "Display.h"
#include "Edid.h"
#include "Hdmi.h"
#include "Lvds.h"
#include "DisplayInterface.h"
#include "DisplayController.h"
#include "CPMem.h"
#include "Ipu.h"
#include "IoMux.h"

IMX_DISPLAY_TIMING DefaultTiming = {
  65000000,   // PixelClock
  1024,       // HActive
  320,        // HBlank
  768,        // VActive
  38,         // VBlank
  136,        // HSync
  6,          // VSync
  24,         // HSyncOffset;
  3,          // VSyncOffset;
  1024,       // HImageSize
  768,        // VImageSize
  0,          // HBorder
  0,          // VBorder
  0,          // EdidFlags
  0,          // Flags
  1,          // PixelRepetition
  32,          // Bpp
  PIXEL_FORMAT_BGRA32,       // PixelFormat
};

IMX_DISPLAY_TIMING Hannstar_XGA = {
  65000000,   // PixelClock
  1024,       // HActive
  320,        // HBlank
  768,        // VActive
  38,         // VBlank
  60,         // HSync
  10,         // VSync
  24,         // HSyncOffset;
  3,          // VSyncOffset;
  1024,       // HImageSize
  768,        // VImageSize
  0,          // HBorder
  0,          // VBorder
  0,          // EdidFlags
  0,          // Flags
  1,          // PixelRepetition
  32,         // Bpp
  PIXEL_FORMAT_BGRA32,       // PixelFormat
};

IMX_DISPLAY_TIMING DefaultX2LvdsPanel = {
  29500000,   // PixelClock
  800,        // HActive
  320,        // HBlank
  480,        // VActive
  38,         // VBlank
  32,         // HSync
  4,          // VSync
  100,        // HSyncOffset;
  10,         // VSyncOffset;
  800,        // HImageSize
  480,        // VImageSize
  0,          // HBorder
  0,          // VBorder
  0,          // EdidFlags
  0,          // Flags
  1,          // PixelRepetition
  32,          // Bpp
  PIXEL_FORMAT_BGRA32,       // PixelFormat
};

EFI_STATUS
GetPreferredTiming (
  IN  UINT8               *EdidDataPtr,
  IN  UINT32              EdidDataSize,
  IN  IMX_DISPLAY_TIMING  *PreferredTimingPtr
  )
{
  EFI_STATUS Status;

  if (FeaturePcdGet (PcdLvdsEnable)) {
    if (FeaturePcdGet (PcdUseDefaultX2LvdsPanel)) {
      *PreferredTimingPtr = DefaultX2LvdsPanel;
      Status = EFI_SUCCESS;
    } else {
      *PreferredTimingPtr = Hannstar_XGA;
      Status = EFI_SUCCESS;
    }
  } else {
    Status = GetEdidPreferredTiming (EdidDataPtr, EdidDataSize, PreferredTimingPtr);
    if (Status != EFI_SUCCESS) {
      // If EDID is unavailable use the default timing
      Status = EFI_SUCCESS;
      *PreferredTimingPtr = DefaultTiming;
      DEBUG ((DEBUG_WARN,
        "%a: EDID data not available, falling back to default timing\n",
        __FUNCTION__));
    }
  }

  // Only support 8 bit per pixel and no pixel repetition for now
  PreferredTimingPtr->PixelRepetition = 1;
  PreferredTimingPtr->PixelFormat = PIXEL_FORMAT_BGRA32;
  PreferredTimingPtr->Bpp = 32;

#ifdef DEBUG
  ImxPrintDisplayTiming ("Preferred Timing", PreferredTimingPtr);
#endif /* DEBUG */
  DEBUG ((DEBUG_WARN, "%a: --GetPreferredTiming\n", __FUNCTION__));
  return Status;
}

EFI_STATUS
InitDisplay (
  IN  DISPLAY_CONTEXT   **DisplayConfigPPtr
  )
{
  DISPLAY_CONTEXT     *pTempDisplayContext;
  DISPLAY_INTERFACE_TYPE   DisplayCounter;
  EFI_STATUS          Status;

  pTempDisplayContext = AllocateRuntimePool (sizeof (*pTempDisplayContext));
  if (pTempDisplayContext == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to allocate display context\n", __FUNCTION__));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  ZeroMem (pTempDisplayContext, sizeof (*pTempDisplayContext));

  pTempDisplayContext->IoMuxMmioBasePtr = (VOID *)IOMUXC_GPR_BASE_ADDRESS;
  if (pTempDisplayContext->IoMuxMmioBasePtr == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to map IO Mux register\n", __FUNCTION__));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  pTempDisplayContext->IpuMmioBasePtr[IPU1] = (VOID *)IPU1_BASE;
  if (pTempDisplayContext->IoMuxMmioBasePtr == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to map IPU1 IO Mux register\n", __FUNCTION__));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
#if !defined(CPU_IMX6S) && !defined(CPU_IMX6DL)
  pTempDisplayContext->IpuMmioBasePtr[IPU2] = (VOID *)IPU2_BASE;
  if (pTempDisplayContext->IoMuxMmioBasePtr == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to map IPU2 IO Mux register\n", __FUNCTION__));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
#endif
  for (DisplayCounter = HdmiDisplay; DisplayCounter < DisplayTypeMax; ++DisplayCounter) {
    pTempDisplayContext->DiContext[DisplayCounter].displayInterface =
      DisplayCounter;
  }

  if (FeaturePcdGet (PcdLvdsEnable)) {
    Status = InitLvds (pTempDisplayContext);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "%a: Fail to intialize LVDS\n", __FUNCTION__));
      goto Exit;
    }
  } else {
    Status = InitHdmi (pTempDisplayContext);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "%a: Fail to intialize HDMI\n", __FUNCTION__));
      goto Exit;
    }
  }

  *DisplayConfigPPtr = pTempDisplayContext;

Exit:
  return Status;
}

EFI_STATUS
ValidateDisplayConfig (
  IN  DISPLAY_CONTEXT     *DisplayContextPtr,
  IN  DISPLAY_MODE        DisplayMode,
  IN  DISPLAY_INTERFACE_TYPE   *DiOrder
  )
{
  DISPLAY_INTERFACE_TYPE   DisplayDevice;
  EFI_STATUS          Status;

  if (FeaturePcdGet (PcdLvdsEnable)) {
    DisplayDevice = Lvds0Display;
  } else {
    DisplayDevice = HdmiDisplay;
  }

  // Currently only support single display mode on HDMI/LVDS
  if (DisplayMode != SINGLE_MODE && DiOrder[0] != DisplayDevice) {
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  // Currently going to a very simplistic approach of enabling HDMI/LVDS single
  // display on HDMI/LVDS port. This configuration is applied regardless if
  // there is a monitor connected. No hot plug, monitor detection support.
  Status = EFI_SUCCESS;

Exit:
  return Status;
}

EFI_STATUS
SetDisplayConfig (
  IN OUT  DISPLAY_CONTEXT     *DisplayContextPtr,
  IN      DISPLAY_MODE        DisplayMode,
  IN      DISPLAY_INTERFACE_TYPE   *DiOrder
  )
{
  DISPLAY_INTERFACE_CONTEXT   *pDisplayInterfaceContext;
  DISPLAY_INTERFACE_INDEX     DisplayInterfaceIndex;
  UINT32                      DisplayInterfaceOffset[DisplayInterfaceMax];
  UINT32                      DisplayModeIndex;
  IPU_INDEX                   IpuIndex;
  EFI_STATUS                  Status;

  DisplayInterfaceOffset[0] = IPU_DISPLAY_INTERFACE_0_OFFSET;
  DisplayInterfaceOffset[1] = IPU_DISPLAY_INTERFACE_1_OFFSET;
  Status = ValidateDisplayConfig (DisplayContextPtr, DisplayMode, DiOrder);
  if (Status != EFI_SUCCESS) {
    DisplayContextPtr->DisplayConfig.DisplayMode = UNKNOWN_MODE;
    ZeroMem (
      DisplayContextPtr->DisplayConfig.DiOrder,
      sizeof (DisplayContextPtr->DisplayConfig.DiOrder)
    );
    DEBUG ((DEBUG_ERROR, "%a: Unsupported display configuration\n", __FUNCTION__));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DisplayContextPtr->DisplayConfig.DisplayMode = DisplayMode;
  ZeroMem (
    DisplayContextPtr->DisplayConfig.DiOrder,
    sizeof (DisplayContextPtr->DisplayConfig.DiOrder)
  );

  // Assigning display interface in order. Require mode information on IPU
  // and Display Interface valid combination
  DisplayModeIndex = 0;
  for (IpuIndex = IPU1; IpuIndex < IPU_TOTAL; IpuIndex++) {
    for (DisplayInterfaceIndex = DisplayInterface0;
         DisplayInterfaceIndex < DisplayInterfaceMax;
         DisplayInterfaceIndex++)
    {
      if (DisplayModeIndex >= (UINT32)DisplayMode) {
        break;
      }
      DisplayModeIndex++;

      DisplayContextPtr->DisplayConfig.DiOrder[DisplayInterfaceIndex] =
        DiOrder[DisplayInterfaceIndex];
      pDisplayInterfaceContext =
        &DisplayContextPtr->DiContext[DiOrder[DisplayInterfaceIndex]];
      pDisplayInterfaceContext->IpuMmioBasePtr =
        DisplayContextPtr->IpuMmioBasePtr[IpuIndex];
      pDisplayInterfaceContext->IpuDiRegsPtr =
        (IPU_DIx_REGS *)(((UINTN)DisplayContextPtr->IpuMmioBasePtr[IpuIndex]) +
                                 DisplayInterfaceOffset[DisplayInterfaceIndex]);
      pDisplayInterfaceContext->CpMemParamBasePtr =
        (VOID *)(((UINTN)pDisplayInterfaceContext->IpuMmioBasePtr) +
                         CSP_IPUV3_CPMEM_REGS_OFFSET);
    }
  }

  Status = EFI_SUCCESS;

Exit:
  return Status;
}

EFI_STATUS
ApplyDisplayConfig (
  IN OUT  DISPLAY_CONTEXT     *DisplayContextPtr,
  IN      DISPLAY_MODE        DisplayMode,
  IN      DISPLAY_INTERFACE_TYPE   *DiOrder
  )
{
  IMX_DISPLAY_TIMING          *pCurrentDisplayTiming;
  DISPLAY_CONFIG              *pDisplayConfig;
  DISPLAY_INTERFACE_CONTEXT   *pDisplayInterfaceContext;
  UINT32                      CurrentDisplayInterface;
  UINT32                      DisplayModeIndex;
  EFI_STATUS                  Status;

  Status = EFI_SUCCESS;
  pDisplayConfig = &DisplayContextPtr->DisplayConfig;
  Status = SetDisplayConfig (DisplayContextPtr, DisplayMode, DiOrder);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: Fail to set display configuration %d\n",
      __FUNCTION__, DisplayMode));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  // Setup muxing first before configuring DI and DC
  Status = SetupDisplayMux (DisplayContextPtr);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: SetDisplayMux failed \n", __FUNCTION__));
    goto Exit;
  }

  for (DisplayModeIndex = 0;
       DisplayModeIndex < (UINT32)pDisplayConfig->DisplayMode;
       ++DisplayModeIndex)
  {
    CurrentDisplayInterface = pDisplayConfig->DiOrder[DisplayModeIndex];
    pDisplayInterfaceContext = &DisplayContextPtr->DiContext[CurrentDisplayInterface];
    pCurrentDisplayTiming = &pDisplayConfig->DisplayTiming[DisplayModeIndex];

    Status = ConfigureDisplayControllerChannel (
               pDisplayInterfaceContext,
               CurrentDisplayInterface,
               DisplayModeIndex,
               pCurrentDisplayTiming
             );
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR,
        "%a:ConfigureDisplayControllerChannel fail display %d index %d\n",
        __FUNCTION__, CurrentDisplayInterface, DisplayModeIndex));
      goto Exit;
    }

    Status = ConfigureDisplayInterface (
               pDisplayInterfaceContext,
               DisplayModeIndex,
               pCurrentDisplayTiming
             );
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "%a: Fail to configure DI\n", __FUNCTION__));
      goto Exit;
    }

    switch (CurrentDisplayInterface) {
    case HdmiDisplay:
      Status = SetHdmiDisplay (pDisplayInterfaceContext, pCurrentDisplayTiming);
      if (Status != EFI_SUCCESS) {
        DEBUG ((DEBUG_ERROR, "%a: Fail to set HDMI timing\n", __FUNCTION__));
        goto Exit;
      }
      break;
    case Lvds0Display:
    case Lvds1Display:
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
    }
    if (EFI_ERROR (Status)) {
      goto Exit;
    }

    Status = ConfigureFrameBuffer (
               pDisplayInterfaceContext,
               &pDisplayConfig->DisplaySurface[DisplayModeIndex]
             );
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR,
        "%a: Fail to configure frame buffer (%d)\n",
        __FUNCTION__, DisplayModeIndex));
      goto Exit;
    }
  }

Exit:
  return Status;
}

EFI_STATUS
AllocateFrameBuffer (
  IN OUT  SURFACE_INFO  *SurfaceInfoPtr
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "%a: Enter\n", __FUNCTION__));
  if ((SurfaceInfoPtr->Width == 0) || (SurfaceInfoPtr->Height == 0)) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "%a: Frame Buffer AddrP=%Xh\n",
    __FUNCTION__, FixedPcdGet32 (PcdFrameBufferBase)));
  DEBUG ((DEBUG_INFO, "%a: Frame Buffer Size=%Xh\n",
    __FUNCTION__, FixedPcdGet32 (PcdFrameBufferSize)));

  SurfaceInfoPtr->VirtAddrPtr = (VOID *)(UINTN)FixedPcdGet32 (PcdFrameBufferBase);
  SurfaceInfoPtr->PhyAddr = (UINT32)(SurfaceInfoPtr->VirtAddrPtr);
  SurfaceInfoPtr->Pitch = SurfaceInfoPtr->Width;

  DEBUG ((DEBUG_INFO,
    "%a: Allocate FB PhyAddr %x VirtAddr %x\n",
    __FUNCTION__, SurfaceInfoPtr->PhyAddr, SurfaceInfoPtr->VirtAddrPtr));

  Status = EFI_SUCCESS;

Exit:
  DEBUG ((DEBUG_INFO, "%a: Exit = %Xh\n",
    __FUNCTION__, Status));
  return Status;
}

EFI_STATUS
ConfigureFrameBuffer (
  IN  DISPLAY_INTERFACE_CONTEXT   *DisplayInterfaceContextPtr,
  IN  SURFACE_INFO                *FrameBufferPtr
  )
{
  EFI_STATUS  Status;

  // Only support single display for now
  Status = ConfigureCpmemFrameBuffer (
             DisplayInterfaceContextPtr,
             IDMAC_CHANNEL_DP_PRIMARY_FLOW_MAIN_PLANE,
             FrameBufferPtr
           );
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to configure CPMEM\n", __FUNCTION__));
    goto Exit;
  }

Exit:
  return Status;
}

UINT32
GetColorDepth (
  IN  IMX_PIXEL_FORMAT  PixelFormat
  )
{
  UINT32  BitDepth;

  switch (PixelFormat) {
  case PIXEL_FORMAT_ARGB32:
  case PIXEL_FORMAT_BGRA32:
    BitDepth = 8;
    break;
  default:
    BitDepth = 0;
    break;
  }

  return BitDepth;
}
