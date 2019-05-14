/** @file
*
*  Copyright (c) 2019 Microsoft Corporation. All rights reserved.
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

#include "VirtualGopDxe.h"

#define BYTES_PER_PIXEL 4 // PixelFormat=BGRA8888

CONST DISPLAY_TIMING mDisplayTiming = {
  1920,  // Horizontal Resolution
  1080,  // Vertical Resolution
};

STATIC VID_DEVICE_PATH mVidDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)sizeof (VENDOR_DEVICE_PATH),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8),
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
  OutputMode->PixelFormat = VidGopModeInfo.PixelFormat;
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
    BufferWidth = Delta / BYTES_PER_PIXEL;
  }

  if (BltOperation == EfiBltVideoFill) {
    FrameOffset = FrameWidth * DestinationY + DestinationX;
    for (i = DestinationY; i < DestinationY + Height; i++) {
      SetMem32 (
        FrameBuffer + FrameOffset,
        Width * BYTES_PER_PIXEL,
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
        Width * BYTES_PER_PIXEL
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
        Width * BYTES_PER_PIXEL
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
        Width * BYTES_PER_PIXEL
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

STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL VidGop = {
  VidGopQueryMode, // QueryMode
  VidGopSetMode,   // SetMode
  VidGopBlt,       // Blt
  &VidGopMode      // Mode
};

EFI_STATUS
VirtualGopDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  UINT32                  FrameBufferSize;
  EFI_PHYSICAL_ADDRESS    FrameBufferBase;
  EFI_STATUS              Status;

  DEBUG ((DEBUG_INFO, "%a: Enter \n", __FUNCTION__));

  // Allocate Frame Buffer
  FrameBufferSize = mDisplayTiming.HActive * mDisplayTiming.VActive * BYTES_PER_PIXEL;
  DEBUG ((DEBUG_INFO, "%a: Frame Buffer Size = %d \n", __FUNCTION__, FrameBufferSize));
  Status = DmaAllocateBuffer (
             EfiRuntimeServicesData,
             EFI_SIZE_TO_PAGES(FrameBufferSize),
             (VOID **)&FrameBufferBase);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to allocate fb, Status=%r\n",
      __FUNCTION__, Status));
    goto Exit;
  };

  // Initialize the frame buffer to black
  SetMem32 ((VOID *)((UINTN)FrameBufferBase), FrameBufferSize, 0xFF000000);

  // Configure Mode Info
  VidGopModeInfo.Version = 0;
  VidGopModeInfo.HorizontalResolution = mDisplayTiming.HActive;
  VidGopModeInfo.VerticalResolution = mDisplayTiming.VActive;
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
  VidGopMode.FrameBufferBase = FrameBufferBase;
  VidGopMode.FrameBufferSize = FrameBufferSize;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  &VidGop,
                  &gEfiDevicePathProtocolGuid,
                  &mVidDevicePath,
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

