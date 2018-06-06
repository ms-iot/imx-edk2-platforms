/** @file
*
*  Header defining IMX7 Muxing
*
*  Copyright (c), Microsoft Corporation. All rights reserved.
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
#ifndef _IMX7_IOMUX_H_
#define _IMX7_IOMUX_H_

//
// IOMux common definition
//

#include <iMXIoMux.h>

//
// GPIO common definition
//

#include <iMXGpio.h>

//
// Pad control settings
//

typedef enum {
    IMX_PS_0_100K_PD,
    IMX_PS_1_5K_PU,
    IMX_PS_2_47K_PU,
    IMX_PS_3_100K_PU,
} IMX_PS;

typedef enum {
    IMX_PE_0_Pull_Disabled,
    IMX_PE_1_Pull_Enabled,
} IMX_PE;

typedef enum {
    IMX_HYS_DISABLED,
    IMX_HYS_ENABLED,
} IMX_HYS;

typedef enum {
    IMX_SRE_FAST,
    IMX_SRE_SLOW,
} IMX_SRE;

typedef enum {
    IMX_DSE_DSE_0_X1,
    IMX_DSE_DSE_1_X4,
    IMX_DSE_DSE_2_X2,
    IMX_DSE_DSE_3_X6,
} IMX_DSE;

typedef enum {
    IMX_SION_DISABLED,
    IMX_SION_ENABLED,
} IMX_IOMUXC_CTL_SION;

typedef union {
    UINT32 AsUint32;
    struct {
        UINT32 DSE : 2;
        UINT32 SRE : 1;
        UINT32 HYS : 1;
        UINT32 PE : 1;
        UINT32 PS : 2;
        UINT32 reserved : 25;
    } Fields;
} IMX_IOMUXC_PAD_CTL;

typedef union {
    UINT32 AsUint32;
    struct {
        UINT32 MUX_MODE : 4;
        UINT32 SION : 1;
        UINT32 reserved1 : 27;
    } Fields;
} IMX_IOMUXC_MUX_CTL;

typedef struct {
    UINT32 DAISY : 3;
    UINT32 reserved : 29;
} IMX_IOMUXC_SEL_INP_CTL;

#define _IMX_SEL_INP_VALUE(InpSel) \
          (((InpSel) >> 8) & 0x07)

#define _IMX_SEL_INP_REGISTER(InpSel) \
          ((((InpSel) & 0xFF) * 4) + IOMUXC_SELECT_INPUT_BASE_ADDRESS)

#define _IMX_MAKE_INP_SEL(InpSelReg, InpSelVal) \
          (((((InpSelReg) - IOMUXC_SELECT_INPUT_BASE_ADDRESS) / 4) & 0xFF) | \
          (((InpSelVal) & 0x7) << 8))

#define _IMX_MAKE_MUX_CTL(Sion, MuxAlt) \
          (((MuxAlt) & 0xF) | \
          (((Sion) & 0x1) << 4))

#define _IMX_MAKE_PAD_CTL(Dse, Sre, Hys, Pe, Ps) \
          (((Dse) & 0x3) | \
          (((Sre) & 0x1) << 2) | \
          (((Hys) & 0x1) << 3) | \
          (((Pe) & 0x1) << 4) | \
          (((Ps) & 0x3) << 5))

/**
  Define a configuration for a pad, including drive settings,
  MUX setting and Select Input setting and offset.

  Dse - IMX_DSE - Drive strength (Bit 0-1)
  Sre - IMX_SRE - Slew Rate setting (Bit 2)
  Hys - IMX_HYS - Hysteresis enable/disable (Bit 3)
  Pe - IMX_PE - Pull enabled filed (Bit 4)
  Ps - IMX_PS - Pull Select Field (Bit 5-6)

  Sion - Software Input on Field (Bit 0-2)
  MuxAlt- Alternate function number (Bit 4)

  SelInpReg - select input register offset div 4
  SelInpVal - select input value (Bit 0-2)

  Pack format
  <Select Input 12-22><Mux CTL Bit 7-11><Pad CTL Bit 0-6>

**/

#define IMX_PAD_CTL_PACK_OFFSET         0
#define IMX_MUX_CTL_PACK_OFFSET         7
#define IMX_SELECT_INPUT_PACK_OFFSET    12

#define _IMX_MAKE_PADCFG_INPSEL(Dse, Sre, Hys, Pe, Ps, Sion, MuxAlt, SelInpReg, SelInpValue) \
          (_IMX_MAKE_PAD_CTL(Dse, Sre, Hys, Pe, Ps) | \
          (_IMX_MAKE_MUX_CTL(Sion, MuxAlt) << IMX_MUX_CTL_PACK_OFFSET) | \
          (_IMX_MAKE_INP_SEL(SelInpReg, SelInpValue) << IMX_SELECT_INPUT_PACK_OFFSET))

#define _IMX_MAKE_PADCFG(Dse, Sre, Hys, Pe, Ps, Sion, MuxAlt) \
          (_IMX_MAKE_PAD_CTL(Dse, Sre, Hys, Pe, Ps) | \
           _IMX_MAKE_MUX_CTL(Sion, MuxAlt) << IMX_MUX_CTL_PACK_OFFSET)

#define _IMX_PADCFG_PAD_CTL(PadCfg) ((PadCfg) & 0x0000007F)
#define _IMX_PADCFG_MUX_CTL(PadCfg) (((PadCfg) >> IMX_MUX_CTL_PACK_OFFSET) & 0x00000017)
#define _IMX_PADCFG_SEL_INP(PadCfg) (((PadCfg) >> IMX_SELECT_INPUT_PACK_OFFSET) & 0x000007FF)

typedef UINT32 IMX_PADCFG;

typedef union {
    UINT32 AsUint32;
    struct {
        UINT32 MuxCtlRegOffset : 16;
        UINT32 PadCtlRegOffset : 16;
    } Fields;
} IMX_PAD_BITS;

typedef union {
    UINT32 AsUint32;
    struct {
        UINT32 DSE : 2;
        UINT32 SRE : 1;
        UINT32 HYS : 1;
        UINT32 PE : 1;
        UINT32 PS : 2;
        UINT32 MuxAlt : 3;
        UINT32 Reserved0 : 1;
        UINT32 Sion : 1;
        UINT32 InpSelReg : 8;
        UINT32 InpSel : 3;
        UINT32 Reserved1 : 9;
    } Fields;
} IMX_PADCFG_BITS;

/**

    IOMux configuration dump created from iMX7 documentation

**/

