/** @file
* iMX6 Sabre board initialization
*
*  Copyright (c) Microsoft Corporation. All rights reserved.
*  Copyright 2019 NXP
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
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>
#include <iMX6.h>
#include <iMXUart.h>
#include <iMX6Audio.h>
#include <iMX6BoardLib.h>
#include <iMX6ClkPwr.h>
#include <iMX6IoMux.h>
#include <iMX6UsbPhy.h>
#include <imx6sxdef.inc>

static void MmioSetBits32(UINT32 Addr, UINT32 Bits)
{
    MmioWrite32(Addr, MmioRead32(Addr) | Bits);
}

static void MmioClrBits32(UINT32 Addr, UINT32 Bits)
{
    MmioWrite32(Addr, MmioRead32(Addr) & ~Bits);
}

//
// Prebaked pad configurations that include mux and drive settings where
// each enum named as IMX_<MODULE-NAME>_PADCFG contains configurations
// for pads used by that module
//
typedef enum {
    // I2C1 PMIC, LCD 
    IMX_PAD_CFG_GPIO1_IO01_I2C1_SDA = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_ENABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_ENABLED,
                              IMX_IOMUXC_GPIO1_IO01_ALT0_I2C1_SDA,
                              IOMUXC_I2C1_IPP_SDA_IN_SELECT_INPUT,
                              1),
    IMX_PAD_CFG_GPIO1_IO00_I2C1_SCL = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_ENABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_ENABLED,
                              IMX_IOMUXC_GPIO1_IO00_ALT0_I2C1_SCL,
                              IOMUXC_I2C1_IPP_SCL_IN_SELECT_INPUT,
                              1),
    // I2C2 LVDS I2C
    IMX_PAD_CFG_GPIO1_IO03_I2C2_SDA = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_ENABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_ENABLED,
                              IMX_IOMUXC_GPIO1_IO03_ALT0_I2C2_SDA,
                              IOMUXC_I2C2_IPP_SDA_IN_SELECT_INPUT,
                              1),
    IMX_PAD_CFG_GPIO1_IO02_I2C2_SCL = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_ENABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_ENABLED,
                              IMX_IOMUXC_GPIO1_IO02_ALT0_I2C2_SCL,
                              IOMUXC_I2C2_IPP_SCL_IN_SELECT_INPUT,
                              1),
    // I2C3 for PCIE, ACC, COMP, ALS devices
    IMX_PAD_CFG_KEY_ROW4_I2C3_SDA = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_ENABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_ENABLED,
                              IMX_IOMUXC_KEY_ROW4_ALT2_I2C3_SDA,
                              IOMUXC_I2C3_IPP_SDA_IN_SELECT_INPUT,
                              2),
    IMX_PAD_CFG_KEY_COL4_I2C3_SCL = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_ENABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_ENABLED,
                              IMX_IOMUXC_KEY_COL4_ALT2_I2C3_SCL,
                              IOMUXC_I2C3_IPP_SCL_IN_SELECT_INPUT,
                              2),
    //I2C4 for Audio codec
    IMX_PAD_CFG_CSI_DATA06_I2C4_SCL = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_ENABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_ENABLED,
                              IMX_IOMUXC_CSI_DATA06_ALT2_I2C4_SCL,
                              IOMUXC_I2C4_IPP_SCL_IN_SELECT_INPUT,
                              2),
    IMX_PAD_CFG_CSI_DATA07_I2C4_SDA = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_ENABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_ENABLED,
                              IMX_IOMUXC_CSI_DATA07_ALT2_I2C4_SDA,
                              IOMUXC_I2C4_IPP_SDA_IN_SELECT_INPUT,
                              2),
} IMX_I2C_PADCFG;

typedef enum {
    // MCLK out
    IMX_PAD_CFG_CSI_PIXCLK_AUDIO_CLK_OUT = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_CSI_PIXCLK_ALT2_AUDIO_CLK_OUT),
    // SSI via AUDMUX6
    IMX_PAD_CFG_CSI_DATA00_AUD6_TXC = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_CSI_DATA00_ALT2_AUD6_TXC),

    IMX_PAD_CFG_CSI_DATA01_AUD6_TXFS = IMX_PAD_CFG_CSI_DATA00_AUD6_TXC,
    IMX_PAD_CFG_CSI_VSYNC_AUD6_RXD = IMX_PAD_CFG_CSI_DATA00_AUD6_TXC,
    IMX_PAD_CFG_CSI_HSYNC_AUD6_TXD = IMX_PAD_CFG_CSI_DATA00_AUD6_TXC,
} IMX_AUDIO_PADCFG;

typedef enum {
    /* MD pins*/
    IMX_PAD_ENET1_MDIO_ENET_MDIO = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_ENABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_ENET1_MDIO_ALT0_ENET1_MDIO,
                              IOMUXC_ENET1_IPP_IND_MAC0_MDIO_SELECT_INPUT,
                              ENET1_MDIO_ALT0),
    IMX_PAD_ENET1_MDC_ENET_MDC = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_ENET1_MDC_ALT0_ENET1_MDC),
    IMX_PAD_ENET2_CRS_GPIO2_IO07 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_260_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_DISABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_ENET2_CRS_ALT5_GPIO2_IO07),
    IMX_PAD_ENET2_COL_GPIO2_IO06 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_260_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_DISABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_ENET2_COL_ALT5_GPIO2_IO06),

    IMX_PAD_ENET2_RX_CLK_ENET_RX_CLK = _IMX_MAKE_PADCFG(
                              IMX_SRE_FAST,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_DISABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_ENET2_RX_CLK_ALT1_ENET2_REF_CLK_25M),
    /* RGMII TX pins */
    IMX_PAD_RGMII1_TD0_RGMII_TD0 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII1_TD0_ALT0_ENET1_TX_DATA0),
    IMX_PAD_RGMII1_TD1_RGMII_TD1 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII1_TD1_ALT0_ENET1_TX_DATA1),
    IMX_PAD_RGMII1_TD2_RGMII_TD2 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII1_TD2_ALT0_ENET1_TX_DATA2),
    IMX_PAD_RGMII1_TD3_RGMII_TD3 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII1_TD3_ALT0_ENET1_TX_DATA3),
    IMX_PAD_RGMII1_TXC_RGMII_TXC = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII1_TXC_ALT0_ENET1_RGMII_TXC),
    IMX_PAD_RGMII1_TX_CTL_RGMII_TX_CTL = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII1_TX_CTL_ALT0_ENET1_TX_EN),
    /* RGMII RX pins */
    IMX_PAD_RGMII1_RD0_ENET_RGMII_RD0 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII1_RD0_ALT0_ENET1_RX_DATA0),
    IMX_PAD_RGMII1_RD1_ENET_RGMII_RD1 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII1_RD1_ALT0_ENET1_RX_DATA1),
    IMX_PAD_RGMII1_RD2_ENET_RGMII_RD2 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII1_RD2_ALT0_ENET1_RX_DATA2),
    IMX_PAD_RGMII1_RD3_ENET_RGMII_RD3 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII1_RD3_ALT0_ENET1_RX_DATA3),
    IMX_PAD_RGMII1_RXC_ENET_RGMII_RXC = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII1_RXC_ALT0_ENET1_RGMII_RXC,
                              IOMUXC_ENET1_IPP_IND_MAC0_RXCLK_SELECT_INPUT,
                              RGMII1_RXC_ALT0),
    IMX_PAD_RGMII1_RX_CTL_RGMII_RX_CTL = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII1_RX_CTL_ALT0_ENET1_RX_EN),

    IMX_PAD_RGMII2_RXC_ENET_RGMII_RXC = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII1_RXC_ALT0_ENET1_RGMII_RXC,
                              IOMUXC_ENET2_IPP_IND_MAC0_RXCLK_SELECT_INPUT,
                              RGMII1_RXC_ALT0)
} IMX_ENET_PADCFG;

