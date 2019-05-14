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

STATIC CONST DISPLAY_TIMING mDisplayTiming = {
  1920,  // Horizontal Resolution
  1080,  // Vertical Resolution
};

STATIC CONST VID_DEVICE_PATH mVidDevicePath = {
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
             EFI_SIZE_TO_PAGES (FrameBufferSize),
             (VOID **)&FrameBufferBase);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to allocate fb, Status=%r\n",
      __FUNCTION__, Status));
    goto Exit;
  };

  // Initialize the frame buffer to black
  SetMem32 ((VOID *)((UINTN)FrameBufferBase), FrameBufferSize, 0xFF000000);

  // Configure Mode Info
  ZeroMem (&VidGopModeInfo, sizeof (VidGopModeInfo));
  VidGopModeInfo.Version = 0;
  VidGopModeInfo.HorizontalResolution = mDisplayTiming.HActive;
  VidGopModeInfo.VerticalResolution = mDisplayTiming.VActive;
  VidGopModeInfo.PixelFormat = PixelBlueGreenRedReserved8BitPerColor;

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
    DmaFreeBuffer (EFI_SIZE_TO_PAGES (FrameBufferSize), (VOID *)FrameBufferBase);
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "%a: Exit = %Xh\n",
    __FUNCTION__, Status));
  return Status;
}

