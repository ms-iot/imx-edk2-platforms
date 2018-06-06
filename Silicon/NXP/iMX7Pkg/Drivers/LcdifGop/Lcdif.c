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
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>

#include <iMXDisplay.h>
#include <iMX7ClkPwr.h>

#include "Lcdif.h"

//
// Board specific implementation
//
VOID LcdifBoardConfigureDisplay (
    IMX_DISPLAY_TIMING* Timing,
    VOID* FrameBuffer
    );

EFI_STATUS
LcdifInitDisplay (
    IMX_DISPLAY_TIMING* Timing,
    VOID* FrameBuffer,
    UINT32 LcdifBase
    )
{
    volatile UINT32 value32;
    DEBUG ((DEBUG_INFO, "LcdifInitDisplay\n"));

    // TODO: add timeouts to initialization loops

    //
    // Reset LCD controller (dancing bits reset procedure)
    //
    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_RL_CLR,
        IMX_LCDIF_RL_SFTRST);

    do {
        value32 = MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_RL);
    } while (value32 & IMX_LCDIF_RL_SFTRST);

    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_RL_CLR,
        IMX_LCDIF_RL_CLKGATE);

    do {
        value32 = MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_RL);
    } while (value32 & IMX_LCDIF_RL_CLKGATE);

    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_RL_SET,
        IMX_LCDIF_RL_SFTRST);

    do {
        value32 = MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_RL);
    } while (!(value32 & IMX_LCDIF_RL_CLKGATE));

    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_RL_CLR,
        IMX_LCDIF_RL_SFTRST);

    do {
        value32 = MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_RL);
    } while (value32 & IMX_LCDIF_RL_SFTRST);

    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_RL_CLR,
        IMX_LCDIF_RL_CLKGATE);

    do {
        value32 = MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_RL);
    } while (value32 & IMX_LCDIF_RL_CLKGATE);

    //
    // Setup dot clock, Bypass Count, Master and 24 bit
    //
    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_RL,
        IMX_LCDIF_RL_DOTCLK_MODE |
        IMX_LCDIF_RL_BYPASS_COUNT |
        IMX_LCDIF_RL_MASTER |
        IMX_LCDIF_RL_LCD_DATABUS_WIDTH_24 |
        IMX_LCDIF_RL_WORD_LENGTH_24);

    //
    // V Sync and Blank period
    //
    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_VDCTRL0,
        IMX_LCDIF_VDCTRL0_ENABLE_PRESENT |
        IMX_LCDIF_VDCTRL0_ENABLE_POL |
        IMX_LCDIF_VDCTRL0_VSYNC_PERIOD_UNIT |
        IMX_LCDIF_VDCTRL0_VSYNC_PULSE_UNIT |
        Timing->VSync);

    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_VDCTRL1,
        Timing->VActive + Timing->VBlank);

    //
    // H-Sync length and period
    //
    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_VDCTRL2,
        (Timing->HSync << IMX_LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH_SHL) |
        (Timing->HActive + Timing->HBlank));

    //
    // Start of sync to end of signal
    //
    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_VDCTRL3,
        ((Timing->HBlank - Timing->HSyncOffset) << IMX_LCDIF_VDCTRL3_HORZ_WAIT_CNT_SHL) |
        ((Timing->VBlank - Timing->VSyncOffset)));

    //
    // H active
    //
    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_VDCTRL4,
        Timing->HActive);

    //
    // 0x7 if the display data is arranged in the 24-bit unpacked format (A-R-G-B where A
    // value does not have be transmitted).
    //
    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_CTRL1,
        (0x7 << IMX_LCDIF_CTRL1_BYTE_PACKING_FORMAT_SHL) | 0x01000000);

    //
    // Valid vertical and horizontal data
    //
    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_TRANSFER_COUNT,
        (Timing->VActive << IMX_LCDIF_TRANSFER_COUNT_V_SHL) |
        Timing->HActive);

    //
    // Per spec : Generally, 4 outstanding requests of length 16 will
    // provide enough performance to drive any standard display resolution.
    //
    {
        LCDIFx_CTRL2_REG lcdifCtrl2Reg =
            { MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_CTRL2) };

        lcdifCtrl2Reg.BURST_LEN_8 = 0; // Burst of 16
        lcdifCtrl2Reg.OUTSTANDING_REQS = 0x02; // REQ_4 �

        MmioWrite32(
            LcdifBase + IMX_LCDIF_OFFSET_CTRL2, lcdifCtrl2Reg.AsUint32);
    }

    //
    // Frame buffer physical address
    //
    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_CUR_BUF,
        (UINT32)FrameBuffer);

    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_NEXT_BUF,
        (UINT32)FrameBuffer);

    //
    // Clear FIFO
    //
    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_CTRL1_SET,
        IMX_LCDIF_CTRL1_FIFO_CLEAR);

    //
    // Turn on sync signals
    //
    value32 = MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_VDCTRL4);
    value32 |= IMX_LCDIF_VDCTRL4_SYNC_SIGNALS_ON;
    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_VDCTRL4,
        value32);

    //
    // Clear FIFO clear add set run bit
    //
    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_CTRL1_CLR,
        IMX_LCDIF_CTRL1_FIFO_CLEAR);

    MmioWrite32(
        LcdifBase + IMX_LCDIF_OFFSET_RL_SET,
        IMX_LCDIF_RL_RUN);

    DEBUG_CODE_BEGIN();
    DEBUG((DEBUG_INIT, "IMX_LCDIF_OFFSET_RL 0x%08x : 0x%08x\n", LcdifBase + IMX_LCDIF_OFFSET_RL, MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_RL)));
    DEBUG((DEBUG_INIT, "IMX_LCDIF_OFFSET_CTRL1 0x%08x : 0x%08x\n", LcdifBase + IMX_LCDIF_OFFSET_CTRL1, MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_CTRL1)));
    DEBUG((DEBUG_INIT, "IMX_LCDIF_OFFSET_CTRL2 0x%08x : 0x%08x\n", LcdifBase + IMX_LCDIF_OFFSET_CTRL2, MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_CTRL2)));
    DEBUG((DEBUG_INIT, "IMX_LCDIF_OFFSET_TRANSFER_COUNT 0x%08x : 0x%08x\n", LcdifBase + IMX_LCDIF_OFFSET_TRANSFER_COUNT, MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_TRANSFER_COUNT)));
    DEBUG((DEBUG_INIT, "IMX_LCDIF_OFFSET_VDCTRL0 0x%08x : 0x%08x\n", LcdifBase + IMX_LCDIF_OFFSET_VDCTRL0, MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_VDCTRL0)));
    DEBUG((DEBUG_INIT, "IMX_LCDIF_OFFSET_VDCTRL1 0x%08x : 0x%08x\n", LcdifBase + IMX_LCDIF_OFFSET_VDCTRL1, MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_VDCTRL1)));
    DEBUG((DEBUG_INIT, "IMX_LCDIF_OFFSET_VDCTRL2 0x%08x : 0x%08x\n", LcdifBase + IMX_LCDIF_OFFSET_VDCTRL2, MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_VDCTRL2)));
    DEBUG((DEBUG_INIT, "IMX_LCDIF_OFFSET_VDCTRL3 0x%08x : 0x%08x\n", LcdifBase + IMX_LCDIF_OFFSET_VDCTRL3, MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_VDCTRL3)));
    DEBUG((DEBUG_INIT, "IMX_LCDIF_OFFSET_VDCTRL4 0x%08x : 0x%08x\n", LcdifBase + IMX_LCDIF_OFFSET_VDCTRL4, MmioRead32(LcdifBase + IMX_LCDIF_OFFSET_VDCTRL4)));
    DEBUG_CODE_END();

    return EFI_SUCCESS;
}