typedef enum {
    IMX_USDHC_PAD_CTL = _IMX_MAKE_PAD_CTL(
                              IMX_SRE_FAST,
                              IMX_DSE_33_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_47K_OHM_PU,
                              IMX_HYS_ENABLED),

    IMX_USDHC_CLK_PAD_CTL = _IMX_MAKE_PAD_CTL(
                              IMX_SRE_FAST,
                              IMX_DSE_33_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              0,
                              0,
                              IMX_HYS_ENABLED),

    IMX_USDHC_GPIO_PAD_CTL = _IMX_MAKE_PAD_CTL(
                              IMX_SRE_FAST,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_22K_OHM_PU,
                              IMX_HYS_ENABLED),
    // uSDHC2
    IMX_PAD_CFG_SD2_CLK_SD2_CLK = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_CLK_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD2_CLK_ALT0_SD2_CLK),

    IMX_PAD_CFG_SD2_CMD_SD2_CMD = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD2_CMD_ALT0_SD2_CMD),

    IMX_PAD_CFG_SD2_DATA0_SD2_DATA0 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD2_DATA0_ALT0_SD2_DATA0),

    IMX_PAD_CFG_SD2_DATA1_SD2_DATA1 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD2_DATA1_ALT0_SD2_DATA1),

    IMX_PAD_CFG_SD2_DATA2_SD2_DATA2 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD2_DATA2_ALT0_SD2_DATA2),

    IMX_PAD_CFG_SD2_DATA3_SD2_DATA3 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD2_DATA3_ALT0_SD2_DATA3),
    // uSDHC3
    IMX_PAD_CFG_SD3_CLK_SD3_CLK = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_CLK_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_CLK_ALT0_SD3_CLK),

    IMX_PAD_CFG_SD3_CMD_SD3_CMD = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_CMD_ALT0_SD3_CMD),

    IMX_PAD_CFG_SD3_DATA0_SD3_DATA0 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DATA0_ALT0_SD3_DATA0),

    IMX_PAD_CFG_SD3_DATA1_SD3_DATA1 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DATA1_ALT0_SD3_DATA1),

    IMX_PAD_CFG_SD3_DATA2_SD3_DATA2 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DATA2_ALT0_SD3_DATA2),

    IMX_PAD_CFG_SD3_DATA3_SD3_DATA3 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DATA3_ALT0_SD3_DATA3),

    IMX_PAD_CFG_SD3_DATA4_SD3_DATA4 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DATA4_ALT0_SD3_DATA4),

    IMX_PAD_CFG_SD3_DATA5_SD3_DATA5 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DATA5_ALT0_SD3_DATA5),

    IMX_PAD_CFG_SD3_DATA6_SD3_DATA6 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DATA6_ALT0_SD3_DATA6),

    IMX_PAD_CFG_SD3_DATA7_SD3_DATA7 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DATA7_ALT0_SD3_DATA7),

    IMX_PAD_KEY_COL0_GPIO2_IO10 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_GPIO_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_KEY_COL0_ALT5_GPIO2_IO10),

    IMX_PAD_KEY_ROW0_SD3_WP = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_KEY_ROW0_ALT1_SD3_WP),
    // uSDHC4
    IMX_PAD_CFG_SD4_CLK_SD4_CLK = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_CLK_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD4_CLK_ALT0_SD4_CLK),

    IMX_PAD_CFG_SD4_CMD_SD4_CMD = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD4_CMD_ALT0_SD4_CMD),

    IMX_PAD_CFG_SD4_DATA0_SD4_DATA0 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD4_DATA0_ALT0_SD4_DATA0),

    IMX_PAD_CFG_SD4_DATA1_SD4_DATA1 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD4_DATA1_ALT0_SD4_DATA1),

    IMX_PAD_CFG_SD4_DATA2_SD4_DATA2 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD4_DATA2_ALT0_SD4_DATA2),

    IMX_PAD_CFG_SD4_DATA3_SD4_DATA3 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD4_DATA3_ALT0_SD4_DATA3),

    IMX_PAD_SD4_DATA7_SD4_CD_B = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD4_DATA7_ALT6_SD4_CD_B),

    IMX_PAD_SD4_DATA6_SD4_WP = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD4_DATA6_ALT6_SD4_WP),
} IMX_USDHC_PADCFG;

