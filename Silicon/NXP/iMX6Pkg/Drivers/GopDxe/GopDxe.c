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
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EmbeddedExternalDevice.h>
#include <Protocol/BlockIo.h>
#include <Protocol/Cpu.h>
#include <Protocol/DevicePath.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/EdidDiscovered.h>
#include <Protocol/EdidActive.h>

#include <iMX6.h>
#include <iMX6ClkPwr.h>
#include <iMXDisplay.h>

#include "Display.h"
#include "GopDxe.h"
#include "Hdmi.h"
#include "Lvds.h"

#define PIXEL_BYTES 4

typedef struct {
  VENDOR_DEVICE_PATH Mmc;
  EFI_DEVICE_PATH End;
} VID_DEVICE_PATH;

IMX_DISPLAY_TIMING CONST FullHDTiming = {
  148500000,  // Full 1080p HD PixelClock
  1920,       // HActive
  280,        // HBlank
  1080,       // VActive
  45,         // VBlank
  44,         // HSync
  5,          // VSync
  88,         // HSyncOffset;
  4,          // VSyncOffset;
  1920,       // HImageSize
  1080,       // VImageSize
  0,          // HBorder
  0,          // VBorder
  30,         // EdidFlags
  0,          // Flags
  1,          // PixelRepetition
  32,         // Bpp
  PIXEL_FORMAT_BGRA32,       // PixelFormat
};

EFI_STATUS
EFIAPI
VidGopQueryMode (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL           *This,
  IN UINT32                                 ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  **Info
  );

EFI_STATUS
VidGopSetMode (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL   *This,
  IN UINT32                         ModeNumber
  );

EFI_STATUS
VidGopBlt (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL       *This,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *BltBuffer,
  IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION  BltOperation,
  IN UINTN                              SourceX,
  IN UINTN                              SourceY,
  IN UINTN                              DestinationX,
  IN UINTN                              DestinationY,
  IN UINTN                              Width,
  IN UINTN                              Height,
  IN UINTN                              Delta
  );

STATIC VID_DEVICE_PATH VidDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)sizeof (VENDOR_DEVICE_PATH),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8),
      }
    },
    {
      0xa6b94ebe,
      0x5ba3,
      0x44b0,
      { 0x95, 0x92, 0xdc, 0x04, 0x5e, 0xb8, 0xf8, 0x9e }
    }
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      sizeof (EFI_DEVICE_PATH_PROTOCOL),
      0
    }
  }
};

STATIC EFI_GRAPHICS_OUTPUT_MODE_INFORMATION VidGopModeInfo;
STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE VidGopMode;

STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL VidGop = {
  VidGopQueryMode, // QueryMode
  VidGopSetMode,   // SetMode
  VidGopBlt,       // Blt
  &VidGopMode    // Mode
};

EFI_EDID_DISCOVERED_PROTOCOL EdidDiscovered = {
  0,
  NULL
};

EFI_EDID_ACTIVE_PROTOCOL EdidActive = {
  0,
  NULL
};

DISPLAY_CONTEXT *DisplayContextPtr;

DISPLAY_INTERFACE_TYPE DisplayDevice;

