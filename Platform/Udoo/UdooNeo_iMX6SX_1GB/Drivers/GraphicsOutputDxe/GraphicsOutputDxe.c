//// 
//  Microsoft Corporation
//  SiArch
//
//  Portions Copyright (c) Microsoft Corporation
//  Portions Copyright (c) Intel Corporation
//  Portions Copyright (c) Apple
//  Portions Copyright (c) ARM Ltd.
//  Portions Copyright (c) Freescale
////

//
// Adapted from Silicon/NXP/iMX6Pkg/Drivers/GOP and UdooNeoPkg/Drivers/GraphicsOutputDxe
// TODO: Converge with iMXVideoDxe.c
//

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcdLib.h>

#include <iMX6.h>
#include <iMXDisplay.h>

typedef struct {
    VENDOR_DEVICE_PATH Mmc;
    EFI_DEVICE_PATH End;
} VID_DEVICE_PATH; // copied from iMXVideoDxe.c

#define GFX_VENDOR_BYTES                    (sizeof(VENDOR_DEVICE_PATH)+sizeof(UINT32))

typedef struct _GFX_INSTANCE
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL            Protocol;       // must be first thing in structure (&Protocol == &GFX_INSTANCE)
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION    ModeInfo;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE       Mode;

    UINT8 EdidData[256];
    IMX_DISPLAY_TIMING PreferredTiming;
    VOID* FrameBuffer;
} GFX_INSTANCE;

//
// Globals
//
static GFX_INSTANCE sgInst;
static VID_DEVICE_PATH g_VidDevicePath =
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
}; // copied from iMXVideoDxe.c

VOID
InitLCD(
    VOID
    );

EFI_STATUS
ConfigureLCD(
    IMX_DISPLAY_TIMING* Timing,
    VOID* FrameBuffer
    );

VOID
InitializeTDA(
    VOID
    );

EFI_STATUS
ConfigureTDA(
    IMX_DISPLAY_TIMING* Timing
    );

BOOLEAN
ReadEDID(
    UINT8* EdidDataRead
    );

//
// Default timing
//
#ifdef USE_1024_768_DEFAULT_TIMING