typedef enum {
    IMX_PAD_QSPI1A_DATA0_ALT5_GPIO4_IO16 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_260_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_DISABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_QSPI1A_DATA0_ALT5_GPIO4_IO16),

    IMX_PAD_CFG_ENET1_CRS_ALT5_GPIO2_IO01 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_260_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_DISABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_ENET1_CRS_ALT5_GPIO2_IO01),

    IMX_PAD_CFG_QSPI1A_DATA2_ALT5_GPIO4_IO18 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_260_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_DISABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_QSPI1A_DATA2_ALT5_GPIO4_IO18),

    IMX_PAD_CFG_SD1_DATA1_ALT5_GPIO6_IO03 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_260_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_DISABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD1_DATA1_ALT5_GPIO6_IO03),

    IMX_PAD_CFG_KEY_COL1_ALT5_GPIO2_IO11 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_260_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_DISABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_KEY_COL1_ALT5_GPIO2_IO11)
} IMX_PWR_PADCFG;

typedef enum {
    IMX_PAD_CFG_ENET1_COL_GPIO2_IO00 = _IMX_MAKE_PADCFG (
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_ENET1_COL_ALT5_GPIO2_IO00),
} IMX_IMX6Q_PCIE_PADCFG;

/**
  Initialize SDHC modules on the SOC and perform required pin-muxing
**/
VOID SdhcInit ()
{
    DebugPrint(0xFFFFFFFF, "VOID SdhcInit()\n");

    //
    // uSDHC2: SDIO Socket
    //
    ImxPadConfig(IMX_PAD_SD2_CLK, IMX_PAD_CFG_SD2_CLK_SD2_CLK);
    ImxPadConfig(IMX_PAD_SD2_CMD, IMX_PAD_CFG_SD2_CMD_SD2_CMD);
    ImxPadConfig(IMX_PAD_SD2_DATA0, IMX_PAD_CFG_SD2_DATA0_SD2_DATA0);
    ImxPadConfig(IMX_PAD_SD2_DATA1, IMX_PAD_CFG_SD2_DATA1_SD2_DATA1);
    ImxPadConfig(IMX_PAD_SD2_DATA2, IMX_PAD_CFG_SD2_DATA2_SD2_DATA2);
    ImxPadConfig(IMX_PAD_SD2_DATA3, IMX_PAD_CFG_SD2_DATA3_SD2_DATA3);
    // SD CardDetect
    // Not available
    // SD WriteProtect
    // Not avilable

    //
    // uSDHC3: SDCard Socket (8-bit)
    //
    ImxPadConfig(IMX_PAD_SD3_CLK, IMX_PAD_CFG_SD3_CLK_SD3_CLK);
    ImxPadConfig(IMX_PAD_SD3_CMD, IMX_PAD_CFG_SD3_CMD_SD3_CMD);
    ImxPadConfig(IMX_PAD_SD3_DATA0, IMX_PAD_CFG_SD3_DATA0_SD3_DATA0);
    ImxPadConfig(IMX_PAD_SD3_DATA1, IMX_PAD_CFG_SD3_DATA1_SD3_DATA1);
    ImxPadConfig(IMX_PAD_SD3_DATA2, IMX_PAD_CFG_SD3_DATA2_SD3_DATA2);
    ImxPadConfig(IMX_PAD_SD3_DATA3, IMX_PAD_CFG_SD3_DATA3_SD3_DATA3);
    ImxPadConfig(IMX_PAD_SD3_DATA4, IMX_PAD_CFG_SD3_DATA4_SD3_DATA4);
    ImxPadConfig(IMX_PAD_SD3_DATA5, IMX_PAD_CFG_SD3_DATA5_SD3_DATA5);
    ImxPadConfig(IMX_PAD_SD3_DATA6, IMX_PAD_CFG_SD3_DATA6_SD3_DATA6);
    ImxPadConfig(IMX_PAD_SD3_DATA7, IMX_PAD_CFG_SD3_DATA7_SD3_DATA7);
    // SD CardDetect
    ImxPadConfig (IMX_PAD_KEY_COL0, IMX_PAD_KEY_COL0_GPIO2_IO10);
    // SD WriteProtect
    ImxPadConfig (IMX_PAD_KEY_ROW0, IMX_PAD_KEY_ROW0_SD3_WP);

    //
    // uSDHC4: SDCard Socket (boot)
    //
    ImxPadConfig(IMX_PAD_SD4_CLK, IMX_PAD_CFG_SD4_CLK_SD4_CLK);
    ImxPadConfig(IMX_PAD_SD4_CMD, IMX_PAD_CFG_SD4_CMD_SD4_CMD);
    ImxPadConfig(IMX_PAD_SD4_DATA0, IMX_PAD_CFG_SD4_DATA0_SD4_DATA0);
    ImxPadConfig(IMX_PAD_SD4_DATA1, IMX_PAD_CFG_SD4_DATA1_SD4_DATA1);
    ImxPadConfig(IMX_PAD_SD4_DATA2, IMX_PAD_CFG_SD4_DATA2_SD4_DATA2);
    ImxPadConfig(IMX_PAD_SD4_DATA3, IMX_PAD_CFG_SD4_DATA3_SD4_DATA3);
    // SD CardDetect
    ImxPadConfig (IMX_PAD_SD4_DATA7, IMX_PAD_SD4_DATA7_SD4_CD_B);
    // SD WriteProtect
    ImxPadConfig (IMX_PAD_SD4_DATA6, IMX_PAD_SD4_DATA6_SD4_WP);
}