EFI_STATUS
LcdifConfigureDisplay (
    IMX_DISPLAY_TIMING* Timing,
    VOID* FrameBuffer
    )
{
    EFI_STATUS status;
    volatile UINT32 value32;
    DEBUG ((DEBUG_INFO, "LcdifConfigureDisplay\n"));
    UINT32 lcdifBase = FixedPcdGet32(PcdLCDIFBase);

    //
    // Stop LCD controller in case it was left on.  This will
    // also cancel any current transfers.
    //
    MmioWrite32(
        lcdifBase + IMX_LCDIF_OFFSET_RL_CLR,
        IMX_LCDIF_RL_RUN);

    do {
        value32 = MmioRead32(lcdifBase + IMX_LCDIF_OFFSET_RL);
    } while (value32 & IMX_LCDIF_RL_RUN);

    status = ImxSetLcdIfClockRate(Timing->PixelClock);
    if (EFI_ERROR(status)) {
        DEBUG((DEBUG_ERROR, "ImxSetLcdIfClockRate failed %r\n", status));
        goto Exit;
    }

    status = LcdifInitDisplay(
        Timing,
        FrameBuffer,
        lcdifBase);
    if (EFI_ERROR(status)) {
        DEBUG((DEBUG_ERROR, "LcdifInitDisplay failed %r\n", status));
        goto Exit;
    }

    //
    // Board specific display initialization
    //
    LcdifBoardConfigureDisplay(
        Timing,
        FrameBuffer);

Exit:
    return status;
}