typedef enum {
    //
    // Pad Control Register (IOMUXC_LPSR_SW_PAD_CTL_PAD_GPIO1_IO00)
    // Pad Mux Register (IOMUXC_LPSR_SW_MUX_CTL_PAD_GPIO1_IO00)
    //
    IMX_PAD_GPIO1_IO00 = _IMX_PAD(0x30, 0x0), // GPIO1_IO0

    //
    // Pad Control Register (IOMUXC_LPSR_SW_PAD_CTL_PAD_GPIO1_IO01)
    // Pad Mux Register (IOMUXC_LPSR_SW_MUX_CTL_PAD_GPIO1_IO01)
    //
    IMX_PAD_GPIO1_IO01 = _IMX_PAD(0x34, 0x4), // GPIO1_IO1

    //
    // Pad Control Register (IOMUXC_LPSR_SW_PAD_CTL_PAD_GPIO1_IO02)
    // Pad Mux Register (IOMUXC_LPSR_SW_MUX_CTL_PAD_GPIO1_IO02)
    //
    IMX_PAD_GPIO1_IO02 = _IMX_PAD(0x38, 0x8), // GPIO1_IO2

    //
    // Pad Control Register (IOMUXC_LPSR_SW_PAD_CTL_PAD_GPIO1_IO03)
    // Pad Mux Register (IOMUXC_LPSR_SW_MUX_CTL_PAD_GPIO1_IO03)
    //
    IMX_PAD_GPIO1_IO03 = _IMX_PAD(0x3C, 0xC), // GPIO1_IO3

    //
    // Pad Control Register (IOMUXC_LPSR_SW_PAD_CTL_PAD_GPIO1_IO04)
    // Pad Mux Register (IOMUXC_LPSR_SW_MUX_CTL_PAD_GPIO1_IO04)
    //
    IMX_PAD_GPIO1_IO04 = _IMX_PAD(0x40, 0x10), // GPIO1_IO4

    //
    // Pad Control Register (IOMUXC_LPSR_SW_PAD_CTL_PAD_GPIO1_IO05)
    // Pad Mux Register (IOMUXC_LPSR_SW_MUX_CTL_PAD_GPIO1_IO05)
    //
    IMX_PAD_GPIO1_IO05 = _IMX_PAD(0x44, 0x14), // GPIO1_IO5

    //
    // Pad Control Register (IOMUXC_LPSR_SW_PAD_CTL_PAD_GPIO1_IO06)
    // Pad Mux Register (IOMUXC_LPSR_SW_MUX_CTL_PAD_GPIO1_IO06)
    //
    IMX_PAD_GPIO1_IO06 = _IMX_PAD(0x48, 0x18), // GPIO1_IO6

    //
    // Pad Control Register (IOMUXC_LPSR_SW_PAD_CTL_PAD_GPIO1_IO07)
    // Pad Mux Register (IOMUXC_LPSR_SW_MUX_CTL_PAD_GPIO1_IO07)
    //
    IMX_PAD_GPIO1_IO07 = _IMX_PAD(0x4C, 0x1C), // GPIO1_IO7

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO08)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO08)
    //
    IMX_PAD_GPIO1_IO08 = _IMX_PAD(0x26C, 0x14), // GPIO1_IO8

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO09)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO09)
    //
    IMX_PAD_GPIO1_IO09 = _IMX_PAD(0x270, 0x18), // GPIO1_IO9

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO10)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO10)
    //
    IMX_PAD_GPIO1_IO10 = _IMX_PAD(0x274, 0x1C), // GPIO1_IO10

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO11)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO11)
    //
    IMX_PAD_GPIO1_IO11 = _IMX_PAD(0x278, 0x20), // GPIO1_IO11

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO12)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO12)
    //
    IMX_PAD_GPIO1_IO12 = _IMX_PAD(0x27C, 0x24), // GPIO1_IO12

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO13)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO13)
    //
    IMX_PAD_GPIO1_IO13 = _IMX_PAD(0x280, 0x28), // GPIO1_IO13

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO14)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO14)
    //
    IMX_PAD_GPIO1_IO14 = _IMX_PAD(0x284, 0x2C), // GPIO1_IO14

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO15)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO15)
    //
    IMX_PAD_GPIO1_IO15 = _IMX_PAD(0x288, 0x30), // GPIO1_IO15

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA00)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA00)
    //
    IMX_PAD_EPDC_DATA00 = _IMX_PAD(0x2A4, 0x34), // EPDC_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA01)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA01)
    //
    IMX_PAD_EPDC_DATA01 = _IMX_PAD(0x2A8, 0x38), // EPDC_DATA1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA02)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA02)
    //
    IMX_PAD_EPDC_DATA02 = _IMX_PAD(0x2AC, 0x3C), // EPDC_DATA2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA03)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA03)
    //
    IMX_PAD_EPDC_DATA03 = _IMX_PAD(0x2B0, 0x40), // EPDC_DATA3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA04)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA04)
    //
    IMX_PAD_EPDC_DATA04 = _IMX_PAD(0x2B4, 0x44), // EPDC_DATA4

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA05)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA05)
    //
    IMX_PAD_EPDC_DATA05 = _IMX_PAD(0x2B8, 0x48), // EPDC_DATA5

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA06)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA06)
    //
    IMX_PAD_EPDC_DATA06 = _IMX_PAD(0x2BC, 0x4C), // EPDC_DATA6

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA07)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA07)
    //
    IMX_PAD_EPDC_DATA07 = _IMX_PAD(0x2C0, 0x50), // EPDC_DATA7

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA08)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA08)
    //
    IMX_PAD_EPDC_DATA08 = _IMX_PAD(0x2C4, 0x54), // EPDC_DATA8

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA09)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA09)
    //
    IMX_PAD_EPDC_DATA09 = _IMX_PAD(0x2C8, 0x58), // EPDC_DATA9

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA10)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA10)
    //
    IMX_PAD_EPDC_DATA10 = _IMX_PAD(0x2CC, 0x5C), // EPDC_DATA10

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA11)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA11)
    //
    IMX_PAD_EPDC_DATA11 = _IMX_PAD(0x2D0, 0x60), // EPDC_DATA11

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA12)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA12)
    //
    IMX_PAD_EPDC_DATA12 = _IMX_PAD(0x2D4, 0x64), // EPDC_DATA12

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA13)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA13)
    //
    IMX_PAD_EPDC_DATA13 = _IMX_PAD(0x2D8, 0x68), // EPDC_DATA13

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA14)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA14)
    //
    IMX_PAD_EPDC_DATA14 = _IMX_PAD(0x2DC, 0x6C), // EPDC_DATA14

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA15)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA15)
    //
    IMX_PAD_EPDC_DATA15 = _IMX_PAD(0x2E0, 0x70), // EPDC_DATA15

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_SDCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_SDCLK)
    //
    IMX_PAD_EPDC_SDCLK = _IMX_PAD(0x2E4, 0x74), // EPDC_SDCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_SDLE)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_SDLE)
    //
    IMX_PAD_EPDC_SDLE = _IMX_PAD(0x2E8, 0x78), // EPDC_SDLE

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_SDOE)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_SDOE)
    //
    IMX_PAD_EPDC_SDOE = _IMX_PAD(0x2EC, 0x7C), // EPDC_SDOE

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_SDSHR)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_SDSHR)
    //
    IMX_PAD_EPDC_SDSHR = _IMX_PAD(0x2F0, 0x80), // EPDC_SDSHR

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_SDCE0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_SDCE0)
    //
    IMX_PAD_EPDC_SDCE0 = _IMX_PAD(0x2F4, 0x84), // EPDC_SDCE0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_SDCE1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_SDCE1)
    //
    IMX_PAD_EPDC_SDCE1 = _IMX_PAD(0x2F8, 0x88), // EPDC_SDCE1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_SDCE2)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_SDCE2)
    //
    IMX_PAD_EPDC_SDCE2 = _IMX_PAD(0x2FC, 0x8C), // EPDC_SDCE2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_SDCE3)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_SDCE3)
    //
    IMX_PAD_EPDC_SDCE3 = _IMX_PAD(0x300, 0x90), // EPDC_SDCE3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_GDCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_GDCLK)
    //
    IMX_PAD_EPDC_GDCLK = _IMX_PAD(0x304, 0x94), // EPDC_GDCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_GDOE)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_GDOE)
    //
    IMX_PAD_EPDC_GDOE = _IMX_PAD(0x308, 0x98), // EPDC_GDOE

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_GDRL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_GDRL)
    //
    IMX_PAD_EPDC_GDRL = _IMX_PAD(0x30C, 0x9C), // EPDC_GDRL

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_GDSP)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_GDSP)
    //
    IMX_PAD_EPDC_GDSP = _IMX_PAD(0x310, 0xA0), // EPDC_GDSP

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_BDR0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_BDR0)
    //
    IMX_PAD_EPDC_BDR0 = _IMX_PAD(0x314, 0xA4), // EPDC_BDR0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_BDR1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_BDR1)
    //
    IMX_PAD_EPDC_BDR1 = _IMX_PAD(0x318, 0xA8), // EPDC_BDR1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_PWR_COM)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_PWR_COM)
    //
    IMX_PAD_EPDC_PWR_COM = _IMX_PAD(0x31C, 0xAC), // EPDC_PWR_COM

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_EPDC_PWR_STAT)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_EPDC_PWR_STAT)
    //
    IMX_PAD_EPDC_PWR_STAT = _IMX_PAD(0x320, 0xB0), // EPDC_PWR_STAT

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_CLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_CLK)
    //
    IMX_PAD_LCD_CLK = _IMX_PAD(0x324, 0xB4), // LCD_CLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_ENABLE)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_ENABLE)
    //
    IMX_PAD_LCD_ENABLE = _IMX_PAD(0x328, 0xB8), // LCD_ENABLE

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_HSYNC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_HSYNC)
    //
    IMX_PAD_LCD_HSYNC = _IMX_PAD(0x32C, 0xBC), // LCD_HSYNC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_VSYNC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_VSYNC)
    //
    IMX_PAD_LCD_VSYNC = _IMX_PAD(0x330, 0xC0), // LCD_VSYNC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_RESET)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_RESET)
    //
    IMX_PAD_LCD_RESET = _IMX_PAD(0x334, 0xC4), // LCD_RESET

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA00)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA00)
    //
    IMX_PAD_LCD_DATA00 = _IMX_PAD(0x338, 0xC8), // LCD_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA01)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA01)
    //
    IMX_PAD_LCD_DATA01 = _IMX_PAD(0x33C, 0xCC), // LCD_DATA1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA02)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA02)
    //
    IMX_PAD_LCD_DATA02 = _IMX_PAD(0x340, 0xD0), // LCD_DATA2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA03)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA03)
    //
    IMX_PAD_LCD_DATA03 = _IMX_PAD(0x344, 0xD4), // LCD_DATA3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA04)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA04)
    //
    IMX_PAD_LCD_DATA04 = _IMX_PAD(0x348, 0xD8), // LCD_DATA4

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA05)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA05)
    //
    IMX_PAD_LCD_DATA05 = _IMX_PAD(0x34C, 0xDC), // LCD_DATA5

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA06)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA06)
    //
    IMX_PAD_LCD_DATA06 = _IMX_PAD(0x350, 0xE0), // LCD_DATA6

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA07)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA07)
    //
    IMX_PAD_LCD_DATA07 = _IMX_PAD(0x354, 0xE4), // LCD_DATA7

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA08)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA08)
    //
    IMX_PAD_LCD_DATA08 = _IMX_PAD(0x358, 0xE8), // LCD_DATA8

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA09)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA09)
    //
    IMX_PAD_LCD_DATA09 = _IMX_PAD(0x35C, 0xEC), // LCD_DATA9

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA10)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA10)
    //
    IMX_PAD_LCD_DATA10 = _IMX_PAD(0x360, 0xF0), // LCD_DATA10

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA11)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA11)
    //
    IMX_PAD_LCD_DATA11 = _IMX_PAD(0x364, 0xF4), // LCD_DATA11

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA12)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA12)
    //
    IMX_PAD_LCD_DATA12 = _IMX_PAD(0x368, 0xF8), // LCD_DATA12

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA13)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA13)
    //
    IMX_PAD_LCD_DATA13 = _IMX_PAD(0x36C, 0xFC), // LCD_DATA13

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA14)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA14)
    //
    IMX_PAD_LCD_DATA14 = _IMX_PAD(0x370, 0x100), // LCD_DATA14

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA15)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA15)
    //
    IMX_PAD_LCD_DATA15 = _IMX_PAD(0x374, 0x104), // LCD_DATA15

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA16)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA16)
    //
    IMX_PAD_LCD_DATA16 = _IMX_PAD(0x378, 0x108), // LCD_DATA16

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA17)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA17)
    //
    IMX_PAD_LCD_DATA17 = _IMX_PAD(0x37C, 0x10C), // LCD_DATA17

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA18)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA18)
    //
    IMX_PAD_LCD_DATA18 = _IMX_PAD(0x380, 0x110), // LCD_DATA18

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA19)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA19)
    //
    IMX_PAD_LCD_DATA19 = _IMX_PAD(0x384, 0x114), // LCD_DATA19

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA20)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA20)
    //
    IMX_PAD_LCD_DATA20 = _IMX_PAD(0x388, 0x118), // LCD_DATA20

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA21)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA21)
    //
    IMX_PAD_LCD_DATA21 = _IMX_PAD(0x38C, 0x11C), // LCD_DATA21

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA22)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA22)
    //
    IMX_PAD_LCD_DATA22 = _IMX_PAD(0x390, 0x120), // LCD_DATA22

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_LCD_DATA23)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_LCD_DATA23)
    //
    IMX_PAD_LCD_DATA23 = _IMX_PAD(0x394, 0x124), // LCD_DATA23

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART1_RX_DATA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA)
    //
    IMX_PAD_UART1_RX_DATA = _IMX_PAD(0x398, 0x128), // UART1_RX_DATA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART1_TX_DATA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA)
    //
    IMX_PAD_UART1_TX_DATA = _IMX_PAD(0x39C, 0x12C), // UART1_TX_DATA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART2_RX_DATA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART2_RX_DATA)
    //
    IMX_PAD_UART2_RX_DATA = _IMX_PAD(0x3A0, 0x130), // UART2_RX_DATA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART2_TX_DATA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART2_TX_DATA)
    //
    IMX_PAD_UART2_TX_DATA = _IMX_PAD(0x3A4, 0x134), // UART2_TX_DATA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART3_RX_DATA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART3_RX_DATA)
    //
    IMX_PAD_UART3_RX_DATA = _IMX_PAD(0x3A8, 0x138), // UART3_RX_DATA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART3_TX_DATA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART3_TX_DATA)
    //
    IMX_PAD_UART3_TX_DATA = _IMX_PAD(0x3AC, 0x13C), // UART3_TX_DATA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART3_RTS_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART3_RTS_B)
    //
    IMX_PAD_UART3_RTS_B = _IMX_PAD(0x3B0, 0x140), // UART3_RTS_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART3_CTS_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART3_CTS_B)
    //
    IMX_PAD_UART3_CTS_B = _IMX_PAD(0x3B4, 0x144), // UART3_CTS_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C1_SCL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C1_SCL)
    //
    IMX_PAD_I2C1_SCL = _IMX_PAD(0x3B8, 0x148), // I2C1_SCL

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C1_SDA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C1_SDA)
    //
    IMX_PAD_I2C1_SDA = _IMX_PAD(0x3BC, 0x14C), // I2C1_SDA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C2_SCL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C2_SCL)
    //
    IMX_PAD_I2C2_SCL = _IMX_PAD(0x3C0, 0x150), // I2C2_SCL

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C2_SDA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C2_SDA)
    //
    IMX_PAD_I2C2_SDA = _IMX_PAD(0x3C4, 0x154), // I2C2_SDA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C3_SCL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C3_SCL)
    //
    IMX_PAD_I2C3_SCL = _IMX_PAD(0x3C8, 0x158), // I2C3_SCL

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C3_SDA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C3_SDA)
    //
    IMX_PAD_I2C3_SDA = _IMX_PAD(0x3CC, 0x15C), // I2C3_SDA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C4_SCL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C4_SCL)
    //
    IMX_PAD_I2C4_SCL = _IMX_PAD(0x3D0, 0x160), // I2C4_SCL

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_I2C4_SDA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_I2C4_SDA)
    //
    IMX_PAD_I2C4_SDA = _IMX_PAD(0x3D4, 0x164), // I2C4_SDA

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI1_SCLK)
    //
    IMX_PAD_ECSPI1_SCLK = _IMX_PAD(0x3D8, 0x168), // ECSPI1_SCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI1_MOSI)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI1_MOSI)
    //
    IMX_PAD_ECSPI1_MOSI = _IMX_PAD(0x3DC, 0x16C), // ECSPI1_MOSI

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI1_MISO)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI1_MISO)
    //
    IMX_PAD_ECSPI1_MISO = _IMX_PAD(0x3E0, 0x170), // ECSPI1_MISO

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SS0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI1_SS0)
    //
    IMX_PAD_ECSPI1_SS0 = _IMX_PAD(0x3E4, 0x174), // ECSPI1_SS0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI2_SCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI2_SCLK)
    //
    IMX_PAD_ECSPI2_SCLK = _IMX_PAD(0x3E8, 0x178), // ECSPI2_SCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI2_MOSI)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI2_MOSI)
    //
    IMX_PAD_ECSPI2_MOSI = _IMX_PAD(0x3EC, 0x17C), // ECSPI2_MOSI

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI2_MISO)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI2_MISO)
    //
    IMX_PAD_ECSPI2_MISO = _IMX_PAD(0x3F0, 0x180), // GPIO4_IO22

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ECSPI2_SS0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ECSPI2_SS0)
    //
    IMX_PAD_ECSPI2_SS0 = _IMX_PAD(0x3F4, 0x184), // ECSPI2_SS0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_CD_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_CD_B)
    //
    IMX_PAD_SD1_CD_B = _IMX_PAD(0x3F8, 0x188), // SD1_CD_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_WP)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_WP)
    //
    IMX_PAD_SD1_WP = _IMX_PAD(0x3FC, 0x18C), // SD1_WP

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_RESET_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_RESET_B)
    //
    IMX_PAD_SD1_RESET_B = _IMX_PAD(0x400, 0x190), // SD1_RESET_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_CLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_CLK)
    //
    IMX_PAD_SD1_CLK = _IMX_PAD(0x404, 0x194), // SD1_CLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_CMD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_CMD)
    //
    IMX_PAD_SD1_CMD = _IMX_PAD(0x408, 0x198), // SD1_CMD

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_DATA0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_DATA0)
    //
    IMX_PAD_SD1_DATA0 = _IMX_PAD(0x40C, 0x19C), // SD1_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_DATA1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_DATA1)
    //
    IMX_PAD_SD1_DATA1 = _IMX_PAD(0x410, 0x1A0), // SD1_DATA1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_DATA2)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_DATA2)
    //
    IMX_PAD_SD1_DATA2 = _IMX_PAD(0x414, 0x1A4), // SD1_DATA2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD1_DATA3)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD1_DATA3)
    //
    IMX_PAD_SD1_DATA3 = _IMX_PAD(0x418, 0x1A8), // SD1_DATA3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_CD_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_CD_B)
    //
    IMX_PAD_SD2_CD_B = _IMX_PAD(0x41C, 0x1AC), // SD2_CD_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_WP)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_WP)
    //
    IMX_PAD_SD2_WP = _IMX_PAD(0x420, 0x1B0), // SD2_WP

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_RESET_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_RESET_B)
    //
    IMX_PAD_SD2_RESET_B = _IMX_PAD(0x424, 0x1B4), // SD2_RESET_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_CLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_CLK)
    //
    IMX_PAD_SD2_CLK = _IMX_PAD(0x428, 0x1B8), // SD2_CLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_CMD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_CMD)
    //
    IMX_PAD_SD2_CMD = _IMX_PAD(0x42C, 0x1BC), // SD2_CMD

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_DATA0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_DATA0)
    //
    IMX_PAD_SD2_DATA0 = _IMX_PAD(0x430, 0x1C0), // SD2_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_DATA1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_DATA1)
    //
    IMX_PAD_SD2_DATA1 = _IMX_PAD(0x434, 0x1C4), // SD2_DATA1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_DATA2)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_DATA2)
    //
    IMX_PAD_SD2_DATA2 = _IMX_PAD(0x438, 0x1C8), // SD2_DATA2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD2_DATA3)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD2_DATA3)
    //
    IMX_PAD_SD2_DATA3 = _IMX_PAD(0x43C, 0x1CC), // SD2_DATA3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD3_CLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD3_CLK)
    //
    IMX_PAD_SD3_CLK = _IMX_PAD(0x440, 0x1D0), // SD3_CLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD3_CMD)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD3_CMD)
    //
    IMX_PAD_SD3_CMD = _IMX_PAD(0x444, 0x1D4), // SD3_CMD

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD3_DATA0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD3_DATA0)
    //
    IMX_PAD_SD3_DATA0 = _IMX_PAD(0x448, 0x1D8), // SD3_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD3_DATA1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD3_DATA1)
    //
    IMX_PAD_SD3_DATA1 = _IMX_PAD(0x44C, 0x1DC), // SD3_DATA1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD3_DATA2)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD3_DATA2)
    //
    IMX_PAD_SD3_DATA2 = _IMX_PAD(0x450, 0x1E0), // SD3_DATA2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD3_DATA3)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD3_DATA3)
    //
    IMX_PAD_SD3_DATA3 = _IMX_PAD(0x454, 0x1E4), // SD3_DATA3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD3_DATA4)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD3_DATA4)
    //
    IMX_PAD_SD3_DATA4 = _IMX_PAD(0x458, 0x1E8), // SD3_DATA4

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD3_DATA5)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD3_DATA5)
    //
    IMX_PAD_SD3_DATA5 = _IMX_PAD(0x45C, 0x1EC), // SD3_DATA5

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD3_DATA6)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD3_DATA6)
    //
    IMX_PAD_SD3_DATA6 = _IMX_PAD(0x460, 0x1F0), // SD3_DATA6

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD3_DATA7)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD3_DATA7)
    //
    IMX_PAD_SD3_DATA7 = _IMX_PAD(0x464, 0x1F4), // SD3_DATA7

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD3_STROBE)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD3_STROBE)
    //
    IMX_PAD_SD3_STROBE = _IMX_PAD(0x468, 0x1F8), // SD3_STROBE

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SD3_RESET_B)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SD3_RESET_B)
    //
    IMX_PAD_SD3_RESET_B = _IMX_PAD(0x46C, 0x1FC), // SD3_RESET_B

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_RX_DATA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_RX_DATA)
    //
    IMX_PAD_SAI1_RX_DATA = _IMX_PAD(0x470, 0x200), // SAI1_RX_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_TX_BCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_TX_BCLK)
    //
    IMX_PAD_SAI1_TX_BCLK = _IMX_PAD(0x474, 0x204), // SAI1_TX_BCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_TX_SYNC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_TX_SYNC)
    //
    IMX_PAD_SAI1_TX_SYNC = _IMX_PAD(0x478, 0x208), // SAI1_TX_SYNC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_TX_DATA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_TX_DATA)
    //
    IMX_PAD_SAI1_TX_DATA = _IMX_PAD(0x47C, 0x20C), // SAI1_TX_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_RX_SYNC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_RX_SYNC)
    //
    IMX_PAD_SAI1_RX_SYNC = _IMX_PAD(0x480, 0x210), // SAI1_RX_SYNC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_RX_BCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_RX_BCLK)
    //
    IMX_PAD_SAI1_RX_BCLK = _IMX_PAD(0x484, 0x214), // SAI1_RX_BCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI1_MCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI1_MCLK)
    //
    IMX_PAD_SAI1_MCLK = _IMX_PAD(0x488, 0x218), // SAI1_MCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI2_TX_SYNC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI2_TX_SYNC)
    //
    IMX_PAD_SAI2_TX_SYNC = _IMX_PAD(0x48C, 0x21C), // SAI2_TX_SYNC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI2_TX_BCLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI2_TX_BCLK)
    //
    IMX_PAD_SAI2_TX_BCLK = _IMX_PAD(0x490, 0x220), // SAI2_TX_BCLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI2_RX_DATA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI2_RX_DATA)
    //
    IMX_PAD_SAI2_RX_DATA = _IMX_PAD(0x494, 0x224), // SAI2_RX_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_SAI2_TX_DATA)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_SAI2_TX_DATA)
    //
    IMX_PAD_SAI2_TX_DATA = _IMX_PAD(0x498, 0x228), // SAI2_TX_DATA0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_RGMII_RD0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_RGMII_RD0)
    //
    IMX_PAD_ENET1_RGMII_RD0 = _IMX_PAD(0x49C, 0x22C), // ENET1_RGMII_RD0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_RGMII_RD1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_RGMII_RD1)
    //
    IMX_PAD_ENET1_RGMII_RD1 = _IMX_PAD(0x4A0, 0x230), // ENET1_RGMII_RD1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_RGMII_RD2)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_RGMII_RD2)
    //
    IMX_PAD_ENET1_RGMII_RD2 = _IMX_PAD(0x4A4, 0x234), // ENET1_RGMII_RD2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_RGMII_RD3)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_RGMII_RD3)
    //
    IMX_PAD_ENET1_RGMII_RD3 = _IMX_PAD(0x4A8, 0x238), // ENET1_RGMII_RD3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_RGMII_RX_CTL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_RGMII_RX_CTL)
    //
    IMX_PAD_ENET1_RGMII_RX_CTL = _IMX_PAD(0x4AC, 0x23C), // ENET1_RGMII_RX_CTL

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_RGMII_RXC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_RGMII_RXC)
    //
    IMX_PAD_ENET1_RGMII_RXC = _IMX_PAD(0x4B0, 0x240), // ENET1_RGMII_RXC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_RGMII_TD0)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_RGMII_TD0)
    //
    IMX_PAD_ENET1_RGMII_TD0 = _IMX_PAD(0x4B4, 0x244), // ENET1_RGMII_TD0

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_RGMII_TD1)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_RGMII_TD1)
    //
    IMX_PAD_ENET1_RGMII_TD1 = _IMX_PAD(0x4B8, 0x248), // ENET1_RGMII_TD1

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_RGMII_TD2)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_RGMII_TD2)
    //
    IMX_PAD_ENET1_RGMII_TD2 = _IMX_PAD(0x4BC, 0x24C), // ENET1_RGMII_TD2

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_RGMII_TD3)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_RGMII_TD3)
    //
    IMX_PAD_ENET1_RGMII_TD3 = _IMX_PAD(0x4C0, 0x250), // ENET1_RGMII_TD3

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_RGMII_TX_CTL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_RGMII_TX_CTL)
    //
    IMX_PAD_ENET1_RGMII_TX_CTL = _IMX_PAD(0x4C4, 0x254), // ENET1_RGMII_TX_CTL

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_RGMII_TXC)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_RGMII_TXC)
    //
    IMX_PAD_ENET1_RGMII_TXC = _IMX_PAD(0x4C8, 0x258), // ENET1_RGMII_TXC

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_TX_CLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_TX_CLK)
    //
    IMX_PAD_ENET1_TX_CLK = _IMX_PAD(0x4CC, 0x25C), // ENET1_TX_CLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_RX_CLK)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_RX_CLK)
    //
    IMX_PAD_ENET1_RX_CLK = _IMX_PAD(0x4D0, 0x260), // ENET1_RX_CLK

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_CRS)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_CRS)
    //
    IMX_PAD_ENET1_CRS = _IMX_PAD(0x4D4, 0x264), // ENET1_CRS

    //
    // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET1_COL)
    // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET1_COL)
    //
    IMX_PAD_ENET1_COL = _IMX_PAD(0x4D8, 0x268), // ENET1_COL

} IMX_PAD;