/**
  Initialize ENETs modules on the SOC and perform required pin-muxing.
**/
VOID EnetInit ()
{
    UINT32                              timeout = 100;
    IMX_CCM_ANALOG_PLL_ENET_REG         ccmAnalogPllReg;
    volatile IMX_CCM_ANALOG_REGISTERS  *ccmAnalogRegistersPtr = (IMX_CCM_ANALOG_REGISTERS *)IMX_CCM_ANALOG_BASE;

    //
    // Apply ENET pin-muxing configurations
    //
    ImxPadConfig(IMX_PAD_ENET1_MDC,     IMX_PAD_ENET1_MDC_ENET_MDC);             // ENET1/ENET2 MDC
    ImxPadConfig(IMX_PAD_ENET1_MDIO,    IMX_PAD_ENET1_MDIO_ENET_MDIO);           // ENET1/ENET2 MDIO
    ImxPadConfig(IMX_PAD_ENET2_CRS,     IMX_PAD_ENET2_CRS_GPIO2_IO07);           // ENET1/ENET2 RST_B
    ImxPadConfig(IMX_PAD_ENET2_COL,     IMX_PAD_ENET2_COL_GPIO2_IO06);           // ENET1/ENET2 PWR_EN_B
    ImxPadConfig(IMX_PAD_ENET2_RX_CLK,  IMX_PAD_ENET2_RX_CLK_ENET_RX_CLK);       // ENET1/ENET2 REF_CLK_25M
    ImxPadConfig(IMX_PAD_RGMII1_TD0,    IMX_PAD_RGMII1_TD0_RGMII_TD0);           // ENET1 RGMII1_TXD0
    ImxPadConfig(IMX_PAD_RGMII1_TD1,    IMX_PAD_RGMII1_TD1_RGMII_TD1);           // ENET1 RGMII1_TXD1
    ImxPadConfig(IMX_PAD_RGMII1_TD2,    IMX_PAD_RGMII1_TD2_RGMII_TD2);           // ENET1 RGMII1_TXD2
    ImxPadConfig(IMX_PAD_RGMII1_TD3,    IMX_PAD_RGMII1_TD3_RGMII_TD3);           // ENET1 RGMII1_TXD3
    ImxPadConfig(IMX_PAD_RGMII1_TXC,    IMX_PAD_RGMII1_TXC_RGMII_TXC);           // ENET1 RGMII1_TXCLK
    ImxPadConfig(IMX_PAD_RGMII1_TX_CTL, IMX_PAD_RGMII1_TX_CTL_RGMII_TX_CTL);     // ENET1 RGMII1_TXEN
    ImxPadConfig(IMX_PAD_RGMII1_RD0,    IMX_PAD_RGMII1_RD0_ENET_RGMII_RD0);      // ENET1 RGMII1_RXD0
    ImxPadConfig(IMX_PAD_RGMII1_RD1,    IMX_PAD_RGMII1_RD1_ENET_RGMII_RD1);      // ENET1 RGMII1_RXD1
    ImxPadConfig(IMX_PAD_RGMII1_RD2,    IMX_PAD_RGMII1_RD2_ENET_RGMII_RD2);      // ENET1 RGMII1_RXD2
    ImxPadConfig(IMX_PAD_RGMII1_RD3,    IMX_PAD_RGMII1_RD3_ENET_RGMII_RD3);      // ENET1 RGMII1_RXD3
    ImxPadConfig(IMX_PAD_RGMII1_RXC,    IMX_PAD_RGMII1_RXC_ENET_RGMII_RXC);      // ENET1 RGMII1_RXCLK
    ImxPadConfig(IMX_PAD_RGMII1_RX_CTL, IMX_PAD_RGMII1_RX_CTL_RGMII_RX_CTL);     // ENET1 RGMII1_RXDV
    ImxPadConfig(IMX_PAD_RGMII2_TD0,    IMX_PAD_RGMII1_TD0_RGMII_TD0);           // ENET2 RGMII2_TXD0
    ImxPadConfig(IMX_PAD_RGMII2_TD1,    IMX_PAD_RGMII1_TD1_RGMII_TD1);           // ENET2 RGMII2_TXD1
    ImxPadConfig(IMX_PAD_RGMII2_TD2,    IMX_PAD_RGMII1_TD2_RGMII_TD2);           // ENET2 RGMII2_TXD2
    ImxPadConfig(IMX_PAD_RGMII2_TD3,    IMX_PAD_RGMII1_TD3_RGMII_TD3);           // ENET2 RGMII2_TXD3
    ImxPadConfig(IMX_PAD_RGMII2_TXC,    IMX_PAD_RGMII1_TXC_RGMII_TXC);           // ENET2 RGMII2_TXCLK
    ImxPadConfig(IMX_PAD_RGMII2_TX_CTL, IMX_PAD_RGMII1_TX_CTL_RGMII_TX_CTL);     // ENET2 RGMII2_TXEN
    ImxPadConfig(IMX_PAD_RGMII2_RD0,    IMX_PAD_RGMII1_RD0_ENET_RGMII_RD0);      // ENET2 RGMII2_RXD0
    ImxPadConfig(IMX_PAD_RGMII2_RD1,    IMX_PAD_RGMII1_RD1_ENET_RGMII_RD1);      // ENET2 RGMII2_RXD1
    ImxPadConfig(IMX_PAD_RGMII2_RD2,    IMX_PAD_RGMII1_RD2_ENET_RGMII_RD2);      // ENET2 RGMII2_RXD2
    ImxPadConfig(IMX_PAD_RGMII2_RD3,    IMX_PAD_RGMII1_RD3_ENET_RGMII_RD3);      // ENET2 RGMII2_RXD3
    ImxPadConfig(IMX_PAD_RGMII2_RXC,    IMX_PAD_RGMII2_RXC_ENET_RGMII_RXC);      // ENET2 RGMII2_RXCLK
    ImxPadConfig(IMX_PAD_RGMII2_RX_CTL, IMX_PAD_RGMII1_RX_CTL_RGMII_RX_CTL);     // ENET2 RGMII2_RXDV
    // Enable ENET_3V3 (PWR_EN_B, GPIO2_IO06)
    ImxGpioDirection(IMX_GPIO_BANK2, 6, IMX_GPIO_DIR_OUTPUT);
    ImxGpioWrite(IMX_GPIO_BANK2, 6, IMX_GPIO_LOW);
    // Reset PHY (RST_B, GPIO2_IO07)
    ImxGpioDirection(IMX_GPIO_BANK2, 7, IMX_GPIO_DIR_OUTPUT);
    ImxGpioWrite(IMX_GPIO_BANK2, 7, IMX_GPIO_LOW);  // RST_B = 0
    MicroSecondDelay(500);
    ImxGpioWrite(IMX_GPIO_BANK2, 7, IMX_GPIO_HIGH); // RST_B = 1
    // Enable ENET PLL, also required for PCIe
    ccmAnalogPllReg.AsUint32 = MmioRead32((UINTN)&ccmAnalogRegistersPtr->PLL_ENET);
    // For RGMII the TX_CLK must be 125 MHz, set this frequency for both ENETs;
    ccmAnalogPllReg.AsUint32 |= 0x0000000F;
    MmioWrite32((UINTN)&ccmAnalogRegistersPtr->PLL_ENET, ccmAnalogPllReg.AsUint32);
    ccmAnalogPllReg.AsUint32 = MmioRead32((UINTN)&ccmAnalogRegistersPtr->PLL_ENET);
    // If ENET PLL is not enabled and locked enable it and wait for lock.
    if ((ccmAnalogPllReg.AsUint32 & (IMX6SX_ANALOG_PLL_ENET_POWERDOWN | IMX6SX_ANALOG_PLL_ENET_LOCK)) != IMX6SX_ANALOG_PLL_ENET_LOCK) {
		ccmAnalogPllReg.AsUint32 &= ~IMX6SX_ANALOG_PLL_ENET_POWERDOWN;
        MmioWrite32((UINTN)&ccmAnalogRegistersPtr->PLL_ENET, ccmAnalogPllReg.AsUint32);
        while (timeout--) {
            if ((ccmAnalogPllReg.AsUint32 = MmioRead32((UINTN)&ccmAnalogRegistersPtr->PLL_ENET)) & IMX6SX_ANALOG_PLL_ENET_LOCK) {
                break;
            }
            MicroSecondDelay(100);
        }
		if (timeout < 0) {
            DEBUG ((DEBUG_ERROR, "PLL_ENET is not locked!\n"));
            return;
        }
    }
    // Enable ENETs 125Mhz output and 25 MHz reference clock
    ccmAnalogPllReg.AsUint32 &= ~(IMX6SX_ANALOG_PLL_ENET_BYPASS);
    ccmAnalogPllReg.AsUint32 |= (IMX6SX_ANALOG_PLL_ENET_ENET1_125M_REF_EN | IMX6SX_ANALOG_PLL_ENET_ENET2_125M_REF_EN | \
                                 IMX6SX_ANALOG_PLL_ENET_ENET_25M_REF_EN | IMX6SX_ANALOG_PLL_ENET_ENABLE_125M);
    MmioWrite32((UINTN)&ccmAnalogRegistersPtr->PLL_ENET, ccmAnalogPllReg.AsUint32);
}

