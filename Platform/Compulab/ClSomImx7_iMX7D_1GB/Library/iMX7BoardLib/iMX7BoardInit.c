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

#include <Library/IoLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>

#include "iMX7.h"
#include "iMX7IoMux.h"
#include "iMX7ClkPwr.h"
#include "iMXGpio.h"
#include "iMX7ClkPwrReg.h"

typedef enum {

    IMX_PAD_LCD_DATA00_LCD_DATA00 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_LCD_DATA00_ALT0_LCD_DATA0),

    IMX_PAD_LCD_DATA01_LCD_DATA01 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_LCD_DATA01_ALT0_LCD_DATA1),

    IMX_PAD_LCD_DATA02_LCD_DATA02 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_LCD_DATA02_ALT0_LCD_DATA2),

    IMX_PAD_LCD_DATA03_LCD_DATA03 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_LCD_DATA03_ALT0_LCD_DATA3),

    IMX_PAD_LCD_DATA16_LCD_DATA16 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_LCD_DATA16_ALT0_LCD_DATA16),

    IMX_PAD_LCD_DATA17_LCD_DATA17 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_LCD_DATA17_ALT0_LCD_DATA17),

    IMX_PAD_LCD_DATA18_LCD_DATA18 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_LCD_DATA18_ALT0_LCD_DATA18),

    IMX_PAD_LCD_DATA19_LCD_DATA19 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_LCD_DATA19_ALT0_LCD_DATA19),

    IMX_PAD_LCD_DATA20_LCD_DATA20 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_LCD_DATA20_ALT0_LCD_DATA20),

    IMX_PAD_LCD_DATA21_LCD_DATA21 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_LCD_DATA21_ALT0_LCD_DATA21),

    IMX_PAD_LCD_DATA22_LCD_DATA22 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_LCD_DATA22_ALT0_LCD_DATA22),

    IMX_PAD_LCD_DATA23_LCD_DATA23 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_LCD_DATA23_ALT0_LCD_DATA23),

    IMX_PAD_EPDC_DATA00_LCD_CLK = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA00_ALT7_LCD_CLK),

    IMX_PAD_EPDC_DATA01_LCD_ENABLE = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA01_ALT7_LCD_ENABLE),

    IMX_PAD_EPDC_DATA02_LCD_VSYNC = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA02_ALT7_LCD_VSYNC),

    IMX_PAD_EPDC_DATA03_LCD_HSYNC = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA03_ALT7_LCD_HSYNC),

    IMX_PAD_EPDC_DATA04_LCD_DATA4 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA04_ALT6_LCD_DATA4),

    IMX_PAD_EPDC_DATA05_LCD_DATA5 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA05_ALT6_LCD_DATA5),

    IMX_PAD_EPDC_DATA06_LCD_DATA6 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA06_ALT6_LCD_DATA6),

     IMX_PAD_EPDC_DATA07_LCD_DATA7 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA07_ALT6_LCD_DATA7),

    IMX_PAD_EPDC_DATA08_LCD_DATA8 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA08_ALT6_LCD_DATA8),

     IMX_PAD_EPDC_DATA09_LCD_DATA9 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA09_ALT6_LCD_DATA9),

    IMX_PAD_EPDC_DATA10_LCD_DATA10 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA10_ALT6_LCD_DATA10),

     IMX_PAD_EPDC_DATA11_LCD_DATA11 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA11_ALT6_LCD_DATA11),

    IMX_PAD_EPDC_DATA12_LCD_DATA12 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA12_ALT6_LCD_DATA12),

     IMX_PAD_EPDC_DATA13_LCD_DATA13 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA13_ALT6_LCD_DATA13),

    IMX_PAD_EPDC_DATA14_LCD_DATA14 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA14_ALT6_LCD_DATA14),

     IMX_PAD_EPDC_DATA15_LCD_DATA15 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_EPDC_DATA15_ALT6_LCD_DATA15),

} IMX_LCD_PADCFG;

