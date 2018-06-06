/** @file
*
*  Copyright (c) Microsoft Corporation. All rights reserved.
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
#include <Library/DmaLib.h>
#include <Protocol/EmbeddedExternalDevice.h>
#include <Protocol/BlockIo.h>
#include <Protocol/Cpu.h>
#include <Protocol/DevicePath.h>
#include <Protocol/GraphicsOutput.h>

#include <iMXDisplay.h>

#include "Lcdif.h"

#define PIXEL_BYTES 4

typedef struct _LCDIF_DISPLAY_CONTEXT {

    //
    // Display configuration
    //
    IMX_DISPLAY_TIMING PreferedTiming;

    //
    // Frame buffer
    //
    VOID* FrameBuffer;

} LCDIF_DISPLAY_CONTEXT;

typedef struct {
    VENDOR_DEVICE_PATH Mmc;
    EFI_DEVICE_PATH End;
} VID_DEVICE_PATH;


EFI_STATUS
EFIAPI
LcdifGopQueryMode(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    IN UINT32 ModeNumber,
    OUT UINTN *SizeOfInfo,
    OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info
    );

EFI_STATUS
LcdifGopSetMode(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    IN UINT32 ModeNumber
    );

EFI_STATUS
LcdifGopBlt(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer,
    IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
    IN UINTN SourceX,
    IN UINTN SourceY,
    IN UINTN DestinationX,
    IN UINTN DestinationY,
    IN UINTN Width,
    IN UINTN Height,
    IN UINTN Delta
    );

static VID_DEVICE_PATH LcdifVidDevicePath =
{
    {
        {
            HARDWARE_DEVICE_PATH,
            HW_VENDOR_DP,
            {
                (UINT8)sizeof(VENDOR_DEVICE_PATH),
                (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8),
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
            sizeof(EFI_DEVICE_PATH_PROTOCOL),
            0
        }
    }
};

static LCDIF_DISPLAY_CONTEXT LcdifDisplayContext;
static EFI_GRAPHICS_OUTPUT_MODE_INFORMATION LcdifGopModeInfo;
static EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE LcdifGopProtocolMode;

static EFI_GRAPHICS_OUTPUT_PROTOCOL LcdifGopProtocol =
{
    LcdifGopQueryMode,      // QueryMode
    LcdifGopSetMode,        // SetMode
    LcdifGopBlt,            // Blt
    &LcdifGopProtocolMode   // Mode
};

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
    32,         // BPP
    PIXEL_FORMAT_BGRA32,       // PixelFormat
};

//
// Board specific implementation
//
VOID LcdifBoardInitialize (
    );

RETURN_STATUS LcdifBoardQueryEdidTiming (
    OUT IMX_DISPLAY_TIMING* PreferredTiming
    );

//
// Query for preferred timing
//
VOID
GetPreferredTiming (
    OUT IMX_DISPLAY_TIMING* PreferredTiming
    )
{
    EFI_STATUS status;

    status = LcdifBoardQueryEdidTiming(PreferredTiming);
    if (EFI_ERROR(status)) {

        //
        // Use default timing if fail to query EDID
        //
        *PreferredTiming = DefaultTiming;

    }

    PreferredTiming->PixelRepetition = 1;
    PreferredTiming->PixelFormat = PIXEL_FORMAT_BGRA32;
    PreferredTiming->Bpp = 32;

    ImxPrintDisplayTiming(
        "Preferred Timing",
        PreferredTiming);

    return;
}

//
// GOP driver entry point
//
EFI_STATUS
LcdifGopInitialize (
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
    )
{
    EFI_STATUS status;

    DEBUG ((DEBUG_INIT, "LcdIfGopInitialize\n"));

    ZeroMem(&LcdifDisplayContext, sizeof(LcdifDisplayContext));

    //
    // Board specific initialization
    //
    LcdifBoardInitialize();

    DEBUG ((DEBUG_INIT, "LcdifInitialize returned\n"));
    //
    // Retrieve the preferred timing
    //
    GetPreferredTiming(&LcdifDisplayContext.PreferedTiming);

    DEBUG ((DEBUG_INIT, "GetPreferredTiming returned\n"));
    //
    // Allocate frame buffer
    //
    {
        UINT32 fbSize =
            LcdifDisplayContext.PreferedTiming.HActive *
            LcdifDisplayContext.PreferedTiming.VActive *
            (LcdifDisplayContext.PreferedTiming.Bpp / 8);
        DEBUG((DEBUG_INIT, "Framebuffer size = %x\n", fbSize));

        //
        // Use NoncoherentDmaLib API to allocate uncached EFI Runtime
        // memory for our frame buffer
        //
        status = DmaAllocateBuffer(
                    EfiRuntimeServicesData,
                    EFI_SIZE_TO_PAGES(fbSize),
                    (VOID **)&LcdifDisplayContext.FrameBuffer);
        if (EFI_ERROR(status)) {
            DEBUG((DEBUG_ERROR, "Framebuffer allocation failed\n"));
            goto Exit;
        }
        DEBUG((DEBUG_INIT, "Framebuffer address = %x\n", LcdifDisplayContext.FrameBuffer));

        //
        // Initialize the frame buffer to black
        //
        SetMem32(
            LcdifDisplayContext.FrameBuffer,
            fbSize,
            0xFF000000);
    }

    DEBUG ((DEBUG_INIT, "calling LcdIfConfigureDisplay\n"));

    status = LcdifConfigureDisplay(
        &LcdifDisplayContext.PreferedTiming,
        LcdifDisplayContext.FrameBuffer);
    if (EFI_ERROR(status)) {
        DEBUG((EFI_D_ERROR, "LcdifConfigureDisplay failed %r\n", status));
        goto Exit;
    }

    DEBUG ((DEBUG_INIT, "LcdIfConfigureDisplay returned\n"));

    LcdifGopModeInfo.Version = 0;
    LcdifGopModeInfo.HorizontalResolution =
        LcdifDisplayContext.PreferedTiming.HActive;
    LcdifGopModeInfo.VerticalResolution =
        LcdifDisplayContext.PreferedTiming.VActive;
    LcdifGopModeInfo.PixelFormat = PixelBlueGreenRedReserved8BitPerColor;

    ZeroMem(
        &LcdifGopModeInfo.PixelInformation,
        sizeof(LcdifGopModeInfo.PixelInformation));

    LcdifGopModeInfo.PixelsPerScanLine = LcdifGopModeInfo.HorizontalResolution;
    LcdifGopProtocolMode.MaxMode = 1;
    LcdifGopProtocolMode.Mode = 0;
    LcdifGopProtocolMode.Info = &LcdifGopModeInfo;
    LcdifGopProtocolMode.SizeOfInfo = sizeof(LcdifGopModeInfo);
    LcdifGopProtocolMode.FrameBufferBase = (UINT32)LcdifDisplayContext.FrameBuffer;
    LcdifGopProtocolMode.FrameBufferSize =
        LcdifGopModeInfo.HorizontalResolution *
            LcdifGopModeInfo.VerticalResolution *
            LcdifDisplayContext.PreferedTiming.Bpp / 8;

    DEBUG ((DEBUG_INIT, "calling InstallMultipleProtocolInterfaces\n"));

    status = gBS->InstallMultipleProtocolInterfaces(
        &ImageHandle,
        &gEfiGraphicsOutputProtocolGuid,
        &LcdifGopProtocol,
        &gEfiDevicePathProtocolGuid,
        &LcdifVidDevicePath,
        NULL);
    if (EFI_ERROR(status)) {
        DEBUG(
            (DEBUG_ERROR,
                "Fail to install protocol, status=%r\n",
                status));
        goto Exit;
    }

    status = EFI_SUCCESS;
Exit:

    DEBUG ((DEBUG_INIT, "exiting LcdIfGopInitialized return=%ld\n", status));
    return status;
}

//
// GraphicsOutput Protocol function, mapping to
// EFI_GRAPHICS_OUTPUT_PROTOCOL.QueryMode
//
EFI_STATUS
EFIAPI
LcdifGopQueryMode (
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    IN UINT32 ModeNumber,
    OUT UINTN *SizeOfInfo,
    OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info
    )
{
    EFI_STATUS status;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* outputModePtr;

    DEBUG ((DEBUG_INIT, "entering LcdifGopQueryMode\n"));

    if (ModeNumber != 0) {
        DEBUG((EFI_D_ERROR, "LcdifGopQueryMode: Saw request to query mode %d\n", ModeNumber));
        status = EFI_INVALID_PARAMETER;
        goto Exit;
    }

    outputModePtr = (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *)
        AllocatePool (sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
    if (outputModePtr == NULL) {
        status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }

    outputModePtr->Version = 0;
    outputModePtr->HorizontalResolution = LcdifGopModeInfo.HorizontalResolution;
    outputModePtr->VerticalResolution = LcdifGopModeInfo.VerticalResolution;
    outputModePtr->PixelFormat = PixelBlueGreenRedReserved8BitPerColor;

    outputModePtr->PixelsPerScanLine = LcdifGopModeInfo.HorizontalResolution;
    *SizeOfInfo = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
    *Info = outputModePtr;

    status = EFI_SUCCESS;

Exit:
    return status;
}

//
// GraphicsOutput Protocol function, mapping to
// EFI_GRAPHICS_OUTPUT_PROTOCOL.SetMode
//

EFI_STATUS
LcdifGopSetMode (
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    IN UINT32 ModeNumber
    )
{
    DEBUG ((DEBUG_INIT, "entering LcdifGopSetMode\n"));
    EFI_STATUS status;

    if (ModeNumber != 0) {
        DEBUG((EFI_D_ERROR, "LcdifGopSetMode: Saw request to set mode to %d\n", ModeNumber));
        status = EFI_UNSUPPORTED;
        goto Exit;
    }

    status = EFI_SUCCESS;

Exit:
    return status;
}

//
// GraphicsOutput Protocol function, mapping to
// EFI_GRAPHICS_OUTPUT_PROTOCOL. Blt
//
EFI_STATUS
LcdifGopBlt (
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer, OPTIONAL
    IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
    IN UINTN SourceX,
    IN UINTN SourceY,
    IN UINTN DestinationX,
    IN UINTN DestinationY,
    IN UINTN Width,
    IN UINTN Height,
    IN UINTN Delta OPTIONAL
    )
{
    DEBUG ((DEBUG_VERBOSE, "entering LcdifGopBlt\n"));
    UINT32 *frameBuffer;
    UINT32 frameWidth;
    UINT32 frameOffset;
    UINT32 bufferOffset;
    UINT32 bufferWidth;
    UINT32 i;

    frameBuffer = (UINT32 *) (UINTN) LcdifGopProtocolMode.FrameBufferBase;
    frameWidth = LcdifGopModeInfo.HorizontalResolution;

    if (Delta == 0) {
        bufferWidth = Width;
    } else {
        bufferWidth = Delta / PIXEL_BYTES;
    }

    if (BltOperation == EfiBltVideoFill) {

        frameOffset = frameWidth * DestinationY + DestinationX;
        for (i = DestinationY; i < DestinationY + Height; i++) {
            SetMem32(frameBuffer + frameOffset,
                     Width * PIXEL_BYTES,
                     *(UINT32 *)BltBuffer
                     );
            frameOffset += frameWidth;
        }
    } else if (BltOperation == EfiBltVideoToBltBuffer) {

        frameOffset = frameWidth * SourceY + SourceX;
        bufferOffset = bufferWidth * DestinationY + DestinationX;

        for (i = SourceY; i < SourceY + Height; i++) {
            CopyMem(BltBuffer + bufferOffset,
                    frameBuffer + frameOffset,
                    Width * PIXEL_BYTES);
            frameOffset += frameWidth;
            bufferOffset += bufferWidth;
        }
    } else if (BltOperation == EfiBltBufferToVideo) {

        frameOffset = frameWidth * DestinationY + DestinationX;
        bufferOffset = bufferWidth * SourceY + SourceX;

        for (i = SourceY; i < SourceY + Height; i++) {
            CopyMem(frameBuffer + frameOffset,
                    BltBuffer + bufferOffset,
                    Width * PIXEL_BYTES);
            frameOffset += frameWidth;
            bufferOffset += bufferWidth;
        }

    } else if (BltOperation == EfiBltVideoToVideo) {

        frameOffset = frameWidth * DestinationY + DestinationX;
        bufferOffset = frameWidth * SourceY + SourceX;

        for (i = SourceY; i < SourceY + Height; i++) {
            CopyMem(frameBuffer + frameOffset,
                    frameBuffer + bufferOffset,
                    Width * PIXEL_BYTES);
            frameOffset += frameWidth;
            bufferOffset += frameWidth;
        }

    } else {
        DEBUG((EFI_D_ERROR, "LcdifGopBlt:LcdifGopBlt not implemented %d\n", BltOperation));
        return EFI_INVALID_PARAMETER;
    }

    return EFI_SUCCESS;
}

