/** @file
*  platform initialization for iMX6 DualLite SOM
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
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>

#include <iMX6.h>
#include <iMX6Audio.h>
#include <iMX6ClkPwr.h>
#include <iMX6IoMux.h>
#include <iMX6Timer.h>
#include <iMX6UsbPhy.h>

#include <IndustryStandard/Tpm20.h>
#include <IndustryStandard/Tpm2Acpi.h>

//
// Prebaked pad configurations that include mux and drive settings where
// each enum named as IMX_<MODULE-NAME>_PADCFG contains configurations
// for pads used by that module
//

typedef enum {
    IMX_PAD_ENET_MDIO_ENET_MDIO = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_ENABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_ENET_MDIO_ALT1_ENET_MDIO),
                              
    IMX_PAD_ENET_MDC_ENET_MDC = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_ENET_MDC_ALT1_ENET_MDC),
                              
    IMX_PAD_CFG_KEY_ROW4_GPIO4_IO15 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PD,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_KEY_ROW4_ALT5_GPIO4_IO15),

    IMX_PAD_DI0_PIN2_GPIO4_IO18 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PD,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_DI0_PIN2_ALT5_GPIO4_IO18),
                              
    IMX_PAD_GPIO_16_ENET_REF_CLK = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO_16_ALT2_ENET_REF_CLK,
                              IOMUXC_ENET_REF_CLK_SELECT_INPUT,
                              GPIO16_ALT2),
                              
    IMX_PAD_RGMII_TXC_RGMII_TXC = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII_TXC_ALT1_RGMII_TXC),
                              
    IMX_PAD_RGMII_TD0_RGMII_TD0 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII_TD0_ALT1_RGMII_TD0),
                              
    IMX_PAD_RGMII_TD1_RGMII_TD1 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII_TD1_ALT1_RGMII_TD1),
                              
    IMX_PAD_RGMII_TD2_RGMII_TD2 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII_TD2_ALT1_RGMII_TD2),
                              
    IMX_PAD_RGMII_TD3_RGMII_TD3 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII_TD3_ALT1_RGMII_TD3),
                              
    IMX_PAD_RGMII_TX_CTL_RGMII_TX_CTL = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII_TX_CTL_ALT1_RGMII_TX_CTL),

    IMX_PAD_ENET_REF_CLK_ENET_REF_CLK = _IMX_MAKE_PADCFG(
                              IMX_SRE_FAST,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PD,
                              IMX_HYS_DISABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_ENET_REF_CLK_ALT1_ENET_TX_CLK),
                              
    IMX_PAD_RGMII_RXC_ENET_RGMII_RXC = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII_RXC_ALT1_RGMII_RXC),
                              
    IMX_PAD_RGMII_RD0_ENET_RGMII_RD0 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PD,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII_RD0_ALT1_RGMII_RD0),
                              
    IMX_PAD_RGMII_RD1_ENET_RGMII_RD1 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PD,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII_RD1_ALT1_RGMII_RD1),

    IMX_PAD_RGMII_RD2_ENET_RGMII_RD2 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII_RD2_ALT1_RGMII_RD2),
                              
    IMX_PAD_RGMII_RD3_ENET_RGMII_RD3 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII_RD3_ALT1_RGMII_RD3),
                              
    IMX_PAD_RGMII_RX_CTL_RGMII_RX_CTL = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_KEEP,
                              IMX_PUS_100K_OHM_PD,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_RGMII_RX_CTL_ALT1_RGMII_RX_CTL),
} IMX_ENET_PADCFG;

typedef enum {
    IMX_PAD_CFG_EIM_DATA22_GPIO3_IO22_USB_OTG_PWR = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_EIM_D22_ALT5_GPIO3_IO22),

    IMX_PAD_CFG_KEY_COL4_USB_OTG_OC = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_KEY_COL4_ALT2_USB_OTG_OC,
                              IOMUXC_USB_OTG_OC_SELECT_INPUT,
                              KEY_COL4_ALT2),

    IMX_PAD_CFG_GPIO0_GPIO1_IO0_USBH1_PWR = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO_0_ALT5_GPIO1_IO00),

    IMX_PAD_CFG_GPIO01_USB_OTG_ID_HOST = _IMX_MAKE_PADCFG(
                              IMX_SRE_FAST,
                              IMX_DSE_90_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PD,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO_1_ALT3_USB_OTG_ID),

    IMX_PAD_CFG_GPIO01_USB_OTG_ID_DEVICE = _IMX_MAKE_PADCFG(
                              IMX_SRE_FAST,
                              IMX_DSE_90_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO_1_ALT3_USB_OTG_ID),

} IMX_EHCI_PADCFG;

typedef enum {
    IMX_PAD_CFG_SD4_DATA3_GPIO2_IO11 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD4_DAT3_ALT5_GPIO2_IO11),

} IMX_IMX6Q_PCIE_PADCFG;

typedef enum {

  IMX_PAD_CFG_EIM_EB3_GPIO2_IO31 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_260_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_DISABLED,
                              IMX_SION_ENABLED,
                              IMX_IOMUXC_EIM_EB3_ALT5_GPIO2_IO31),

  IMX_PAD_CFG_EIM_D21_I2C1_SCL = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST, // 1
                              IMX_DSE_40_OHM, // 110
                              IMX_SPEED_MEDIUM, // 10
                              IMX_ODE_ENABLE, // 1
                              IMX_PKE_ENABLE, // 1
                              IMX_PUE_PULL, // 1
                              IMX_PUS_47K_OHM_PU,
                              IMX_HYS_ENABLED, // 1
                              IMX_SION_ENABLED, // 1
                              IMX_IOMUXC_EIM_D21_ALT6_I2C1_SCL,
                              IOMUXC_I2C1_SCL_IN_SELECT_INPUT,
                              EIM_DATA21_ALT6),

  IMX_PAD_CFG_EIM_D28_I2C1_SDA = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST, // 1
                              IMX_DSE_40_OHM, // 110
                              IMX_SPEED_MEDIUM, // 10
                              IMX_ODE_ENABLE, // 1
                              IMX_PKE_ENABLE, // 1
                              IMX_PUE_PULL, // 1
                              IMX_PUS_47K_OHM_PU,
                              IMX_HYS_ENABLED, // 1
                              IMX_SION_ENABLED, // 1
                              IMX_IOMUXC_EIM_D28_ALT1_I2C1_SDA,
                              IOMUXC_I2C1_SDA_IN_SELECT_INPUT,
                              EIM_DATA28_ALT1),

  IMX_PAD_CFG_EIM_D17_I2C3_SCL = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST, // 1
                              IMX_DSE_40_OHM, // 110
                              IMX_SPEED_MEDIUM, // 10
                              IMX_ODE_ENABLE, // 1
                              IMX_PKE_ENABLE, // 1
                              IMX_PUE_PULL, // 1
                              IMX_PUS_47K_OHM_PU,
                              IMX_HYS_ENABLED, // 1
                              IMX_SION_ENABLED, // 1
                              IMX_IOMUXC_EIM_D17_ALT6_I2C3_SCL,
                              IOMUXC_I2C3_SCL_IN_SELECT_INPUT,
                              EIM_DATA17_ALT6),

  IMX_PAD_CFG_EIM_D18_I2C3_SDA = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST, // 1
                              IMX_DSE_40_OHM, // 110
                              IMX_SPEED_MEDIUM, // 10
                              IMX_ODE_ENABLE, // 1
                              IMX_PKE_ENABLE, // 1
                              IMX_PUE_PULL, // 1
                              IMX_PUS_47K_OHM_PU,
                              IMX_HYS_ENABLED, // 1
                              IMX_SION_ENABLED, // 1
                              IMX_IOMUXC_EIM_D18_ALT6_I2C3_SDA,
                              IOMUXC_I2C3_SDA_IN_SELECT_INPUT,
                              EIM_DATA18_ALT6),

  IMX_PAD_CFG_NANDF_WP_B_I2C4_SCL = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST, // 1
                              IMX_DSE_40_OHM, // 110
                              IMX_SPEED_MEDIUM, // 10
                              IMX_ODE_ENABLE, // 1
                              IMX_PKE_ENABLE, // 1
                              IMX_PUE_PULL, // 1
                              IMX_PUS_47K_OHM_PU,
                              IMX_HYS_ENABLED, // 1
                              IMX_SION_ENABLED, // 1
                              IMX_IOMUXC_NANDF_WP_B_ALT9_I2C4_SCL,
                              IOMUXC_I2C4_SCL_IN_SELECT_INPUT,
                              NAND_WP_B_ALT9),

  IMX_PAD_CFG_NANDF_CS3_I2C4_SDA = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_FAST, // 1
                              IMX_DSE_40_OHM, // 110
                              IMX_SPEED_MEDIUM, // 10
                              IMX_ODE_ENABLE, // 1
                              IMX_PKE_ENABLE, // 1
                              IMX_PUE_PULL, // 1
                              IMX_PUS_47K_OHM_PU,
                              IMX_HYS_ENABLED, // 1
                              IMX_SION_ENABLED, // 1
                              IMX_IOMUXC_NANDF_CS3_ALT6_I2C4_SDA,
                              IOMUXC_I2C4_SDA_IN_SELECT_INPUT,
                              NAND_CS3_B_ALT9)

} IMX_I2C_PADCFG;

typedef enum {
  //
  // CCM_CLK01 - this output is the master clock to the audio codec.
  //
  IMX_PAD_CFG_GPIO_5_CCM_CLKO1 = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO_5_ALT3_CCM_CLKO1),
  //
  // KEY_COL0, KEY_ROW0, KEY_COL1, DSP_DAT19 are alt2 to select the AUD5 I2s lines (TXC/TXD/TXFS).
  //
  IMX_PAD_CFG_KEY_COL0_AUD5_TXC = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_KEY_COL0_ALT2_AUD5_TXC,
                              IOMUXC_AUD5_INPUT_TXCLK_AMX_SELECT_INPUT,
                              KEY_COL0_ALT2),

  IMX_PAD_CFG_KEY_ROW0_AUD5_TXD = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_KEY_ROW0_ALT2_AUD5_TXD,
                              IOMUXC_AUD5_INPUT_DB_AMX_SELECT_INPUT,
                              KEY_ROW0_ALT2),

  IMX_PAD_CFG_KEY_COL1_AUD5_TXFS = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_KEY_COL1_ALT2_AUD5_TXFS,
                              IOMUXC_AUD5_INPUT_TXFS_AMX_SELECT_INPUT,
                              KEY_COL1_ALT2),

  //
  // DISP0_DAT19 is alt3 for AUD5_RXD
  //
  IMX_PAD_CFG_DISP0_DAT19_AUD5_RXD = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_DISP0_DAT19_ALT3_AUD5_RXD,
                              IOMUXC_AUD5_INPUT_DA_AMX_SELECT_INPUT,
                              DISP0_DATA19_ALT3),
} IMX_AUDIO_PADCFG;


typedef enum {
  IMX_USDHC_CMD_PAD_CTL = _IMX_MAKE_PAD_CTL(
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

  IMX_USDHC_DAT_PAD_CTL = _IMX_MAKE_PAD_CTL(
                              IMX_SRE_FAST,
                              IMX_DSE_33_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_47K_OHM_PU,
                              IMX_HYS_ENABLED),

  IMX_USDHC_GPIO_PAD_CTL = _IMX_MAKE_PAD_CTL(
                              IMX_SRE_FAST,
                              IMX_DSE_33_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              0,
                              0,
                              IMX_HYS_ENABLED),

  IMX_USDHC_CD_PAD_CTL = _IMX_MAKE_PAD_CTL(
                              IMX_SRE_FAST,
                              IMX_DSE_33_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_47K_OHM_PU,
                              IMX_HYS_ENABLED),

  IMX_PAD_CFG_SD2_CLK_SD2_CLK = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_CLK_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD2_CLK_ALT0_SD2_CLK),

  IMX_PAD_CFG_SD2_CMD_SD2_CMD = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_CMD_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD2_CMD_ALT0_SD2_CMD),

  IMX_PAD_CFG_SD2_DAT0_SD2_DATA0 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_DAT_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD2_DAT0_ALT0_SD2_DATA0),

  IMX_PAD_CFG_SD2_DAT1_SD2_DATA1 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_DAT_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD2_DAT1_ALT0_SD2_DATA1),

  IMX_PAD_CFG_SD2_DAT2_SD2_DATA2 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_DAT_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD2_DAT2_ALT0_SD2_DATA2),

  IMX_PAD_CFG_SD2_DAT3_SD2_DATA3 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_DAT_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD2_DAT3_ALT0_SD2_DATA3),

  IMX_PAD_CFG_KEY_ROW1_SD2_VSELECT = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_GPIO_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_KEY_ROW1_ALT6_SD2_VSELECT),

  IMX_PAD_CFG_GPIO_4_SD2_CD_B = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_CD_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_GPIO_4_ALT6_SD2_CD_B),

  IMX_PAD_CFG_DISP0_DAT9_GPIO4_IO30 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_GPIO_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_DISP0_DAT9_ALT5_GPIO4_IO30),

} IMX_USDHC2_PADCFG;

typedef enum {

  IMX_PAD_CFG_SD3_CLK_SD3_CLK = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_CLK_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_CLK_ALT0_SD3_CLK),

  IMX_PAD_CFG_SD3_CMD_SD3_CMD = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_CMD_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_CMD_ALT0_SD3_CMD),

  IMX_PAD_CFG_SD3_RST_SD3_RST = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_GPIO_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_RST_ALT0_SD3_RESET),

  IMX_PAD_CFG_SD3_DAT0_SD3_DATA0 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_DAT_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DAT0_ALT0_SD3_DATA0),

  IMX_PAD_CFG_SD3_DAT1_SD3_DATA1 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_DAT_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DAT1_ALT0_SD3_DATA1),

  IMX_PAD_CFG_SD3_DAT2_SD3_DATA2 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_DAT_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DAT2_ALT0_SD3_DATA2),

  IMX_PAD_CFG_SD3_DAT3_SD3_DATA3 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_DAT_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DAT3_ALT0_SD3_DATA3),

  IMX_PAD_CFG_SD3_DAT4_SD3_DATA4 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_DAT_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DAT4_ALT0_SD3_DATA4),

  IMX_PAD_CFG_SD3_DAT5_SD3_DATA5 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_DAT_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DAT5_ALT0_SD3_DATA5),

  IMX_PAD_CFG_SD3_DAT6_SD3_DATA6 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_DAT_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DAT6_ALT0_SD3_DATA6),

  IMX_PAD_CFG_SD3_DAT7_SD3_DATA7 = _IMX_MAKE_PADCFG2(
                              IMX_USDHC_DAT_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_SD3_DAT7_ALT0_SD3_DATA7),

} IMX_USDHC3_PADCFG;

typedef enum {
  IMX_PWM_CLK_PAD_CTL = _IMX_MAKE_PAD_CTL(
                              IMX_SRE_FAST,
                              IMX_DSE_33_OHM,
                              IMX_SPEED_LOW,
                              IMX_ODE_DISABLE,
                              IMX_PKE_DISABLE,
                              0,
                              0,
                              IMX_HYS_ENABLED),

  IMX_PAD_CFG_DISP0_DAT8_PWM1_OUT = _IMX_MAKE_PADCFG2(
                              IMX_PWM_CLK_PAD_CTL,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_DISP0_DAT8_ALT2_PWM1_OUT),
} IMX_PWM_PADCFG;

typedef enum 
{
    IMX_PAD_UART1_RX_DATA = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,           // 110 40_OHM  25 Ohm @ 3.3V, 40 Ohm @ 1.8V
                              IMX_SPEED_MEDIUM,         // 10 MEDIUM  Medium frequency (100, 150 MHz)
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,           // 12=1 ENABLED  Pull/Keeper Enabled
                              IMX_PUE_PULL,             // 13=1 PULL  Pull Enabled
                              IMX_PUS_100K_OHM_PU,      // 14-15 10  100K Ohm Pull Up
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_CSI0_DAT11_ALT3_UART1_RX_DATA,
                              IOMUXC_UART1_UART_RX_DATA_SELECT_INPUT,
                              CSI0_DATA11_ALT3),

    IMX_PAD_UART1_TX_DATA = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_CSI0_DAT10_ALT3_UART1_TX_DATA),

    IMX_PAD_UART3_RX_DATA = _IMX_MAKE_PADCFG_INPSEL(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_EIM_D25_ALT2_UART3_RX_DATA, // 0x001B0B0
                              IOMUXC_UART3_UART_RX_DATA_SELECT_INPUT,
                              EIM_DATA25_ALT2),


    IMX_PAD_UART3_TX_DATA = _IMX_MAKE_PADCFG(
                              IMX_SRE_SLOW,
                              IMX_DSE_40_OHM,
                              IMX_SPEED_MEDIUM,
                              IMX_ODE_DISABLE,
                              IMX_PKE_ENABLE,
                              IMX_PUE_PULL,
                              IMX_PUS_100K_OHM_PU,
                              IMX_HYS_ENABLED,
                              IMX_SION_DISABLED,
                              IMX_IOMUXC_EIM_D24_ALT2_UART3_TX_DATA),
} IMX_UART_PADCFG;

/**
  Turn off IMX6QD clock gates which are not needed during boot. The iMXQD PEP is responsible
  to ungate clocks as needed. 
  Not used for iMX6 DualLite 

static VOID GateUnusedClocks()
{
  static const IMX_CLK_GATE gatesToTurnOff[] = {
    IMX_ASRC_CLK_ENABLE,
    IMX_CAAM_SECURE_MEM_CLK_ENABLE,
    IMX_CAAM_WRAPPER_ACLK_ENABLE,
    IMX_CAAM_WRAPPER_IPG_ENABLE,
    IMX_CAN1_CLK_ENABLE,
    IMX_CAN1_SERIAL_CLK_ENABLE,
    IMX_CAN2_CLK_ENABLE,
    IMX_CAN2_SERIAL_CLK_ENABLE,
    IMX_DCIC1_CLK_ENABLE,
    IMX_DCIC2_CLK_ENABLE,
    IMX_DTCP_CLK_ENABLE,
    IMX_ECSPI1_CLK_ENABLE,
    IMX_ECSPI2_CLK_ENABLE,
    IMX_ECSPI3_CLK_ENABLE,
    IMX_ECSPI4_CLK_ENABLE,
    IMX_ECSPI5_CLK_ENABLE,
    IMX_ENET_CLK_ENABLE,
    IMX_ESAI_CLK_ENABLE,
    // IMX_GPT_SERIAL_CLK_ENABLE,
    IMX_I2C1_SERIAL_CLK_ENABLE,
    IMX_I2C2_SERIAL_CLK_ENABLE,
    IMX_I2C3_SERIAL_CLK_ENABLE,
    // IMX_I2C4_SERIAL_CLK_ENABLE - i2c4 clock is not managed through CCGR2 (p.791 of iMX6SDL RM)
    IMX_IIM_CLK_ENABLE,
    IMX_IPSYNC_IP2APB_TZASC1_IPG_MASTER_CLK_ENABLE,
    IMX_IPSYNC_IP2APB_TZASC2_IPG_MASTER_CLK_ENABLE,
    IMX_IPSYNC_VDOA_IPG_MASTER_CLK_ENABLE,
    IMX_IPU1_DI1_CLK_ENABLE,
    IMX_LDB_DI0_CLK_ENABLE,
    IMX_LDB_DI1_CLK_ENABLE,
    IMX_MIPI_CORE_CFG_CLK_ENABLE,
    IMX_MLB_CLK_ENABLE,
    IMX_PL301_MX6QPER1_BCHCLK_ENABLE,
    // IMX_PWM1_CLK_ENABLE,
    IMX_PWM2_CLK_ENABLE,
    IMX_PWM3_CLK_ENABLE,
    IMX_PWM4_CLK_ENABLE,
    IMX_RAWNAND_U_BCH_INPUT_APB_CLK_ENABLE,
    IMX_RAWNAND_U_GPMI_BCH_INPUT_BCH_CLK_ENABLE,
    IMX_RAWNAND_U_GPMI_BCH_INPUT_GPMI_IO_CLK_ENABLE,
    IMX_RAWNAND_U_GPMI_INPUT_APB_CLK_ENABLE,
    IMX_SPDIF_CLK_ENABLE,
    IMX_SSI1_CLK_ENABLE,
    IMX_SSI2_CLK_ENABLE,
    IMX_SSI3_CLK_ENABLE,
    IMX_EIM_SLOW_CLK_ENABLE,
    IMX_VDOAXICLK_CLK_ENABLE,
    IMX_VPU_CLK_ENABLE,
  };

  DEBUG ((EFI_D_INIT, "++GateUnusedClocks() DL\r\n"));
  ImxClkPwrSetClockGates (
      gatesToTurnOff,
      sizeof (gatesToTurnOff) / sizeof (gatesToTurnOff[0]),
      IMX_CLOCK_GATE_STATE_OFF);
   DEBUG ((EFI_D_INIT, "--GateUnusedClocks() DL\r\n"));
}

  Turn on required clocks, that are not handled by the iMX6DQ PEP.
  Not applicable to iMX6 DualLite

static VOID UngateRequiredClocks ()
{
    static const IMX_CLK_GATE gatesToTurnOn[] = {
        IMX_SDMA_CLK_ENABLE,
        IMX_SPBA_CLK_ENABLE,
        IMX_GPT_SERIAL_CLK_ENABLE,
        IMX_USBOH3_CLK_ENABLE,
        IMX_PWM1_CLK_ENABLE
    };
    DEBUG ((EFI_D_INIT, "++UngateRequiredClocks() DL\r\n"));
    ImxClkPwrSetClockGates(
        gatesToTurnOn,
        sizeof(gatesToTurnOn) / sizeof(gatesToTurnOn[0]),
        IMX_CLOCK_GATE_STATE_ON);
    DEBUG ((EFI_D_INIT, "--UngateRequiredClocks() DL\r\n"));
}
**/
static void EnetInit ()
{
  //
  // Apply ENET pin-muxing configurations
  //
  ImxPadConfig(IMX_PAD_ENET_MDIO, IMX_PAD_ENET_MDIO_ENET_MDIO); // pad ENET_MDIO (mode ALT1)
  ImxPadConfig(IMX_PAD_ENET_MDC, IMX_PAD_ENET_MDC_ENET_MDC); // pad ENET_MDC (mode ALT1)
  ImxPadConfig(IMX_PAD_KEY_ROW4, IMX_PAD_CFG_KEY_ROW4_GPIO4_IO15);
  ImxPadConfig(IMX_PAD_DI0_PIN2, IMX_PAD_DI0_PIN2_GPIO4_IO18);
  ImxPadConfig(IMX_PAD_GPIO_16, IMX_PAD_GPIO_16_ENET_REF_CLK); 
  ImxPadConfig(IMX_PAD_ENET_REF_CLK, IMX_PAD_ENET_REF_CLK_ENET_REF_CLK); // pad ENET_REF_CLK (mode ALT1)
  ImxPadConfig(IMX_PAD_RGMII_TD0, IMX_PAD_RGMII_TD0_RGMII_TD0); // RGMII_TD0 (ALT1)
  ImxPadConfig(IMX_PAD_RGMII_TD1, IMX_PAD_RGMII_TD1_RGMII_TD1); // RGMII_TD1 (ALT1)
  ImxPadConfig(IMX_PAD_RGMII_TD2, IMX_PAD_RGMII_TD2_RGMII_TD2); // RGMII_TD2 (ALT1)
  ImxPadConfig(IMX_PAD_RGMII_TD3, IMX_PAD_RGMII_TD3_RGMII_TD3); // RGMII_TD3 (ALT1) bugbug
  ImxPadConfig(IMX_PAD_RGMII_TXC, IMX_PAD_RGMII_TXC_RGMII_TXC); // pad RGMII_TXC (ALT1)
  ImxPadConfig(IMX_PAD_RGMII_TX_CTL, IMX_PAD_RGMII_TX_CTL_RGMII_TX_CTL); // RGMII_TX_CTL (ALT1)

  ImxPadConfig(IMX_PAD_RGMII_RD0, IMX_PAD_RGMII_RD0_ENET_RGMII_RD0); // RGMII_RD0 (ALT1)
  ImxPadConfig(IMX_PAD_RGMII_RD1, IMX_PAD_RGMII_RD1_ENET_RGMII_RD1); // RGMII_RD1 (ALT1)
  ImxPadConfig(IMX_PAD_RGMII_RD2, IMX_PAD_RGMII_RD2_ENET_RGMII_RD2); // RGMII_RD2 (ALT1)
  ImxPadConfig(IMX_PAD_RGMII_RD3, IMX_PAD_RGMII_RD3_ENET_RGMII_RD3); // RGMII_RD3 (ALT1)
  ImxPadConfig(IMX_PAD_RGMII_RXC, IMX_PAD_RGMII_RXC_ENET_RGMII_RXC); // RGMII_RXC (ALT1)
  ImxPadConfig(IMX_PAD_RGMII_RX_CTL, IMX_PAD_RGMII_RX_CTL_RGMII_RX_CTL); // RGMII_RX_CTL (ALT1)

  //
  // Enable ENET PLL, also required for PCIe
  //
  {
    UINT32 counter = 0;
    IMX_CCM_ANALOG_PLL_ENET_REG ccmAnalogPllReg;
    volatile IMX_CCM_ANALOG_REGISTERS *ccmAnalogRegistersPtr = 
        (IMX_CCM_ANALOG_REGISTERS *)IMX_CCM_ANALOG_BASE;

    ccmAnalogPllReg.AsUint32 = MmioRead32((UINTN)&ccmAnalogRegistersPtr->PLL_ENET);
    ccmAnalogPllReg.POWERDOWN = 0;
    ccmAnalogPllReg.ENABLE = 1;

    MmioWrite32(
        (UINTN)&ccmAnalogRegistersPtr->PLL_ENET,
        ccmAnalogPllReg.AsUint32);

    do {
        ccmAnalogPllReg.AsUint32 =
            MmioRead32((UINTN)&ccmAnalogRegistersPtr->PLL_ENET);

        MicroSecondDelay(100);
        ++counter;
    } while ((ccmAnalogPllReg.LOCK == 0) && (counter < 100));

    //
    // Enable PCIe output 125Mhz
    //
    ccmAnalogPllReg.BYPASS = 0;
    ccmAnalogPllReg.ENABLE_125M = 1;
	ccmAnalogPllReg.DIV_SELECT = PLL_ENET_DIV_SELECT_25MHZ;

    MmioWrite32(
        (UINTN)&ccmAnalogRegistersPtr->PLL_ENET,
        ccmAnalogPllReg.AsUint32);

    MicroSecondDelay(50000);
    
    if (ccmAnalogPllReg.LOCK == 0) {
        DEBUG ((DEBUG_ERROR, "PLL_ENET is not locked!\n"));
    }    
  }
}