EFI_STATUS
GopDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  DISPLAY_INTERFACE_TYPE  DisplayInterfaceOrder[DisplayTypeMax];
  UINT32                  i;
  UINT32                  RequestedDisplayMemorySize;
  UINT32                  ReservedDisplayMemorySize;
  EFI_STATUS              Status;

  DEBUG ((DEBUG_INFO, "%a: Enter \n", __FUNCTION__));

  for (i = 0; i < DisplayTypeMax; i++) {
    DisplayInterfaceOrder[i] = NoDisplayType;
  }
  ReservedDisplayMemorySize = FixedPcdGet32 (PcdFrameBufferSize);
  if (FeaturePcdGet (PcdLvdsEnable)) {
    DisplayDevice = Lvds0Display;
  } else {
    DisplayDevice = HdmiDisplay;
  }

  Status = InitDisplay (&DisplayContextPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Fail to init display, Status=%r\n", __FUNCTION__,
      Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "%a: - Allocate frame buffer\n", __FUNCTION__));
  // To allocate frame buffer dynamically, there isn`t a built in graphic memory
  // manager for UEFI, so we are allocating frame buffer manually. Currently only
  // support single display, so allocate single(1) frame buffer
  // Allocate frame buffer
  DisplayContextPtr->DisplayConfig.DisplaySurface[0].Width =
    DisplayContextPtr->DiContext[DisplayDevice].PreferredTiming.HActive;
  DisplayContextPtr->DisplayConfig.DisplaySurface[0].Height =
    DisplayContextPtr->DiContext[DisplayDevice].PreferredTiming.VActive;
  DisplayContextPtr->DisplayConfig.DisplaySurface[0].Bpp =
    DisplayContextPtr->DiContext[DisplayDevice].PreferredTiming.Bpp;
  DisplayContextPtr->DisplayConfig.DisplaySurface[0].PixelFormat =
    DisplayContextPtr->DiContext[DisplayDevice].PreferredTiming.PixelFormat;

  // iMX6 UEFI reserves display memory for fullHD buffer size.
  // PcdFrameBufferSize=800000h or 8388608 bytes - 1920x1080x4 bytes
  // to prevent larger displays overrun our reserved memory size,
  // cap display resolution to fullHD
  // NOTE: Displays which do not have support for 1920x1080 mode may
  // have poor or missing picture
  RequestedDisplayMemorySize =
    DisplayContextPtr->DisplayConfig.DisplaySurface[0].Width *
    DisplayContextPtr->DisplayConfig.DisplaySurface[0].Height *
    (DisplayContextPtr->DisplayConfig.DisplaySurface[0].Bpp / 8);

  DEBUG ((DEBUG_INFO, "%a: Display Memory: Needed=%d, Available=%d\n",
    __FUNCTION__, RequestedDisplayMemorySize, ReservedDisplayMemorySize));

  if (RequestedDisplayMemorySize > ReservedDisplayMemorySize) {
    DEBUG ((DEBUG_INFO,
      "%a: WARNING. Need more video memory than reserved by %d bytes\n",
      __FUNCTION__, RequestedDisplayMemorySize - ReservedDisplayMemorySize));
    DEBUG ((DEBUG_ERROR,
      "%a: - display resolution too big. Cap to HD 1080p\n",
      __FUNCTION__));
    DisplayContextPtr->DisplayConfig.DisplaySurface[0].Width = FullHDTiming.HActive;
    DisplayContextPtr->DisplayConfig.DisplaySurface[0].Height =
      FullHDTiming.VActive;
    DisplayContextPtr->DisplayConfig.DisplaySurface[0].Bpp = FullHDTiming.Bpp;
    CopyMem (
      &DisplayContextPtr->DiContext[DisplayDevice].PreferredTiming,
      &FullHDTiming,
      sizeof (IMX_DISPLAY_TIMING)
    );
  }

  DEBUG ((DEBUG_INFO, "%a: - allocating frame buffer... \n",
    __FUNCTION__));
  Status = AllocateFrameBuffer (&DisplayContextPtr->DisplayConfig.DisplaySurface[0]);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to allocate fb, Status=%r\n",
      __FUNCTION__, Status));
    goto Exit;
  };

  DEBUG ((DEBUG_INFO, "%a: - Initialize the frame buffer to black\n",
    __FUNCTION__));
  // Initialize the frame buffer to black
  SetMem32 (
    (VOID *)DisplayContextPtr->DisplayConfig.DisplaySurface[0].PhyAddr,
    DisplayContextPtr->DisplayConfig.DisplaySurface[0].Width *
    DisplayContextPtr->DisplayConfig.DisplaySurface[0].Height * 4,
    0xFF000000
  );

  DEBUG ((DEBUG_INFO, "%a: - set display configuration to single HDMI\n",
    __FUNCTION__));
  // Set the display configuration to single HDMI/LVDS mode
  DisplayInterfaceOrder[0] = DisplayDevice;
  DisplayContextPtr->DisplayConfig.DisplayTiming[0] =
    DisplayContextPtr->DiContext[DisplayDevice].PreferredTiming;

  Status = ApplyDisplayConfig (
              DisplayContextPtr,
              SINGLE_MODE,
              DisplayInterfaceOrder
            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to set display. Exit Status=%r\n",
      __FUNCTION__, Status));
    goto Exit;
  }

  VidGopModeInfo.Version = 0;
  VidGopModeInfo.HorizontalResolution =
    DisplayContextPtr->DisplayConfig.DisplayTiming[0].HActive;
  VidGopModeInfo.VerticalResolution =
    DisplayContextPtr->DisplayConfig.DisplayTiming[0].VActive;
  VidGopModeInfo.PixelFormat = PixelBlueGreenRedReserved8BitPerColor;
  ZeroMem (
    &VidGopModeInfo.PixelInformation,
    sizeof (VidGopModeInfo.PixelInformation)
  );

  VidGopModeInfo.PixelsPerScanLine = VidGopModeInfo.HorizontalResolution;
  VidGopMode.MaxMode = 1;
  VidGopMode.Mode = 0;
  VidGopMode.Info = &VidGopModeInfo;
  VidGopMode.SizeOfInfo = sizeof (VidGopModeInfo);
  VidGopMode.FrameBufferBase =
    (EFI_PHYSICAL_ADDRESS) DisplayContextPtr->DisplayConfig.DisplaySurface[0].PhyAddr;
  VidGopMode.FrameBufferSize =
    VidGopModeInfo.HorizontalResolution *
    VidGopModeInfo.VerticalResolution *
    (DisplayContextPtr->DisplayConfig.DisplaySurface[0].Bpp / 8);
  DisplayContextPtr->DisplayConfig.OsHandle[0] = (UINT32)&ImageHandle;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  &VidGop,
                  &gEfiDevicePathProtocolGuid,
                  &VidDevicePath,
                  NULL
                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to install protocol, Status=%x\n",
      __FUNCTION__, Status));
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "%a: Exit = %Xh\n",
    __FUNCTION__, Status));
  return Status;
}