/**
  Initialize I2C modules on the SOC and perform required pin-muxing
**/
VOID I2cInit ()
{
    // I2C1
    ImxPadConfig (IMX_PAD_GPIO1_IO00, IMX_PAD_CFG_GPIO1_IO00_I2C1_SCL);
    ImxPadConfig (IMX_PAD_GPIO1_IO01, IMX_PAD_CFG_GPIO1_IO01_I2C1_SDA);
    // I2C2
    ImxPadConfig (IMX_PAD_GPIO1_IO02, IMX_PAD_CFG_GPIO1_IO02_I2C2_SCL);
    ImxPadConfig (IMX_PAD_GPIO1_IO03, IMX_PAD_CFG_GPIO1_IO03_I2C2_SDA);
    // I2C3
    ImxPadConfig (IMX_PAD_KEY_COL4, IMX_PAD_CFG_KEY_COL4_I2C3_SCL);
    ImxPadConfig (IMX_PAD_KEY_ROW4, IMX_PAD_CFG_KEY_ROW4_I2C3_SDA);
    // I2C4
    ImxPadConfig (IMX_PAD_CSI_DATA06, IMX_PAD_CFG_CSI_DATA06_I2C4_SCL);
    ImxPadConfig (IMX_PAD_CSI_DATA07, IMX_PAD_CFG_CSI_DATA07_I2C4_SDA);
}

/**
  Initialize PCI Express module on the SOC and perform required pin-muxing
**/
VOID PcieInit ()
{
    volatile IMX_CCM_ANALOG_REGISTERS  *ccmAnalogRegistersPtr = (IMX_CCM_ANALOG_REGISTERS *)IMX_CCM_ANALOG_BASE;
    volatile IMX_CCM_REGISTERS         *ccmRegistersPtr = (IMX_CCM_REGISTERS *)IMX_CCM_BASE;
    IMX_CCM_ANALOG_MISC1_REG            ccmAnalogMisc1Reg;
    IMX_CCM_CBCMR_REG                   ccmCbcmrReg;

    // PCIe GPIO Reset
    ImxPadConfig (IMX_PAD_ENET1_COL, IMX_PAD_CFG_ENET1_COL_GPIO2_IO00);
    ImxGpioWrite (IMX_GPIO_BANK2, 0, IMX_GPIO_LOW);
    ImxGpioDirection (IMX_GPIO_BANK2, 0, IMX_GPIO_DIR_OUTPUT);

    ccmAnalogMisc1Reg.AsUint32 = MmioRead32((UINTN)&ccmAnalogRegistersPtr->MISC1);
    ccmAnalogMisc1Reg.LVDS1_CLK_SEL = 0x0A;
    ccmAnalogMisc1Reg.LVDSCLK1_OBEN = 0x01;   // Enable the LVDS output buffer for anaclk1/1b.
    ccmAnalogMisc1Reg.LVDSCLK1_IBEN = 0x00;   // Disable the LVDS input buffer for anaclk1/1b.
    MmioWrite32((UINTN)&ccmAnalogRegistersPtr->MISC1, ccmAnalogMisc1Reg.AsUint32);

    // Reference clock sourced from AXI
    ccmCbcmrReg.AsUint32 = MmioRead32((UINTN)&ccmRegistersPtr->CBCMR);
    ccmCbcmrReg.pcie_axi_clock_sel = 0;
    MmioWrite32((UINTN)&ccmRegistersPtr->CBCMR, ccmCbcmrReg.AsUint32);
}

