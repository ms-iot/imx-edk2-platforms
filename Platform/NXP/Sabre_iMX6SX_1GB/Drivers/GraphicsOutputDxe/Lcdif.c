//// 
//  Microsoft Corporation
//  SiArch
//
//  Portions Copyright (c) Microsoft Corporation
//  Portions Copyright (c) Intel Corporation
//  Portions Copyright (c) Apple
//  Portions Copyright (c) ARM Ltd.
//  Portions Copyright (c) Freescale
//  Copyright 2019 NXP
////

#include <Library/DebugLib.h>
#include <Library/IoLib.h>

#include <iMX6.h>
#include <iMX6ClkPwr.h>
#include <iMXDisplay.h>

#include <imx6sxdef.inc>

#define STATIC_CHECK(cond)  typedef int check_video[(cond)?1:-1]

static
VOID
sResetLCDIF(
    VOID    
    )
{
    UINT32 value32;
    DEBUG((DEBUG_INFO,"sResetLCDIF\n"));

    //
    // reset LCD controller (dancing bits reset procedure)
    //
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_RL_CLR, IMX6SX_LCDIF_RL_SFTRST);
    do
    {
        value32 = MmioRead32(IMX6SX_PHYSADDR_LCDIF2_RL);
    } while (value32 & IMX6SX_LCDIF_RL_SFTRST);
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_RL_CLR, IMX6SX_LCDIF_RL_CLKGATE);
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_RL_SET, IMX6SX_LCDIF_RL_SFTRST);
    do
    {
        value32 = MmioRead32(IMX6SX_PHYSADDR_LCDIF2_RL);
    } while (!(value32 & IMX6SX_LCDIF_RL_CLKGATE));
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_RL_CLR, IMX6SX_LCDIF_RL_SFTRST);
    do
    {
        value32 = MmioRead32(IMX6SX_PHYSADDR_LCDIF2_RL);
    } while (value32 & IMX6SX_LCDIF_RL_SFTRST);
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_RL_CLR, IMX6SX_LCDIF_RL_CLKGATE);
    do
    {
        value32 = MmioRead32(IMX6SX_PHYSADDR_LCDIF2_RL);
    } while (value32 & IMX6SX_LCDIF_RL_CLKGATE);
}

static
VOID
sConfigLCDIF(
    IMX_DISPLAY_TIMING* Timing,
    VOID* FrameBuffer
    )
{
    UINT32 value32;
    DEBUG((DEBUG_INFO,"sConfigLCDIF\n"));

    // 24-bit hard coded
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_RL,
        IMX6SX_LCDIF_RL_BYPASS_COUNT |
        IMX6SX_LCDIF_RL_DOTCLK_MODE |
        IMX6SX_LCDIF_RL_LCD_DATABUS_WIDTH_18 |
        IMX6SX_LCDIF_RL_WORD_LENGTH_24 |
        IMX6SX_LCDIF_RL_MASTER
        );

    // first three bytes out of every 32 contain valid data
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_CTRL1,
        (0x07 << IMX6SX_LCDIF_CTRL1_BYTE_PACKING_FORMAT_SHL));

    // Valid vertical and horizontal data
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_TRANSFER_COUNT,
        (Timing->VActive << IMX6SX_LCDIF_TRANSFER_COUNT_V_SHL) |
        Timing->HActive);

    // V-Sync period
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_VDCTRL0,
        IMX6SX_LCDIF_VDCTRL0_ENABLE_PRESENT |
        IMX6SX_LCDIF_VDCTRL0_ENABLE_POL |
        IMX6SX_LCDIF_VDCTRL0_VSYNC_PERIOD_UNIT |
        IMX6SX_LCDIF_VDCTRL0_VSYNC_PULSE_UNIT |
        Timing->VSync);

    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_VDCTRL1, Timing->VActive + Timing->VBlank);

    // H-Sync length and period
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_VDCTRL2,
        (Timing->HSync << IMX6SX_LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH_SHL) |
        (Timing->HActive + Timing->HBlank));

    // wait counts from start of sync to end of v/h line
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_VDCTRL3,
        ((Timing->HBlank - Timing->HSyncOffset) << IMX6SX_LCDIF_VDCTRL3_HORZ_WAIT_CNT_SHL) |
        ((Timing->VBlank - Timing->VSyncOffset)));

    // valid data horizontal
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_VDCTRL4, Timing->HActive);

    // frame buffer location
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_CUR_BUF, (UINT32)FrameBuffer);
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_NEXT_BUF, (UINT32)FrameBuffer);

    // set clear
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_CTRL1_SET, IMX6SX_LCDIF_CTRL1_FIFO_CLEAR);

    // turn on sync signals
    value32 = MmioRead32(IMX6SX_PHYSADDR_LCDIF2_VDCTRL4);
    value32 |= IMX6SX_LCDIF_VDCTRL4_SYNC_SIGNALS_ON;
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_VDCTRL4, value32);

    // clear FIFO clear
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_CTRL1_CLR, IMX6SX_LCDIF_CTRL1_FIFO_CLEAR);

    // set run bit
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_RL_SET, IMX6SX_LCDIF_RL_RUN);