// Alternate function numbers
typedef enum {
    IMX_IOMUXC_GPIO1_IO00_ALT0_GPIO1_IO0 = 0,
    IMX_IOMUXC_GPIO1_IO00_ALT1_PWM4_OUT = 1,
    IMX_IOMUXC_GPIO1_IO00_ALT2_WDOG1_WDOG_ANY = 2,
    IMX_IOMUXC_GPIO1_IO00_ALT3_WDOG1_WDOG_B = 3,
    IMX_IOMUXC_GPIO1_IO00_ALT4_WDOG1_WDOG_RST_B_DEB = 4,
} IMX_IOMUXC_GPIO1_IO00_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO01_ALT0_GPIO1_IO1 = 0,
    IMX_IOMUXC_GPIO1_IO01_ALT1_PWM1_OUT = 1,
    IMX_IOMUXC_GPIO1_IO01_ALT2_CCM_ENET_REF_CLK3 = 2,
    IMX_IOMUXC_GPIO1_IO01_ALT3_SAI1_MCLK = 3,
} IMX_IOMUXC_GPIO1_IO01_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO02_ALT0_GPIO1_IO2 = 0,
    IMX_IOMUXC_GPIO1_IO02_ALT1_PWM2_OUT = 1,
    IMX_IOMUXC_GPIO1_IO02_ALT2_CCM_ENET_REF_CLK1 = 2,
    IMX_IOMUXC_GPIO1_IO02_ALT3_SAI2_MCLK = 3,
    IMX_IOMUXC_GPIO1_IO02_ALT5_CCM_CLKO1 = 5,
    IMX_IOMUXC_GPIO1_IO02_ALT7_USB_OTG1_ID = 7,
} IMX_IOMUXC_GPIO1_IO02_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO03_ALT0_GPIO1_IO3 = 0,
    IMX_IOMUXC_GPIO1_IO03_ALT1_PWM3_OUT = 1,
    IMX_IOMUXC_GPIO1_IO03_ALT2_CCM_ENET_REF_CLK2 = 2,
    IMX_IOMUXC_GPIO1_IO03_ALT3_SAI3_MCLK = 3,
    IMX_IOMUXC_GPIO1_IO03_ALT5_CCM_CLKO2 = 5,
    IMX_IOMUXC_GPIO1_IO03_ALT7_USB_OTG2_ID = 7,
} IMX_IOMUXC_GPIO1_IO03_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO04_ALT0_GPIO1_IO4 = 0,
    IMX_IOMUXC_GPIO1_IO04_ALT1_USB_OTG1_OC = 1,
    IMX_IOMUXC_GPIO1_IO04_ALT2_FLEXTIMER1_CH4 = 2,
    IMX_IOMUXC_GPIO1_IO04_ALT3_UART5_CTS_B = 3,
    IMX_IOMUXC_GPIO1_IO04_ALT4_I2C1_SCL = 4,
} IMX_IOMUXC_GPIO1_IO04_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO05_ALT0_GPIO1_IO5 = 0,
    IMX_IOMUXC_GPIO1_IO05_ALT1_USB_OTG1_PWR = 1,
    IMX_IOMUXC_GPIO1_IO05_ALT2_FLEXTIMER1_CH5 = 2,
    IMX_IOMUXC_GPIO1_IO05_ALT3_UART5_RTS_B = 3,
    IMX_IOMUXC_GPIO1_IO05_ALT4_I2C1_SDA = 4,
} IMX_IOMUXC_GPIO1_IO05_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO06_ALT0_GPIO1_IO6 = 0,
    IMX_IOMUXC_GPIO1_IO06_ALT1_USB_OTG2_OC = 1,
    IMX_IOMUXC_GPIO1_IO06_ALT2_FLEXTIMER1_CH6 = 2,
    IMX_IOMUXC_GPIO1_IO06_ALT3_UART5_RX_DATA = 3,
    IMX_IOMUXC_GPIO1_IO06_ALT4_I2C2_SCL = 4,
    IMX_IOMUXC_GPIO1_IO06_ALT5_CCM_WAIT = 5,
    IMX_IOMUXC_GPIO1_IO06_ALT6_KPP_ROW4 = 6,
} IMX_IOMUXC_GPIO1_IO06_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO07_ALT0_GPIO1_IO7 = 0,
    IMX_IOMUXC_GPIO1_IO07_ALT1_USB_OTG2_PWR = 1,
    IMX_IOMUXC_GPIO1_IO07_ALT2_FLEXTIMER1_CH7 = 2,
    IMX_IOMUXC_GPIO1_IO07_ALT3_UART5_TX_DATA = 3,
    IMX_IOMUXC_GPIO1_IO07_ALT4_I2C2_SDA = 4,
    IMX_IOMUXC_GPIO1_IO07_ALT5_CCM_STOP = 5,
    IMX_IOMUXC_GPIO1_IO07_ALT6_KPP_COL4 = 6,
} IMX_IOMUXC_GPIO1_IO07_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO08_ALT0_GPIO1_IO8 = 0,
    IMX_IOMUXC_GPIO1_IO08_ALT1_SD1_VSELECT = 1,
    IMX_IOMUXC_GPIO1_IO08_ALT2_WDOG1_WDOG_B = 2,
    IMX_IOMUXC_GPIO1_IO08_ALT3_UART3_RX_DATA = 3,
    IMX_IOMUXC_GPIO1_IO08_ALT4_I2C3_SCL = 4,
    IMX_IOMUXC_GPIO1_IO08_ALT6_KPP_COL5 = 6,
    IMX_IOMUXC_GPIO1_IO08_ALT7_PWM1_OUT = 7,
} IMX_IOMUXC_GPIO1_IO08_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO09_ALT0_GPIO1_IO9 = 0,
    IMX_IOMUXC_GPIO1_IO09_ALT1_SD1_LCTL = 1,
    IMX_IOMUXC_GPIO1_IO09_ALT2_CCM_ENET_PHY_REF_CLK = 2,
    IMX_IOMUXC_GPIO1_IO09_ALT3_UART3_TX_DATA = 3,
    IMX_IOMUXC_GPIO1_IO09_ALT4_I2C3_SDA = 4,
    IMX_IOMUXC_GPIO1_IO09_ALT5_CCM_PMIC_READY = 5,
    IMX_IOMUXC_GPIO1_IO09_ALT6_KPP_ROW5 = 6,
    IMX_IOMUXC_GPIO1_IO09_ALT7_PWM2_OUT = 7,
} IMX_IOMUXC_GPIO1_IO09_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO10_ALT0_GPIO1_IO10 = 0,
    IMX_IOMUXC_GPIO1_IO10_ALT1_SD2_LCTL = 1,
    IMX_IOMUXC_GPIO1_IO10_ALT2_ENET1_MDIO = 2,
    IMX_IOMUXC_GPIO1_IO10_ALT3_UART3_RTS_B = 3,
    IMX_IOMUXC_GPIO1_IO10_ALT4_I2C4_SCL = 4,
    IMX_IOMUXC_GPIO1_IO10_ALT5_FLEXTIMER1_PHA = 5,
    IMX_IOMUXC_GPIO1_IO10_ALT6_KPP_COL6 = 6,
    IMX_IOMUXC_GPIO1_IO10_ALT7_PWM3_OUT = 7,
} IMX_IOMUXC_GPIO1_IO10_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO11_ALT0_GPIO1_IO11 = 0,
    IMX_IOMUXC_GPIO1_IO11_ALT1_SD3_LCTL = 1,
    IMX_IOMUXC_GPIO1_IO11_ALT2_ENET1_MDC = 2,
    IMX_IOMUXC_GPIO1_IO11_ALT3_UART3_CTS_B = 3,
    IMX_IOMUXC_GPIO1_IO11_ALT4_I2C4_SDA = 4,
    IMX_IOMUXC_GPIO1_IO11_ALT5_FLEXTIMER1_PHB = 5,
    IMX_IOMUXC_GPIO1_IO11_ALT6_KPP_ROW6 = 6,
    IMX_IOMUXC_GPIO1_IO11_ALT7_PWM4_OUT = 7,
} IMX_IOMUXC_GPIO1_IO11_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO12_ALT0_GPIO1_IO12 = 0,
    IMX_IOMUXC_GPIO1_IO12_ALT1_SD2_VSELECT = 1,
    IMX_IOMUXC_GPIO1_IO12_ALT2_CCM_ENET1_REF_CLK = 2,
    IMX_IOMUXC_GPIO1_IO12_ALT3_FLEXCAN1_RX = 3,
    IMX_IOMUXC_GPIO1_IO12_ALT4_CM4_NMI = 4,
    IMX_IOMUXC_GPIO1_IO12_ALT5_CCM_EXT_CLK1 = 5,
    IMX_IOMUXC_GPIO1_IO12_ALT6_SNVS_VIO_5 = 6,
    IMX_IOMUXC_GPIO1_IO12_ALT7_USB_OTG1_ID = 7,
} IMX_IOMUXC_GPIO1_IO12_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO13_ALT0_GPIO1_IO13 = 0,
    IMX_IOMUXC_GPIO1_IO13_ALT1_SD3_VSELECT = 1,
    IMX_IOMUXC_GPIO1_IO13_ALT2_CCM_ENET2_REF_CLK = 2,
    IMX_IOMUXC_GPIO1_IO13_ALT3_FLEXCAN1_TX = 3,
    IMX_IOMUXC_GPIO1_IO13_ALT4_CCM_PMIC_READY = 4,
    IMX_IOMUXC_GPIO1_IO13_ALT5_CCM_EXT_CLK2 = 5,
    IMX_IOMUXC_GPIO1_IO13_ALT6_SNVS_VIO_5_CTL = 6,
    IMX_IOMUXC_GPIO1_IO13_ALT7_USB_OTG2_ID = 7,
} IMX_IOMUXC_GPIO1_IO13_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO14_ALT0_GPIO1_IO14 = 0,
    IMX_IOMUXC_GPIO1_IO14_ALT1_SD3_CD_B = 1,
    IMX_IOMUXC_GPIO1_IO14_ALT2_ENET2_MDIO = 2,
    IMX_IOMUXC_GPIO1_IO14_ALT3_FLEXCAN2_RX = 3,
    IMX_IOMUXC_GPIO1_IO14_ALT4_WDOG3_WDOG_B = 4,
    IMX_IOMUXC_GPIO1_IO14_ALT5_CCM_EXT_CLK3 = 5,
    IMX_IOMUXC_GPIO1_IO14_ALT6_SDMA_EXT_EVENT0 = 6,
} IMX_IOMUXC_GPIO1_IO14_ALT;