/* Audmux initialization. Supports all audmux regs instead of hardcoded orig. version in 
   iMXAudio.c. This should be moved to audmux HAL layer together with audmux regs */
VOID InitializeAudmuxSX (UINT32 SsiPortNumber, UINT32 ExternalPortNumber)
{
    volatile AUDMUX_REGISTER_ARRAY*  pAudmuxReg;
    AUDMUX_PDCR_REG             ExternalPdcr;
    AUDMUX_PTCR_REG             ExternalPtcr;
    AUDMUX_PDCR_REG             SsiPdcr;
    AUDMUX_PTCR_REG             SsiPtcr;

    ASSERT (SsiPortNumber < AUDMUX_PORT_MAX);
    ASSERT (ExternalPortNumber < AUDMUX_PORT_MAX);

    pAudmuxReg = (AUDMUX_REGISTER_ARRAY *) AUDMUX_BASE;
    ExternalPdcr = (AUDMUX_PDCR_REG) MmioRead32 ((UINTN)&pAudmuxReg->mux[ExternalPortNumber].Pdcr);
    ExternalPdcr.MODE = AUDMUX_NORMAL_MODE;
    ExternalPdcr.RXDSEL = SsiPortNumber;
    // According to the spec, ReceiveClockDirectionControl and
    // SynchronousAsynchronousSelect should never be changed at the same time.
    MmioWrite32 ((UINTN)&pAudmuxReg->mux[ExternalPortNumber].Pdcr, ExternalPdcr.AsUint32);
    ExternalPdcr.TXRXEN = AUDMUX_TRSE_NOSWITCH;
    MmioWrite32 ((UINTN)&pAudmuxReg->mux[ExternalPortNumber].Pdcr, ExternalPdcr.AsUint32);

    ExternalPtcr = (AUDMUX_PTCR_REG) MmioRead32 ((UINTN)&pAudmuxReg->mux[ExternalPortNumber].Ptcr);
    ExternalPtcr.TFS_DIR = AUDMUX_OUTPUT;
    ExternalPtcr.TFSEL = SsiPortNumber;
    ExternalPtcr.TCLKDIR = AUDMUX_OUTPUT;
    ExternalPtcr.TCSEL = SsiPortNumber;
    ExternalPtcr.RFS_DIR = AUDMUX_OUTPUT;
    ExternalPtcr.RFSEL = SsiPortNumber;
    ExternalPtcr.RCLKDIR = AUDMUX_OUTPUT;
    ExternalPtcr.RCSEL = SsiPortNumber;
    ExternalPtcr.SYN = AUDMUX_SYNCHRONOUS_MODE;
    MmioWrite32 ((UINTN)&pAudmuxReg->mux[ExternalPortNumber].Ptcr, ExternalPtcr.AsUint32);

    SsiPdcr = (AUDMUX_PDCR_REG) MmioRead32 ((UINTN)&pAudmuxReg->mux[SsiPortNumber].Pdcr);
    SsiPdcr.MODE = AUDMUX_NORMAL_MODE;
    SsiPdcr.RXDSEL = ExternalPortNumber;
    MmioWrite32 ((UINTN)&pAudmuxReg->mux[SsiPortNumber].Pdcr, SsiPdcr.AsUint32);
    // According to the spec, ReceiveClockDirectionControl and
    // SynchronousAsynchronousSelect should never be changed at the same time.
    SsiPdcr.TXRXEN = AUDMUX_TRSE_NOSWITCH;
    MmioWrite32 ((UINTN)&pAudmuxReg->mux[SsiPortNumber].Pdcr, SsiPdcr.AsUint32);

    SsiPtcr = (AUDMUX_PTCR_REG) MmioRead32 ((UINTN)&pAudmuxReg->mux[SsiPortNumber].Ptcr);
    SsiPtcr.TFS_DIR = AUDMUX_INPUT;
    SsiPtcr.TCLKDIR = AUDMUX_INPUT;
    SsiPtcr.RFS_DIR = AUDMUX_INPUT;
    SsiPtcr.RCLKDIR = AUDMUX_INPUT;
    MmioWrite32 ((UINTN)&pAudmuxReg->mux[SsiPortNumber].Ptcr, SsiPtcr.AsUint32);
    // According to the spec, ReceiveClockDirectionControl and
    // SynchronousAsynchronousSelect should never be changed at the same time.
    SsiPtcr.SYN = AUDMUX_SYNCHRONOUS_MODE;
    MmioWrite32 ((UINTN)&pAudmuxReg->mux[SsiPortNumber].Ptcr, SsiPtcr.AsUint32);
}

/** Audio init */
VOID SetupAudio ()
{

    volatile IMX_CCM_REGISTERS *ccmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;
  
    IMX_CCM_CS1CDR_REG csicdr = (IMX_CCM_CS1CDR_REG) MmioRead32((UINTN) &ccmRegisters->CS1CDR);

    csicdr.ssi3_clk_pred = 0; // divide by 1.
    csicdr.ssi3_clk_podf = (15 - 1); // divide by 15.

    MmioWrite32((UINTN) &ccmRegisters->CS1CDR, csicdr.AsUint32);

    ImxPadConfig(IMX_PAD_CSI_PIXCLK, IMX_PAD_CFG_CSI_PIXCLK_AUDIO_CLK_OUT);
    ImxPadConfig(IMX_PAD_CSI_DATA00, IMX_PAD_CFG_CSI_DATA00_AUD6_TXC);
    ImxPadConfig(IMX_PAD_CSI_DATA01, IMX_PAD_CFG_CSI_DATA01_AUD6_TXFS);
    ImxPadConfig(IMX_PAD_CSI_VSYNC, IMX_PAD_CFG_CSI_VSYNC_AUD6_RXD);
    ImxPadConfig(IMX_PAD_CSI_HSYNC, IMX_PAD_CFG_CSI_HSYNC_AUD6_TXD);

    // Set up Audmux where Ssi3 port = 7 and external port = 6
    InitializeAudmuxSX (AUDMUX_PORT7, AUDMUX_PORT6);
}

