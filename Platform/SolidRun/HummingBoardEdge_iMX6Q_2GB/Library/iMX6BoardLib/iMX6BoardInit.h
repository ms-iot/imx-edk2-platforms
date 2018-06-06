/** @file
*
*  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
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

#ifndef _IMX6_BOARD_INIT_H_
#define _IMX6_BOARD_INIT_H_

#include <iMX6.h>
#include <iMX6ClkPwr.h>
#include <iMX6IoMux.h>
#include <iMX6Timer.h>
#include <iMX6UsbPhy.h>
#include <iMX6Audio.h>

//  Prebaked pad configurations that include mux and drive settings where
//  each enum named as IMX_<MODULE-NAME>_PADCFG contains configurations
//  for pads used by that module
typedef enum {
  IMX_PAD_ENET_MDIO_ENET_MDIO = _IMX_MAKE_PADCFG (
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

  IMX_PAD_ENET_MDC_ENET_MDC = _IMX_MAKE_PADCFG (
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

  IMX_PAD_CFG_KEY_ROW4_GPIO4_IO15 = _IMX_MAKE_PADCFG (
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

  IMX_PAD_DI0_PIN2_GPIO4_IO18 = _IMX_MAKE_PADCFG (
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

  IMX_PAD_GPIO_16_ENET_REF_CLK = _IMX_MAKE_PADCFG_INPSEL (
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

  IMX_PAD_RGMII_TXC_RGMII_TXC = _IMX_MAKE_PADCFG (
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

  IMX_PAD_RGMII_TD0_RGMII_TD0 = _IMX_MAKE_PADCFG (
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

  IMX_PAD_RGMII_TD1_RGMII_TD1 = _IMX_MAKE_PADCFG (
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

  IMX_PAD_RGMII_TD2_RGMII_TD2 = _IMX_MAKE_PADCFG (
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

  IMX_PAD_RGMII_TD3_RGMII_TD3 = _IMX_MAKE_PADCFG (
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

  IMX_PAD_RGMII_TX_CTL_RGMII_TX_CTL = _IMX_MAKE_PADCFG (
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

  IMX_PAD_ENET_REF_CLK_ENET_REF_CLK = _IMX_MAKE_PADCFG (
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

  IMX_PAD_RGMII_RXC_ENET_RGMII_RXC = _IMX_MAKE_PADCFG (
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

  IMX_PAD_RGMII_RD0_ENET_RGMII_RD0 = _IMX_MAKE_PADCFG (
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

  IMX_PAD_RGMII_RD1_ENET_RGMII_RD1 = _IMX_MAKE_PADCFG (
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

  IMX_PAD_RGMII_RD2_ENET_RGMII_RD2 = _IMX_MAKE_PADCFG (
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

  IMX_PAD_RGMII_RD3_ENET_RGMII_RD3 = _IMX_MAKE_PADCFG (
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

  IMX_PAD_RGMII_RX_CTL_RGMII_RX_CTL = _IMX_MAKE_PADCFG (
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
  IMX_PAD_CFG_EIM_DATA22_GPIO3_IO22_USB_OTG_PWR = _IMX_MAKE_PADCFG (
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

  IMX_PAD_CFG_KEY_COL4_USB_OTG_OC = _IMX_MAKE_PADCFG_INPSEL (
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

  IMX_PAD_CFG_GPIO0_GPIO1_IO0_USBH1_PWR = _IMX_MAKE_PADCFG (
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

  IMX_PAD_CFG_GPIO01_USB_OTG_ID_HOST = _IMX_MAKE_PADCFG (
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

  IMX_PAD_CFG_GPIO01_USB_OTG_ID_DEVICE = _IMX_MAKE_PADCFG (
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
  IMX_PAD_CFG_SD4_DATA3_GPIO2_IO11 = _IMX_MAKE_PADCFG (
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
  IMX_PAD_CFG_EIM_EB3_GPIO2_IO31 = _IMX_MAKE_PADCFG (
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

  IMX_PAD_CFG_EIM_D21_I2C1_SCL = _IMX_MAKE_PADCFG_INPSEL (
                                   IMX_SRE_FAST,
                                   IMX_DSE_40_OHM,
                                   IMX_SPEED_MEDIUM,
                                   IMX_ODE_ENABLE,
                                   IMX_PKE_ENABLE,
                                   IMX_PUE_PULL,
                                   IMX_PUS_47K_OHM_PU,
                                   IMX_HYS_ENABLED,
                                   IMX_SION_ENABLED,
                                   IMX_IOMUXC_EIM_D21_ALT6_I2C1_SCL,
                                   IOMUXC_I2C1_SCL_IN_SELECT_INPUT,
                                   EIM_DATA21_ALT6),

  IMX_PAD_CFG_EIM_D28_I2C1_SDA = _IMX_MAKE_PADCFG_INPSEL (
                                   IMX_SRE_FAST,
                                   IMX_DSE_40_OHM,
                                   IMX_SPEED_MEDIUM,
                                   IMX_ODE_ENABLE,
                                   IMX_PKE_ENABLE,
                                   IMX_PUE_PULL,
                                   IMX_PUS_47K_OHM_PU,
                                   IMX_HYS_ENABLED,
                                   IMX_SION_ENABLED,
                                   IMX_IOMUXC_EIM_D28_ALT1_I2C1_SDA,
                                   IOMUXC_I2C1_SDA_IN_SELECT_INPUT,
                                   EIM_DATA28_ALT1),

  IMX_PAD_CFG_EIM_D17_I2C3_SCL = _IMX_MAKE_PADCFG_INPSEL (
                                   IMX_SRE_FAST,
                                   IMX_DSE_40_OHM,
                                   IMX_SPEED_MEDIUM,
                                   IMX_ODE_ENABLE,
                                   IMX_PKE_ENABLE,
                                   IMX_PUE_PULL,
                                   IMX_PUS_47K_OHM_PU,
                                   IMX_HYS_ENABLED,
                                   IMX_SION_ENABLED,
                                   IMX_IOMUXC_EIM_D17_ALT6_I2C3_SCL,
                                   IOMUXC_I2C3_SCL_IN_SELECT_INPUT,
                                   EIM_DATA17_ALT6),

  IMX_PAD_CFG_EIM_D18_I2C3_SDA = _IMX_MAKE_PADCFG_INPSEL (
                                   IMX_SRE_FAST,
                                   IMX_DSE_40_OHM,
                                   IMX_SPEED_MEDIUM,
                                   IMX_ODE_ENABLE,
                                   IMX_PKE_ENABLE,
                                   IMX_PUE_PULL,
                                   IMX_PUS_47K_OHM_PU,
                                   IMX_HYS_ENABLED,
                                   IMX_SION_ENABLED,
                                   IMX_IOMUXC_EIM_D18_ALT6_I2C3_SDA,
                                   IOMUXC_I2C3_SDA_IN_SELECT_INPUT,
                                   EIM_DATA18_ALT6),
} IMX_I2C_PADCFG;

typedef enum {
  IMX_PAD_CFG_GPIO_5_CCM_CLKO1 = _IMX_MAKE_PADCFG (
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

  IMX_PAD_CFG_KEY_COL0_AUD5_TXC = _IMX_MAKE_PADCFG_INPSEL (
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

  IMX_PAD_CFG_KEY_ROW0_AUD5_TXD = _IMX_MAKE_PADCFG_INPSEL (
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

  IMX_PAD_CFG_KEY_COL1_AUD5_TXFS = _IMX_MAKE_PADCFG_INPSEL (
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

  IMX_PAD_CFG_DISP0_DAT19_AUD5_RXD = _IMX_MAKE_PADCFG_INPSEL (
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
  IMX_USDHC_CMD_PAD_CTL = _IMX_MAKE_PAD_CTL (
                            IMX_SRE_FAST,
                            IMX_DSE_33_OHM,
                            IMX_SPEED_MEDIUM,
                            IMX_ODE_DISABLE,
                            IMX_PKE_ENABLE,
                            IMX_PUE_PULL,
                            IMX_PUS_47K_OHM_PU,
                            IMX_HYS_ENABLED),

  IMX_USDHC_CLK_PAD_CTL = _IMX_MAKE_PAD_CTL (
                            IMX_SRE_FAST,
                            IMX_DSE_33_OHM,
                            IMX_SPEED_MEDIUM,
                            IMX_ODE_DISABLE,
                            IMX_PKE_DISABLE,
                            0,
                            0,
                            IMX_HYS_ENABLED),

  IMX_USDHC_DAT_PAD_CTL = _IMX_MAKE_PAD_CTL (
                            IMX_SRE_FAST,
                            IMX_DSE_33_OHM,
                            IMX_SPEED_MEDIUM,
                            IMX_ODE_DISABLE,
                            IMX_PKE_ENABLE,
                            IMX_PUE_PULL,
                            IMX_PUS_47K_OHM_PU,
                            IMX_HYS_ENABLED),

  IMX_USDHC_GPIO_PAD_CTL = _IMX_MAKE_PAD_CTL (
                             IMX_SRE_FAST,
                             IMX_DSE_33_OHM,
                             IMX_SPEED_LOW,
                             IMX_ODE_DISABLE,
                             IMX_PKE_DISABLE,
                             0,
                             0,
                             IMX_HYS_ENABLED),

  IMX_USDHC_CD_PAD_CTL = _IMX_MAKE_PAD_CTL (
                           IMX_SRE_FAST,
                           IMX_DSE_33_OHM,
                           IMX_SPEED_LOW,
                           IMX_ODE_DISABLE,
                           IMX_PKE_ENABLE,
                           IMX_PUE_PULL,
                           IMX_PUS_47K_OHM_PU,
                           IMX_HYS_ENABLED),

  IMX_PAD_CFG_SD2_CLK_SD2_CLK = _IMX_MAKE_PADCFG2 (
                                  IMX_USDHC_CLK_PAD_CTL,
                                  IMX_SION_DISABLED,
                                  IMX_IOMUXC_SD2_CLK_ALT0_SD2_CLK),

  IMX_PAD_CFG_SD2_CMD_SD2_CMD = _IMX_MAKE_PADCFG2 (
                                  IMX_USDHC_CMD_PAD_CTL,
                                  IMX_SION_DISABLED,
                                  IMX_IOMUXC_SD2_CMD_ALT0_SD2_CMD),

  IMX_PAD_CFG_SD2_DAT0_SD2_DATA0 = _IMX_MAKE_PADCFG2 (
                                     IMX_USDHC_DAT_PAD_CTL,
                                     IMX_SION_DISABLED,
                                     IMX_IOMUXC_SD2_DAT0_ALT0_SD2_DATA0),

  IMX_PAD_CFG_SD2_DAT1_SD2_DATA1 = _IMX_MAKE_PADCFG2 (
                                     IMX_USDHC_DAT_PAD_CTL,
                                     IMX_SION_DISABLED,
                                     IMX_IOMUXC_SD2_DAT1_ALT0_SD2_DATA1),

  IMX_PAD_CFG_SD2_DAT2_SD2_DATA2 = _IMX_MAKE_PADCFG2 (
                                     IMX_USDHC_DAT_PAD_CTL,
                                     IMX_SION_DISABLED,
                                     IMX_IOMUXC_SD2_DAT2_ALT0_SD2_DATA2),

  IMX_PAD_CFG_SD2_DAT3_SD2_DATA3 = _IMX_MAKE_PADCFG2 (
                                     IMX_USDHC_DAT_PAD_CTL,
                                     IMX_SION_DISABLED,
                                     IMX_IOMUXC_SD2_DAT3_ALT0_SD2_DATA3),

  IMX_PAD_CFG_KEY_ROW1_SD2_VSELECT = _IMX_MAKE_PADCFG2 (
                                       IMX_USDHC_GPIO_PAD_CTL,
                                       IMX_SION_DISABLED,
                                       IMX_IOMUXC_KEY_ROW1_ALT6_SD2_VSELECT),

  IMX_PAD_CFG_GPIO_4_SD2_CD_B = _IMX_MAKE_PADCFG2 (
                                  IMX_USDHC_CD_PAD_CTL,
                                  IMX_SION_DISABLED,
                                  IMX_IOMUXC_GPIO_4_ALT6_SD2_CD_B),

  IMX_PAD_CFG_DISP0_DAT9_GPIO4_IO30 = _IMX_MAKE_PADCFG2 (
                                        IMX_USDHC_GPIO_PAD_CTL,
                                        IMX_SION_DISABLED,
                                        IMX_IOMUXC_DISP0_DAT9_ALT5_GPIO4_IO30),
} IMX_USDHC2_PADCFG;

typedef enum {
  IMX_PAD_CFG_SD3_CLK_SD3_CLK = _IMX_MAKE_PADCFG2 (
                                  IMX_USDHC_CLK_PAD_CTL,
                                  IMX_SION_DISABLED,
                                  IMX_IOMUXC_SD3_CLK_ALT0_SD3_CLK),

  IMX_PAD_CFG_SD3_CMD_SD3_CMD = _IMX_MAKE_PADCFG2 (
                                  IMX_USDHC_CMD_PAD_CTL,
                                  IMX_SION_DISABLED,
                                  IMX_IOMUXC_SD3_CMD_ALT0_SD3_CMD),

  IMX_PAD_CFG_SD3_RST_SD3_RST = _IMX_MAKE_PADCFG2 (
                                  IMX_USDHC_GPIO_PAD_CTL,
                                  IMX_SION_DISABLED,
                                  IMX_IOMUXC_SD3_RST_ALT0_SD3_RESET),

  IMX_PAD_CFG_SD3_DAT0_SD3_DATA0 = _IMX_MAKE_PADCFG2 (
                                     IMX_USDHC_DAT_PAD_CTL,
                                     IMX_SION_DISABLED,
                                     IMX_IOMUXC_SD3_DAT0_ALT0_SD3_DATA0),

  IMX_PAD_CFG_SD3_DAT1_SD3_DATA1 = _IMX_MAKE_PADCFG2 (
                                     IMX_USDHC_DAT_PAD_CTL,
                                     IMX_SION_DISABLED,
                                     IMX_IOMUXC_SD3_DAT1_ALT0_SD3_DATA1),

  IMX_PAD_CFG_SD3_DAT2_SD3_DATA2 = _IMX_MAKE_PADCFG2 (
                                     IMX_USDHC_DAT_PAD_CTL,
                                     IMX_SION_DISABLED,
                                     IMX_IOMUXC_SD3_DAT2_ALT0_SD3_DATA2),

  IMX_PAD_CFG_SD3_DAT3_SD3_DATA3 = _IMX_MAKE_PADCFG2 (
                                     IMX_USDHC_DAT_PAD_CTL,
                                     IMX_SION_DISABLED,
                                     IMX_IOMUXC_SD3_DAT3_ALT0_SD3_DATA3),

  IMX_PAD_CFG_SD3_DAT4_SD3_DATA4 = _IMX_MAKE_PADCFG2 (
                                     IMX_USDHC_DAT_PAD_CTL,
                                     IMX_SION_DISABLED,
                                     IMX_IOMUXC_SD3_DAT4_ALT0_SD3_DATA4),

  IMX_PAD_CFG_SD3_DAT5_SD3_DATA5 = _IMX_MAKE_PADCFG2 (
                                     IMX_USDHC_DAT_PAD_CTL,
                                     IMX_SION_DISABLED,
                                     IMX_IOMUXC_SD3_DAT5_ALT0_SD3_DATA5),

  IMX_PAD_CFG_SD3_DAT6_SD3_DATA6 = _IMX_MAKE_PADCFG2 (
                                     IMX_USDHC_DAT_PAD_CTL,
                                     IMX_SION_DISABLED,
                                     IMX_IOMUXC_SD3_DAT6_ALT0_SD3_DATA6),

  IMX_PAD_CFG_SD3_DAT7_SD3_DATA7 = _IMX_MAKE_PADCFG2 (
                                     IMX_USDHC_DAT_PAD_CTL,
                                     IMX_SION_DISABLED,
                                     IMX_IOMUXC_SD3_DAT7_ALT0_SD3_DATA7),
} IMX_USDHC3_PADCFG;

typedef enum {
  IMX_PAD_CFG_SPI2_CSPI_CLK_IN = _IMX_MAKE_PADCFG_INPSEL (
                            IMX_SRE_FAST,
                            IMX_DSE_40_OHM,
                            IMX_SPEED_MEDIUM,
                            IMX_ODE_DISABLE,
                            IMX_PKE_DISABLE,
                            IMX_PUE_KEEP,
                            IMX_PUS_100K_OHM_PD,
                            IMX_HYS_ENABLED,
                            IMX_SION_DISABLED,
                            IMX_IOMUXC_EIM_CS0_ALT2_ECSPI2_SCLK,
                            IOMUXC_ECSPI2_CSPI_CLK_IN_SELECT_INPUT,
                            IMX_IOMUXC_ECSPI2_CSPI_CLK_IN_EIM_CS0_B_ALT2),

  IMX_PAD_CFG_SPI2_MISO = _IMX_MAKE_PADCFG_INPSEL (
                            IMX_SRE_FAST,
                            IMX_DSE_40_OHM,
                            IMX_SPEED_MEDIUM,
                            IMX_ODE_DISABLE,
                            IMX_PKE_DISABLE,
                            IMX_PUE_KEEP,
                            IMX_PUS_100K_OHM_PD,
                            IMX_HYS_ENABLED,
                            IMX_SION_DISABLED,
                            IMX_IOMUXC_EIM_OE_ALT2_ECSPI2_MISO,
                            IOMUXC_ECSPI2_MISO_SELECT_INPUT,
                            IMX_IOMUXC_ECSPI2_MISO_EIM_OE_B_ALT2),

  IMX_PAD_CFG_SPI2_MOSI = _IMX_MAKE_PADCFG_INPSEL (
                            IMX_SRE_FAST,
                            IMX_DSE_40_OHM,
                            IMX_SPEED_MEDIUM,
                            IMX_ODE_DISABLE,
                            IMX_PKE_DISABLE,
                            IMX_PUE_KEEP,
                            IMX_PUS_100K_OHM_PD,
                            IMX_HYS_ENABLED,
                            IMX_SION_DISABLED,
                            IMX_IOMUXC_EIM_CS1_ALT2_ECSPI2_MOSI,
                            IOMUXC_ECSPI2_MOSI_SELECT_INPUT,
                            IMX_IOMUXC_ECSPI2_MOSI_EIM_CS1_B_ALT2),

  IMX_PAD_CFG_SPI2_SS0 = _IMX_MAKE_PADCFG_INPSEL (
                            IMX_SRE_FAST,
                            IMX_DSE_40_OHM,
                            IMX_SPEED_MEDIUM,
                            IMX_ODE_DISABLE,
                            IMX_PKE_ENABLE,
                            IMX_PUE_PULL,
                            IMX_PUS_100K_OHM_PU,
                            IMX_HYS_DISABLED,
                            IMX_SION_DISABLED,
                            IMX_IOMUXC_EIM_RW_ALT2_ECSPI2_SS0,
                            IOMUXC_ECSPI2_SS0_SELECT_INPUT,
                            IMX_IOMUXC_ECSPI2_SS0_EIM_RW_ALT2),

  IMX_PAD_CFG_SPI2_SS1 = _IMX_MAKE_PADCFG_INPSEL (
                            IMX_SRE_FAST,
                            IMX_DSE_40_OHM,
                            IMX_SPEED_MEDIUM,
                            IMX_ODE_DISABLE,
                            IMX_PKE_ENABLE,
                            IMX_PUE_PULL,
                            IMX_PUS_100K_OHM_PU,
                            IMX_HYS_DISABLED,
                            IMX_SION_DISABLED,
                            IMX_IOMUXC_EIM_LBA_ALT2_ECSPI2_SS1,
                            IOMUXC_ECSPI2_SS1_SELECT_INPUT,
                            IMX_IOMUXC_ECSPI2_SS1_EIM_LBA_B_ALT2),
} IMX_SPI_PADCFG;

typedef enum {
  IMX_PWM_CLK_PAD_CTL = _IMX_MAKE_PAD_CTL (
                          IMX_SRE_FAST,
                          IMX_DSE_33_OHM,
                          IMX_SPEED_LOW,
                          IMX_ODE_DISABLE,
                          IMX_PKE_DISABLE,
                          0,
                          0,
                          IMX_HYS_ENABLED),

  IMX_PAD_CFG_DISP0_DAT8_PWM1_OUT = _IMX_MAKE_PADCFG2 (
                                      IMX_PWM_CLK_PAD_CTL,
                                      IMX_SION_DISABLED,
                                      IMX_IOMUXC_DISP0_DAT8_ALT2_PWM1_OUT),
} IMX_PWM_PADCFG;

typedef enum {
  IMX_PAD_UART1_RX_DATA = _IMX_MAKE_PADCFG_INPSEL (
                            IMX_SRE_SLOW,
                            IMX_DSE_40_OHM,     // 25 Ohm @ 3.3V, 40 Ohm @ 1.8V
                            IMX_SPEED_MEDIUM,   // 10 MEDIUM - 100, 150 MHz
                            IMX_ODE_DISABLE,
                            IMX_PKE_ENABLE,       // 12 Pull/Keeper Enabled
                            IMX_PUE_PULL,         // 13 Pull Enabled
                            IMX_PUS_100K_OHM_PU,  // 14-15 100K Ohm Pull Up
                            IMX_HYS_ENABLED,
                            IMX_SION_DISABLED,
                            IMX_IOMUXC_CSI0_DAT11_ALT3_UART1_RX_DATA,
                            IOMUXC_UART1_UART_RX_DATA_SELECT_INPUT,
                            CSI0_DATA11_ALT3),

  IMX_PAD_UART1_TX_DATA = _IMX_MAKE_PADCFG (
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

  IMX_PAD_UART2_RX_DATA = _IMX_MAKE_PADCFG (
                            IMX_SRE_SLOW,
                            IMX_DSE_40_OHM,
                            IMX_SPEED_MEDIUM,
                            IMX_ODE_DISABLE,
                            IMX_PKE_ENABLE,
                            IMX_PUE_PULL,
                            IMX_PUS_100K_OHM_PU,
                            IMX_HYS_ENABLED,
                            IMX_SION_DISABLED,
                            IMX_IOMUXC_SD4_DAT4_ALT2_UART2_RX_DATA),

  IMX_PAD_UART2_TX_DATA = _IMX_MAKE_PADCFG (
                            IMX_SRE_SLOW,
                            IMX_DSE_40_OHM,
                            IMX_SPEED_MEDIUM,
                            IMX_ODE_DISABLE,
                            IMX_PKE_ENABLE,
                            IMX_PUE_PULL,
                            IMX_PUS_100K_OHM_PU,
                            IMX_HYS_ENABLED,
                            IMX_SION_DISABLED,
                            IMX_IOMUXC_SD4_DAT7_ALT2_UART2_TX_DATA),

  IMX_PAD_UART3_RX_DATA = _IMX_MAKE_PADCFG_INPSEL (
                            IMX_SRE_SLOW,
                            IMX_DSE_40_OHM,
                            IMX_SPEED_MEDIUM,
                            IMX_ODE_DISABLE,
                            IMX_PKE_ENABLE,
                            IMX_PUE_PULL,
                            IMX_PUS_100K_OHM_PU,
                            IMX_HYS_ENABLED,
                            IMX_SION_DISABLED,
                            IMX_IOMUXC_EIM_D25_ALT2_UART3_RX_DATA,
                            IOMUXC_UART3_UART_RX_DATA_SELECT_INPUT,
                            EIM_DATA25_ALT2),

  IMX_PAD_UART3_TX_DATA = _IMX_MAKE_PADCFG (
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

typedef enum {
  IMX_PAD_CFG_SD4_DATA1_GPIO2_IO09 = _IMX_MAKE_PADCFG (
                                       IMX_SRE_SLOW,
                                       IMX_DSE_40_OHM,
                                       IMX_SPEED_MEDIUM,
                                       IMX_ODE_DISABLE,
                                       IMX_PKE_ENABLE,
                                       IMX_PUE_PULL,
                                       IMX_PUS_100K_OHM_PU,
                                       IMX_HYS_ENABLED,
                                       IMX_SION_DISABLED,
                                       IMX_IOMUXC_SD4_DAT1_ALT5_GPIO2_IO09),

} IMX_IMX6Q_LDB_PADCFG;

#endif /* _IMX6_BOARD_INIT_H_ */