typedef enum {
    IMX_IOMUXC_GPIO1_IO15_ALT0_GPIO1_IO15 = 0,
    IMX_IOMUXC_GPIO1_IO15_ALT1_SD3_WP = 1,
    IMX_IOMUXC_GPIO1_IO15_ALT2_ENET2_MDC = 2,
    IMX_IOMUXC_GPIO1_IO15_ALT3_FLEXCAN2_TX = 3,
    IMX_IOMUXC_GPIO1_IO15_ALT4_WDOG4_WDOG_B = 4,
    IMX_IOMUXC_GPIO1_IO15_ALT5_CCM_EXT_CLK4 = 5,
    IMX_IOMUXC_GPIO1_IO15_ALT6_SDMA_EXT_EVENT1 = 6,
} IMX_IOMUXC_GPIO1_IO15_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA00_ALT0_EPDC_DATA0 = 0,
    IMX_IOMUXC_EPDC_DATA00_ALT1_SIM1_PORT2_TRXD = 1,
    IMX_IOMUXC_EPDC_DATA00_ALT2_QSPI_A_DATA0 = 2,
    IMX_IOMUXC_EPDC_DATA00_ALT3_KPP_ROW3 = 3,
    IMX_IOMUXC_EPDC_DATA00_ALT4_EIM_AD0 = 4,
    IMX_IOMUXC_EPDC_DATA00_ALT5_GPIO2_IO0 = 5,
    IMX_IOMUXC_EPDC_DATA00_ALT6_LCD_DATA0 = 6,
    IMX_IOMUXC_EPDC_DATA00_ALT7_LCD_CLK = 7,
} IMX_IOMUXC_EPDC_DATA00_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA01_ALT0_EPDC_DATA1 = 0,
    IMX_IOMUXC_EPDC_DATA01_ALT1_SIM1_PORT2_CLK = 1,
    IMX_IOMUXC_EPDC_DATA01_ALT2_QSPI_A_DATA1 = 2,
    IMX_IOMUXC_EPDC_DATA01_ALT3_KPP_COL3 = 3,
    IMX_IOMUXC_EPDC_DATA01_ALT4_EIM_AD1 = 4,
    IMX_IOMUXC_EPDC_DATA01_ALT5_GPIO2_IO1 = 5,
    IMX_IOMUXC_EPDC_DATA01_ALT6_LCD_DATA1 = 6,
    IMX_IOMUXC_EPDC_DATA01_ALT7_LCD_ENABLE = 7,
} IMX_IOMUXC_EPDC_DATA01_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA02_ALT0_EPDC_DATA2 = 0,
    IMX_IOMUXC_EPDC_DATA02_ALT1_SIM1_PORT2_RST_B = 1,
    IMX_IOMUXC_EPDC_DATA02_ALT2_QSPI_A_DATA2 = 2,
    IMX_IOMUXC_EPDC_DATA02_ALT3_KPP_ROW2 = 3,
    IMX_IOMUXC_EPDC_DATA02_ALT4_EIM_AD2 = 4,
    IMX_IOMUXC_EPDC_DATA02_ALT5_GPIO2_IO2 = 5,
    IMX_IOMUXC_EPDC_DATA02_ALT6_LCD_DATA2 = 6,
    IMX_IOMUXC_EPDC_DATA02_ALT7_LCD_VSYNC = 7,
} IMX_IOMUXC_EPDC_DATA02_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA03_ALT0_EPDC_DATA3 = 0,
    IMX_IOMUXC_EPDC_DATA03_ALT1_SIM1_PORT2_SVEN = 1,
    IMX_IOMUXC_EPDC_DATA03_ALT2_QSPI_A_DATA3 = 2,
    IMX_IOMUXC_EPDC_DATA03_ALT3_KPP_COL2 = 3,
    IMX_IOMUXC_EPDC_DATA03_ALT4_EIM_AD3 = 4,
    IMX_IOMUXC_EPDC_DATA03_ALT5_GPIO2_IO3 = 5,
    IMX_IOMUXC_EPDC_DATA03_ALT6_LCD_DATA3 = 6,
    IMX_IOMUXC_EPDC_DATA03_ALT7_LCD_HSYNC = 7,
} IMX_IOMUXC_EPDC_DATA03_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA04_ALT0_EPDC_DATA4 = 0,
    IMX_IOMUXC_EPDC_DATA04_ALT1_SIM1_PORT2_PD = 1,
    IMX_IOMUXC_EPDC_DATA04_ALT2_QSPI_A_DQS = 2,
    IMX_IOMUXC_EPDC_DATA04_ALT3_KPP_ROW1 = 3,
    IMX_IOMUXC_EPDC_DATA04_ALT4_EIM_AD4 = 4,
    IMX_IOMUXC_EPDC_DATA04_ALT5_GPIO2_IO4 = 5,
    IMX_IOMUXC_EPDC_DATA04_ALT6_LCD_DATA4 = 6,
    IMX_IOMUXC_EPDC_DATA04_ALT7_JTAG_FAIL = 7,
} IMX_IOMUXC_EPDC_DATA04_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA05_ALT0_EPDC_DATA5 = 0,
    IMX_IOMUXC_EPDC_DATA05_ALT1_SIM2_PORT2_TRXD = 1,
    IMX_IOMUXC_EPDC_DATA05_ALT2_QSPI_A_SCLK = 2,
    IMX_IOMUXC_EPDC_DATA05_ALT3_KPP_COL1 = 3,
    IMX_IOMUXC_EPDC_DATA05_ALT4_EIM_AD5 = 4,
    IMX_IOMUXC_EPDC_DATA05_ALT5_GPIO2_IO5 = 5,
    IMX_IOMUXC_EPDC_DATA05_ALT6_LCD_DATA5 = 6,
    IMX_IOMUXC_EPDC_DATA05_ALT7_JTAG_ACTIVE = 7,
} IMX_IOMUXC_EPDC_DATA05_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA06_ALT0_EPDC_DATA6 = 0,
    IMX_IOMUXC_EPDC_DATA06_ALT1_SIM2_PORT2_CLK = 1,
    IMX_IOMUXC_EPDC_DATA06_ALT2_QSPI_A_SS0_B = 2,
    IMX_IOMUXC_EPDC_DATA06_ALT3_KPP_ROW0 = 3,
    IMX_IOMUXC_EPDC_DATA06_ALT4_EIM_AD6 = 4,
    IMX_IOMUXC_EPDC_DATA06_ALT5_GPIO2_IO6 = 5,
    IMX_IOMUXC_EPDC_DATA06_ALT6_LCD_DATA6 = 6,
    IMX_IOMUXC_EPDC_DATA06_ALT7_JTAG_DE_B = 7,
} IMX_IOMUXC_EPDC_DATA06_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA07_ALT0_EPDC_DATA7 = 0,
    IMX_IOMUXC_EPDC_DATA07_ALT1_SIM2_PORT2_RST_B = 1,
    IMX_IOMUXC_EPDC_DATA07_ALT2_QSPI_A_SS1_B = 2,
    IMX_IOMUXC_EPDC_DATA07_ALT3_KPP_COL0 = 3,
    IMX_IOMUXC_EPDC_DATA07_ALT4_EIM_AD7 = 4,
    IMX_IOMUXC_EPDC_DATA07_ALT5_GPIO2_IO7 = 5,
    IMX_IOMUXC_EPDC_DATA07_ALT6_LCD_DATA7 = 6,
    IMX_IOMUXC_EPDC_DATA07_ALT7_JTAG_DONE = 7,
} IMX_IOMUXC_EPDC_DATA07_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA08_ALT0_EPDC_DATA8 = 0,
    IMX_IOMUXC_EPDC_DATA08_ALT1_SIM1_PORT1_TRXD = 1,
    IMX_IOMUXC_EPDC_DATA08_ALT2_QSPI_B_DATA0 = 2,
    IMX_IOMUXC_EPDC_DATA08_ALT3_UART6_RX_DATA = 3,
    IMX_IOMUXC_EPDC_DATA08_ALT4_EIM_OE = 4,
    IMX_IOMUXC_EPDC_DATA08_ALT5_GPIO2_IO8 = 5,
    IMX_IOMUXC_EPDC_DATA08_ALT6_LCD_DATA8 = 6,
    IMX_IOMUXC_EPDC_DATA08_ALT7_LCD_BUSY = 7,
    IMX_IOMUXC_EPDC_DATA08_ALT8_EPDC_SDCLK = 8,
} IMX_IOMUXC_EPDC_DATA08_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA09_ALT0_EPDC_DATA9 = 0,
    IMX_IOMUXC_EPDC_DATA09_ALT1_SIM1_PORT1_CLK = 1,
    IMX_IOMUXC_EPDC_DATA09_ALT2_QSPI_B_DATA1 = 2,
    IMX_IOMUXC_EPDC_DATA09_ALT3_UART6_TX_DATA = 3,
    IMX_IOMUXC_EPDC_DATA09_ALT4_EIM_RW = 4,
    IMX_IOMUXC_EPDC_DATA09_ALT5_GPIO2_IO9 = 5,
    IMX_IOMUXC_EPDC_DATA09_ALT6_LCD_DATA9 = 6,
    IMX_IOMUXC_EPDC_DATA09_ALT7_LCD_DATA0 = 7,
    IMX_IOMUXC_EPDC_DATA09_ALT8_EPDC_SDLE = 8,
} IMX_IOMUXC_EPDC_DATA09_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA10_ALT0_EPDC_DATA10 = 0,
    IMX_IOMUXC_EPDC_DATA10_ALT1_SIM1_PORT1_RST_B = 1,
    IMX_IOMUXC_EPDC_DATA10_ALT2_QSPI_B_DATA2 = 2,
    IMX_IOMUXC_EPDC_DATA10_ALT3_UART6_RTS_B = 3,
    IMX_IOMUXC_EPDC_DATA10_ALT4_EIM_CS0_B = 4,
    IMX_IOMUXC_EPDC_DATA10_ALT5_GPIO2_IO10 = 5,
    IMX_IOMUXC_EPDC_DATA10_ALT6_LCD_DATA10 = 6,
    IMX_IOMUXC_EPDC_DATA10_ALT7_LCD_DATA9 = 7,
    IMX_IOMUXC_EPDC_DATA10_ALT8_EPDC_SDOE = 8,
} IMX_IOMUXC_EPDC_DATA10_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA11_ALT0_EPDC_DATA11 = 0,
    IMX_IOMUXC_EPDC_DATA11_ALT1_SIM1_PORT1_SVEN = 1,
    IMX_IOMUXC_EPDC_DATA11_ALT2_QSPI_B_DATA3 = 2,
    IMX_IOMUXC_EPDC_DATA11_ALT3_UART6_CTS_B = 3,
    IMX_IOMUXC_EPDC_DATA11_ALT4_EIM_BCLK = 4,
    IMX_IOMUXC_EPDC_DATA11_ALT5_GPIO2_IO11 = 5,
    IMX_IOMUXC_EPDC_DATA11_ALT6_LCD_DATA11 = 6,
    IMX_IOMUXC_EPDC_DATA11_ALT7_LCD_DATA1 = 7,
    IMX_IOMUXC_EPDC_DATA11_ALT8_EPDC_SDCE0 = 8,
} IMX_IOMUXC_EPDC_DATA11_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA12_ALT0_EPDC_DATA12 = 0,
    IMX_IOMUXC_EPDC_DATA12_ALT1_SIM1_PORT1_PD = 1,
    IMX_IOMUXC_EPDC_DATA12_ALT2_QSPI_B_DQS = 2,
    IMX_IOMUXC_EPDC_DATA12_ALT3_UART7_RX_DATA = 3,
    IMX_IOMUXC_EPDC_DATA12_ALT4_EIM_LBA_B = 4,
    IMX_IOMUXC_EPDC_DATA12_ALT5_GPIO2_IO12 = 5,
    IMX_IOMUXC_EPDC_DATA12_ALT6_LCD_DATA12 = 6,
    IMX_IOMUXC_EPDC_DATA12_ALT7_LCD_DATA21 = 7,
    IMX_IOMUXC_EPDC_DATA12_ALT8_EPDC_GDCLK = 8,
} IMX_IOMUXC_EPDC_DATA12_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA13_ALT0_EPDC_DATA13 = 0,
    IMX_IOMUXC_EPDC_DATA13_ALT1_SIM2_PORT1_TRXD = 1,
    IMX_IOMUXC_EPDC_DATA13_ALT2_QSPI_B_SCLK = 2,
    IMX_IOMUXC_EPDC_DATA13_ALT3_UART7_TX_DATA = 3,
    IMX_IOMUXC_EPDC_DATA13_ALT4_EIM_WAIT = 4,
    IMX_IOMUXC_EPDC_DATA13_ALT5_GPIO2_IO13 = 5,
    IMX_IOMUXC_EPDC_DATA13_ALT6_LCD_DATA13 = 6,
    IMX_IOMUXC_EPDC_DATA13_ALT7_LCD_CS = 7,
    IMX_IOMUXC_EPDC_DATA13_ALT8_EPDC_GDOE = 8,
} IMX_IOMUXC_EPDC_DATA13_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA14_ALT0_EPDC_DATA14 = 0,
    IMX_IOMUXC_EPDC_DATA14_ALT1_SIM2_PORT1_CLK = 1,
    IMX_IOMUXC_EPDC_DATA14_ALT2_QSPI_B_SS0_B = 2,
    IMX_IOMUXC_EPDC_DATA14_ALT3_UART7_RTS_B = 3,
    IMX_IOMUXC_EPDC_DATA14_ALT4_EIM_EB_B0 = 4,
    IMX_IOMUXC_EPDC_DATA14_ALT5_GPIO2_IO14 = 5,
    IMX_IOMUXC_EPDC_DATA14_ALT6_LCD_DATA14 = 6,
    IMX_IOMUXC_EPDC_DATA14_ALT7_LCD_DATA22 = 7,
    IMX_IOMUXC_EPDC_DATA14_ALT8_EPDC_GDSP = 8,
} IMX_IOMUXC_EPDC_DATA14_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_DATA15_ALT0_EPDC_DATA15 = 0,
    IMX_IOMUXC_EPDC_DATA15_ALT1_SIM2_PORT1_RST_B = 1,
    IMX_IOMUXC_EPDC_DATA15_ALT2_QSPI_B_SS1_B = 2,
    IMX_IOMUXC_EPDC_DATA15_ALT3_UART7_CTS_B = 3,
    IMX_IOMUXC_EPDC_DATA15_ALT4_EIM_CS1_B = 4,
    IMX_IOMUXC_EPDC_DATA15_ALT5_GPIO2_IO15 = 5,
    IMX_IOMUXC_EPDC_DATA15_ALT6_LCD_DATA15 = 6,
    IMX_IOMUXC_EPDC_DATA15_ALT7_LCD_WR_RWN = 7,
    IMX_IOMUXC_EPDC_DATA15_ALT8_EPDC_PWR_COM = 8,
} IMX_IOMUXC_EPDC_DATA15_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_SDCLK_ALT0_EPDC_SDCLK = 0,
    IMX_IOMUXC_EPDC_SDCLK_ALT1_SIM2_PORT2_SVEN = 1,
    IMX_IOMUXC_EPDC_SDCLK_ALT2_ENET2_RGMII_RD0 = 2,
    IMX_IOMUXC_EPDC_SDCLK_ALT3_KPP_ROW4 = 3,
    IMX_IOMUXC_EPDC_SDCLK_ALT4_EIM_AD10 = 4,
    IMX_IOMUXC_EPDC_SDCLK_ALT5_GPIO2_IO16 = 5,
    IMX_IOMUXC_EPDC_SDCLK_ALT6_LCD_CLK = 6,
    IMX_IOMUXC_EPDC_SDCLK_ALT7_LCD_DATA20 = 7,
} IMX_IOMUXC_EPDC_SDCLK_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_SDLE_ALT0_EPDC_SDLE = 0,
    IMX_IOMUXC_EPDC_SDLE_ALT1_SIM2_PORT2_PD = 1,
    IMX_IOMUXC_EPDC_SDLE_ALT2_ENET2_RGMII_RD1 = 2,
    IMX_IOMUXC_EPDC_SDLE_ALT3_KPP_COL4 = 3,
    IMX_IOMUXC_EPDC_SDLE_ALT4_EIM_AD11 = 4,
    IMX_IOMUXC_EPDC_SDLE_ALT5_GPIO2_IO17 = 5,
    IMX_IOMUXC_EPDC_SDLE_ALT6_LCD_DATA16 = 6,
    IMX_IOMUXC_EPDC_SDLE_ALT7_LCD_DATA8 = 7,
} IMX_IOMUXC_EPDC_SDLE_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_SDOE_ALT0_EPDC_SDOE = 0,
    IMX_IOMUXC_EPDC_SDOE_ALT1_FLEXTIMER1_CH0 = 1,
    IMX_IOMUXC_EPDC_SDOE_ALT2_ENET2_RGMII_RD2 = 2,
    IMX_IOMUXC_EPDC_SDOE_ALT3_KPP_COL5 = 3,
    IMX_IOMUXC_EPDC_SDOE_ALT4_EIM_AD12 = 4,
    IMX_IOMUXC_EPDC_SDOE_ALT5_GPIO2_IO18 = 5,
    IMX_IOMUXC_EPDC_SDOE_ALT6_LCD_DATA17 = 6,
    IMX_IOMUXC_EPDC_SDOE_ALT7_LCD_DATA23 = 7,
} IMX_IOMUXC_EPDC_SDOE_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_SDSHR_ALT0_EPDC_SDSHR = 0,
    IMX_IOMUXC_EPDC_SDSHR_ALT1_FLEXTIMER1_CH1 = 1,
    IMX_IOMUXC_EPDC_SDSHR_ALT2_ENET2_RGMII_RD3 = 2,
    IMX_IOMUXC_EPDC_SDSHR_ALT3_KPP_ROW5 = 3,
    IMX_IOMUXC_EPDC_SDSHR_ALT4_EIM_AD13 = 4,
    IMX_IOMUXC_EPDC_SDSHR_ALT5_GPIO2_IO19 = 5,
    IMX_IOMUXC_EPDC_SDSHR_ALT6_LCD_DATA18 = 6,
    IMX_IOMUXC_EPDC_SDSHR_ALT7_LCD_DATA10 = 7,
} IMX_IOMUXC_EPDC_SDSHR_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_SDCE0_ALT0_EPDC_SDCE0 = 0,
    IMX_IOMUXC_EPDC_SDCE0_ALT1_FLEXTIMER1_CH2 = 1,
    IMX_IOMUXC_EPDC_SDCE0_ALT2_ENET2_RGMII_RX_CTL = 2,
    IMX_IOMUXC_EPDC_SDCE0_ALT4_EIM_AD14 = 4,
    IMX_IOMUXC_EPDC_SDCE0_ALT5_GPIO2_IO20 = 5,
    IMX_IOMUXC_EPDC_SDCE0_ALT6_LCD_DATA19 = 6,
    IMX_IOMUXC_EPDC_SDCE0_ALT7_LCD_DATA5 = 7,
} IMX_IOMUXC_EPDC_SDCE0_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_SDCE1_ALT0_EPDC_SDCE1 = 0,
    IMX_IOMUXC_EPDC_SDCE1_ALT1_FLEXTIMER1_CH3 = 1,
    IMX_IOMUXC_EPDC_SDCE1_ALT2_ENET2_RGMII_RXC = 2,
    IMX_IOMUXC_EPDC_SDCE1_ALT3_ENET2_RX_ER = 3,
    IMX_IOMUXC_EPDC_SDCE1_ALT4_EIM_AD15 = 4,
    IMX_IOMUXC_EPDC_SDCE1_ALT5_GPIO2_IO21 = 5,
    IMX_IOMUXC_EPDC_SDCE1_ALT6_LCD_DATA20 = 6,
    IMX_IOMUXC_EPDC_SDCE1_ALT7_LCD_DATA4 = 7,
} IMX_IOMUXC_EPDC_SDCE1_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_SDCE2_ALT0_EPDC_SDCE2 = 0,
    IMX_IOMUXC_EPDC_SDCE2_ALT1_SIM2_PORT1_SVEN = 1,
    IMX_IOMUXC_EPDC_SDCE2_ALT2_ENET2_RGMII_TD0 = 2,
    IMX_IOMUXC_EPDC_SDCE2_ALT3_KPP_COL6 = 3,
    IMX_IOMUXC_EPDC_SDCE2_ALT4_EIM_ADDR16 = 4,
    IMX_IOMUXC_EPDC_SDCE2_ALT5_GPIO2_IO22 = 5,
    IMX_IOMUXC_EPDC_SDCE2_ALT6_LCD_DATA21 = 6,
    IMX_IOMUXC_EPDC_SDCE2_ALT7_LCD_DATA3 = 7,
} IMX_IOMUXC_EPDC_SDCE2_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_SDCE3_ALT0_EPDC_SDCE3 = 0,
    IMX_IOMUXC_EPDC_SDCE3_ALT1_SIM2_PORT1_PD = 1,
    IMX_IOMUXC_EPDC_SDCE3_ALT2_ENET2_RGMII_TD1 = 2,
    IMX_IOMUXC_EPDC_SDCE3_ALT3_KPP_ROW6 = 3,
    IMX_IOMUXC_EPDC_SDCE3_ALT4_EIM_ADDR17 = 4,
    IMX_IOMUXC_EPDC_SDCE3_ALT5_GPIO2_IO23 = 5,
    IMX_IOMUXC_EPDC_SDCE3_ALT6_LCD_DATA22 = 6,
    IMX_IOMUXC_EPDC_SDCE3_ALT7_LCD_DATA2 = 7,
} IMX_IOMUXC_EPDC_SDCE3_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_GDCLK_ALT0_EPDC_GDCLK = 0,
    IMX_IOMUXC_EPDC_GDCLK_ALT1_FLEXTIMER2_CH0 = 1,
    IMX_IOMUXC_EPDC_GDCLK_ALT2_ENET2_RGMII_TD2 = 2,
    IMX_IOMUXC_EPDC_GDCLK_ALT3_KPP_COL7 = 3,
    IMX_IOMUXC_EPDC_GDCLK_ALT4_EIM_ADDR18 = 4,
    IMX_IOMUXC_EPDC_GDCLK_ALT5_GPIO2_IO24 = 5,
    IMX_IOMUXC_EPDC_GDCLK_ALT6_LCD_DATA23 = 6,
    IMX_IOMUXC_EPDC_GDCLK_ALT7_LCD_DATA16 = 7,
} IMX_IOMUXC_EPDC_GDCLK_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_GDOE_ALT0_EPDC_GDOE = 0,
    IMX_IOMUXC_EPDC_GDOE_ALT1_FLEXTIMER2_CH1 = 1,
    IMX_IOMUXC_EPDC_GDOE_ALT2_ENET2_RGMII_TD3 = 2,
    IMX_IOMUXC_EPDC_GDOE_ALT3_KPP_ROW7 = 3,
    IMX_IOMUXC_EPDC_GDOE_ALT4_EIM_ADDR19 = 4,
    IMX_IOMUXC_EPDC_GDOE_ALT5_GPIO2_IO25 = 5,
    IMX_IOMUXC_EPDC_GDOE_ALT6_LCD_WR_RWN = 6,
    IMX_IOMUXC_EPDC_GDOE_ALT7_LCD_DATA18 = 7,
} IMX_IOMUXC_EPDC_GDOE_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_GDRL_ALT0_EPDC_GDRL = 0,
    IMX_IOMUXC_EPDC_GDRL_ALT1_FLEXTIMER2_CH2 = 1,
    IMX_IOMUXC_EPDC_GDRL_ALT2_ENET2_RGMII_TX_CTL = 2,
    IMX_IOMUXC_EPDC_GDRL_ALT4_EIM_ADDR20 = 4,
    IMX_IOMUXC_EPDC_GDRL_ALT5_GPIO2_IO26 = 5,
    IMX_IOMUXC_EPDC_GDRL_ALT6_LCD_RD_E = 6,
    IMX_IOMUXC_EPDC_GDRL_ALT7_LCD_DATA19 = 7,
} IMX_IOMUXC_EPDC_GDRL_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_GDSP_ALT0_EPDC_GDSP = 0,
    IMX_IOMUXC_EPDC_GDSP_ALT1_FLEXTIMER2_CH3 = 1,
    IMX_IOMUXC_EPDC_GDSP_ALT2_ENET2_RGMII_TXC = 2,
    IMX_IOMUXC_EPDC_GDSP_ALT3_ENET2_TX_ER = 3,
    IMX_IOMUXC_EPDC_GDSP_ALT4_EIM_ADDR21 = 4,
    IMX_IOMUXC_EPDC_GDSP_ALT5_GPIO2_IO27 = 5,
    IMX_IOMUXC_EPDC_GDSP_ALT6_LCD_BUSY = 6,
    IMX_IOMUXC_EPDC_GDSP_ALT7_LCD_DATA17 = 7,
} IMX_IOMUXC_EPDC_GDSP_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_BDR0_ALT0_EPDC_BDR0 = 0,
    IMX_IOMUXC_EPDC_BDR0_ALT2_ENET2_TX_CLK = 2,
    IMX_IOMUXC_EPDC_BDR0_ALT3_CCM_ENET2_REF_CLK = 3,
    IMX_IOMUXC_EPDC_BDR0_ALT4_EIM_ADDR22 = 4,
    IMX_IOMUXC_EPDC_BDR0_ALT5_GPIO2_IO28 = 5,
    IMX_IOMUXC_EPDC_BDR0_ALT6_LCD_CS = 6,
    IMX_IOMUXC_EPDC_BDR0_ALT7_LCD_DATA7 = 7,
} IMX_IOMUXC_EPDC_BDR0_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_BDR1_ALT0_EPDC_BDR1 = 0,
    IMX_IOMUXC_EPDC_BDR1_ALT1_EPDC_SDCLKN = 1,
    IMX_IOMUXC_EPDC_BDR1_ALT2_ENET2_RX_CLK = 2,
    IMX_IOMUXC_EPDC_BDR1_ALT4_EIM_AD8 = 4,
    IMX_IOMUXC_EPDC_BDR1_ALT5_GPIO2_IO29 = 5,
    IMX_IOMUXC_EPDC_BDR1_ALT6_LCD_ENABLE = 6,
    IMX_IOMUXC_EPDC_BDR1_ALT7_LCD_DATA6 = 7,
} IMX_IOMUXC_EPDC_BDR1_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_PWR_COM_ALT0_EPDC_PWR_COM = 0,
    IMX_IOMUXC_EPDC_PWR_COM_ALT1_FLEXTIMER2_PHA = 1,
    IMX_IOMUXC_EPDC_PWR_COM_ALT2_ENET2_CRS = 2,
    IMX_IOMUXC_EPDC_PWR_COM_ALT4_EIM_AD9 = 4,
    IMX_IOMUXC_EPDC_PWR_COM_ALT5_GPIO2_IO30 = 5,
    IMX_IOMUXC_EPDC_PWR_COM_ALT6_LCD_HSYNC = 6,
    IMX_IOMUXC_EPDC_PWR_COM_ALT7_LCD_DATA11 = 7,
} IMX_IOMUXC_EPDC_PWR_COM_ALT;

typedef enum {
    IMX_IOMUXC_EPDC_PWR_STAT_ALT0_EPDC_PWR_STAT = 0,
    IMX_IOMUXC_EPDC_PWR_STAT_ALT1_FLEXTIMER2_PHB = 1,
    IMX_IOMUXC_EPDC_PWR_STAT_ALT2_ENET2_COL = 2,
    IMX_IOMUXC_EPDC_PWR_STAT_ALT4_EIM_EB_B1 = 4,
    IMX_IOMUXC_EPDC_PWR_STAT_ALT5_GPIO2_IO31 = 5,
    IMX_IOMUXC_EPDC_PWR_STAT_ALT6_LCD_VSYNC = 6,
    IMX_IOMUXC_EPDC_PWR_STAT_ALT7_LCD_DATA12 = 7,
} IMX_IOMUXC_EPDC_PWR_STAT_ALT;

typedef enum {
    IMX_IOMUXC_LCD_CLK_ALT0_LCD_CLK = 0,
    IMX_IOMUXC_LCD_CLK_ALT1_ECSPI4_MISO = 1,
    IMX_IOMUXC_LCD_CLK_ALT2_ENET1_1588_EVENT2_IN = 2,
    IMX_IOMUXC_LCD_CLK_ALT3_CSI_DATA16 = 3,
    IMX_IOMUXC_LCD_CLK_ALT4_UART2_RX_DATA = 4,
    IMX_IOMUXC_LCD_CLK_ALT5_GPIO3_IO0 = 5,
} IMX_IOMUXC_LCD_CLK_ALT;

typedef enum {
    IMX_IOMUXC_LCD_ENABLE_ALT0_LCD_ENABLE = 0,
    IMX_IOMUXC_LCD_ENABLE_ALT1_ECSPI4_MOSI = 1,
    IMX_IOMUXC_LCD_ENABLE_ALT2_ENET1_1588_EVENT3_IN = 2,
    IMX_IOMUXC_LCD_ENABLE_ALT3_CSI_DATA17 = 3,
    IMX_IOMUXC_LCD_ENABLE_ALT4_UART2_TX_DATA = 4,
    IMX_IOMUXC_LCD_ENABLE_ALT5_GPIO3_IO1 = 5,
} IMX_IOMUXC_LCD_ENABLE_ALT;

typedef enum {
    IMX_IOMUXC_LCD_HSYNC_ALT0_LCD_HSYNC = 0,
    IMX_IOMUXC_LCD_HSYNC_ALT1_ECSPI4_SCLK = 1,
    IMX_IOMUXC_LCD_HSYNC_ALT2_ENET2_1588_EVENT2_IN = 2,
    IMX_IOMUXC_LCD_HSYNC_ALT3_CSI_DATA18 = 3,
    IMX_IOMUXC_LCD_HSYNC_ALT4_UART2_RTS_B = 4,
    IMX_IOMUXC_LCD_HSYNC_ALT5_GPIO3_IO2 = 5,
} IMX_IOMUXC_LCD_HSYNC_ALT;

typedef enum {
    IMX_IOMUXC_LCD_VSYNC_ALT0_LCD_VSYNC = 0,
    IMX_IOMUXC_LCD_VSYNC_ALT1_ECSPI4_SS0 = 1,
    IMX_IOMUXC_LCD_VSYNC_ALT2_ENET2_1588_EVENT3_IN = 2,
    IMX_IOMUXC_LCD_VSYNC_ALT3_CSI_DATA19 = 3,
    IMX_IOMUXC_LCD_VSYNC_ALT4_UART2_CTS_B = 4,
    IMX_IOMUXC_LCD_VSYNC_ALT5_GPIO3_IO3 = 5,
} IMX_IOMUXC_LCD_VSYNC_ALT;