/**
  Set SSI3 clock dividers to provide a bit clock capable of
  providing a clock suitable for stereo 44.1 KHz playback at 32 bit
  from PLL3 PFD2 which runs at 508.2 MHz.

  We only need to scale down the PLL3 FPD2 clock by 1:15 to provide a
  reference clock.  The WDM audio class driver will later setup additional
  dividers in the SSI3 block to provide the required bit clock rate.

  This routine also handles all GPIO/PadConfig init required for audio.
**/
static void SetupAudio ()
{
    volatile IMX_CCM_REGISTERS *ccmRegisters = (IMX_CCM_REGISTERS *) IMX_CCM_BASE;

    IMX_CCM_CS1CDR_REG csicdr = (IMX_CCM_CS1CDR_REG) MmioRead32((UINTN) &ccmRegisters->CS1CDR);

    csicdr.ssi3_clk_pred = 0; // divide by 1.
    csicdr.ssi3_clk_podf = (15 - 1); // divide by 15.

    MmioWrite32((UINTN) &ccmRegisters->CS1CDR, csicdr.AsUint32);

    // Enable output on CCM_CLKO1, select pll3_sw_clk/2 as the source.
    IMX_CCM_CCOSR_REG ccosr = (IMX_CCM_CCOSR_REG) MmioRead32((UINTN) &ccmRegisters->CCOSR);

    ccosr.CLKO1_SEL = 0;
    ccosr.CLKO1_DIV = 0;
    ccosr.CLKO1_EN = 1;

    MmioWrite32((UINTN) &ccmRegisters->CCOSR, ccosr.AsUint32);

    //
    // Set GPIO05 alt config to ALT3 (CCM_CLKO1).
    // This clock drives the master clock for the audio codec.
    //
    ImxPadConfig(IMX_PAD_GPIO_5, IMX_PAD_CFG_GPIO_5_CCM_CLKO1);

    //
    // Set alt config for KEY_COL0, KEY_ROW0, KEY_COL1, to ALT2 to select AUD5_TXC/TXD/TXFS.
    // DSP_DAT19 is set to ALT3 for AUD5_RXD.
    //

    ImxPadConfig(IMX_PAD_KEY_COL0, IMX_PAD_CFG_KEY_COL0_AUD5_TXC);
    ImxPadConfig(IMX_PAD_KEY_ROW0, IMX_PAD_CFG_KEY_ROW0_AUD5_TXD);
    ImxPadConfig(IMX_PAD_KEY_COL1, IMX_PAD_CFG_KEY_COL1_AUD5_TXFS);
    ImxPadConfig(IMX_PAD_DISP0_DAT19, IMX_PAD_CFG_DISP0_DAT19_AUD5_RXD);

    // Set up Audmux where Ssi port = 7 and external port = 5
    InitializeAudmux (AUDMUX_PORT7, AUDMUX_PORT5);
}