/**
    Initialize LCDIF module on the SOC and perform required pin-muxing.
**/
void LcdifInit ()
{
    //
    // Setup muxing
    //
    ImxPadConfig(IMX_PAD_LCD_DATA00, IMX_PAD_LCD_DATA00_LCD_DATA00);
    ImxPadConfig(IMX_PAD_LCD_DATA01, IMX_PAD_LCD_DATA01_LCD_DATA01);
    ImxPadConfig(IMX_PAD_LCD_DATA02, IMX_PAD_LCD_DATA02_LCD_DATA02);
    ImxPadConfig(IMX_PAD_LCD_DATA03, IMX_PAD_LCD_DATA03_LCD_DATA03);
    ImxPadConfig(IMX_PAD_LCD_DATA16, IMX_PAD_LCD_DATA16_LCD_DATA16);
    ImxPadConfig(IMX_PAD_LCD_DATA17, IMX_PAD_LCD_DATA17_LCD_DATA17);
    ImxPadConfig(IMX_PAD_LCD_DATA18, IMX_PAD_LCD_DATA18_LCD_DATA18);
    ImxPadConfig(IMX_PAD_LCD_DATA19, IMX_PAD_LCD_DATA19_LCD_DATA19);
    ImxPadConfig(IMX_PAD_LCD_DATA20, IMX_PAD_LCD_DATA20_LCD_DATA20);
    ImxPadConfig(IMX_PAD_LCD_DATA21, IMX_PAD_LCD_DATA21_LCD_DATA21);
    ImxPadConfig(IMX_PAD_LCD_DATA22, IMX_PAD_LCD_DATA22_LCD_DATA22);
    ImxPadConfig(IMX_PAD_LCD_DATA23, IMX_PAD_LCD_DATA23_LCD_DATA23);

    ImxPadConfig(IMX_PAD_EPDC_DATA00, IMX_PAD_EPDC_DATA00_LCD_CLK);
    ImxPadConfig(IMX_PAD_EPDC_DATA01, IMX_PAD_EPDC_DATA01_LCD_ENABLE);
    ImxPadConfig(IMX_PAD_EPDC_DATA02, IMX_PAD_EPDC_DATA02_LCD_VSYNC);
    ImxPadConfig(IMX_PAD_EPDC_DATA03, IMX_PAD_EPDC_DATA03_LCD_HSYNC);

    ImxPadConfig(IMX_PAD_EPDC_DATA04, IMX_PAD_EPDC_DATA04_LCD_DATA4);
    ImxPadConfig(IMX_PAD_EPDC_DATA05, IMX_PAD_EPDC_DATA05_LCD_DATA5);
    ImxPadConfig(IMX_PAD_EPDC_DATA06, IMX_PAD_EPDC_DATA06_LCD_DATA6);
    ImxPadConfig(IMX_PAD_EPDC_DATA07, IMX_PAD_EPDC_DATA07_LCD_DATA7);
    ImxPadConfig(IMX_PAD_EPDC_DATA08, IMX_PAD_EPDC_DATA08_LCD_DATA8);
    ImxPadConfig(IMX_PAD_EPDC_DATA09, IMX_PAD_EPDC_DATA09_LCD_DATA9);
    ImxPadConfig(IMX_PAD_EPDC_DATA10, IMX_PAD_EPDC_DATA10_LCD_DATA10);
    ImxPadConfig(IMX_PAD_EPDC_DATA11, IMX_PAD_EPDC_DATA11_LCD_DATA11);
    ImxPadConfig(IMX_PAD_EPDC_DATA12, IMX_PAD_EPDC_DATA12_LCD_DATA12);
    ImxPadConfig(IMX_PAD_EPDC_DATA13, IMX_PAD_EPDC_DATA13_LCD_DATA13);
    ImxPadConfig(IMX_PAD_EPDC_DATA14, IMX_PAD_EPDC_DATA14_LCD_DATA14);
    ImxPadConfig(IMX_PAD_EPDC_DATA15, IMX_PAD_EPDC_DATA15_LCD_DATA15);

    DEBUG_CODE_BEGIN();
    DEBUG((DEBUG_ERROR, "LCDIF MUX Dump\n"));

    ImxPadDumpConfig("IMX_PAD_LCD_DATA00", IMX_PAD_LCD_DATA00);
    ImxPadDumpConfig("IMX_PAD_LCD_DATA01", IMX_PAD_LCD_DATA01);
    ImxPadDumpConfig("IMX_PAD_LCD_DATA02", IMX_PAD_LCD_DATA02);
    ImxPadDumpConfig("IMX_PAD_LCD_DATA03", IMX_PAD_LCD_DATA03);
    ImxPadDumpConfig("IMX_PAD_LCD_DATA16", IMX_PAD_LCD_DATA16);
    ImxPadDumpConfig("IMX_PAD_LCD_DATA17", IMX_PAD_LCD_DATA17);
    ImxPadDumpConfig("IMX_PAD_LCD_DATA18", IMX_PAD_LCD_DATA18);
    ImxPadDumpConfig("IMX_PAD_LCD_DATA19", IMX_PAD_LCD_DATA19);
    ImxPadDumpConfig("IMX_PAD_LCD_DATA20", IMX_PAD_LCD_DATA20);
    ImxPadDumpConfig("IMX_PAD_LCD_DATA21", IMX_PAD_LCD_DATA21);
    ImxPadDumpConfig("IMX_PAD_LCD_DATA22", IMX_PAD_LCD_DATA22);
    ImxPadDumpConfig("IMX_PAD_LCD_DATA23", IMX_PAD_LCD_DATA23);

    ImxPadDumpConfig("IMX_PAD_EPDC_DATA00", IMX_PAD_EPDC_DATA00);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA01", IMX_PAD_EPDC_DATA01);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA02", IMX_PAD_EPDC_DATA02);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA03", IMX_PAD_EPDC_DATA03);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA04", IMX_PAD_EPDC_DATA04);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA05", IMX_PAD_EPDC_DATA05);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA06", IMX_PAD_EPDC_DATA06);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA07", IMX_PAD_EPDC_DATA07);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA08", IMX_PAD_EPDC_DATA08);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA09", IMX_PAD_EPDC_DATA09);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA10", IMX_PAD_EPDC_DATA10);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA11", IMX_PAD_EPDC_DATA11);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA12", IMX_PAD_EPDC_DATA12);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA13", IMX_PAD_EPDC_DATA13);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA14", IMX_PAD_EPDC_DATA14);
    ImxPadDumpConfig("IMX_PAD_EPDC_DATA15", IMX_PAD_EPDC_DATA15);
    DEBUG_CODE_END();
}