typedef enum {
    IMX_IOMUXC_LCD_RESET_ALT0_LCD_RESET = 0,
    IMX_IOMUXC_LCD_RESET_ALT1_GPT1_COMPARE1 = 1,
    IMX_IOMUXC_LCD_RESET_ALT2_ARM_PLATFORM_EVENTI = 2,
    IMX_IOMUXC_LCD_RESET_ALT3_CSI_FIELD = 3,
    IMX_IOMUXC_LCD_RESET_ALT4_EIM_DTACK_B = 4,
    IMX_IOMUXC_LCD_RESET_ALT5_GPIO3_IO4 = 5,
} IMX_IOMUXC_LCD_RESET_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA00_ALT0_LCD_DATA0 = 0,
    IMX_IOMUXC_LCD_DATA00_ALT1_GPT1_COMPARE2 = 1,
    IMX_IOMUXC_LCD_DATA00_ALT3_CSI_DATA20 = 3,
    IMX_IOMUXC_LCD_DATA00_ALT4_EIM_DATA0 = 4,
    IMX_IOMUXC_LCD_DATA00_ALT5_GPIO3_IO5 = 5,
    IMX_IOMUXC_LCD_DATA00_ALT6_SRC_BOOT_CFG0 = 6,
} IMX_IOMUXC_LCD_DATA00_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA01_ALT0_LCD_DATA1 = 0,
    IMX_IOMUXC_LCD_DATA01_ALT1_GPT1_COMPARE3 = 1,
    IMX_IOMUXC_LCD_DATA01_ALT3_CSI_DATA21 = 3,
    IMX_IOMUXC_LCD_DATA01_ALT4_EIM_DATA1 = 4,
    IMX_IOMUXC_LCD_DATA01_ALT5_GPIO3_IO6 = 5,
    IMX_IOMUXC_LCD_DATA01_ALT6_SRC_BOOT_CFG1 = 6,
} IMX_IOMUXC_LCD_DATA01_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA02_ALT0_LCD_DATA2 = 0,
    IMX_IOMUXC_LCD_DATA02_ALT1_GPT1_CLK = 1,
    IMX_IOMUXC_LCD_DATA02_ALT3_CSI_DATA22 = 3,
    IMX_IOMUXC_LCD_DATA02_ALT4_EIM_DATA2 = 4,
    IMX_IOMUXC_LCD_DATA02_ALT5_GPIO3_IO7 = 5,
    IMX_IOMUXC_LCD_DATA02_ALT6_SRC_BOOT_CFG2 = 6,
} IMX_IOMUXC_LCD_DATA02_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA03_ALT0_LCD_DATA3 = 0,
    IMX_IOMUXC_LCD_DATA03_ALT1_GPT1_CAPTURE1 = 1,
    IMX_IOMUXC_LCD_DATA03_ALT3_CSI_DATA23 = 3,
    IMX_IOMUXC_LCD_DATA03_ALT4_EIM_DATA3 = 4,
    IMX_IOMUXC_LCD_DATA03_ALT5_GPIO3_IO8 = 5,
    IMX_IOMUXC_LCD_DATA03_ALT6_SRC_BOOT_CFG3 = 6,
} IMX_IOMUXC_LCD_DATA03_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA04_ALT0_LCD_DATA4 = 0,
    IMX_IOMUXC_LCD_DATA04_ALT1_GPT1_CAPTURE2 = 1,
    IMX_IOMUXC_LCD_DATA04_ALT3_CSI_VSYNC = 3,
    IMX_IOMUXC_LCD_DATA04_ALT4_EIM_DATA4 = 4,
    IMX_IOMUXC_LCD_DATA04_ALT5_GPIO3_IO9 = 5,
    IMX_IOMUXC_LCD_DATA04_ALT6_SRC_BOOT_CFG4 = 6,
} IMX_IOMUXC_LCD_DATA04_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA05_ALT0_LCD_DATA5 = 0,
    IMX_IOMUXC_LCD_DATA05_ALT3_CSI_HSYNC = 3,
    IMX_IOMUXC_LCD_DATA05_ALT4_EIM_DATA5 = 4,
    IMX_IOMUXC_LCD_DATA05_ALT5_GPIO3_IO10 = 5,
    IMX_IOMUXC_LCD_DATA05_ALT6_SRC_BOOT_CFG5 = 6,
} IMX_IOMUXC_LCD_DATA05_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA06_ALT0_LCD_DATA6 = 0,
    IMX_IOMUXC_LCD_DATA06_ALT3_CSI_PIXCLK = 3,
    IMX_IOMUXC_LCD_DATA06_ALT4_EIM_DATA6 = 4,
    IMX_IOMUXC_LCD_DATA06_ALT5_GPIO3_IO11 = 5,
    IMX_IOMUXC_LCD_DATA06_ALT6_SRC_BOOT_CFG6 = 6,
} IMX_IOMUXC_LCD_DATA06_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA07_ALT0_LCD_DATA7 = 0,
    IMX_IOMUXC_LCD_DATA07_ALT3_CSI_MCLK = 3,
    IMX_IOMUXC_LCD_DATA07_ALT4_EIM_DATA7 = 4,
    IMX_IOMUXC_LCD_DATA07_ALT5_GPIO3_IO12 = 5,
    IMX_IOMUXC_LCD_DATA07_ALT6_SRC_BOOT_CFG7 = 6,
} IMX_IOMUXC_LCD_DATA07_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA08_ALT0_LCD_DATA8 = 0,
    IMX_IOMUXC_LCD_DATA08_ALT3_CSI_DATA9 = 3,
    IMX_IOMUXC_LCD_DATA08_ALT4_EIM_DATA8 = 4,
    IMX_IOMUXC_LCD_DATA08_ALT5_GPIO3_IO13 = 5,
    IMX_IOMUXC_LCD_DATA08_ALT6_SRC_BOOT_CFG8 = 6,
} IMX_IOMUXC_LCD_DATA08_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA09_ALT0_LCD_DATA9 = 0,
    IMX_IOMUXC_LCD_DATA09_ALT3_CSI_DATA8 = 3,
    IMX_IOMUXC_LCD_DATA09_ALT4_EIM_DATA9 = 4,
    IMX_IOMUXC_LCD_DATA09_ALT5_GPIO3_IO14 = 5,
    IMX_IOMUXC_LCD_DATA09_ALT6_SRC_BOOT_CFG9 = 6,
} IMX_IOMUXC_LCD_DATA09_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA10_ALT0_LCD_DATA10 = 0,
    IMX_IOMUXC_LCD_DATA10_ALT3_CSI_DATA7 = 3,
    IMX_IOMUXC_LCD_DATA10_ALT4_EIM_DATA10 = 4,
    IMX_IOMUXC_LCD_DATA10_ALT5_GPIO3_IO15 = 5,
    IMX_IOMUXC_LCD_DATA10_ALT6_SRC_BOOT_CFG10 = 6,
} IMX_IOMUXC_LCD_DATA10_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA11_ALT0_LCD_DATA11 = 0,
    IMX_IOMUXC_LCD_DATA11_ALT3_CSI_DATA6 = 3,
    IMX_IOMUXC_LCD_DATA11_ALT4_EIM_DATA11 = 4,
    IMX_IOMUXC_LCD_DATA11_ALT5_GPIO3_IO16 = 5,
    IMX_IOMUXC_LCD_DATA11_ALT6_SRC_BOOT_CFG11 = 6,
} IMX_IOMUXC_LCD_DATA11_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA12_ALT0_LCD_DATA12 = 0,
    IMX_IOMUXC_LCD_DATA12_ALT3_CSI_DATA5 = 3,
    IMX_IOMUXC_LCD_DATA12_ALT4_EIM_DATA12 = 4,
    IMX_IOMUXC_LCD_DATA12_ALT5_GPIO3_IO17 = 5,
    IMX_IOMUXC_LCD_DATA12_ALT6_SRC_BOOT_CFG12 = 6,
} IMX_IOMUXC_LCD_DATA12_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA13_ALT0_LCD_DATA13 = 0,
    IMX_IOMUXC_LCD_DATA13_ALT3_CSI_DATA4 = 3,
    IMX_IOMUXC_LCD_DATA13_ALT4_EIM_DATA13 = 4,
    IMX_IOMUXC_LCD_DATA13_ALT5_GPIO3_IO18 = 5,
    IMX_IOMUXC_LCD_DATA13_ALT6_SRC_BOOT_CFG13 = 6,
} IMX_IOMUXC_LCD_DATA13_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA14_ALT0_LCD_DATA14 = 0,
    IMX_IOMUXC_LCD_DATA14_ALT3_CSI_DATA3 = 3,
    IMX_IOMUXC_LCD_DATA14_ALT4_EIM_DATA14 = 4,
    IMX_IOMUXC_LCD_DATA14_ALT5_GPIO3_IO19 = 5,
    IMX_IOMUXC_LCD_DATA14_ALT6_SRC_BOOT_CFG14 = 6,
} IMX_IOMUXC_LCD_DATA14_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA15_ALT0_LCD_DATA15 = 0,
    IMX_IOMUXC_LCD_DATA15_ALT3_CSI_DATA2 = 3,
    IMX_IOMUXC_LCD_DATA15_ALT4_EIM_DATA15 = 4,
    IMX_IOMUXC_LCD_DATA15_ALT5_GPIO3_IO20 = 5,
    IMX_IOMUXC_LCD_DATA15_ALT6_SRC_BOOT_CFG15 = 6,
} IMX_IOMUXC_LCD_DATA15_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA16_ALT0_LCD_DATA16 = 0,
    IMX_IOMUXC_LCD_DATA16_ALT1_FLEXTIMER1_CH4 = 1,
    IMX_IOMUXC_LCD_DATA16_ALT3_CSI_DATA1 = 3,
    IMX_IOMUXC_LCD_DATA16_ALT4_EIM_CRE = 4,
    IMX_IOMUXC_LCD_DATA16_ALT5_GPIO3_IO21 = 5,
    IMX_IOMUXC_LCD_DATA16_ALT6_SRC_BOOT_CFG16 = 6,
} IMX_IOMUXC_LCD_DATA16_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA17_ALT0_LCD_DATA17 = 0,
    IMX_IOMUXC_LCD_DATA17_ALT1_FLEXTIMER1_CH5 = 1,
    IMX_IOMUXC_LCD_DATA17_ALT3_CSI_DATA0 = 3,
    IMX_IOMUXC_LCD_DATA17_ALT4_EIM_ACLK_FREERUN = 4,
    IMX_IOMUXC_LCD_DATA17_ALT5_GPIO3_IO22 = 5,
    IMX_IOMUXC_LCD_DATA17_ALT6_SRC_BOOT_CFG17 = 6,
} IMX_IOMUXC_LCD_DATA17_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA18_ALT0_LCD_DATA18 = 0,
    IMX_IOMUXC_LCD_DATA18_ALT1_FLEXTIMER1_CH6 = 1,
    IMX_IOMUXC_LCD_DATA18_ALT2_ARM_PLATFORM_EVENTO = 2,
    IMX_IOMUXC_LCD_DATA18_ALT3_CSI_DATA15 = 3,
    IMX_IOMUXC_LCD_DATA18_ALT4_EIM_CS2_B = 4,
    IMX_IOMUXC_LCD_DATA18_ALT5_GPIO3_IO23 = 5,
    IMX_IOMUXC_LCD_DATA18_ALT6_SRC_BOOT_CFG18 = 6,
} IMX_IOMUXC_LCD_DATA18_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA19_ALT0_LCD_DATA19 = 0,
    IMX_IOMUXC_LCD_DATA19_ALT1_FLEXTIMER1_CH7 = 1,
    IMX_IOMUXC_LCD_DATA19_ALT3_CSI_DATA14 = 3,
    IMX_IOMUXC_LCD_DATA19_ALT4_EIM_CS3_B = 4,
    IMX_IOMUXC_LCD_DATA19_ALT5_GPIO3_IO24 = 5,
    IMX_IOMUXC_LCD_DATA19_ALT6_SRC_BOOT_CFG19 = 6,
} IMX_IOMUXC_LCD_DATA19_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA20_ALT0_LCD_DATA20 = 0,
    IMX_IOMUXC_LCD_DATA20_ALT1_FLEXTIMER2_CH4 = 1,
    IMX_IOMUXC_LCD_DATA20_ALT2_ENET1_1588_EVENT2_OUT = 2,
    IMX_IOMUXC_LCD_DATA20_ALT3_CSI_DATA13 = 3,
    IMX_IOMUXC_LCD_DATA20_ALT4_EIM_ADDR23 = 4,
    IMX_IOMUXC_LCD_DATA20_ALT5_GPIO3_IO25 = 5,
    IMX_IOMUXC_LCD_DATA20_ALT6_I2C3_SCL = 6,
} IMX_IOMUXC_LCD_DATA20_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA21_ALT0_LCD_DATA21 = 0,
    IMX_IOMUXC_LCD_DATA21_ALT1_FLEXTIMER2_CH5 = 1,
    IMX_IOMUXC_LCD_DATA21_ALT2_ENET1_1588_EVENT3_OUT = 2,
    IMX_IOMUXC_LCD_DATA21_ALT3_CSI_DATA12 = 3,
    IMX_IOMUXC_LCD_DATA21_ALT4_EIM_ADDR24 = 4,
    IMX_IOMUXC_LCD_DATA21_ALT5_GPIO3_IO26 = 5,
    IMX_IOMUXC_LCD_DATA21_ALT6_I2C3_SDA = 6,
} IMX_IOMUXC_LCD_DATA21_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA22_ALT0_LCD_DATA22 = 0,
    IMX_IOMUXC_LCD_DATA22_ALT1_FLEXTIMER2_CH6 = 1,
    IMX_IOMUXC_LCD_DATA22_ALT2_ENET2_1588_EVENT2_OUT = 2,
    IMX_IOMUXC_LCD_DATA22_ALT3_CSI_DATA11 = 3,
    IMX_IOMUXC_LCD_DATA22_ALT4_EIM_ADDR25 = 4,
    IMX_IOMUXC_LCD_DATA22_ALT5_GPIO3_IO27 = 5,
    IMX_IOMUXC_LCD_DATA22_ALT6_I2C4_SCL = 6,
} IMX_IOMUXC_LCD_DATA22_ALT;

typedef enum {
    IMX_IOMUXC_LCD_DATA23_ALT0_LCD_DATA23 = 0,
    IMX_IOMUXC_LCD_DATA23_ALT1_FLEXTIMER2_CH7 = 1,
    IMX_IOMUXC_LCD_DATA23_ALT2_ENET2_1588_EVENT3_OUT = 2,
    IMX_IOMUXC_LCD_DATA23_ALT3_CSI_DATA10 = 3,
    IMX_IOMUXC_LCD_DATA23_ALT4_EIM_ADDR26 = 4,
    IMX_IOMUXC_LCD_DATA23_ALT5_GPIO3_IO28 = 5,
    IMX_IOMUXC_LCD_DATA23_ALT6_I2C4_SDA = 6,
} IMX_IOMUXC_LCD_DATA23_ALT;

typedef enum {
    IMX_IOMUXC_UART1_RX_DATA_ALT0_UART1_RX_DATA = 0,
    IMX_IOMUXC_UART1_RX_DATA_ALT1_I2C1_SCL = 1,
    IMX_IOMUXC_UART1_RX_DATA_ALT2_CCM_PMIC_READY = 2,
    IMX_IOMUXC_UART1_RX_DATA_ALT3_ECSPI1_SS1 = 3,
    IMX_IOMUXC_UART1_RX_DATA_ALT4_ENET2_1588_EVENT0_IN = 4,
    IMX_IOMUXC_UART1_RX_DATA_ALT5_GPIO4_IO0 = 5,
    IMX_IOMUXC_UART1_RX_DATA_ALT6_ENET1_MDIO = 6,
} IMX_IOMUXC_UART1_RX_DATA_ALT;

typedef enum {
    IMX_IOMUXC_UART1_TX_DATA_ALT0_UART1_TX_DATA = 0,
    IMX_IOMUXC_UART1_TX_DATA_ALT1_I2C1_SDA = 1,
    IMX_IOMUXC_UART1_TX_DATA_ALT2_SAI3_MCLK = 2,
    IMX_IOMUXC_UART1_TX_DATA_ALT3_ECSPI1_SS2 = 3,
    IMX_IOMUXC_UART1_TX_DATA_ALT4_ENET2_1588_EVENT0_OUT = 4,
    IMX_IOMUXC_UART1_TX_DATA_ALT5_GPIO4_IO1 = 5,
    IMX_IOMUXC_UART1_TX_DATA_ALT6_ENET1_MDC = 6,
} IMX_IOMUXC_UART1_TX_DATA_ALT;

typedef enum {
    IMX_IOMUXC_UART2_RX_DATA_ALT0_UART2_RX_DATA = 0,
    IMX_IOMUXC_UART2_RX_DATA_ALT1_I2C2_SCL = 1,
    IMX_IOMUXC_UART2_RX_DATA_ALT2_SAI3_RX_BCLK = 2,
    IMX_IOMUXC_UART2_RX_DATA_ALT3_ECSPI1_SS3 = 3,
    IMX_IOMUXC_UART2_RX_DATA_ALT4_ENET2_1588_EVENT1_IN = 4,
    IMX_IOMUXC_UART2_RX_DATA_ALT5_GPIO4_IO2 = 5,
    IMX_IOMUXC_UART2_RX_DATA_ALT6_ENET2_MDIO = 6,
} IMX_IOMUXC_UART2_RX_DATA_ALT;

typedef enum {
    IMX_IOMUXC_UART2_TX_DATA_ALT0_UART2_TX_DATA = 0,
    IMX_IOMUXC_UART2_TX_DATA_ALT1_I2C2_SDA = 1,
    IMX_IOMUXC_UART2_TX_DATA_ALT2_SAI3_RX_DATA0 = 2,
    IMX_IOMUXC_UART2_TX_DATA_ALT3_ECSPI1_RDY = 3,
    IMX_IOMUXC_UART2_TX_DATA_ALT4_ENET2_1588_EVENT1_OUT = 4,
    IMX_IOMUXC_UART2_TX_DATA_ALT5_GPIO4_IO3 = 5,
    IMX_IOMUXC_UART2_TX_DATA_ALT6_ENET2_MDC = 6,
} IMX_IOMUXC_UART2_TX_DATA_ALT;

typedef enum {
    IMX_IOMUXC_UART3_RX_DATA_ALT0_UART3_RX_DATA = 0,
    IMX_IOMUXC_UART3_RX_DATA_ALT1_USB_OTG1_OC = 1,
    IMX_IOMUXC_UART3_RX_DATA_ALT2_SAI3_RX_SYNC = 2,
    IMX_IOMUXC_UART3_RX_DATA_ALT3_ECSPI1_MISO = 3,
    IMX_IOMUXC_UART3_RX_DATA_ALT4_ENET1_1588_EVENT0_IN = 4,
    IMX_IOMUXC_UART3_RX_DATA_ALT5_GPIO4_IO4 = 5,
    IMX_IOMUXC_UART3_RX_DATA_ALT6_SD1_LCTL = 6,
} IMX_IOMUXC_UART3_RX_DATA_ALT;

typedef enum {
    IMX_IOMUXC_UART3_TX_DATA_ALT0_UART3_TX_DATA = 0,
    IMX_IOMUXC_UART3_TX_DATA_ALT1_USB_OTG1_PWR = 1,
    IMX_IOMUXC_UART3_TX_DATA_ALT2_SAI3_TX_BCLK = 2,
    IMX_IOMUXC_UART3_TX_DATA_ALT3_ECSPI1_MOSI = 3,
    IMX_IOMUXC_UART3_TX_DATA_ALT4_ENET1_1588_EVENT0_OUT = 4,
    IMX_IOMUXC_UART3_TX_DATA_ALT5_GPIO4_IO5 = 5,
    IMX_IOMUXC_UART3_TX_DATA_ALT6_SD2_LCTL = 6,
} IMX_IOMUXC_UART3_TX_DATA_ALT;