/**
  Initialize clock and power for modules on the iMX6 DualLite SOM.
  Gate unused clocks is not called since it is Quad-Dual specific and not applcable to SDL
**/
VOID ImxClkPwrInit ()
{
  EFI_STATUS status;
  DEBUG ((EFI_D_INIT, "++ImxClkPwrInit DL\r\n"));

  //GateUnusedClocks();
  //DEBUG ((EFI_D_INIT, "ImxClkPwrInit DL gated unused clocks\r\n"));
  //UngateRequiredClocks();
  //DEBUG ((EFI_D_INIT, "ImxClkPwrInit DL ungated required clocks\r\n"));

  if (FeaturePcdGet (PcdGpuEnable)) 
  {
    DEBUG ((EFI_D_INIT, "ImxClkPwrInit DL will call ImxClkPwrGpuEnable()\r\n"));
    status = ImxClkPwrGpuEnable ();
    ASSERT_EFI_ERROR (status);
  }
  DEBUG ((EFI_D_INIT, "ImxClkPwrInit DL will call ImxClkPwrIpuDIxEnable()\r\n"));
  status = ImxClkPwrIpuDIxEnable();
  ASSERT_EFI_ERROR (status);
  DEBUG ((EFI_D_INIT, "ImxClkPwrInit DL will call ImxClkPwrValidateClocks()\r\n"));
  status = ImxClkPwrValidateClocks();
  ASSERT_EFI_ERROR(status);
  DEBUG ((EFI_D_INIT, "--ImxClkPwrInit DL\r\n"));
}