typedef enum {

     IMX_PAD_PWM3_GPIO1_IO03 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_1_X4,
        IMX_SRE_FAST,
        IMX_HYS_DISABLED,
        IMX_PE_0_Pull_Disabled,
        IMX_PS_3_100K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_GPIO1_IO03_ALT1_PWM3_OUT),

} IMX_PWM_PADCFG;

/**
  Initalize the PWM controllers
**/
VOID PwmInit ()
{
    MmioWrite32((UINTN)IMX_CCM_CCGR(132), IMX_CCM_CCGR_GATE_RUN_WAIT);
    MmioWrite32((UINTN)IMX_CCM_CCGR(133), IMX_CCM_CCGR_GATE_RUN_WAIT);
    MmioWrite32((UINTN)IMX_CCM_CCGR(134), IMX_CCM_CCGR_GATE_RUN_WAIT);
    MmioWrite32((UINTN)IMX_CCM_CCGR(135), IMX_CCM_CCGR_GATE_RUN_WAIT);

    ImxPadConfig(IMX_PAD_GPIO1_IO03, IMX_PAD_PWM3_GPIO1_IO03);
}

typedef enum {

    // dts value 0x14 -- output
     IMX_PAD_GPIO1_IO01_SAI1_MCLK = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_0_X1,
        IMX_SRE_SLOW,
        IMX_HYS_DISABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_0_100K_PD,
        IMX_SION_DISABLED,
        IMX_IOMUXC_GPIO1_IO01_ALT3_SAI1_MCLK),

    // setup for WM8731 as slave

    // dts value 0x1f
    IMX_PAD_ENET1_TX_CLK_SAI1_RX_DATA0 = _IMX_MAKE_PADCFG_INPSEL(
        IMX_DSE_DSE_3_X6,
        IMX_SRE_SLOW,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_0_100K_PD,
        IMX_SION_DISABLED,
        IMX_IOMUXC_ENET1_TX_CLK_ALT2_SAI1_RX_DATA0,
        IOMUXC_SAI1_RX_DATA_SELECT_INPUT,
        1),

    // dts value 0x1f
    IMX_PAD_ENET1_RX_CLK_SAI1_TX_BCLK = _IMX_MAKE_PADCFG_INPSEL(
        IMX_DSE_DSE_3_X6,
        IMX_SRE_SLOW,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_0_100K_PD,
        IMX_SION_DISABLED,
        IMX_IOMUXC_ENET1_RX_CLK_ALT2_SAI1_TX_BCLK,
        IOMUXC_SAI1_TX_BCLK_SELECT_INPUT,
        1),

    // dts value 0x1f
    IMX_PAD_ENET1_CRS_SAI1_TX_SYNC = _IMX_MAKE_PADCFG_INPSEL(
        IMX_DSE_DSE_3_X6,
        IMX_SRE_SLOW,
        IMX_HYS_ENABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_0_100K_PD,
        IMX_SION_DISABLED,
        IMX_IOMUXC_ENET1_CRS_ALT2_SAI1_TX_SYNC,
        IOMUXC_SAI1_TX_SYNC_SELECT_INPUT,
        1),

    // dts value 0x30
    IMX_PAD_ENET1_COL_SAI1_TX_DATA0 = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_0_X1,
        IMX_SRE_FAST,
        IMX_HYS_DISABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_1_5K_PU,
        IMX_SION_DISABLED,
        IMX_IOMUXC_ENET1_COL_ALT2_SAI1_TX_DATA0),

    // dts value 0x14 -- output
    IMX_PAD_SD1_RESET_B_SAI3_MCLK = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_0_X1,
        IMX_SRE_SLOW,
        IMX_HYS_DISABLED,
        IMX_PE_1_Pull_Enabled,
        IMX_PS_0_100K_PD,
        IMX_SION_DISABLED,
        IMX_IOMUXC_SD1_RESET_B_ALT1_SAI3_MCLK),

} IMX_AUDIO_PADCFG;