typedef enum {
    IMX_IOMUXC_UART3_RTS_B_ALT0_UART3_RTS_B = 0,
    IMX_IOMUXC_UART3_RTS_B_ALT1_USB_OTG2_OC = 1,
    IMX_IOMUXC_UART3_RTS_B_ALT2_SAI3_TX_DATA0 = 2,
    IMX_IOMUXC_UART3_RTS_B_ALT3_ECSPI1_SCLK = 3,
    IMX_IOMUXC_UART3_RTS_B_ALT4_ENET1_1588_EVENT1_IN = 4,
    IMX_IOMUXC_UART3_RTS_B_ALT5_GPIO4_IO6 = 5,
    IMX_IOMUXC_UART3_RTS_B_ALT6_SD3_LCTL = 6,
} IMX_IOMUXC_UART3_RTS_B_ALT;

typedef enum {
    IMX_IOMUXC_UART3_CTS_B_ALT0_UART3_CTS_B = 0,
    IMX_IOMUXC_UART3_CTS_B_ALT1_USB_OTG2_PWR = 1,
    IMX_IOMUXC_UART3_CTS_B_ALT2_SAI3_TX_SYNC = 2,
    IMX_IOMUXC_UART3_CTS_B_ALT3_ECSPI1_SS0 = 3,
    IMX_IOMUXC_UART3_CTS_B_ALT4_ENET1_1588_EVENT1_OUT = 4,
    IMX_IOMUXC_UART3_CTS_B_ALT5_GPIO4_IO7 = 5,
    IMX_IOMUXC_UART3_CTS_B_ALT6_SD1_VSELECT = 6,
} IMX_IOMUXC_UART3_CTS_B_ALT;

typedef enum {
    IMX_IOMUXC_I2C1_SCL_ALT0_I2C1_SCL = 0,
    IMX_IOMUXC_I2C1_SCL_ALT1_UART4_CTS_B = 1,
    IMX_IOMUXC_I2C1_SCL_ALT2_FLEXCAN1_RX = 2,
    IMX_IOMUXC_I2C1_SCL_ALT3_ECSPI3_MISO = 3,
    IMX_IOMUXC_I2C1_SCL_ALT5_GPIO4_IO8 = 5,
    IMX_IOMUXC_I2C1_SCL_ALT6_SD2_VSELECT = 6,
} IMX_IOMUXC_I2C1_SCL_ALT;

typedef enum {
    IMX_IOMUXC_I2C1_SDA_ALT0_I2C1_SDA = 0,
    IMX_IOMUXC_I2C1_SDA_ALT1_UART4_RTS_B = 1,
    IMX_IOMUXC_I2C1_SDA_ALT2_FLEXCAN1_TX = 2,
    IMX_IOMUXC_I2C1_SDA_ALT3_ECSPI3_MOSI = 3,
    IMX_IOMUXC_I2C1_SDA_ALT4_CCM_ENET1_REF_CLK = 4,
    IMX_IOMUXC_I2C1_SDA_ALT5_GPIO4_IO9 = 5,
    IMX_IOMUXC_I2C1_SDA_ALT6_SD3_VSELECT = 6,
} IMX_IOMUXC_I2C1_SDA_ALT;

typedef enum {
    IMX_IOMUXC_I2C2_SCL_ALT0_I2C2_SCL = 0,
    IMX_IOMUXC_I2C2_SCL_ALT1_UART4_RX_DATA = 1,
    IMX_IOMUXC_I2C2_SCL_ALT2_WDOG3_WDOG_B = 2,
    IMX_IOMUXC_I2C2_SCL_ALT3_ECSPI3_SCLK = 3,
    IMX_IOMUXC_I2C2_SCL_ALT4_CCM_ENET2_REF_CLK = 4,
    IMX_IOMUXC_I2C2_SCL_ALT5_GPIO4_IO10 = 5,
    IMX_IOMUXC_I2C2_SCL_ALT6_SD3_CD_B = 6,
} IMX_IOMUXC_I2C2_SCL_ALT;

typedef enum {
    IMX_IOMUXC_I2C2_SDA_ALT0_I2C2_SDA = 0,
    IMX_IOMUXC_I2C2_SDA_ALT1_UART4_TX_DATA = 1,
    IMX_IOMUXC_I2C2_SDA_ALT2_WDOG3_WDOG_RST_B_DEB = 2,
    IMX_IOMUXC_I2C2_SDA_ALT3_ECSPI3_SS0 = 3,
    IMX_IOMUXC_I2C2_SDA_ALT4_CCM_ENET_PHY_REF_CLK = 4,
    IMX_IOMUXC_I2C2_SDA_ALT5_GPIO4_IO11 = 5,
    IMX_IOMUXC_I2C2_SDA_ALT6_SD3_WP = 6,
} IMX_IOMUXC_I2C2_SDA_ALT;

typedef enum {
    IMX_IOMUXC_I2C3_SCL_ALT0_I2C3_SCL = 0,
    IMX_IOMUXC_I2C3_SCL_ALT1_UART5_CTS_B = 1,
    IMX_IOMUXC_I2C3_SCL_ALT2_FLEXCAN2_RX = 2,
    IMX_IOMUXC_I2C3_SCL_ALT3_CSI_VSYNC = 3,
    IMX_IOMUXC_I2C3_SCL_ALT4_SDMA_EXT_EVENT0 = 4,
    IMX_IOMUXC_I2C3_SCL_ALT5_GPIO4_IO12 = 5,
    IMX_IOMUXC_I2C3_SCL_ALT6_EPDC_BDR0 = 6,
} IMX_IOMUXC_I2C3_SCL_ALT;

typedef enum {
    IMX_IOMUXC_I2C3_SDA_ALT0_I2C3_SDA = 0,
    IMX_IOMUXC_I2C3_SDA_ALT1_UART5_RTS_B = 1,
    IMX_IOMUXC_I2C3_SDA_ALT2_FLEXCAN2_TX = 2,
    IMX_IOMUXC_I2C3_SDA_ALT3_CSI_HSYNC = 3,
    IMX_IOMUXC_I2C3_SDA_ALT4_SDMA_EXT_EVENT1 = 4,
    IMX_IOMUXC_I2C3_SDA_ALT5_GPIO4_IO13 = 5,
    IMX_IOMUXC_I2C3_SDA_ALT6_EPDC_BDR1 = 6,
} IMX_IOMUXC_I2C3_SDA_ALT;

typedef enum {
    IMX_IOMUXC_I2C4_SCL_ALT0_I2C4_SCL = 0,
    IMX_IOMUXC_I2C4_SCL_ALT1_UART5_RX_DATA = 1,
    IMX_IOMUXC_I2C4_SCL_ALT2_WDOG4_WDOG_B = 2,
    IMX_IOMUXC_I2C4_SCL_ALT3_CSI_PIXCLK = 3,
    IMX_IOMUXC_I2C4_SCL_ALT4_USB_OTG1_ID = 4,
    IMX_IOMUXC_I2C4_SCL_ALT5_GPIO4_IO14 = 5,
    IMX_IOMUXC_I2C4_SCL_ALT6_EPDC_VCOM0 = 6,
} IMX_IOMUXC_I2C4_SCL_ALT;

typedef enum {
    IMX_IOMUXC_I2C4_SDA_ALT0_I2C4_SDA = 0,
    IMX_IOMUXC_I2C4_SDA_ALT1_UART5_TX_DATA = 1,
    IMX_IOMUXC_I2C4_SDA_ALT2_WDOG4_WDOG_RST_B_DEB = 2,
    IMX_IOMUXC_I2C4_SDA_ALT3_CSI_MCLK = 3,
    IMX_IOMUXC_I2C4_SDA_ALT4_USB_OTG2_ID = 4,
    IMX_IOMUXC_I2C4_SDA_ALT5_GPIO4_IO15 = 5,
    IMX_IOMUXC_I2C4_SDA_ALT6_EPDC_VCOM1 = 6,
} IMX_IOMUXC_I2C4_SDA_ALT;

typedef enum {
    IMX_IOMUXC_ECSPI1_SCLK_ALT0_ECSPI1_SCLK = 0,
    IMX_IOMUXC_ECSPI1_SCLK_ALT1_UART6_RX_DATA = 1,
    IMX_IOMUXC_ECSPI1_SCLK_ALT2_SD2_DATA4 = 2,
    IMX_IOMUXC_ECSPI1_SCLK_ALT3_CSI_DATA2 = 3,
    IMX_IOMUXC_ECSPI1_SCLK_ALT5_GPIO4_IO16 = 5,
    IMX_IOMUXC_ECSPI1_SCLK_ALT6_EPDC_PWR_COM = 6,
} IMX_IOMUXC_ECSPI1_SCLK_ALT;

typedef enum {
    IMX_IOMUXC_ECSPI1_MOSI_ALT0_ECSPI1_MOSI = 0,
    IMX_IOMUXC_ECSPI1_MOSI_ALT1_UART6_TX_DATA = 1,
    IMX_IOMUXC_ECSPI1_MOSI_ALT2_SD2_DATA5 = 2,
    IMX_IOMUXC_ECSPI1_MOSI_ALT3_CSI_DATA3 = 3,
    IMX_IOMUXC_ECSPI1_MOSI_ALT5_GPIO4_IO17 = 5,
    IMX_IOMUXC_ECSPI1_MOSI_ALT6_EPDC_PWR_STAT = 6,
} IMX_IOMUXC_ECSPI1_MOSI_ALT;

typedef enum {
    IMX_IOMUXC_ECSPI1_MISO_ALT0_ECSPI1_MISO = 0,
    IMX_IOMUXC_ECSPI1_MISO_ALT1_UART6_RTS_B = 1,
    IMX_IOMUXC_ECSPI1_MISO_ALT2_SD2_DATA6 = 2,
    IMX_IOMUXC_ECSPI1_MISO_ALT3_CSI_DATA4 = 3,
    IMX_IOMUXC_ECSPI1_MISO_ALT5_GPIO4_IO18 = 5,
    IMX_IOMUXC_ECSPI1_MISO_ALT6_EPDC_PWR_IRQ = 6,
} IMX_IOMUXC_ECSPI1_MISO_ALT;

typedef enum {
    IMX_IOMUXC_ECSPI1_SS0_ALT0_ECSPI1_SS0 = 0,
    IMX_IOMUXC_ECSPI1_SS0_ALT1_UART6_CTS_B = 1,
    IMX_IOMUXC_ECSPI1_SS0_ALT2_SD2_DATA7 = 2,
    IMX_IOMUXC_ECSPI1_SS0_ALT3_CSI_DATA5 = 3,
    IMX_IOMUXC_ECSPI1_SS0_ALT5_GPIO4_IO19 = 5,
    IMX_IOMUXC_ECSPI1_SS0_ALT6_EPDC_PWR_CTRL3 = 6,
} IMX_IOMUXC_ECSPI1_SS0_ALT;

typedef enum {
    IMX_IOMUXC_ECSPI2_SCLK_ALT0_ECSPI2_SCLK = 0,
    IMX_IOMUXC_ECSPI2_SCLK_ALT1_UART7_RX_DATA = 1,
    IMX_IOMUXC_ECSPI2_SCLK_ALT2_SD1_DATA4 = 2,
    IMX_IOMUXC_ECSPI2_SCLK_ALT3_CSI_DATA6 = 3,
    IMX_IOMUXC_ECSPI2_SCLK_ALT4_LCD_DATA13 = 4,
    IMX_IOMUXC_ECSPI2_SCLK_ALT5_GPIO4_IO20 = 5,
    IMX_IOMUXC_ECSPI2_SCLK_ALT6_EPDC_PWR_CTRL0 = 6,
} IMX_IOMUXC_ECSPI2_SCLK_ALT;

typedef enum {
    IMX_IOMUXC_ECSPI2_MOSI_ALT0_ECSPI2_MOSI = 0,
    IMX_IOMUXC_ECSPI2_MOSI_ALT1_UART7_TX_DATA = 1,
    IMX_IOMUXC_ECSPI2_MOSI_ALT2_SD1_DATA5 = 2,
    IMX_IOMUXC_ECSPI2_MOSI_ALT3_CSI_DATA7 = 3,
    IMX_IOMUXC_ECSPI2_MOSI_ALT4_LCD_DATA14 = 4,
    IMX_IOMUXC_ECSPI2_MOSI_ALT5_GPIO4_IO21 = 5,
    IMX_IOMUXC_ECSPI2_MOSI_ALT6_EPDC_PWR_CTRL1 = 6,
} IMX_IOMUXC_ECSPI2_MOSI_ALT;

typedef enum {
    IMX_IOMUXC_ECSPI2_MISO_ALT5_GPIO4_IO22 = 5,
    IMX_IOMUXC_ECSPI2_MISO_ALT6_EPDC_PWR_CTRL2 = 6,
    IMX_IOMUXC_ECSPI2_MISO_ALT0_ECSPI2_MISO = 0,
    IMX_IOMUXC_ECSPI2_MISO_ALT1_UART7_RTS_B = 1,
    IMX_IOMUXC_ECSPI2_MISO_ALT2_SD1_DATA6 = 2,
    IMX_IOMUXC_ECSPI2_MISO_ALT3_CSI_DATA8 = 3,
    IMX_IOMUXC_ECSPI2_MISO_ALT4_LCD_DATA15 = 4,
} IMX_IOMUXC_ECSPI2_MISO_ALT;

typedef enum {
    IMX_IOMUXC_ECSPI2_SS0_ALT0_ECSPI2_SS0 = 0,
    IMX_IOMUXC_ECSPI2_SS0_ALT1_UART7_CTS_B = 1,
    IMX_IOMUXC_ECSPI2_SS0_ALT2_SD1_DATA7 = 2,
    IMX_IOMUXC_ECSPI2_SS0_ALT3_CSI_DATA9 = 3,
    IMX_IOMUXC_ECSPI2_SS0_ALT4_LCD_RESET = 4,
    IMX_IOMUXC_ECSPI2_SS0_ALT5_GPIO4_IO23 = 5,
    IMX_IOMUXC_ECSPI2_SS0_ALT6_EPDC_PWR_WAKE = 6,
} IMX_IOMUXC_ECSPI2_SS0_ALT;

typedef enum {
    IMX_IOMUXC_SD1_CD_B_ALT0_SD1_CD_B = 0,
    IMX_IOMUXC_SD1_CD_B_ALT2_UART6_RX_DATA = 2,
    IMX_IOMUXC_SD1_CD_B_ALT3_ECSPI4_MISO = 3,
    IMX_IOMUXC_SD1_CD_B_ALT4_FLEXTIMER1_CH0 = 4,
    IMX_IOMUXC_SD1_CD_B_ALT5_GPIO5_IO0 = 5,
    IMX_IOMUXC_SD1_CD_B_ALT6_CCM_CLKO1 = 6,
} IMX_IOMUXC_SD1_CD_B_ALT;

typedef enum {
    IMX_IOMUXC_SD1_WP_ALT0_SD1_WP = 0,
    IMX_IOMUXC_SD1_WP_ALT2_UART6_TX_DATA = 2,
    IMX_IOMUXC_SD1_WP_ALT3_ECSPI4_MOSI = 3,
    IMX_IOMUXC_SD1_WP_ALT4_FLEXTIMER1_CH1 = 4,
    IMX_IOMUXC_SD1_WP_ALT5_GPIO5_IO1 = 5,
    IMX_IOMUXC_SD1_WP_ALT6_CCM_CLKO2 = 6,
} IMX_IOMUXC_SD1_WP_ALT;

typedef enum {
    IMX_IOMUXC_SD1_RESET_B_ALT0_SD1_RESET_B = 0,
    IMX_IOMUXC_SD1_RESET_B_ALT1_SAI3_MCLK = 1,
    IMX_IOMUXC_SD1_RESET_B_ALT2_UART6_RTS_B = 2,
    IMX_IOMUXC_SD1_RESET_B_ALT3_ECSPI4_SCLK = 3,
    IMX_IOMUXC_SD1_RESET_B_ALT4_FLEXTIMER1_CH2 = 4,
    IMX_IOMUXC_SD1_RESET_B_ALT5_GPIO5_IO2 = 5,
} IMX_IOMUXC_SD1_RESET_B_ALT;

typedef enum {
    IMX_IOMUXC_SD1_CLK_ALT0_SD1_CLK = 0,
    IMX_IOMUXC_SD1_CLK_ALT1_SAI3_RX_SYNC = 1,
    IMX_IOMUXC_SD1_CLK_ALT2_UART6_CTS_B = 2,
    IMX_IOMUXC_SD1_CLK_ALT3_ECSPI4_SS0 = 3,
    IMX_IOMUXC_SD1_CLK_ALT4_FLEXTIMER1_CH3 = 4,
    IMX_IOMUXC_SD1_CLK_ALT5_GPIO5_IO3 = 5,
} IMX_IOMUXC_SD1_CLK_ALT;

typedef enum {
    IMX_IOMUXC_SD1_CMD_ALT0_SD1_CMD = 0,
    IMX_IOMUXC_SD1_CMD_ALT1_SAI3_RX_BCLK = 1,
    IMX_IOMUXC_SD1_CMD_ALT3_ECSPI4_SS1 = 3,
    IMX_IOMUXC_SD1_CMD_ALT4_FLEXTIMER2_CH0 = 4,
    IMX_IOMUXC_SD1_CMD_ALT5_GPIO5_IO4 = 5,
} IMX_IOMUXC_SD1_CMD_ALT;

typedef enum {
    IMX_IOMUXC_SD1_DATA0_ALT0_SD1_DATA0 = 0,
    IMX_IOMUXC_SD1_DATA0_ALT1_SAI3_RX_DATA0 = 1,
    IMX_IOMUXC_SD1_DATA0_ALT2_UART7_RX_DATA = 2,
    IMX_IOMUXC_SD1_DATA0_ALT3_ECSPI4_SS2 = 3,
    IMX_IOMUXC_SD1_DATA0_ALT4_FLEXTIMER2_CH1 = 4,
    IMX_IOMUXC_SD1_DATA0_ALT5_GPIO5_IO5 = 5,
    IMX_IOMUXC_SD1_DATA0_ALT6_CCM_EXT_CLK1 = 6,
} IMX_IOMUXC_SD1_DATA0_ALT;

typedef enum {
    IMX_IOMUXC_SD1_DATA1_ALT0_SD1_DATA1 = 0,
    IMX_IOMUXC_SD1_DATA1_ALT1_SAI3_TX_BCLK = 1,
    IMX_IOMUXC_SD1_DATA1_ALT2_UART7_TX_DATA = 2,
    IMX_IOMUXC_SD1_DATA1_ALT3_ECSPI4_SS3 = 3,
    IMX_IOMUXC_SD1_DATA1_ALT4_FLEXTIMER2_CH2 = 4,
    IMX_IOMUXC_SD1_DATA1_ALT5_GPIO5_IO6 = 5,
    IMX_IOMUXC_SD1_DATA1_ALT6_CCM_EXT_CLK2 = 6,
} IMX_IOMUXC_SD1_DATA1_ALT;