#define DEBUG_SDHC_PINMUXING  0

/**
  Initialize SDHC2 module on the SOC and perform required pin-muxing for iMX6 DualLite and Solo
**/
VOID SdhcInit ()
{
  //
  // uSDHC1: Wifi Module
  //
  // TODO
  //

  //
  //
  // uSDHC2: SDCard Socket
  //
  // NOTE: SDR104 may require higher pad speed than MEDIUM
  // currently we dont support SDR104 due to design limitations
  //
  ImxPadConfig(IMX_PAD_SD2_CLK, IMX_PAD_CFG_SD2_CLK_SD2_CLK);

  ImxPadConfig(IMX_PAD_SD2_CMD, IMX_PAD_CFG_SD2_CMD_SD2_CMD);

  ImxPadConfig(IMX_PAD_SD2_DAT0, IMX_PAD_CFG_SD2_DAT0_SD2_DATA0);

  ImxPadConfig(IMX_PAD_SD2_DAT1, IMX_PAD_CFG_SD2_DAT1_SD2_DATA1);

  ImxPadConfig(IMX_PAD_SD2_DAT2, IMX_PAD_CFG_SD2_DAT2_SD2_DATA2);

  ImxPadConfig(IMX_PAD_SD2_DAT3, IMX_PAD_CFG_SD2_DAT3_SD2_DATA3);

  ImxPadConfig(IMX_PAD_GPIO_4, IMX_PAD_CFG_GPIO_4_SD2_CD_B);

  ImxPadConfig(IMX_PAD_KEY_ROW1, IMX_PAD_CFG_KEY_ROW1_SD2_VSELECT);

#if DEBUG_SDHC_PINMUXING
    DEBUG((DEBUG_INIT, "uSDHC2 PAD Config:\n"));
    ImxPadDumpConfig("CLK",IMX_PAD_SD2_CLK);
    ImxPadDumpConfig("CMD",IMX_PAD_SD2_CMD);
    ImxPadDumpConfig("DAT0",IMX_PAD_SD2_DAT0);
    ImxPadDumpConfig("DAT1",IMX_PAD_SD2_DAT1);
    ImxPadDumpConfig("DAT2",IMX_PAD_SD2_DAT2);
    ImxPadDumpConfig("DAT3",IMX_PAD_SD2_DAT3);
    ImxPadDumpConfig("VSELECT",IMX_PAD_KEY_ROW1);
    ImxPadDumpConfig("CD",IMX_PAD_GPIO_4);
#endif // DEBUG_SDHC_PINMUXING

  //
  // Power-on SDCard through the GPIO signal connected to
  // Q1 MOSFET Gate acting as a switch to SDCard VDD supply voltage
  //
  ImxPadConfig(IMX_PAD_DISP0_DAT9, IMX_PAD_CFG_DISP0_DAT9_GPIO4_IO30);
  DEBUG ((EFI_D_INIT, "SdhcInit2 DL GPIO4_IO30\r\n"));
  ImxGpioDirection(IMX_GPIO_BANK4, 30, IMX_GPIO_DIR_OUTPUT);
  DEBUG ((EFI_D_INIT, "SdhcInit2 DL GPIO 30 DIR_OUTPUT\r\n"));
  ImxGpioWrite(IMX_GPIO_BANK4, 30, IMX_GPIO_LOW);
  DEBUG ((EFI_D_INIT, "SdhcInit2 DL GPIO 30 LOW\r\n"));
  //
  // Pre-configure the GPIO ALT5 of all SD2_* pads to be input pins. This is
  // required by the SD _DSM for slot power-cycle. The power-on/off _DSM needs to
  // disconnect all SD pins as part of the power-off sequence. This will be
  // achieved by switching the alternate function of the SD pads from ALT0 SD2_*
  // to ALT5 GPIO1_IO* and disable the pull-up resistors. On power-on, the SD2_*
  // pads will be reconfigured as as ALT0 SD2_* functions and the pull-ups will
  // be enabled again. The net effect is that all the SD pins will float and a
  // proper SD power-cycle can be achieved.
  //

  // SD2_CLK: GPIO1_IO10
  ImxGpioDirection(IMX_GPIO_BANK1, 10, IMX_GPIO_DIR_INPUT);
  DEBUG ((EFI_D_INIT, "SdhcInit2 DL SD2_CLK: GPIO1_IO10\r\n"));

  // SD2_CMD: GPIO1_IO11
  ImxGpioDirection(IMX_GPIO_BANK1, 11, IMX_GPIO_DIR_INPUT);
  DEBUG ((EFI_D_INIT, "SdhcInit2 DL SD2_CMD GPIO 11 _DIR_INPUT\r\n"));

  // SD2_DATA0: GPIO1_IO15
  ImxGpioDirection(IMX_GPIO_BANK1, 15, IMX_GPIO_DIR_INPUT);
  DEBUG ((EFI_D_INIT, "SdhcInit2 DL SD2_DAT0 GPIO1_IO15 _DIR_INPUT\r\n"));

  // SD2_DATA1: GPIO1_IO14
  ImxGpioDirection(IMX_GPIO_BANK1, 14, IMX_GPIO_DIR_INPUT);
  DEBUG ((EFI_D_INIT, "SdhcInit2 DL SD2_DAT1 GPIO1_IO14 _DIR_INPUT\r\n"));

  // SD2_DATA2: GPIO1_IO13
  ImxGpioDirection(IMX_GPIO_BANK1, 13, IMX_GPIO_DIR_INPUT);
  DEBUG ((EFI_D_INIT, "SdhcInit2 DL SD2_DAT2 GPIO1_IO13 _DIR_INPUT\r\n"));

  // SD2_DATA3: GPIO1_IO12
  ImxGpioDirection(IMX_GPIO_BANK1, 12, IMX_GPIO_DIR_INPUT);
  DEBUG ((EFI_D_INIT, "SdhcInit2 DL SD2_DAT3 GPIO1_IO12 _DIR_INPUT\r\n"));

  //
  // uSDHC3: eMMC Socket
  //
  ImxPadConfig(IMX_PAD_SD3_CLK, IMX_PAD_CFG_SD3_CLK_SD3_CLK);
  ImxPadConfig(IMX_PAD_SD3_CMD, IMX_PAD_CFG_SD3_CMD_SD3_CMD);
  ImxPadConfig(IMX_PAD_SD3_RST, IMX_PAD_CFG_SD3_RST_SD3_RST);
  ImxPadConfig(IMX_PAD_SD3_DAT0, IMX_PAD_CFG_SD3_DAT0_SD3_DATA0);
  ImxPadConfig(IMX_PAD_SD3_DAT1, IMX_PAD_CFG_SD3_DAT1_SD3_DATA1);
  ImxPadConfig(IMX_PAD_SD3_DAT2, IMX_PAD_CFG_SD3_DAT2_SD3_DATA2);
  ImxPadConfig(IMX_PAD_SD3_DAT3, IMX_PAD_CFG_SD3_DAT3_SD3_DATA3);
  ImxPadConfig(IMX_PAD_SD3_DAT4, IMX_PAD_CFG_SD3_DAT4_SD3_DATA4);
  ImxPadConfig(IMX_PAD_SD3_DAT5, IMX_PAD_CFG_SD3_DAT5_SD3_DATA5);
  ImxPadConfig(IMX_PAD_SD3_DAT6, IMX_PAD_CFG_SD3_DAT6_SD3_DATA6);
  ImxPadConfig(IMX_PAD_SD3_DAT7, IMX_PAD_CFG_SD3_DAT7_SD3_DATA7);
  DEBUG((EFI_D_INIT,"--Sdhc2Init DL\r\n"));
}