/**
  Initialize clock and power for modules on the SOC.
**/
VOID ImxClkPwrInit ()
{
    ImxPadConfig(IMX_PAD_QSPI1A_DATA0, IMX_PAD_QSPI1A_DATA0_ALT5_GPIO4_IO16);             // PERI_PWR_EN
    // Enable ENET_3V3 (PERI_PWR_EN, GPIO4_IO16)
    ImxGpioDirection(IMX_GPIO_BANK4, 16, IMX_GPIO_DIR_OUTPUT);
    ImxGpioWrite(IMX_GPIO_BANK4, 16, IMX_GPIO_HIGH);
    // Enable PCIE_PWR_EN (GPIO2_IO01)
    ImxPadConfig (IMX_PAD_ENET1_CRS, IMX_PAD_CFG_ENET1_CRS_ALT5_GPIO2_IO01);
    ImxGpioWrite (IMX_GPIO_BANK2, 1, IMX_GPIO_HIGH);
    ImxGpioDirection (IMX_GPIO_BANK2, 1, IMX_GPIO_DIR_OUTPUT);

    // Set CABC to high level (GPIO4_IO18)
    ImxPadConfig (IMX_PAD_QSPI1A_DATA2, IMX_PAD_CFG_QSPI1A_DATA2_ALT5_GPIO4_IO18);
    ImxGpioWrite (IMX_GPIO_BANK4, 18, IMX_GPIO_HIGH);
    ImxGpioDirection (IMX_GPIO_BANK4, 18, IMX_GPIO_DIR_OUTPUT);
    // Set LCD PWM4_OUT to high level (GPIO6_IO03)
    ImxPadConfig (IMX_PAD_SD1_DATA1, IMX_PAD_CFG_SD1_DATA1_ALT5_GPIO6_IO03);
    ImxGpioWrite (IMX_GPIO_BANK6, 3, IMX_GPIO_HIGH);
    ImxGpioDirection (IMX_GPIO_BANK6, 3, IMX_GPIO_DIR_OUTPUT);

    // Enable Power for SD3
    ImxPadConfig (IMX_PAD_KEY_COL1, IMX_PAD_CFG_KEY_COL1_ALT5_GPIO2_IO11);
    ImxGpioWrite (IMX_GPIO_BANK2, 11, IMX_GPIO_HIGH);
    ImxGpioDirection (IMX_GPIO_BANK2, 11, IMX_GPIO_DIR_OUTPUT);
}