typedef enum {
    IMX_IOMUXC_SD1_DATA2_ALT0_SD1_DATA2 = 0,
    IMX_IOMUXC_SD1_DATA2_ALT1_SAI3_TX_SYNC = 1,
    IMX_IOMUXC_SD1_DATA2_ALT2_UART7_CTS_B = 2,
    IMX_IOMUXC_SD1_DATA2_ALT3_ECSPI4_RDY = 3,
    IMX_IOMUXC_SD1_DATA2_ALT4_FLEXTIMER2_CH3 = 4,
    IMX_IOMUXC_SD1_DATA2_ALT5_GPIO5_IO7 = 5,
    IMX_IOMUXC_SD1_DATA2_ALT6_CCM_EXT_CLK3 = 6,
} IMX_IOMUXC_SD1_DATA2_ALT;

typedef enum {
    IMX_IOMUXC_SD1_DATA3_ALT0_SD1_DATA3 = 0,
    IMX_IOMUXC_SD1_DATA3_ALT1_SAI3_TX_DATA0 = 1,
    IMX_IOMUXC_SD1_DATA3_ALT2_UART7_RTS_B = 2,
    IMX_IOMUXC_SD1_DATA3_ALT3_ECSPI3_SS1 = 3,
    IMX_IOMUXC_SD1_DATA3_ALT4_FLEXTIMER1_PHA = 4,
    IMX_IOMUXC_SD1_DATA3_ALT5_GPIO5_IO8 = 5,
    IMX_IOMUXC_SD1_DATA3_ALT6_CCM_EXT_CLK4 = 6,
} IMX_IOMUXC_SD1_DATA3_ALT;

typedef enum {
    IMX_IOMUXC_SD2_CD_B_ALT0_SD2_CD_B = 0,
    IMX_IOMUXC_SD2_CD_B_ALT1_ENET1_MDIO = 1,
    IMX_IOMUXC_SD2_CD_B_ALT2_ENET2_MDIO = 2,
    IMX_IOMUXC_SD2_CD_B_ALT3_ECSPI3_SS2 = 3,
    IMX_IOMUXC_SD2_CD_B_ALT4_FLEXTIMER1_PHB = 4,
    IMX_IOMUXC_SD2_CD_B_ALT5_GPIO5_IO9 = 5,
    IMX_IOMUXC_SD2_CD_B_ALT6_SDMA_EXT_EVENT0 = 6,
} IMX_IOMUXC_SD2_CD_B_ALT;

typedef enum {
    IMX_IOMUXC_SD2_WP_ALT0_SD2_WP = 0,
    IMX_IOMUXC_SD2_WP_ALT1_ENET1_MDC = 1,
    IMX_IOMUXC_SD2_WP_ALT2_ENET2_MDC = 2,
    IMX_IOMUXC_SD2_WP_ALT3_ECSPI3_SS3 = 3,
    IMX_IOMUXC_SD2_WP_ALT4_USB_OTG1_ID = 4,
    IMX_IOMUXC_SD2_WP_ALT5_GPIO5_IO10 = 5,
    IMX_IOMUXC_SD2_WP_ALT6_SDMA_EXT_EVENT1 = 6,
} IMX_IOMUXC_SD2_WP_ALT;

typedef enum {
    IMX_IOMUXC_SD2_RESET_B_ALT0_SD2_RESET_B = 0,
    IMX_IOMUXC_SD2_RESET_B_ALT1_SAI2_MCLK = 1,
    IMX_IOMUXC_SD2_RESET_B_ALT2_SD2_RESET = 2,
    IMX_IOMUXC_SD2_RESET_B_ALT3_ECSPI3_RDY = 3,
    IMX_IOMUXC_SD2_RESET_B_ALT4_USB_OTG2_ID = 4,
    IMX_IOMUXC_SD2_RESET_B_ALT5_GPIO5_IO11 = 5,
} IMX_IOMUXC_SD2_RESET_B_ALT;

typedef enum {
    IMX_IOMUXC_SD2_CLK_ALT0_SD2_CLK = 0,
    IMX_IOMUXC_SD2_CLK_ALT1_SAI2_RX_SYNC = 1,
    IMX_IOMUXC_SD2_CLK_ALT2_MQS_RIGHT = 2,
    IMX_IOMUXC_SD2_CLK_ALT3_GPT4_CLK = 3,
    IMX_IOMUXC_SD2_CLK_ALT5_GPIO5_IO12 = 5,
} IMX_IOMUXC_SD2_CLK_ALT;

typedef enum {
    IMX_IOMUXC_SD2_CMD_ALT0_SD2_CMD = 0,
    IMX_IOMUXC_SD2_CMD_ALT1_SAI2_RX_BCLK = 1,
    IMX_IOMUXC_SD2_CMD_ALT2_MQS_LEFT = 2,
    IMX_IOMUXC_SD2_CMD_ALT3_GPT4_CAPTURE1 = 3,
    IMX_IOMUXC_SD2_CMD_ALT4_SIM2_PORT1_TRXD = 4,
    IMX_IOMUXC_SD2_CMD_ALT5_GPIO5_IO13 = 5,
} IMX_IOMUXC_SD2_CMD_ALT;

typedef enum {
    IMX_IOMUXC_SD2_DATA0_ALT0_SD2_DATA0 = 0,
    IMX_IOMUXC_SD2_DATA0_ALT1_SAI2_RX_DATA0 = 1,
    IMX_IOMUXC_SD2_DATA0_ALT2_UART4_RX_DATA = 2,
    IMX_IOMUXC_SD2_DATA0_ALT3_GPT4_CAPTURE2 = 3,
    IMX_IOMUXC_SD2_DATA0_ALT4_SIM2_PORT1_CLK = 4,
    IMX_IOMUXC_SD2_DATA0_ALT5_GPIO5_IO14 = 5,
} IMX_IOMUXC_SD2_DATA0_ALT;

typedef enum {
    IMX_IOMUXC_SD2_DATA1_ALT0_SD2_DATA1 = 0,
    IMX_IOMUXC_SD2_DATA1_ALT1_SAI2_TX_BCLK = 1,
    IMX_IOMUXC_SD2_DATA1_ALT2_UART4_TX_DATA = 2,
    IMX_IOMUXC_SD2_DATA1_ALT3_GPT4_COMPARE1 = 3,
    IMX_IOMUXC_SD2_DATA1_ALT4_SIM2_PORT1_RST_B = 4,
    IMX_IOMUXC_SD2_DATA1_ALT5_GPIO5_IO15 = 5,
} IMX_IOMUXC_SD2_DATA1_ALT;

typedef enum {
    IMX_IOMUXC_SD2_DATA2_ALT0_SD2_DATA2 = 0,
    IMX_IOMUXC_SD2_DATA2_ALT1_SAI2_TX_SYNC = 1,
    IMX_IOMUXC_SD2_DATA2_ALT2_UART4_CTS_B = 2,
    IMX_IOMUXC_SD2_DATA2_ALT3_GPT4_COMPARE2 = 3,
    IMX_IOMUXC_SD2_DATA2_ALT4_SIM2_PORT1_SVEN = 4,
    IMX_IOMUXC_SD2_DATA2_ALT5_GPIO5_IO16 = 5,
} IMX_IOMUXC_SD2_DATA2_ALT;

typedef enum {
    IMX_IOMUXC_SD2_DATA3_ALT0_SD2_DATA3 = 0,
    IMX_IOMUXC_SD2_DATA3_ALT1_SAI2_TX_DATA0 = 1,
    IMX_IOMUXC_SD2_DATA3_ALT2_UART4_RTS_B = 2,
    IMX_IOMUXC_SD2_DATA3_ALT3_GPT4_COMPARE3 = 3,
    IMX_IOMUXC_SD2_DATA3_ALT4_SIM2_PORT1_PD = 4,
    IMX_IOMUXC_SD2_DATA3_ALT5_GPIO5_IO17 = 5,
} IMX_IOMUXC_SD2_DATA3_ALT;

typedef enum {
    IMX_IOMUXC_SD3_CLK_ALT0_SD3_CLK = 0,
    IMX_IOMUXC_SD3_CLK_ALT1_NAND_CLE = 1,
    IMX_IOMUXC_SD3_CLK_ALT2_ECSPI4_MISO = 2,
    IMX_IOMUXC_SD3_CLK_ALT3_SAI3_RX_SYNC = 3,
    IMX_IOMUXC_SD3_CLK_ALT4_GPT3_CLK = 4,
    IMX_IOMUXC_SD3_CLK_ALT5_GPIO6_IO0 = 5,
} IMX_IOMUXC_SD3_CLK_ALT;

typedef enum {
    IMX_IOMUXC_SD3_CMD_ALT0_SD3_CMD = 0,
    IMX_IOMUXC_SD3_CMD_ALT1_NAND_ALE = 1,
    IMX_IOMUXC_SD3_CMD_ALT2_ECSPI4_MOSI = 2,
    IMX_IOMUXC_SD3_CMD_ALT3_SAI3_RX_BCLK = 3,
    IMX_IOMUXC_SD3_CMD_ALT4_GPT3_CAPTURE1 = 4,
    IMX_IOMUXC_SD3_CMD_ALT5_GPIO6_IO1 = 5,
} IMX_IOMUXC_SD3_CMD_ALT;

typedef enum {
    IMX_IOMUXC_SD3_DATA0_ALT0_SD3_DATA0 = 0,
    IMX_IOMUXC_SD3_DATA0_ALT1_NAND_DATA00 = 1,
    IMX_IOMUXC_SD3_DATA0_ALT2_ECSPI4_SS0 = 2,
    IMX_IOMUXC_SD3_DATA0_ALT3_SAI3_RX_DATA0 = 3,
    IMX_IOMUXC_SD3_DATA0_ALT4_GPT3_CAPTURE2 = 4,
    IMX_IOMUXC_SD3_DATA0_ALT5_GPIO6_IO2 = 5,
} IMX_IOMUXC_SD3_DATA0_ALT;

typedef enum {
    IMX_IOMUXC_SD3_DATA1_ALT0_SD3_DATA1 = 0,
    IMX_IOMUXC_SD3_DATA1_ALT1_NAND_DATA01 = 1,
    IMX_IOMUXC_SD3_DATA1_ALT2_ECSPI4_SCLK = 2,
    IMX_IOMUXC_SD3_DATA1_ALT3_SAI3_TX_BCLK = 3,
    IMX_IOMUXC_SD3_DATA1_ALT4_GPT3_COMPARE1 = 4,
    IMX_IOMUXC_SD3_DATA1_ALT5_GPIO6_IO3 = 5,
} IMX_IOMUXC_SD3_DATA1_ALT;

typedef enum {
    IMX_IOMUXC_SD3_DATA2_ALT0_SD3_DATA2 = 0,
    IMX_IOMUXC_SD3_DATA2_ALT1_NAND_DATA02 = 1,
    IMX_IOMUXC_SD3_DATA2_ALT2_I2C3_SDA = 2,
    IMX_IOMUXC_SD3_DATA2_ALT3_SAI3_TX_SYNC = 3,
    IMX_IOMUXC_SD3_DATA2_ALT4_GPT3_COMPARE2 = 4,
    IMX_IOMUXC_SD3_DATA2_ALT5_GPIO6_IO4 = 5,
} IMX_IOMUXC_SD3_DATA2_ALT;

typedef enum {
    IMX_IOMUXC_SD3_DATA3_ALT0_SD3_DATA3 = 0,
    IMX_IOMUXC_SD3_DATA3_ALT1_NAND_DATA03 = 1,
    IMX_IOMUXC_SD3_DATA3_ALT2_I2C3_SCL = 2,
    IMX_IOMUXC_SD3_DATA3_ALT3_SAI3_TX_DATA0 = 3,
    IMX_IOMUXC_SD3_DATA3_ALT4_GPT3_COMPARE3 = 4,
    IMX_IOMUXC_SD3_DATA3_ALT5_GPIO6_IO5 = 5,
} IMX_IOMUXC_SD3_DATA3_ALT;

typedef enum {
    IMX_IOMUXC_SD3_DATA4_ALT0_SD3_DATA4 = 0,
    IMX_IOMUXC_SD3_DATA4_ALT1_NAND_DATA04 = 1,
    IMX_IOMUXC_SD3_DATA4_ALT3_UART3_RX_DATA = 3,
    IMX_IOMUXC_SD3_DATA4_ALT4_FLEXCAN2_RX = 4,
    IMX_IOMUXC_SD3_DATA4_ALT5_GPIO6_IO6 = 5,
} IMX_IOMUXC_SD3_DATA4_ALT;

typedef enum {
    IMX_IOMUXC_SD3_DATA5_ALT0_SD3_DATA5 = 0,
    IMX_IOMUXC_SD3_DATA5_ALT1_NAND_DATA05 = 1,
    IMX_IOMUXC_SD3_DATA5_ALT3_UART3_TX_DATA = 3,
    IMX_IOMUXC_SD3_DATA5_ALT4_FLEXCAN1_TX = 4,
    IMX_IOMUXC_SD3_DATA5_ALT5_GPIO6_IO7 = 5,
} IMX_IOMUXC_SD3_DATA5_ALT;

typedef enum {
    IMX_IOMUXC_SD3_DATA6_ALT0_SD3_DATA6 = 0,
    IMX_IOMUXC_SD3_DATA6_ALT1_NAND_DATA06 = 1,
    IMX_IOMUXC_SD3_DATA6_ALT2_SD3_WP = 2,
    IMX_IOMUXC_SD3_DATA6_ALT3_UART3_RTS_B = 3,
    IMX_IOMUXC_SD3_DATA6_ALT4_FLEXCAN2_TX = 4,
    IMX_IOMUXC_SD3_DATA6_ALT5_GPIO6_IO8 = 5,
} IMX_IOMUXC_SD3_DATA6_ALT;

typedef enum {
    IMX_IOMUXC_SD3_DATA7_ALT0_SD3_DATA7 = 0,
    IMX_IOMUXC_SD3_DATA7_ALT1_NAND_DATA07 = 1,
    IMX_IOMUXC_SD3_DATA7_ALT2_SD3_CD_B = 2,
    IMX_IOMUXC_SD3_DATA7_ALT3_UART3_CTS_B = 3,
    IMX_IOMUXC_SD3_DATA7_ALT4_FLEXCAN1_RX = 4,
    IMX_IOMUXC_SD3_DATA7_ALT5_GPIO6_IO9 = 5,
} IMX_IOMUXC_SD3_DATA7_ALT;

typedef enum {
    IMX_IOMUXC_SD3_STROBE_ALT0_SD3_STROBE = 0,
    IMX_IOMUXC_SD3_STROBE_ALT1_NAND_RE_B = 1,
    IMX_IOMUXC_SD3_STROBE_ALT5_GPIO6_IO10 = 5,
} IMX_IOMUXC_SD3_STROBE_ALT;

typedef enum {
    IMX_IOMUXC_SD3_RESET_B_ALT0_SD3_RESET_B = 0,
    IMX_IOMUXC_SD3_RESET_B_ALT1_NAND_WE_B = 1,
    IMX_IOMUXC_SD3_RESET_B_ALT2_SD3_RESET = 2,
    IMX_IOMUXC_SD3_RESET_B_ALT3_SAI3_MCLK = 3,
    IMX_IOMUXC_SD3_RESET_B_ALT5_GPIO6_IO11 = 5,
} IMX_IOMUXC_SD3_RESET_B_ALT;

typedef enum {
    IMX_IOMUXC_SAI1_RX_DATA_ALT0_SAI1_RX_DATA0 = 0,
    IMX_IOMUXC_SAI1_RX_DATA_ALT1_NAND_CE1_B = 1,
    IMX_IOMUXC_SAI1_RX_DATA_ALT2_UART5_RX_DATA = 2,
    IMX_IOMUXC_SAI1_RX_DATA_ALT3_FLEXCAN1_RX = 3,
    IMX_IOMUXC_SAI1_RX_DATA_ALT4_SIM1_PORT1_TRXD = 4,
    IMX_IOMUXC_SAI1_RX_DATA_ALT5_GPIO6_IO12 = 5,
    IMX_IOMUXC_SAI1_RX_DATA_ALT7_SRC_ANY_PU_RESET = 7,
} IMX_IOMUXC_SAI1_RX_DATA_ALT;

typedef enum {
    IMX_IOMUXC_SAI1_TX_BCLK_ALT0_SAI1_TX_BCLK = 0,
    IMX_IOMUXC_SAI1_TX_BCLK_ALT1_NAND_CE0_B = 1,
    IMX_IOMUXC_SAI1_TX_BCLK_ALT2_UART5_TX_DATA = 2,
    IMX_IOMUXC_SAI1_TX_BCLK_ALT3_FLEXCAN1_TX = 3,
    IMX_IOMUXC_SAI1_TX_BCLK_ALT4_SIM1_PORT1_CLK = 4,
    IMX_IOMUXC_SAI1_TX_BCLK_ALT5_GPIO6_IO13 = 5,
    IMX_IOMUXC_SAI1_TX_BCLK_ALT7_SRC_EARLY_RESET = 7,
} IMX_IOMUXC_SAI1_TX_BCLK_ALT;

typedef enum {
    IMX_IOMUXC_SAI1_TX_SYNC_ALT0_SAI1_TX_SYNC = 0,
    IMX_IOMUXC_SAI1_TX_SYNC_ALT1_NAND_DQS = 1,
    IMX_IOMUXC_SAI1_TX_SYNC_ALT2_UART5_CTS_B = 2,
    IMX_IOMUXC_SAI1_TX_SYNC_ALT3_FLEXCAN2_RX = 3,
    IMX_IOMUXC_SAI1_TX_SYNC_ALT4_SIM1_PORT1_RST_B = 4,
    IMX_IOMUXC_SAI1_TX_SYNC_ALT5_GPIO6_IO14 = 5,
    IMX_IOMUXC_SAI1_TX_SYNC_ALT7_SRC_INT_BOOT = 7,
} IMX_IOMUXC_SAI1_TX_SYNC_ALT;

typedef enum {
    IMX_IOMUXC_SAI1_TX_DATA_ALT0_SAI1_TX_DATA0 = 0,
    IMX_IOMUXC_SAI1_TX_DATA_ALT1_NAND_READY_B = 1,
    IMX_IOMUXC_SAI1_TX_DATA_ALT2_UART5_RTS_B = 2,
    IMX_IOMUXC_SAI1_TX_DATA_ALT3_FLEXCAN2_TX = 3,
    IMX_IOMUXC_SAI1_TX_DATA_ALT4_SIM1_PORT1_SVEN = 4,
    IMX_IOMUXC_SAI1_TX_DATA_ALT5_GPIO6_IO15 = 5,
    IMX_IOMUXC_SAI1_TX_DATA_ALT7_SRC_SYSTEM_RESET = 7,
} IMX_IOMUXC_SAI1_TX_DATA_ALT;