/**
  Initialize EHCI modules on the SOC and perform required pin-muxing.
  This routine also initializes PHY0(OTG) and PHY1 (USBH1).
**/
VOID EhciInit ()
{
    DEBUG ((EFI_D_INIT, "++EhciInit() DL\r\n"));
    //
    // Pin-mux OTG Over Current
    //
    ImxPadConfig(IMX_PAD_KEY_COL4, IMX_PAD_CFG_KEY_COL4_USB_OTG_OC);

    DEBUG ((EFI_D_INIT, "DL EhciInit( )  Pin-muxed OTG Over Current\r\n"));

    //
    // Pin-mux and enable OTG power
    //
    ImxPadConfig(IMX_PAD_EIM_D22, IMX_PAD_CFG_EIM_DATA22_GPIO3_IO22_USB_OTG_PWR);
    ImxGpioDirection(IMX_GPIO_BANK3, 22, IMX_GPIO_DIR_OUTPUT);
    ImxGpioWrite(IMX_GPIO_BANK3, 22, IMX_GPIO_HIGH);
    DEBUG ((EFI_D_INIT, "DL EhciInit( )  Pin-muxed and enabled OTG power\r\n"));

    //
    // Pin-mux and configure USB_OTG_ID as HOST!
    //
    ImxPadConfig(IMX_PAD_GPIO_1, IMX_PAD_CFG_GPIO01_USB_OTG_ID_HOST);
    DEBUG ((EFI_D_INIT, "DL EhciInit( )  Pin-muxed and configured USB_OTG_ID as HOST\r\n"));

    //
    // Configure USB_OTG_ID pin
    //
    {
        volatile IMX_IOMUXC_GPR_REGISTERS* IoMuxGprRegsPtr = (IMX_IOMUXC_GPR_REGISTERS*)IOMUXC_GPR_BASE_ADDRESS;

        IMX_IOMUXC_GPR1_REG IoMuxGpr1Reg = { MmioRead32((UINTN)&IoMuxGprRegsPtr->GPR1) };
        IoMuxGpr1Reg.USB_OTG_ID_SEL = IMX_IOMUXC_GPR1_USB_OTG_ID_SEL_GPIO_1;
        MmioWrite32((UINTN)&IoMuxGprRegsPtr->GPR1, IoMuxGpr1Reg.AsUint32);
    }

    //
    // Pin-mux and enable USBH1 power
    //
    ImxPadConfig(IMX_PAD_GPIO_0, IMX_PAD_CFG_GPIO0_GPIO1_IO0_USBH1_PWR);
    ImxGpioDirection(IMX_GPIO_BANK1, 0, IMX_GPIO_DIR_OUTPUT);
    ImxGpioWrite(IMX_GPIO_BANK1, 0, IMX_GPIO_HIGH);

    //
    // Initialize PHY0 (OTG)
    //
    ImxUsbPhyInit(IMX_USBPHY0);

    //
    // Initialize PHY1 (USBH1)
    //
    ImxUsbPhyInit(IMX_USBPHY1);
}