EFI_STATUS
EFIAPI
VidGopQueryMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
  IN  UINT32 ModeNumber,
  OUT UINTN *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info
  )
{
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *OutputMode;
  EFI_STATUS                            Status;

  if (FeaturePcdGet (PcdLvdsEnable)) {
    DisplayDevice = Lvds0Display;
  } else {
    DisplayDevice = HdmiDisplay;
  }

  EdidDiscovered.SizeOfEdid = DisplayContextPtr->DiContext[DisplayDevice].EdidDataSize;
  EdidDiscovered.Edid = DisplayContextPtr->DiContext[DisplayDevice].EdidData;
  EdidActive.SizeOfEdid = DisplayContextPtr->DiContext[DisplayDevice].EdidDataSize;
  EdidActive.Edid = DisplayContextPtr->DiContext[DisplayDevice].EdidData;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  (EFI_HANDLE)DisplayContextPtr->DisplayConfig.OsHandle[0],
                  &gEfiEdidDiscoveredProtocolGuid,
                  &EdidDiscovered,
                  &gEfiEdidActiveProtocolGuid,
                  &EdidActive,
                  NULL
                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to install EDID protocols Status=%r\n",
      __FUNCTION__, Status));
  }

  if (ModeNumber != 0) {
    DEBUG ((DEBUG_ERROR, "%a: Saw request to query mode %d\n",
      __FUNCTION__, ModeNumber));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  OutputMode = (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *)
    AllocatePool (sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
  if (OutputMode == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  OutputMode->Version = 0;
  OutputMode->HorizontalResolution = VidGopModeInfo.HorizontalResolution;
  OutputMode->VerticalResolution = VidGopModeInfo.VerticalResolution;
  OutputMode->PixelFormat = PixelBlueGreenRedReserved8BitPerColor;
  OutputMode->PixelsPerScanLine = VidGopModeInfo.HorizontalResolution;
  *SizeOfInfo = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  *Info = OutputMode;

  Status = EFI_SUCCESS;

Exit:
  return Status;
}

EFI_STATUS
VidGopSetMode (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL   *This,
  IN UINT32                         ModeNumber
  )
{
  EFI_STATUS  Status;

  if (ModeNumber != 0) {
    DEBUG ((DEBUG_ERROR, "%a: Saw request to set mode to %d\n",
      __FUNCTION__, ModeNumber));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = EFI_SUCCESS;

Exit:
  return Status;
}

EFI_STATUS
VidGopBlt (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL       *This,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *BltBuffer, OPTIONAL
  IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION  BltOperation,
  IN UINTN                              SourceX,
  IN UINTN                              SourceY,
  IN UINTN                              DestinationX,
  IN UINTN                              DestinationY,
  IN UINTN                              Width,
  IN UINTN                              Height,
  IN UINTN                              Delta OPTIONAL
  )
{
  UINT32  *FrameBuffer;
  UINT32  BufferOffset;
  UINT32  BufferWidth;
  UINT32  FrameOffset;
  UINT32  FrameWidth;
  UINT32  i;

  FrameBuffer = (UINT32 *)((UINTN)VidGopMode.FrameBufferBase);
  FrameWidth = VidGopModeInfo.HorizontalResolution;
  if (Delta == 0) {
    BufferWidth = Width;
  } else {
    BufferWidth = Delta / PIXEL_BYTES;
  }

  if (BltOperation == EfiBltVideoFill) {
    FrameOffset = FrameWidth * DestinationY + DestinationX;
    for (i = DestinationY; i < DestinationY + Height; i++) {
      SetMem32 (
        FrameBuffer + FrameOffset,
        Width * PIXEL_BYTES,
        *(UINT32 *)BltBuffer
      );
      FrameOffset += FrameWidth;
    }
  } else if (BltOperation == EfiBltVideoToBltBuffer) {
    FrameOffset = FrameWidth * SourceY + SourceX;
    BufferOffset = BufferWidth * DestinationY + DestinationX;
    for (i = SourceY; i < SourceY + Height; i++) {
      CopyMem (
        BltBuffer + BufferOffset,
        FrameBuffer + FrameOffset,
        Width * PIXEL_BYTES
      );
      FrameOffset += FrameWidth;
      BufferOffset += BufferWidth;
    }
  } else if (BltOperation == EfiBltBufferToVideo) {
    FrameOffset = FrameWidth * DestinationY + DestinationX;
    BufferOffset = BufferWidth * SourceY + SourceX;
    for (i = SourceY; i < SourceY + Height; i++) {
      CopyMem (
        FrameBuffer + FrameOffset,
        BltBuffer + BufferOffset,
        Width * PIXEL_BYTES
      );
      FrameOffset += FrameWidth;
      BufferOffset += BufferWidth;
    }
  } else if (BltOperation == EfiBltVideoToVideo) {
    FrameOffset = FrameWidth * DestinationY + DestinationX;
    BufferOffset = FrameWidth * SourceY + SourceX;
    for (i = SourceY; i < SourceY + Height; i++) {
      CopyMem (
        FrameBuffer + FrameOffset,
        FrameBuffer + BufferOffset,
        Width * PIXEL_BYTES
      );
      FrameOffset += FrameWidth;
      BufferOffset += FrameWidth;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a: Not implemented %d\n",
      __FUNCTION__, BltOperation));
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}