/**
  Initalize the Audio
**/
VOID AudioInit ()
{
    EFI_STATUS status;

    // Mux the SAI1 pins to wm8731 codec
    ImxPadConfig(IMX_PAD_GPIO1_IO01, IMX_PAD_GPIO1_IO01_SAI1_MCLK);
    ImxPadConfig(IMX_PAD_ENET1_TX_CLK, IMX_PAD_ENET1_TX_CLK_SAI1_RX_DATA0);
    ImxPadConfig(IMX_PAD_ENET1_RX_CLK, IMX_PAD_ENET1_RX_CLK_SAI1_TX_BCLK);
    ImxPadConfig(IMX_PAD_ENET1_CRS, IMX_PAD_ENET1_CRS_SAI1_TX_SYNC);
    ImxPadConfig(IMX_PAD_ENET1_COL, IMX_PAD_ENET1_COL_SAI1_TX_DATA0);

#if 0
    // Mux the SAI3 MCLK to 100-mil P5 Pin 2 on Compulab SB-SOM 1.2
    ImxPadConfig(IMX_PAD_SD1_RESET_B, IMX_PAD_SD1_RESET_B_SAI3_MCLK);
#endif

    // enable the AUDIO_PLL - 44,100 Hz * 256
    status = ImxSetAudioMclkClockRate(11289600);
    if (EFI_ERROR(status)) {
        DebugPrint(DEBUG_ERROR, "AudioInit - ImxSetAudioMclkClockRate failed");
    }
}

/**
  Initialize controllers that must setup at the early stage
**/
RETURN_STATUS
ArmPlatformInitialize (
  IN  UINTN                     MpId
  )
{
    if (!ArmPlatformIsPrimaryCore (MpId)) {
        return RETURN_SUCCESS;
    }

    //
    // Fir ungate clocks that is needed but still disabled
    //
    ImxUngateActiveClock();

    //
    // Initialize debug serial port
    //
    SerialPortInitialize();
    SerialPortWrite(
        (UINT8 *)SERIAL_DEBUG_PORT_INIT_MSG,
        (UINTN)sizeof(SERIAL_DEBUG_PORT_INIT_MSG));

    //
    // Initialize peripherals
    //
    LcdifInit();
    PwmInit();
    AudioInit();

    return RETURN_SUCCESS;
}

/**
  Return the current Boot Mode

  This function returns the boot reason on the platform

**/
EFI_BOOT_MODE
ArmPlatformGetBootMode (
  VOID
  )
{
    return BOOT_WITH_FULL_CONFIGURATION;
}