/**
  Initialize I2C modules on the iMX6 Dual SOM and perform required pin-muxing
**/
VOID I2cInit ()
{
    DEBUG ((DEBUG_INFO, "++I2cInit()\r\n"));
    //
    // Enable 1.8V and 3.3V power rails for sensors connected to I2C1.
    // The SENSOR_PWR_EN on EIM_EB3 line powers the pullups on I2c1.
    //
    ImxPadConfig (IMX_PAD_EIM_EB3, IMX_PAD_CFG_EIM_EB3_GPIO2_IO31); // mux EIM_EB3 to GPIO - alt5
    ImxGpioDirection(IMX_GPIO_BANK2, 31, IMX_GPIO_DIR_OUTPUT);
    ImxGpioWrite(IMX_GPIO_BANK2, 31, IMX_GPIO_HIGH); // set GPIO2_IO31 to 1
    DEBUG ((DEBUG_INFO, "muxed IMX_PAD_EIM_EB3 to GPIO via Alt5 and set GPIO2_IO31 to 1\r\n"));

    //
    // Configure I2C1. 
    // EIM_D21 is I2C1_SCL Alt6

    //ImxPadConfig (IMX_PAD_EIM_D21, IMX_PAD_CFG_EIM_D21_I2C1_SCL);

    ////  EIM_D28 is I2C1_SDA Alt1

    //ImxPadConfig (IMX_PAD_EIM_D28, IMX_PAD_CFG_EIM_D28_I2C1_SDA);

    //DEBUG ((DEBUG_INFO, "I2C1 pin muxed via EIM D28,21\r\n"));

    //
    // Do not configure I2C2 SCL via KEY_COL3, SDA via KEY_ROW3
    // GOP in UEFI or Windows display driver might use I2C2 and mux it using Alt2
    //

    //
    // configure I2C3.
    // i2c3 SCL via EIM_D17 Alt6

    ImxPadConfig (IMX_PAD_EIM_D17, IMX_PAD_CFG_EIM_D17_I2C3_SCL);

    // i2c3 SDA via EIM_D18 Alt6

    ImxPadConfig (IMX_PAD_EIM_D18, IMX_PAD_CFG_EIM_D18_I2C3_SDA);

    DEBUG ((DEBUG_INFO, "I2C3 pin muxed EIM_D17, 18\r\n"));

    //
    // configure I2C4.
    // i2c4 SCL via NAND_WP_B Alt9

    ImxPadConfig(IMX_PAD_NANDF_WP_B, IMX_PAD_CFG_NANDF_WP_B_I2C4_SCL);

    // i2c4 SDA via NANDF_CS3 Alt9

    ImxPadConfig(IMX_PAD_NANDF_CS3, IMX_PAD_CFG_NANDF_CS3_I2C4_SDA);

    DEBUG ((DEBUG_INFO, "I2C4 pin muxed NANDF_WP_B, NANDF_CS3\r\n"));
     DEBUG ((DEBUG_INFO, "--I2cInit()\r\n"));
}