IMX_DISPLAY_TIMING DefaultTiming =
{
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

#else

IMX_DISPLAY_TIMING DefaultTiming =
{
    74250000,   // PixelClock
    1280,       // HActive
    370,        // HBlank
    720,        // VActive
    30,         // VBlank
    40,         // HSync
    5,          // VSync
    220,        // HSyncOffset;
    20,         // VSyncOffset;
    1280,       // HImageSize
    720,        // VImageSize
    0,          // HBorder
    0,          // VBorder
    0,          // EdidFlags
    0,          // Flags
    1,          // PixelRepetition
    32,         // BPP
    PIXEL_FORMAT_BGRA32,       // PixelFormat
};

#endif

//
// Query for preferred timing
//
VOID
GetPreferredTiming(
    OUT IMX_DISPLAY_TIMING* PreferredTiming
    )
{
    EFI_STATUS status;
    BOOLEAN validEdid;
    IMX_DETAILED_TIMING_DESCRIPTOR* edidPreferredTiming;

    validEdid = ReadEDID(sgInst.EdidData);
    if (!validEdid) {
        goto DefaultTiming;
    }

    edidPreferredTiming =
        (IMX_DETAILED_TIMING_DESCRIPTOR*)&sgInst.EdidData[IMX_EDID_DTD_1_OFFSET];

    status = ImxConvertDTDToDisplayTiming(edidPreferredTiming, PreferredTiming);
    if (status != EFI_SUCCESS) {
        DebugPrint(DEBUG_ERROR, "Conversion to display timing failed\n");
        goto DefaultTiming;
    }

    //
    // Do not support odd resolution width and height.
    //
    if ((PreferredTiming->HActive < 640) || (PreferredTiming->VActive < 480) ||
        ((PreferredTiming->HActive % 4) != 0) || ((PreferredTiming->VActive % 4) != 0)) {
        goto DefaultTiming;
    }

    goto Exit;

DefaultTiming:
    DebugPrint(DEBUG_ERROR, "Selecting default timings\n");
    *PreferredTiming = DefaultTiming;

Exit:

    // Only support 8 bit per pixel and no pixel repetition for now
    PreferredTiming->PixelRepetition = 1;
    PreferredTiming->PixelFormat = PIXEL_FORMAT_BGRA32;
    PreferredTiming->Bpp = 32;

    ImxPrintDisplayTiming(
        "Preferred Timing",
        PreferredTiming);

    return;
}

//
// Entry Point
//
EFI_STATUS
GraphicsOutputDxeInitialize(
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE *   SystemTable
    )
{
    EFI_STATUS status;
    ZeroMem(&sgInst, sizeof(sgInst));

    DEBUG((DEBUG_INFO,"GraphicsOutputDxeInitialize\n"));

    // Initialize basic LCD so TDA chip is functional
    InitLCD();

    // Configure TDA chip
    InitializeTDA();

    // Figure out what the preferred timing
    GetPreferredTiming(&sgInst.PreferredTiming);

    // Allocate frame buffer
    UINT32 fbSize =
        sgInst.PreferredTiming.HActive *
        sgInst.PreferredTiming.VActive *
        (sgInst.PreferredTiming.Bpp / 8);

    DebugPrint(DEBUG_ERROR, "FbSize = %x\n", fbSize);
    if (fbSize > FixedPcdGet32(PcdFrameBufferSize)) {
        DebugPrint(DEBUG_ERROR, "Requested Frame buffer size too big for reserved region\n");
        status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }

    sgInst.FrameBuffer = (VOID*)FixedPcdGet32(PcdFrameBufferBase);
    DebugPrint(DEBUG_ERROR, "Framebuffer = %x\n", sgInst.FrameBuffer);

    // Initialize the frame buffer to black
    SetMem32(
        sgInst.FrameBuffer,
        fbSize,
        0xFF000000);

    status = ConfigureLCD(
        &sgInst.PreferredTiming,
        sgInst.FrameBuffer);
    if (EFI_ERROR(status)) {
        DebugPrint(DEBUG_ERROR, "GraphicsOutputDxeInitialize - ConfigureLCD failed\n");
        goto Exit;
    }

    status = ConfigureTDA(&sgInst.PreferredTiming);
    if (EFI_ERROR(status)) {
        DebugPrint(DEBUG_ERROR, "GraphicsOutputDxeInitialize - ConfigureTDA failed\n");
        goto Exit;
    }

    // Graphics output protocol
    sgInst.Protocol.QueryMode = VidGopQueryMode;
    sgInst.Protocol.SetMode = VidGopSetMode;
    sgInst.Protocol.Blt = VidGopBlt;
    sgInst.Protocol.Mode = &sgInst.Mode;

    sgInst.ModeInfo.Version = 0;
    sgInst.ModeInfo.HorizontalResolution = sgInst.PreferredTiming.HActive;
    sgInst.ModeInfo.VerticalResolution = sgInst.PreferredTiming.VActive;
    sgInst.ModeInfo.PixelFormat = PixelBlueGreenRedReserved8BitPerColor;
    sgInst.ModeInfo.PixelsPerScanLine = sgInst.ModeInfo.HorizontalResolution;

    sgInst.Mode.MaxMode = 1;
    sgInst.Mode.Mode = 0;
    sgInst.Mode.Info = &sgInst.ModeInfo;
    sgInst.Mode.SizeOfInfo = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
    sgInst.Mode.FrameBufferBase = (UINT32)sgInst.FrameBuffer;
    sgInst.Mode.FrameBufferSize = sgInst.ModeInfo.HorizontalResolution * sgInst.ModeInfo.VerticalResolution * (sgInst.PreferredTiming.Bpp / 8);

    // Install
    status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle,
        &gEfiGraphicsOutputProtocolGuid, &sgInst.Protocol,
        &gEfiDevicePathProtocolGuid, &g_VidDevicePath,
        NULL);

Exit:
    return status;
}