typedef enum {
    IMX_IOMUXC_SAI1_RX_SYNC_ALT0_SAI1_RX_SYNC = 0,
    IMX_IOMUXC_SAI1_RX_SYNC_ALT1_NAND_CE2_B = 1,
    IMX_IOMUXC_SAI1_RX_SYNC_ALT2_SAI2_RX_SYNC = 2,
    IMX_IOMUXC_SAI1_RX_SYNC_ALT3_I2C4_SCL = 3,
    IMX_IOMUXC_SAI1_RX_SYNC_ALT4_SIM1_PORT1_PD = 4,
    IMX_IOMUXC_SAI1_RX_SYNC_ALT5_GPIO6_IO16 = 5,
    IMX_IOMUXC_SAI1_RX_SYNC_ALT6_MQS_RIGHT = 6,
    IMX_IOMUXC_SAI1_RX_SYNC_ALT7_SRC_CA7_RESET_B0 = 7,
} IMX_IOMUXC_SAI1_RX_SYNC_ALT;

typedef enum {
    IMX_IOMUXC_SAI1_RX_BCLK_ALT0_SAI1_RX_BCLK = 0,
    IMX_IOMUXC_SAI1_RX_BCLK_ALT1_NAND_CE3_B = 1,
    IMX_IOMUXC_SAI1_RX_BCLK_ALT2_SAI2_RX_BCLK = 2,
    IMX_IOMUXC_SAI1_RX_BCLK_ALT3_I2C4_SDA = 3,
    IMX_IOMUXC_SAI1_RX_BCLK_ALT4_FLEXTIMER2_PHA = 4,
    IMX_IOMUXC_SAI1_RX_BCLK_ALT5_GPIO6_IO17 = 5,
    IMX_IOMUXC_SAI1_RX_BCLK_ALT6_MQS_LEFT = 6,
    IMX_IOMUXC_SAI1_RX_BCLK_ALT7_SRC_CA7_RESET_B1 = 7,
} IMX_IOMUXC_SAI1_RX_BCLK_ALT;

typedef enum {
    IMX_IOMUXC_SAI1_MCLK_ALT0_SAI1_MCLK = 0,
    IMX_IOMUXC_SAI1_MCLK_ALT1_NAND_WP_B = 1,
    IMX_IOMUXC_SAI1_MCLK_ALT2_SAI2_MCLK = 2,
    IMX_IOMUXC_SAI1_MCLK_ALT3_CCM_PMIC_READY = 3,
    IMX_IOMUXC_SAI1_MCLK_ALT4_FLEXTIMER2_PHB = 4,
    IMX_IOMUXC_SAI1_MCLK_ALT5_GPIO6_IO18 = 5,
    IMX_IOMUXC_SAI1_MCLK_ALT7_SRC_TESTER_ACK = 7,
} IMX_IOMUXC_SAI1_MCLK_ALT;

typedef enum {
    IMX_IOMUXC_SAI2_TX_SYNC_ALT0_SAI2_TX_SYNC = 0,
    IMX_IOMUXC_SAI2_TX_SYNC_ALT1_ECSPI3_MISO = 1,
    IMX_IOMUXC_SAI2_TX_SYNC_ALT2_UART4_RX_DATA = 2,
    IMX_IOMUXC_SAI2_TX_SYNC_ALT3_UART1_CTS_B = 3,
    IMX_IOMUXC_SAI2_TX_SYNC_ALT4_FLEXTIMER2_CH4 = 4,
    IMX_IOMUXC_SAI2_TX_SYNC_ALT5_GPIO6_IO19 = 5,
} IMX_IOMUXC_SAI2_TX_SYNC_ALT;

typedef enum {
    IMX_IOMUXC_SAI2_TX_BCLK_ALT0_SAI2_TX_BCLK = 0,
    IMX_IOMUXC_SAI2_TX_BCLK_ALT1_ECSPI3_MOSI = 1,
    IMX_IOMUXC_SAI2_TX_BCLK_ALT2_UART4_TX_DATA = 2,
    IMX_IOMUXC_SAI2_TX_BCLK_ALT3_UART1_RTS_B = 3,
    IMX_IOMUXC_SAI2_TX_BCLK_ALT4_FLEXTIMER2_CH5 = 4,
    IMX_IOMUXC_SAI2_TX_BCLK_ALT5_GPIO6_IO20 = 5,
} IMX_IOMUXC_SAI2_TX_BCLK_ALT;

typedef enum {
    IMX_IOMUXC_SAI2_RX_DATA_ALT0_SAI2_RX_DATA0 = 0,
    IMX_IOMUXC_SAI2_RX_DATA_ALT1_ECSPI3_SCLK = 1,
    IMX_IOMUXC_SAI2_RX_DATA_ALT2_UART4_CTS_B = 2,
    IMX_IOMUXC_SAI2_RX_DATA_ALT3_UART2_CTS_B = 3,
    IMX_IOMUXC_SAI2_RX_DATA_ALT4_FLEXTIMER2_CH6 = 4,
    IMX_IOMUXC_SAI2_RX_DATA_ALT5_GPIO6_IO21 = 5,
    IMX_IOMUXC_SAI2_RX_DATA_ALT6_KPP_COL7 = 6,
} IMX_IOMUXC_SAI2_RX_DATA_ALT;

typedef enum {
    IMX_IOMUXC_SAI2_TX_DATA_ALT0_SAI2_TX_DATA0 = 0,
    IMX_IOMUXC_SAI2_TX_DATA_ALT1_ECSPI3_SS0 = 1,
    IMX_IOMUXC_SAI2_TX_DATA_ALT2_UART4_RTS_B = 2,
    IMX_IOMUXC_SAI2_TX_DATA_ALT3_UART2_RTS_B = 3,
    IMX_IOMUXC_SAI2_TX_DATA_ALT4_FLEXTIMER2_CH7 = 4,
    IMX_IOMUXC_SAI2_TX_DATA_ALT5_GPIO6_IO22 = 5,
    IMX_IOMUXC_SAI2_TX_DATA_ALT6_KPP_ROW7 = 6,
} IMX_IOMUXC_SAI2_TX_DATA_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_RGMII_RD0_ALT0_ENET1_RGMII_RD0 = 0,
    IMX_IOMUXC_ENET1_RGMII_RD0_ALT1_PWM1_OUT = 1,
    IMX_IOMUXC_ENET1_RGMII_RD0_ALT2_I2C3_SCL = 2,
    IMX_IOMUXC_ENET1_RGMII_RD0_ALT3_UART1_CTS_B = 3,
    IMX_IOMUXC_ENET1_RGMII_RD0_ALT4_EPDC_VCOM0 = 4,
    IMX_IOMUXC_ENET1_RGMII_RD0_ALT5_GPIO7_IO0 = 5,
    IMX_IOMUXC_ENET1_RGMII_RD0_ALT6_KPP_ROW3 = 6,
} IMX_IOMUXC_ENET1_RGMII_RD0_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_RGMII_RD1_ALT0_ENET1_RGMII_RD1 = 0,
    IMX_IOMUXC_ENET1_RGMII_RD1_ALT1_PWM2_OUT = 1,
    IMX_IOMUXC_ENET1_RGMII_RD1_ALT2_I2C3_SDA = 2,
    IMX_IOMUXC_ENET1_RGMII_RD1_ALT3_UART1_RTS_B = 3,
    IMX_IOMUXC_ENET1_RGMII_RD1_ALT4_EPDC_VCOM1 = 4,
    IMX_IOMUXC_ENET1_RGMII_RD1_ALT5_GPIO7_IO1 = 5,
    IMX_IOMUXC_ENET1_RGMII_RD1_ALT6_KPP_COL3 = 6,
} IMX_IOMUXC_ENET1_RGMII_RD1_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_RGMII_RD2_ALT0_ENET1_RGMII_RD2 = 0,
    IMX_IOMUXC_ENET1_RGMII_RD2_ALT1_FLEXCAN1_RX = 1,
    IMX_IOMUXC_ENET1_RGMII_RD2_ALT2_ECSPI2_SCLK = 2,
    IMX_IOMUXC_ENET1_RGMII_RD2_ALT3_UART1_RX_DATA = 3,
    IMX_IOMUXC_ENET1_RGMII_RD2_ALT4_EPDC_SDCE4 = 4,
    IMX_IOMUXC_ENET1_RGMII_RD2_ALT5_GPIO7_IO2 = 5,
    IMX_IOMUXC_ENET1_RGMII_RD2_ALT6_KPP_ROW2 = 6,
} IMX_IOMUXC_ENET1_RGMII_RD2_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_RGMII_RD3_ALT0_ENET1_RGMII_RD3 = 0,
    IMX_IOMUXC_ENET1_RGMII_RD3_ALT1_FLEXCAN1_TX = 1,
    IMX_IOMUXC_ENET1_RGMII_RD3_ALT2_ECSPI2_MOSI = 2,
    IMX_IOMUXC_ENET1_RGMII_RD3_ALT3_UART1_TX_DATA = 3,
    IMX_IOMUXC_ENET1_RGMII_RD3_ALT4_EPDC_SDCE5 = 4,
    IMX_IOMUXC_ENET1_RGMII_RD3_ALT5_GPIO7_IO3 = 5,
    IMX_IOMUXC_ENET1_RGMII_RD3_ALT6_KPP_COL2 = 6,
} IMX_IOMUXC_ENET1_RGMII_RD3_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_RGMII_RX_CTL_ALT0_ENET1_RGMII_RX_CTL = 0,
    IMX_IOMUXC_ENET1_RGMII_RX_CTL_ALT2_ECSPI2_SS1 = 2,
    IMX_IOMUXC_ENET1_RGMII_RX_CTL_ALT4_EPDC_SDCE6 = 4,
    IMX_IOMUXC_ENET1_RGMII_RX_CTL_ALT5_GPIO7_IO4 = 5,
    IMX_IOMUXC_ENET1_RGMII_RX_CTL_ALT6_KPP_ROW1 = 6,
} IMX_IOMUXC_ENET1_RGMII_RX_CTL_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_RGMII_RXC_ALT0_ENET1_RGMII_RXC = 0,
    IMX_IOMUXC_ENET1_RGMII_RXC_ALT1_ENET1_RX_ER = 1,
    IMX_IOMUXC_ENET1_RGMII_RXC_ALT2_ECSPI2_SS2 = 2,
    IMX_IOMUXC_ENET1_RGMII_RXC_ALT4_EPDC_SDCE7 = 4,
    IMX_IOMUXC_ENET1_RGMII_RXC_ALT5_GPIO7_IO5 = 5,
    IMX_IOMUXC_ENET1_RGMII_RXC_ALT6_KPP_COL1 = 6,
} IMX_IOMUXC_ENET1_RGMII_RXC_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_RGMII_TD0_ALT0_ENET1_RGMII_TD0 = 0,
    IMX_IOMUXC_ENET1_RGMII_TD0_ALT1_PWM3_OUT = 1,
    IMX_IOMUXC_ENET1_RGMII_TD0_ALT2_ECSPI2_SS3 = 2,
    IMX_IOMUXC_ENET1_RGMII_TD0_ALT4_EPDC_SDCE8 = 4,
    IMX_IOMUXC_ENET1_RGMII_TD0_ALT5_GPIO7_IO6 = 5,
    IMX_IOMUXC_ENET1_RGMII_TD0_ALT6_KPP_ROW0 = 6,
} IMX_IOMUXC_ENET1_RGMII_TD0_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_RGMII_TD1_ALT0_ENET1_RGMII_TD1 = 0,
    IMX_IOMUXC_ENET1_RGMII_TD1_ALT1_PWM4_OUT = 1,
    IMX_IOMUXC_ENET1_RGMII_TD1_ALT2_ECSPI2_RDY = 2,
    IMX_IOMUXC_ENET1_RGMII_TD1_ALT4_EPDC_SDCE9 = 4,
    IMX_IOMUXC_ENET1_RGMII_TD1_ALT5_GPIO7_IO7 = 5,
    IMX_IOMUXC_ENET1_RGMII_TD1_ALT6_KPP_COL0 = 6,
} IMX_IOMUXC_ENET1_RGMII_TD1_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_RGMII_TD2_ALT0_ENET1_RGMII_TD2 = 0,
    IMX_IOMUXC_ENET1_RGMII_TD2_ALT1_FLEXCAN2_RX = 1,
    IMX_IOMUXC_ENET1_RGMII_TD2_ALT2_ECSPI2_MISO = 2,
    IMX_IOMUXC_ENET1_RGMII_TD2_ALT3_I2C4_SCL = 3,
    IMX_IOMUXC_ENET1_RGMII_TD2_ALT4_EPDC_SDOED = 4,
    IMX_IOMUXC_ENET1_RGMII_TD2_ALT5_GPIO7_IO8 = 5,
} IMX_IOMUXC_ENET1_RGMII_TD2_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_RGMII_TD3_ALT0_ENET1_RGMII_TD3 = 0,
    IMX_IOMUXC_ENET1_RGMII_TD3_ALT1_FLEXCAN2_TX = 1,
    IMX_IOMUXC_ENET1_RGMII_TD3_ALT2_ECSPI2_SS0 = 2,
    IMX_IOMUXC_ENET1_RGMII_TD3_ALT3_I2C4_SDA = 3,
    IMX_IOMUXC_ENET1_RGMII_TD3_ALT4_EPDC_SDOEZ = 4,
    IMX_IOMUXC_ENET1_RGMII_TD3_ALT5_GPIO7_IO9 = 5,
    IMX_IOMUXC_ENET1_RGMII_TD3_ALT7_CAAM_RNG_OSC_OBS = 7,
} IMX_IOMUXC_ENET1_RGMII_TD3_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_RGMII_TX_CTL_ALT0_ENET1_RGMII_TX_CTL = 0,
    IMX_IOMUXC_ENET1_RGMII_TX_CTL_ALT2_SAI1_RX_SYNC = 2,
    IMX_IOMUXC_ENET1_RGMII_TX_CTL_ALT3_GPT2_COMPARE1 = 3,
    IMX_IOMUXC_ENET1_RGMII_TX_CTL_ALT4_EPDC_PWR_CTRL2 = 4,
    IMX_IOMUXC_ENET1_RGMII_TX_CTL_ALT5_GPIO7_IO10 = 5,
} IMX_IOMUXC_ENET1_RGMII_TX_CTL_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_RGMII_TXC_ALT0_ENET1_RGMII_TXC = 0,
    IMX_IOMUXC_ENET1_RGMII_TXC_ALT1_ENET1_TX_ER = 1,
    IMX_IOMUXC_ENET1_RGMII_TXC_ALT2_SAI1_RX_BCLK = 2,
    IMX_IOMUXC_ENET1_RGMII_TXC_ALT3_GPT2_COMPARE2 = 3,
    IMX_IOMUXC_ENET1_RGMII_TXC_ALT4_EPDC_PWR_CTRL3 = 4,
    IMX_IOMUXC_ENET1_RGMII_TXC_ALT5_GPIO7_IO11 = 5,
} IMX_IOMUXC_ENET1_RGMII_TXC_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_TX_CLK_ALT0_ENET1_TX_CLK = 0,
    IMX_IOMUXC_ENET1_TX_CLK_ALT1_CCM_ENET1_REF_CLK = 1,
    IMX_IOMUXC_ENET1_TX_CLK_ALT2_SAI1_RX_DATA0 = 2,
    IMX_IOMUXC_ENET1_TX_CLK_ALT3_GPT2_COMPARE3 = 3,
    IMX_IOMUXC_ENET1_TX_CLK_ALT4_EPDC_PWR_IRQ = 4,
    IMX_IOMUXC_ENET1_TX_CLK_ALT5_GPIO7_IO12 = 5,
    IMX_IOMUXC_ENET1_TX_CLK_ALT6_CCM_EXT_CLK1 = 6,
    IMX_IOMUXC_ENET1_TX_CLK_ALT7_CSU_ALARM_AUT0 = 7,
} IMX_IOMUXC_ENET1_TX_CLK_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_RX_CLK_ALT0_ENET1_RX_CLK = 0,
    IMX_IOMUXC_ENET1_RX_CLK_ALT1_WDOG2_WDOG_B = 1,
    IMX_IOMUXC_ENET1_RX_CLK_ALT2_SAI1_TX_BCLK = 2,
    IMX_IOMUXC_ENET1_RX_CLK_ALT3_GPT2_CLK = 3,
    IMX_IOMUXC_ENET1_RX_CLK_ALT4_EPDC_PWR_WAKE = 4,
    IMX_IOMUXC_ENET1_RX_CLK_ALT5_GPIO7_IO13 = 5,
    IMX_IOMUXC_ENET1_RX_CLK_ALT6_CCM_EXT_CLK2 = 6,
    IMX_IOMUXC_ENET1_RX_CLK_ALT7_CSU_ALARM_AUT1 = 7,
} IMX_IOMUXC_ENET1_RX_CLK_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_CRS_ALT0_ENET1_CRS = 0,
    IMX_IOMUXC_ENET1_CRS_ALT1_WDOG2_WDOG_RST_B_DEB = 1,
    IMX_IOMUXC_ENET1_CRS_ALT2_SAI1_TX_SYNC = 2,
    IMX_IOMUXC_ENET1_CRS_ALT3_GPT2_CAPTURE1 = 3,
    IMX_IOMUXC_ENET1_CRS_ALT4_EPDC_PWR_CTRL0 = 4,
    IMX_IOMUXC_ENET1_CRS_ALT5_GPIO7_IO14 = 5,
    IMX_IOMUXC_ENET1_CRS_ALT6_CCM_EXT_CLK3 = 6,
    IMX_IOMUXC_ENET1_CRS_ALT7_CSU_ALARM_AUT2 = 7,
} IMX_IOMUXC_ENET1_CRS_ALT;

typedef enum {
    IMX_IOMUXC_ENET1_COL_ALT0_ENET1_COL = 0,
    IMX_IOMUXC_ENET1_COL_ALT1_WDOG1_WDOG_ANY = 1,
    IMX_IOMUXC_ENET1_COL_ALT2_SAI1_TX_DATA0 = 2,
    IMX_IOMUXC_ENET1_COL_ALT3_GPT2_CAPTURE2 = 3,
    IMX_IOMUXC_ENET1_COL_ALT4_EPDC_PWR_CTRL1 = 4,
    IMX_IOMUXC_ENET1_COL_ALT5_GPIO7_IO15 = 5,
    IMX_IOMUXC_ENET1_COL_ALT6_CCM_EXT_CLK4 = 6,
    IMX_IOMUXC_ENET1_COL_ALT7_CSU_INT_DEB = 7,
} IMX_IOMUXC_ENET1_COL_ALT;

/**
    Put a pad in the specified configuration.

    For example, to configure GPIO0 as CCM_CLK01 output:
    ImxPadConfig (IMX_PAD_GPIO_0, IMX_PAD_GPIO_0_CCM_CLKO1);

**/
VOID ImxPadConfig (IMX_PAD Pad, IMX_PADCFG PadConfig);

/**

    Dumps to console the specified PAD mux/control configuration.

**/
VOID ImxPadDumpConfig (char *PadName, IMX_PAD Pad);

#endif // _IMX7_IOMUX_H_