/**
  Initialize SPI modules on the SOC and perform required pin-muxing
**/
VOID SpiInit ()
{
    UINT32 RegValue;

    //
    // On Hummingboard only ESPI2 is exposed.
    // Configure the return path for ECSPI2_MISO
    //

    RegValue = MmioRead32(IOMUXC_ECSPI2_MISO_SELECT_INPUT);
    RegValue &= ~3;
    RegValue |= (IMX_IOMUXC_ECSPI2_MISO_EIM_OE_B_ALT2 << 0);
    MmioWrite32(IOMUXC_ECSPI2_MISO_SELECT_INPUT, RegValue);
}

/**
  Initialize PCI Express module on the iMX6 DualLite SOC and perform required pin-muxing
**/
VOID PcieInit ()
{
    //
    // PCIe GPIO Power - on HummingBoard2, PCIe power is connect directly to the 3V3 rail
    //                   and not switched.
    //

    //
    // PCIe GPIO Reset
    //
    ImxPadConfig(IMX_PAD_SD4_DAT3, IMX_PAD_CFG_SD4_DATA3_GPIO2_IO11);
    ImxGpioDirection(IMX_GPIO_BANK2, 11, IMX_GPIO_DIR_OUTPUT);
    ImxGpioWrite(IMX_GPIO_BANK2, 11, IMX_GPIO_LOW);
}

/**
  Initialize exposed GPIO pins on carrier board. By default some
  pins are set to CMOS inputs while others are set to Schmitt
  triggers.  Normalize them all to Schmitt trigger inputs.
**/
VOID GpioInit ()
{
    //
    // Set all exposed GPIO pins to Schmitt trigger inputs by
    // setting the hysteresis bit (16) on the pad ctl register.
    //
    // Temporary measure until vendor data work is done in ACPI
    // gpio table.
    //

    int i;
    IMX_PAD gpioPins[] = {
        IMX_PAD_ENET_RX_ER,
        IMX_PAD_EIM_A22,
        IMX_PAD_EIM_A21,
        IMX_PAD_EIM_A20,
        IMX_PAD_EIM_A19,
        IMX_PAD_EIM_A18,
        IMX_PAD_EIM_A17,
        IMX_PAD_EIM_A16,
        IMX_PAD_EIM_EB0,
        IMX_PAD_EIM_EB1,
        IMX_PAD_EIM_DA0,
        IMX_PAD_EIM_DA1,
        IMX_PAD_EIM_DA2,
        IMX_PAD_EIM_DA3,
        IMX_PAD_EIM_DA4,
        IMX_PAD_EIM_DA5,
        IMX_PAD_EIM_DA6,
        IMX_PAD_EIM_DA7,
        IMX_PAD_EIM_DA8,
        IMX_PAD_EIM_DA9,
        IMX_PAD_EIM_DA10,
        IMX_PAD_EIM_DA11,
        IMX_PAD_EIM_DA12,
        IMX_PAD_EIM_DA13,
        IMX_PAD_EIM_DA14,
        IMX_PAD_EIM_DA15,
        IMX_PAD_EIM_D26,
        IMX_PAD_EIM_D27,
        IMX_PAD_EIM_D30,
        IMX_PAD_EIM_D31,
        IMX_PAD_EIM_A24,
        IMX_PAD_EIM_A23,
        IMX_PAD_GPIO_16,
    };

    for (i = 0 ; i < sizeof(gpioPins)/sizeof(IMX_PAD) ; ++i) {
      IMX_PAD pad = gpioPins[i];
      MmioOr32 (
        IMX_IOMUXC_BASE + IMX_IOMUX_PAD_CTL_OFFSET(pad),
        1 << 16);
    }
}

