/** @file
*
*  Header defining IMX6 ULL Muxing definitions
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
#ifndef _IMX6_IOMUX_ULL_H_
#define _IMX6_IOMUX_ULL_H_
// SELECT INPUT defines

#define EIM_DATA21_ALT6 0x0 // Selecting ALT6 mode of pad EIM_D21 for I2C1_SCL
#define CSI0_DAT9_ALT4 0x1 // Selecting ALT4 mode of pad CSI0_DAT9 for I2C1_SCL

#define EIM_DATA28_ALT1 0x0 // Selecting ALT1 mode of pad EIM_D28 for I2C1_SDA
#define CSI0_DAT8_ALT4 0x1 // Selecting ALT4 mode of pad CSI0_DAT8 for I2C1_SDA

#define EIM_EB2_B_ALT6 0x0 // Selecting ALT6 mode of pad EIM_EB2 for I2C2_SCL
#define KEY_COL3_ALT4 0x1 // Selecting ALT4 mode of pad KEY_COL3 for I2C2_SCL

#define EIM_DATA16_ALT6 0x0 // Selecting ALT6 mode of pad EIM_D16 for I2C2_SDA
#define KEY_ROW3_ALT4 0x1 // Selecting ALT4 mode of pad KEY_ROW3 for I2C2_SDA

#define EIM_DATA17_ALT6 0x0 // Selecting ALT6 mode of pad EIM_D17 for I2C3_SCL
#define GPIO03_ALT2 0x1 // Selecting ALT2 mode of pad GPIO_3 for I2C3_SCL
#define GPIO05_ALT6 0x2 // Selecting ALT6 mode of pad GPIO_5 for I2C3_SCL

#define EIM_DATA18_ALT6 0x0 // Selecting ALT6 mode of pad EIM_D18 for I2C3_SDA
#define GPIO06_ALT2 0x1 // Selecting ALT2 mode of pad GPIO_6 for I2C3_SDA
#define GPIO16_ALT6 0x2 // Selecting ALT6 mode of pad GPIO_16 for I2C3_SDA

#define DISP0_DATA19_ALT3 0
#define KEY_ROW1_ALT2 1

#define DISP0_DATA17_ALT3 0
#define KEY_ROW0_ALT2 1

#define DISP0_DATA16_ALT3 0
#define KEY_COL0_ALT2 1

#define DISP0_DATA18_ALT3 0
#define KEY_COL1_ALT2 1

#define EIM_DATA21_ALT4 0
#define KEY_COL4_ALT2 1

#define EIM_DATA30_ALT6 0
#define GPIO03_ALT6 1

#define RGMII_TX_CTL_ALT7 0
#define GPIO16_ALT2 1

#define UART5_TX_DATA_ALT0 0x6 // Selecting ALT0 of pad UART5_TX_DATA for IOMUXC_UART5_RX_DATA_SELECT_INPUT
#define UART5_TX_DATA_ALT2 0x2 // Selecting ALT2 of pad UART5_TX_DATA for IOMUXC_I2C2_SCL_SELECT_INPUT
#define UART5_RX_DATA_ALT0 0x7 // Selecting ALT0 of pad UART5_RX_DATA for IOMUXC_UART5_RX_DATA_SELECT_INPUT
#define UART5_RX_DATA_ALT2 0x2 // Selecting ALT2 of pad UART5_RX_DATA for IOMUXC_I2C2_SDA_SELECT_INPUT

// AUD5 select input register defines.
typedef enum {
  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO00)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO00)
  IMX_PAD_GPIO_0 = IMX_IOMUX_PAD_DEFINE(0x2E8, 0x5C), // CCM_CLKO1

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO01)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO01)
  IMX_PAD_GPIO_1 = IMX_IOMUX_PAD_DEFINE(0x2EC, 0x60), // ESAI_RX_CLK

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO02)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO02)
  IMX_PAD_GPIO_2 = IMX_IOMUX_PAD_DEFINE(0x2F0, 0x64), // ESAI_TX_FS

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO03)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO03)
  IMX_PAD_GPIO_3 = IMX_IOMUX_PAD_DEFINE(0x2F4, 0x68), // ESAI_RX_HF_CLK

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO04)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO04)
  IMX_PAD_GPIO_4 = IMX_IOMUX_PAD_DEFINE(0x2F8, 0x6C), // ESAI_TX_HF_CLK

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO05)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO05)
  IMX_PAD_GPIO_5 = IMX_IOMUX_PAD_DEFINE(0x2FC, 0x70), // ESAI_TX2_RX3

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO06)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO06)
  IMX_PAD_GPIO_6 = IMX_IOMUX_PAD_DEFINE(0x300, 0x74), // ESAI_TX_CLK

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO07)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO07)
  IMX_PAD_GPIO_7 = IMX_IOMUX_PAD_DEFINE(0x304, 0x78), // ESAI_TX4_RX1

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO08)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO08)
  IMX_PAD_GPIO_8 = IMX_IOMUX_PAD_DEFINE(0x308, 0x7C), // ESAI_TX5_RX0

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_GPIO09)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_GPIO09)
  IMX_PAD_GPIO_9 = IMX_IOMUX_PAD_DEFINE(0x30C, 0x80), // ESAI_RX_FS
  
  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_MDIO)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_MDIO)
  IMX_PAD_ENET1_MDIO = IMX_PAD_GPIO_6, // ENET_MDIO

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_MDC)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_MDC)
  IMX_PAD_ENET1_MDC = IMX_PAD_GPIO_7, // MLB_DAT

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_TX_DATA0)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_TX_DATA0)
  IMX_PAD_ENET1_TXD0 = IMX_IOMUX_PAD_DEFINE(0x35C, 0xD0), // ENET_TX_DATA0

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_TX_DATA1)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_TX_DATA1)
  IMX_PAD_ENET1_TXD1 = IMX_IOMUX_PAD_DEFINE(0x360, 0xD4), // MLB_CLK

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_TX_EN)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_TX_EN)
  IMX_PAD_ENET1_TX_EN = IMX_IOMUX_PAD_DEFINE(0x364, 0xD8), // ENET_TX_EN

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_REF_CLK)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_REF_CLK)
  IMX_PAD_ENET1_REF_CLK = IMX_IOMUX_PAD_DEFINE(0x368, 0xDC), // ENET_TX_CLK

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_RX_ER)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_RX_ER)
  IMX_PAD_ENET1_RX_ER = IMX_IOMUX_PAD_DEFINE(0x36C, 0xE0), // USB_OTG_ID

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_CRS_DV)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_CRS_DV)
  IMX_PAD_ENET1_RX_EN = IMX_IOMUX_PAD_DEFINE(0x358, 0xCC), // ENET_RX_EN

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_RX_DATA0)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_RX_DATA0)
  IMX_PAD_ENET1_RXD0 = IMX_IOMUX_PAD_DEFINE(0x350, 0xC4), // XTALOSC_OSC32K_32K_OUT

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_ENET_RX_DATA1)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_ENET_RX_DATA1)
  IMX_PAD_ENET1_RXD1 = IMX_IOMUX_PAD_DEFINE(0x354, 0xC8), // MLB_SIG

  IMX_PAD_ENET1_PHY_RESET = IMX_IOMUX_PAD_DEFINE(0x64, 0x20), // MLB_SIG

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART5_TX_DATA)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART5_TX_DATA)
  IMX_PAD_UART5_TX_DATA = IMX_IOMUX_PAD_DEFINE(0x348, 0xBC), // GPIO1_IO30

  // Pad Control Register (IOMUXC_SW_PAD_CTL_PAD_UART5_RX_DATA)
  // Pad Mux Register (IOMUXC_SW_MUX_CTL_PAD_UART5_RX_DATA)
  IMX_PAD_UART5_RX_DATA = IMX_IOMUX_PAD_DEFINE(0x34C, 0xC0), // UART5_RX
} IMX_PAD;

// Alternate function numbers
typedef enum {
  IMX_IOMUXC_UART5_TX_DATA_ALT5_GPIO1_IO30 = 5,
  IMX_IOMUXC_UART5_TX_DATA_ALT0_UART5_TX = 0,
  IMX_IOMUXC_UART5_TX_DATA_ALT2_I2C2_SCL = 2,
} IMX_IOMUXC_UART5_TX_DATA_ALT;

typedef enum {
  IMX_IOMUXC_UART5_RX_DATA_ALT0_UART5_RX = 0,
  IMX_IOMUXC_UART5_RX_DATA_ALT2_I2C2_SDA = 2,
  IMX_IOMUXC_UART5_RX_DATA_ALT5_GPIO1_IO31 = 5,
} IMX_IOMUXC_UART5_RX_DATA_ALT;

typedef enum {
  IMX_IOMUXC_GPIO_0_ALT2_USB_OTG = 2,
  IMX_IOMUXC_GPIO_0_ALT5_GPIO = 5,
} IMX_IOMUXC_GPIO_ALT;

typedef enum {
  IMX_IOMUXC_GPIO_0_ALT2_OTG1_ID = 2,
  IMX_IOMUXC_GPIO_0_ALT3_ENET_REF_CLK1 = 3,
  IMX_IOMUXC_GPIO_0_ALT5_GPIO1_IO00 = 5,
} IMX_IOMUXC_GPIO_0_ALT;

typedef enum {
   IMX_IOMUXC_GPIO_0_ALT2_OTG1_OC = 2,
   IMX_IOMUXC_GPIO_1_ALT5_GPIO1_IO01 = 5,
} IMX_IOMUXC_GPIO_1_ALT;

typedef enum {
  IMX_IOMUXC_GPIO_0_ALT2_OTG2_PWR = 2,
  IMX_IOMUXC_GPIO_2_ALT5_GPIO1_IO02 = 5,
} IMX_IOMUXC_GPIO_2_ALT;

typedef enum {
  IMX_IOMUXC_GPIO_0_ALT2_OTG2_OC = 2,
  IMX_IOMUXC_GPIO_3_ALT5_GPIO1_IO03 = 5,
} IMX_IOMUXC_GPIO_3_ALT;

typedef enum {
  IMX_IOMUXC_GPIO_0_ALT2_OTG1_PWR = 2,
  IMX_IOMUXC_GPIO_4_ALT5_GPIO1_IO04 = 5,
} IMX_IOMUXC_GPIO_4_ALT;

typedef enum {
  IMX_IOMUXC_GPIO_0_ALT2_OTG2_ID = 2,
  IMX_IOMUXC_GPIO_5_ALT5_GPIO1_IO05 = 5,
} IMX_IOMUXC_GPIO_5_ALT;

typedef enum {
  IMX_IOMUXC_ALT0 = 0,
  IMX_IOMUXC_ALT1,
  IMX_IOMUXC_ALT2,
  IMX_IOMUXC_ALT3,
  IMX_IOMUXC_ALT4,
  IMX_IOMUXC_ALT5,
  IMX_IOMUXC_ALT6,
  IMX_IOMUXC_ALT7,
  IMX_IOMUXC_ALT8,
} IMX_IOMUXC_ALT;
#endif // _IMX6_IOMUX_DQ_H_
