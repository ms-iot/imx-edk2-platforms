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

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

typedef enum {
  UNKNOWN_MODE,
  SINGLE_MODE,
  DUAL_MODE,
} DISPLAY_MODE;

typedef enum {
  IPU1,
#if !defined(CPU_IMX6S) && !defined(CPU_IMX6DL)
  IPU2,
#endif
  IPU_TOTAL,
} IPU_INDEX;

typedef enum {
  DisplayInterface0,
  DisplayInterface1,
  DisplayInterfaceMax,
} DISPLAY_INTERFACE_INDEX;

typedef enum {
  HdmiDisplay,
  MipiDisplay,
  Lvds0Display,
  Lvds1Display,
  DisplayTypeMax, // Only 4 display types supported by IPU
  NoDisplayType = DisplayTypeMax,
} DISPLAY_INTERFACE_TYPE;

typedef struct _SURFACE_INFO {
  UINT32 PhyAddr;
  UINT32 *VirtAddrPtr;
  UINT32 Width;
  UINT32 Height;
  UINT32 Pitch;
  UINT32 Bpp;
  IMX_PIXEL_FORMAT PixelFormat;
} SURFACE_INFO, *PSURFACE_INFO;

typedef struct _IPU_DIx_REGS {
  UINT32 DIxGENERAL;
  UINT32 DIxBS_CLKGEN0;
  UINT32 DIxBS_CLKGEN1;
  UINT32 DIxSW_GEN0_1;
  UINT32 DIxSW_GEN0_2;
  UINT32 DIxSW_GEN0_3;
  UINT32 DIxSW_GEN0_4;
  UINT32 DIxSW_GEN0_5;
  UINT32 DIxSW_GEN0_6;
  UINT32 DIxSW_GEN0_7;
  UINT32 DIxSW_GEN0_8;
  UINT32 DIxSW_GEN0_9;
  UINT32 DIxSW_GEN1_1;
  UINT32 DIxSW_GEN1_2;
  UINT32 DIxSW_GEN1_3;
  UINT32 DIxSW_GEN1_4;
  UINT32 DIxSW_GEN1_5;
  UINT32 DIxSW_GEN1_6;
  UINT32 DIxSW_GEN1_7;
  UINT32 DIxSW_GEN1_8;
  UINT32 DIxSW_GEN1_9;
  UINT32 DIxSYNC_AS_GEN;
  UINT32 DIxDW_GEN[12];
  UINT32 DIxDW_SET0[12];
  UINT32 DIxDW_SET1[12];
  UINT32 DIxDW_SET2[12];
  UINT32 DIxDW_SET3[12];
  UINT32 DIxSTP_REP[4];
  UINT32 DIxSTP_REP_9;
  UINT32 DIxSER_CONF;
  UINT32 DIxSSC;
  UINT32 DIxPOL;
  UINT32 DIxAW0;
  UINT32 DIxAW1;
  UINT32 DIxSCR_CONF;
  UINT32 DIxSTAT;
} IPU_DIx_REGS, *PIPU_DIx_REGS;

typedef struct _DISPLAY_INTERFACE_CONTEXT {
  DISPLAY_INTERFACE_TYPE displayInterface;

  VOID *MmioBasePtr;
  VOID *IpuMmioBasePtr;
  VOID *CpMemParamBasePtr;
  IPU_DIx_REGS *IpuDiRegsPtr;
  UINT32 EdidDataSize;
  UINT8 EdidData[256];
  IMX_DISPLAY_TIMING PreferredTiming;
} DISPLAY_INTERFACE_CONTEXT, *PDISPLAY_INTERFACE_CONTEXT;

typedef struct _DISPLAY_CONFIG {
  DISPLAY_MODE DisplayMode;
  DISPLAY_INTERFACE_TYPE DiOrder[DisplayTypeMax];
  SURFACE_INFO DisplaySurface[DisplayTypeMax];
  IMX_DISPLAY_TIMING DisplayTiming[DisplayTypeMax];
  UINT32 OsHandle[DisplayTypeMax];
} DISPLAY_CONFIG, *PDISPLAY_CONFIG;

typedef struct _DISPLAY_CONTEXT {
  DISPLAY_CONFIG DisplayConfig;
  VOID *IoMuxMmioBasePtr;
  VOID *IpuMmioBasePtr[IPU_TOTAL];
  DISPLAY_INTERFACE_CONTEXT DiContext[DisplayTypeMax];
} DISPLAY_CONTEXT, *PDISPLAY_CONTEXT;

extern IMX_DISPLAY_TIMING DefaultTiming;

EFI_STATUS
GetPreferredTiming (
  IN  UINT8               *EdidDataPtr,
  IN  UINT32              EdidDataSize,
  IN  IMX_DISPLAY_TIMING  *PreferredTimingPtr
  );

EFI_STATUS
InitDisplay (
  IN  DISPLAY_CONTEXT   **DisplayConfigPPtr
  );

EFI_STATUS
ValidateDisplayConfig (
  IN  DISPLAY_CONTEXT     *DisplayContextPtr,
  IN  DISPLAY_MODE        DisplayMode,
  IN  DISPLAY_INTERFACE_TYPE   *DiOrder
  );

EFI_STATUS
SetDisplayConfig (
  IN OUT  DISPLAY_CONTEXT     *DisplayContextPtr,
  IN      DISPLAY_MODE        DisplayMode,
  IN      DISPLAY_INTERFACE_TYPE   *DiOrder
  );

EFI_STATUS
ApplyDisplayConfig (
  IN OUT  DISPLAY_CONTEXT     *DisplayContextPtr,
  IN      DISPLAY_MODE        DisplayMode,
  IN      DISPLAY_INTERFACE_TYPE   *DiOrder
  );

EFI_STATUS
AllocateFrameBuffer (
  IN OUT  SURFACE_INFO  *SurfaceInfoPtr
  );

EFI_STATUS
ConfigureFrameBuffer (
  IN  DISPLAY_INTERFACE_CONTEXT   *DisplayInterfaceContextPtr,
  IN  SURFACE_INFO                *FrameBufferPtr
  );

UINT32
GetColorDepth (
  IN  IMX_PIXEL_FORMAT  PixelFormat
  );

#endif  /* _DISPLAY_H_ */