/**
  Initalize the PWM controllers
**/
VOID PwmInit ()
{
  //
  // PWM1 is exposed through Mikrobus header
  // Pinmux pad DISP0_DAT8 to (ALT2) PWM1_OUT
  //
  ImxPadConfig(IMX_PAD_DISP0_DAT8, IMX_PAD_CFG_DISP0_DAT8_PWM1_OUT);
}

/**
    Initalize iMX6 DualLite SoM UARTs
**/
VOID UartInit()
{
    const IMX_CLK_GATE UartClockGatesToTurnOn[] = {
        IMX_PLL3_MAIN_CLK, // IMX_UART_CLK_ENABLE,
        IMX_UART_CLK_ROOT  // UART_SERIAL_CLK_ENABLE,
    };
    DEBUG ((EFI_D_INIT, "++UartInit() DL\r\n"));
    //
    // Enable UART clocks
    // imx6SDLrm section  64.3.
    // all UARTs share same Clock: PLL3 (480 MHz) -> pll3_sw_clk -> CG -> /6 -> uart_clk_root = 80 MHz
    //
    ImxClkPwrSetClockGates(
        UartClockGatesToTurnOn,
        sizeof(UartClockGatesToTurnOn) / sizeof(UartClockGatesToTurnOn[0]),
        IMX_CLOCK_GATE_STATE_ON);

    //
    // 2. DualLite pin mux for UART 1 and UART 3
    //
    ImxPadConfig (IMX_PAD_CSI0_DAT11, IMX_PAD_UART1_RX_DATA); // CSI0_DAT11, ALT3
    ImxPadConfig (IMX_PAD_CSI0_DAT10, IMX_PAD_UART1_TX_DATA); // CSI0_DAT10, ALT3

    ImxPadConfig (IMX_PAD_EIM_D25, IMX_PAD_UART3_RX_DATA); // EIM_D25, ALT2
    ImxPadConfig (IMX_PAD_EIM_D24, IMX_PAD_UART3_TX_DATA); // EIM_D24, ALT2

    // option - pinmux uart4

    SerialPortInitialize();
    SerialPortWrite((UINT8*)SERIAL_DEBUG_PORT_INIT_MSG,
                    (UINTN)sizeof(SERIAL_DEBUG_PORT_INIT_MSG));
    DEBUG ((EFI_D_INIT, "--UartInit() DL\r\n"));
}

/**
    Initialize the TPM2 control area.

Note:
    This is temporary, and is normally done in TPM2 OPTEE device lib,
    which will takle place as soon as the TreeDxe driver is enabled.
**/
VOID
Tpm2AcpiControlAreaInit ()
{
    EFI_PHYSICAL_ADDRESS BaseAddress;
    UINT32 BufferSize;
    EFI_TPM2_ACPI_CONTROL_AREA *ControlArea;
   
    BaseAddress = PcdGet64 (PcdTpm2AcpiBufferBase);
    BufferSize = PcdGet32 (PcdTpm2AcpiBufferSize);

    if ((BaseAddress == 0) || (BufferSize == 0)) {
        //
        // TPM not enabled
        //
        return;
    }

    ASSERT (BufferSize >= EFI_PAGE_SIZE * 3);
    
    ControlArea = (EFI_TPM2_ACPI_CONTROL_AREA *)((UINTN)BaseAddress);
    ZeroMem(ControlArea, sizeof(EFI_TPM2_ACPI_CONTROL_AREA));
    BufferSize = EFI_PAGE_SIZE;
    ControlArea->Command = (UINT64)((UINTN)(ControlArea + 1));
    ControlArea->CommandSize = BufferSize;
    ControlArea->Response = ControlArea->Command + BufferSize;
    ControlArea->ResponseSize = BufferSize;
}

/**
  Initialize iMX6 DualLite SOM controllers that must setup at the early stage
**/
RETURN_STATUS
ArmPlatformInitialize(IN UINTN MpId)
{
    DEBUG ((DEBUG_INFO, "++DL ArmPlatformInitialize(%Xh)\r\n", MpId));

    if (!ArmPlatformIsPrimaryCore (MpId)) {
    DEBUG ((DEBUG_INFO, "--DL ArmPlatformInitialize(%Xh)=not primary\r\n", MpId));
    return RETURN_SUCCESS;
    }

    DEBUG ((DEBUG_INIT, "ArmPlatformInitialize(DL %Xh) primary core - will proceed with HW initalization\r\n", MpId));
	  ImxClkPwrInit ();

  // initialize default UEFI debug port early so we can use its debug output

  SerialPortInitialize ();
  SerialPortWrite (
   (UINT8 *)SERIAL_DEBUG_PORT_INIT_MSG,
   (UINTN)sizeof(SERIAL_DEBUG_PORT_INIT_MSG));

  // Initialize timer early on because the following init path will be calling
  // delay functions. PrePi.c calls ArmPlatformInitialize before it calls
  // TimerConstructor to initialize the timer.
  TimerConstructor ();

    SdhcInit ();
    DEBUG ((DEBUG_INIT, "ArmPlatformInitialize(DL %Xh) SDHC2 initialized.\r\n", MpId));
    EhciInit ();
    DEBUG ((DEBUG_INIT, "ArmPlatformInitialize(DL %Xh) USB initialized.\r\n", MpId));
    EnetInit ();
    DEBUG ((DEBUG_INIT, "ArmPlatformInitialize(DL %Xh) Enet initialized.\r\n", MpId));

    I2cInit ();
    DEBUG ((DEBUG_INIT, "ArmPlatformInitialize(DL %Xh) i2c initialized.\r\n", MpId));

    SpiInit ();
    DEBUG ((DEBUG_INIT, "ArmPlatformInitialize(DL %Xh) Spi initialized.\r\n", MpId));
    PcieInit ();
    DEBUG ((DEBUG_INIT, "ArmPlatformInitialize(DL %Xh) PCIe initialized.\r\n", MpId));

    SetupAudio ();
    DEBUG ((DEBUG_INIT, "ArmPlatformInitialize(DL %Xh) Audio initialized.\r\n", MpId));

    GpioInit ();
    DEBUG ((DEBUG_INIT, "ArmPlatformInitialize(DL %Xh) GPIO initialized.\r\n", MpId));

    PwmInit ();

    Tpm2AcpiControlAreaInit ();

    DEBUG ((DEBUG_INIT, "--DL ArmPlatformInitialize(%Xh)=ok\r\n", MpId));
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
