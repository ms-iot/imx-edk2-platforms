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

#include <iMXDisplay.h>

// Enums
typedef enum {
  UNKNOWN_MODE,
  SINGLE_MODE,
  DUAL_MODE,
} DISPLAY_MODE;

typedef enum {
  NullDisplayType, 
  DisplayTypeMax,
} DISPLAY_INTERFACE_TYPE;

// Structs
typedef struct _SURFACE_INFO {
  UINT32 PhyAddr;
  UINT32 *VirtAddrPtr;
  UINT32 Width;
  UINT32 Height;
  UINT32 Pitch;
  UINT32 Bpp;
  IMX_PIXEL_FORMAT PixelFormat;
} SURFACE_INFO, *PSURFACE_INFO;

typedef struct _DISPLAY_INTERFACE_CONTEXT {
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
  DISPLAY_INTERFACE_CONTEXT DiContext[DisplayTypeMax];
} DISPLAY_CONTEXT, *PDISPLAY_CONTEXT;

#endif  /* _GOP_NULL_DXE_H_ */