#if 0
    DebugPrint(DEBUG_INFO, "RL                 %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_RL           ));
    DebugPrint(DEBUG_INFO, "CTRL1              %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_CTRL1        ));
    DebugPrint(DEBUG_INFO, "CTRL2              %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_CTRL2        ));
    DebugPrint(DEBUG_INFO, "TRANSFER_COUNT     %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_TRANSFER_COUNT));       
    DebugPrint(DEBUG_INFO, "CUR_BUF            %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_CUR_BUF      ));
    DebugPrint(DEBUG_INFO, "NEXT_BUF           %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_NEXT_BUF     )); 
    DebugPrint(DEBUG_INFO, "TIMING             %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_TIMING       ));
    DebugPrint(DEBUG_INFO, "VDCTRL0            %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_VDCTRL0      ));
    DebugPrint(DEBUG_INFO, "VDCTRL1            %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_VDCTRL1      ));
    DebugPrint(DEBUG_INFO, "VDCTRL2            %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_VDCTRL2      ));
    DebugPrint(DEBUG_INFO, "VDCTRL3            %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_VDCTRL3      ));
    DebugPrint(DEBUG_INFO, "VDCTRL4            %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_VDCTRL4      ));
    DebugPrint(DEBUG_INFO, "DVICTRL0           %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_DVICTRL0     )); 
    DebugPrint(DEBUG_INFO, "DVICTRL1           %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_DVICTRL1     )); 
    DebugPrint(DEBUG_INFO, "DVICTRL2           %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_DVICTRL2     )); 
    DebugPrint(DEBUG_INFO, "DVICTRL3           %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_DVICTRL3     )); 
    DebugPrint(DEBUG_INFO, "DVICTRL4           %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_DVICTRL4     )); 
    DebugPrint(DEBUG_INFO, "CSC_COEFF0         %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_CSC_COEFF0   ));   
    DebugPrint(DEBUG_INFO, "CSC_COEFF1         %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_CSC_COEFF1   ));   
    DebugPrint(DEBUG_INFO, "CSC_COEFF2         %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_CSC_COEFF2   ));   
    DebugPrint(DEBUG_INFO, "CSC_COEFF3         %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_CSC_COEFF3   ));   
    DebugPrint(DEBUG_INFO, "CSC_COEFF4         %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_CSC_COEFF4   ));   
    DebugPrint(DEBUG_INFO, "CSC_OFFSET         %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_CSC_OFFSET   ));   
    DebugPrint(DEBUG_INFO, "CSC_LIMIT          %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_CSC_LIMIT    ));  
    DebugPrint(DEBUG_INFO, "DATA               %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_DATA         ));
    DebugPrint(DEBUG_INFO, "BM_ERROR_STAT      %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_BM_ERROR_STAT));      
    DebugPrint(DEBUG_INFO, "CRC_STAT           %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_CRC_STAT     )); 
    DebugPrint(DEBUG_INFO, "STAT               %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_STAT         ));
    DebugPrint(DEBUG_INFO, "VERSION            %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_VERSION      ));
    DebugPrint(DEBUG_INFO, "DEBUG0             %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_DEBUG0       ));
    DebugPrint(DEBUG_INFO, "DEBUG1             %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_DEBUG1       ));
    DebugPrint(DEBUG_INFO, "DEBUG2             %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_DEBUG2       ));
    DebugPrint(DEBUG_INFO, "THRES              %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_THRES        ));
    DebugPrint(DEBUG_INFO, "AS_CTRL            %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_AS_CTRL      ));
    DebugPrint(DEBUG_INFO, "AS_BUF             %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_AS_BUF       ));
    DebugPrint(DEBUG_INFO, "AS_NEXT_BUF        %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_AS_NEXT_BUF  ));    
    DebugPrint(DEBUG_INFO, "AS_CLRKEYLOW       %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_AS_CLRKEYLOW ));     
    DebugPrint(DEBUG_INFO, "AS_CLRKEYHIGH      %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_AS_CLRKEYHIGH));      
    DebugPrint(DEBUG_INFO, "SYNC_DELAY         %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_SYNC_DELAY   ));   
    DebugPrint(DEBUG_INFO, "DEBUG3             %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_DEBUG3       ));
    DebugPrint(DEBUG_INFO, "DEBUG4             %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_DEBUG4       ));
    DebugPrint(DEBUG_INFO, "DEBUG5             %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_DEBUG5       ));
#endif
}

VOID
InitLCD(
    VOID
    )
{
    DEBUG((DEBUG_INFO,"InitLCD\n"));

    // turn on LCD clocks
    ImxClkPwrLcdClockEnable(IMX6SX_PHYSADDR_LCDIF2);

    // set run bit
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF2_RL_SET, IMX6SX_LCDIF_RL_RUN);

    return;
}

EFI_STATUS
ConfigureLCD(
    IMX_DISPLAY_TIMING* Timing,
    VOID* FrameBuffer
    )
{
    EFI_STATUS status;

    DEBUG((DEBUG_INFO,"ConfigureLCD\n"));
    status = ImxSetLcdIfClockRate(IMX6SX_PHYSADDR_LCDIF2, Timing->PixelClock);
    if (EFI_ERROR(status)) {
        DebugPrint(DEBUG_ERROR, "ConfigureLCD - ImxClkPwrLcdIfEnable failed");
        return status;
    }

    sResetLCDIF();

    // Debug code to allocate as much bandwidth as possible to LCDIF
#if 0
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF1_CTRL2, 0x00800000);
    DebugPrint(DEBUG_INIT, "CTRL2 %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_CTRL2));
    MmioWrite32(IMX6SX_PHYSADDR_LCDIF1_THRES, (511 << 16) | (510));
    DebugPrint(DEBUG_INIT, "THRES %08X\n", MmioRead32(IMX6SX_PHYSADDR_LCDIF1_THRES));
#endif

    sConfigLCDIF(Timing, FrameBuffer);

    return EFI_SUCCESS;
}

// -------------------------------------------------------------------------------