/**
  Initialize USB modules on the SOC and perform required pin-muxing
**/
VOID USBInit(VOID)
{
    UINT32 regVal;
    UINT32 gpioPadConfig;
    gpioPadConfig = IMX6SX_MUX_PADCTL_SPEED_100MHZ |
        IMX6SX_MUX_PADCTL_PKE |
        IMX6SX_MUX_PADCTL_DSE_43_OHM |
        IMX6SX_MUX_PADCTL_SRE_SLOW;
    //
    // USB 2.0 Controller 0 - USB_OTG1 in OTG mode - micro A/B socket (client/host) - has ID pin
    //
    // USB_OTG1_OC -- pad GPIO1_IO8
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO08, 0);
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_USB_IPP_IND_OTG_OC_SELECT_INPUT, 0);
    // USB_OTG1_PWR -- pad GPIO1_IO9
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO09, 0);
    // USB_OTG1_ID -- pad GPIO1_IO10
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO10, 0);
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_ANATOP_USB_OTG_ID_SELECT_INPUT, 0);
    //
    //  USB 2.0 Controller Core 1 - USB_OTG2 in HOST mode - regular size A socket (host) - no ID pin
    //
    // OC -- GPIO1_IO11
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO11, 1);
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_USB_IPP_IND_OTG2_OC_SELECT_INPUT, 0);
    // PWR -- GPIO1_I12
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO12, 0);
    // ID -- GPIO1_IO13 (not used in host mode)
    // MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO13, 1);
    // MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_ANATOP_USB_UH1_ID_SELECT_INPUT, 0);

    // WIR=0, WKUP_VBUS_EN=0, WKUP_ID_EN=0, WKUP_SW=0, WKUP_SW_EN=0, WIE=0, UTMI_ON_CLOCK=1, SUSPENDM=1, RESET=0, PWR_POL=1, OVER_CUR_POL=1, OVER_CUR_DIS = 1
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_OTG1_CTRL, 0x00003380);
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_OTG2_CTRL, 0x00003380);

    MicroSecondDelay(100);
    // Disable charger detection - these signals are active low.
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_USB1_CHRG_DETECT_SET, IMX6SX_ANALOG_USB_CHRG_DETECT_EN_B | IMX6SX_ANALOG_USB_CHK_CHRG_B);
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_USB2_CHRG_DETECT_SET, IMX6SX_ANALOG_USB_CHRG_DETECT_EN_B | IMX6SX_ANALOG_USB_CHK_CHRG_B);
    // Clear clock bypasses.
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_PLL_USB1_CLR, IMX6SX_ANALOG_PLL_USB_BYPASS);
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_PLL_USB2_CLR, IMX6SX_ANALOG_PLL_USB_BYPASS);
    // Ensure plls are powered up, enabled, and USB clocks are enabled.
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_PLL_USB1_SET, IMX6SX_ANALOG_PLL_USB_ENABLE | IMX6SX_ANALOG_PLL_USB_POWER | IMX6SX_ANALOG_PLL_USB_EN_USB_CLKS);
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_PLL_USB2_SET, IMX6SX_ANALOG_PLL_USB_ENABLE | IMX6SX_ANALOG_PLL_USB_POWER | IMX6SX_ANALOG_PLL_USB_EN_USB_CLKS);
    // Ensure PLLs are locked before we continue.
    do {
        regVal = MmioRead32(IMX6SX_PHYSADDR_ANALOG_PLL_USB1);
    } while (!(regVal & IMX6SX_ANALOG_PLL_USB_LOCK));
    do {
        regVal = MmioRead32(IMX6SX_PHYSADDR_ANALOG_PLL_USB2);
    } while (!(regVal & IMX6SX_ANALOG_PLL_USB_LOCK));
    // Turn on (ungate) internal phy clocks.
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY1_CTRL_CLR, 0x40000000);
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY2_CTRL_CLR, 0x40000000);
    MicroSecondDelay(100);
    // Enable clocks to UTMI blocks.
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_USB1_MISC_SET, 0x40000000);
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_USB2_MISC_SET, 0x40000000);
    MicroSecondDelay(10);
    // Install defaults to registers.
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_UH_CTRL, 0x00001000);
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_UH_HSIC_CTRL, 0x00000042);
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_OTG1_PHY_CTRL_0, 0x00000000);
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_OTG2_PHY_CTRL_0, 0x00000098);
    // Stop the hosts.
    regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBMODE);
    if ((regVal & 3) == 3) {
        MmioClrBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBCMD, 1);
        do {
            regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBSTS);
        } while (!(regVal & 0x00001000));
    }
    regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBMODE);
    if ((regVal & 3) == 3) {
        MmioClrBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBCMD, 1);
        do {
            regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBSTS);
        } while (!(regVal & 0x00001000));
    }
    MmioClrBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UH1_USBCMD, 1);
    do {
        regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UH1_USBSTS);
    } while (!(regVal & 0x00001000));
    // Reset the controllers.
    MmioSetBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBCMD, 2);
    MmioSetBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBCMD, 2);
    MmioSetBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UH1_USBCMD, 2);
    // Wait for controllers to complete reset.
    do {
        regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBCMD);
    } while (regVal & 2);
    do {
        regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBCMD);
    } while (regVal & 2);
    do {
        regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UH1_USBCMD);
    } while (regVal & 2);
    // Confirm UOG1 reset (set mode to zero), then set controller UOG1 as a client.
    regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBMODE);
    ASSERT((regVal & 3) == 0);
    regVal |= (2 | 8);  // Set SLOM to disable lockout.
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBMODE, regVal);
    regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBMODE);
    ASSERT((regVal & 3) == 2);
    // Confirm UOG2 reset (set mode to zero), then set controller UOG2 as a host.
    regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBMODE);
    ASSERT((regVal & 3) == 0);
    regVal |= (3 | 8);  // Set SLOM to disable lockout
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBMODE, regVal);
    regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBMODE);
    ASSERT((regVal & 3) == 3);
    // Set USB PHY resets.
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY1_CTRL_SET, IMX6SX_USBPHY_CTRL_SFTRST);
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY2_CTRL_SET, IMX6SX_USBPHY_CTRL_SFTRST);
    MicroSecondDelay(10);
    // Release resets and ungate clocks.
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY1_CTRL_CLR, IMX6SX_USBPHY_CTRL_SFTRST | IMX6SX_USBPHY_CTRL_CLKGATE);
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY2_CTRL_CLR, IMX6SX_USBPHY_CTRL_SFTRST | IMX6SX_USBPHY_CTRL_CLKGATE);
    MicroSecondDelay(10);
    // Power up phys.
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY1_PWD, 0);
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY2_PWD, 0);
    MicroSecondDelay(10);
    // Enable Full and Low speed devices.
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY1_CTRL_SET, IMX6SX_USBPHY_CTRL_ENUTMILEVEL3 | IMX6SX_USBPHY_CTRL_ENUTMILEVEL2);
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY2_CTRL_SET, IMX6SX_USBPHY_CTRL_ENUTMILEVEL3 | IMX6SX_USBPHY_CTRL_ENUTMILEVEL2);
    MicroSecondDelay(10);
}

/**
  UART initialization code. On imx6SX UBOOT selects OSC as UART_CLK_ROOT source.
  This codes switch the UART_CLK_ROOT source to USB_PLL and updates debug uart baud rate = 115200
**/
VOID UartInit()
{
    volatile MX6UART_REGISTERS   *UartBase;
    volatile IMX_CCM_REGISTERS   *CcmBase = (IMX_CCM_REGISTERS *)IMX_CCM_BASE;

    UartBase = (MX6UART_REGISTERS*)FixedPcdGet32 (PcdSerialRegisterBase);
    // UART_CLK = pll3 (80 MHz), UART_CLK_PODF = 1;
    MmioWrite32((UINTN)&CcmBase->CSCDR1, MmioRead32((UINTN)&CcmBase->CSCDR1) & ~0x7F);
    MmioWrite32((UINTN)&UartBase->Ufcr, (MmioRead32((UINTN)&UartBase->Ufcr) & ~0x380) | (4<<7));
    MmioWrite32((UINTN)&UartBase->Ubir, 0x0F);  // Set baud rate = 115200
    MmioWrite32((UINTN)&UartBase->Ubmr, 348);
    return;
}

/**
  Reset LCD controller.
**/
VOID LcdReset ()
{
    UINT32 value32;
    DEBUG((DEBUG_INFO,"ResetLCDIF2 controller\n"));

    // reset LCD controller (dancing bits reset procedure)
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
}

/**
  Initialize controllers that must setup at the early stage.
**/
RETURN_STATUS ArmPlatformInitialize(IN UINTN MpId)
{
    if (!ArmPlatformIsPrimaryCore(MpId)) {
        return RETURN_SUCCESS;
    }
    ImxClkPwrInit();
    // initialize default UEFI debug port early so we can use its debug output
    UartInit();
    SerialPortInitialize();
    SerialPortWrite((UINT8 *)SERIAL_DEBUG_PORT_INIT_MSG, (UINTN)sizeof (SERIAL_DEBUG_PORT_INIT_MSG));
    I2cInit();
    USBInit();
    SdhcInit();
    EnetInit();
    PcieInit();
    SetupAudio();
    LcdReset();
    return RETURN_SUCCESS;
}

/**
  Return the current Boot Mode.
  This function returns the boot reason on the platform.
**/
EFI_BOOT_MODE ArmPlatformGetBootMode(VOID)
{
    return BOOT_WITH_FULL_CONFIGURATION;
}
