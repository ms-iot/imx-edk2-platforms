/** @file
*
*  Header defining the iMX6DQP constants (Base addresses, sizes, flags)
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

#ifndef __IMX6_DQP_H__
#define __IMX6_DQP_H__

#include "iMX6_common.h"

#pragma pack(push, 1)

// IOMUX Controller (IOMUXC)
#define IMX_IOMUXC_TZASC1_BYP   0x1
#define IMX_IOMUXC_TZASC2_BYP   0x2

// IOMUXC Registers
#define IOMUXC_SW_MUX_CTL_PAD_KEY_COL0      0x020E01F8
#define IOMUXC_SW_MUX_CTL_PAD_KEY_ROW0      0x020E01FC
#define IOMUXC_SW_MUX_CTL_PAD_KEY_COL1      0x020E0200
#define IOMUXC_SW_MUX_CTL_PAD_KEY_ROW1      0x020E0204
#define IOMUXC_SW_MUX_CTL_PAD_KEY_COL2      0x020E0208
#define IOMUXC_SW_MUX_CTL_PAD_KEY_ROW2      0x020E020C
#define IOMUXC_SW_MUX_CTL_PAD_KEY_COL3      0x020E0210
#define IOMUXC_SW_MUX_CTL_PAD_KEY_ROW3      0x020E0214
#define IOMUXC_SW_MUX_CTL_PAD_KEY_COL4      0x020E0218
#define IOMUXC_SW_MUX_CTL_PAD_KEY_ROW4      0x020E021C

// Define base address of Select Input registers to be one word
// less than the minimum value so that a valid Select Input value
// is non-zero.
#define IOMUXC_SELECT_INPUT_BASE_ADDRESS 0x20E07AC

typedef enum {
  IOMUXC_ASRC_ASRCK_CLOCK_6_SELECT_INPUT = 0x20E07B0,
  IOMUXC_AUD4_INPUT_DA_AMX_SELECT_INPUT = 0x20E07B4,
  IOMUXC_AUD4_INPUT_DB_AMX_SELECT_INPUT = 0x20E07B8,
  IOMUXC_AUD4_INPUT_RXCLK_AMX_SELECT_INPUT = 0x20E07BC,
  IOMUXC_AUD4_INPUT_RXFS_AMX_SELECT_INPUT = 0x20E07C0,
  IOMUXC_AUD4_INPUT_TXCLK_AMX_SELECT_INPUT = 0x20E07C4,
  IOMUXC_AUD4_INPUT_TXFS_AMX_SELECT_INPUT = 0x20E07C8,
  IOMUXC_AUD5_INPUT_DA_AMX_SELECT_INPUT = 0x20E07CC,
  IOMUXC_AUD5_INPUT_DB_AMX_SELECT_INPUT = 0x20E07D0,
  IOMUXC_AUD5_INPUT_RXCLK_AMX_SELECT_INPUT = 0x20E07D4,
  IOMUXC_AUD5_INPUT_RXFS_AMX_SELECT_INPUT = 0x20E07D8,
  IOMUXC_AUD5_INPUT_TXCLK_AMX_SELECT_INPUT = 0x20E07DC,
  IOMUXC_AUD5_INPUT_TXFS_AMX_SELECT_INPUT = 0x20E07E0,
  IOMUXC_FLEXCAN1_RX_SELECT_INPUT = 0x20E07E4,
  IOMUXC_FLEXCAN2_RX_SELECT_INPUT = 0x20E07E8,
  IOMUXC_CCM_PMIC_READY_SELECT_INPUT = 0x20E07F0,
  IOMUXC_ECSPI1_CSPI_CLK_IN_SELECT_INPUT = 0x20E07F4,
  IOMUXC_ECSPI1_MISO_SELECT_INPUT = 0x20E07F8,
  IOMUXC_ECSPI1_MOSI_SELECT_INPUT = 0x20E07FC,
  IOMUXC_ECSPI1_SS0_SELECT_INPUT = 0x20E0800,
  IOMUXC_ECSPI1_SS1_SELECT_INPUT = 0x20E0804,
  IOMUXC_ECSPI1_SS2_SELECT_INPUT = 0x20E0808,
  IOMUXC_ECSPI1_SS3_SELECT_INPUT = 0x20E080C,
  IOMUXC_ECSPI2_CSPI_CLK_IN_SELECT_INPUT = 0x20E0810,
  IOMUXC_ECSPI2_MISO_SELECT_INPUT = 0x20E0814,
  IOMUXC_ECSPI2_MOSI_SELECT_INPUT = 0x20E0818,
  IOMUXC_ECSPI2_SS0_SELECT_INPUT = 0x20E081C,
  IOMUXC_ECSPI2_SS1_SELECT_INPUT = 0x20E0820,
  IOMUXC_ECSPI4_SS0_SELECT_INPUT = 0x20E0824,
  IOMUXC_ECSPI5_CSPI_CLK_IN_SELECT_INPUT = 0x20E0828,
  IOMUXC_ECSPI5_MISO_SELECT_INPUT = 0x20E082C,
  IOMUXC_ECSPI5_MOSI_SELECT_INPUT = 0x20E0830,
  IOMUXC_ECSPI5_SS0_SELECT_INPUT = 0x20E0834,
  IOMUXC_ECSPI5_SS1_SELECT_INPUT = 0x20E0838,
  IOMUXC_ENET_REF_CLK_SELECT_INPUT = 0x20E083C,
  IOMUXC_ENET_MAC0_MDIO_SELECT_INPUT = 0x20E0840,
  IOMUXC_ENET_MAC0_RX_CLK_SELECT_INPUT = 0x20E0844,
  IOMUXC_ENET_MAC0_RX_DATA0_SELECT_INPUT = 0x20E0848,
  IOMUXC_ENET_MAC0_RX_DATA1_SELECT_INPUT = 0x20E084C,
  IOMUXC_ENET_MAC0_RX_DATA2_SELECT_INPUT = 0x20E0850,
  IOMUXC_ENET_MAC0_RX_DATA3_SELECT_INPUT = 0x20E0854,
  IOMUXC_ENET_MAC0_RX_EN_SELECT_INPUT = 0x20E0858,
  IOMUXC_ESAI_RX_FS_SELECT_INPUT = 0x20E085C,
  IOMUXC_ESAI_TX_FS_SELECT_INPUT = 0x20E0860,
  IOMUXC_ESAI_RX_HF_CLK_SELECT_INPUT = 0x20E0864,
  IOMUXC_ESAI_TX_HF_CLK_SELECT_INPUT = 0x20E0868,
  IOMUXC_ESAI_RX_CLK_SELECT_INPUT = 0x20E086C,
  IOMUXC_ESAI_TX_CLK_SELECT_INPUT = 0x20E0870,
  IOMUXC_ESAI_SDO0_SELECT_INPUT = 0x20E0874,
  IOMUXC_ESAI_SDO1_SELECT_INPUT = 0x20E0878,
  IOMUXC_ESAI_SDO2_SDI3_SELECT_INPUT = 0x20E087C,
  IOMUXC_ESAI_SDO3_SDI2_SELECT_INPUT = 0x20E0880,
  IOMUXC_ESAI_SDO4_SDI1_SELECT_INPUT = 0x20E0884,
  IOMUXC_ESAI_SDO5_SDI0_SELECT_INPUT = 0x20E0888,
  IOMUXC_HDMI_ICECIN_SELECT_INPUT = 0x20E088C,
  IOMUXC_HDMI_II2C_CLKIN_SELECT_INPUT = 0x20E0890,
  IOMUXC_HDMI_II2C_DATAIN_SELECT_INPUT = 0x20E0894,
  IOMUXC_I2C1_SCL_IN_SELECT_INPUT = 0x20E0898,
  IOMUXC_I2C1_SDA_IN_SELECT_INPUT = 0x20E089C,
  IOMUXC_I2C2_SCL_IN_SELECT_INPUT = 0x20E08A0,
  IOMUXC_I2C2_SDA_IN_SELECT_INPUT = 0x20E08A4,
  IOMUXC_I2C3_SCL_IN_SELECT_INPUT = 0x20E08A8,
  IOMUXC_I2C3_SDA_IN_SELECT_INPUT = 0x20E08AC,
  IOMUXC_IPU2_SENS1_DATA10_SELECT_INPUT = 0x20E08B0,
  IOMUXC_IPU2_SENS1_DATA11_SELECT_INPUT = 0x20E08B4,
  IOMUXC_IPU2_SENS1_DATA12_SELECT_INPUT = 0x20E08B8,
  IOMUXC_IPU2_SENS1_DATA13_SELECT_INPUT = 0x20E08BC,
  IOMUXC_IPU2_SENS1_DATA14_SELECT_INPUT = 0x20E08C0,
  IOMUXC_IPU2_SENS1_DATA15_SELECT_INPUT = 0x20E08C4,
  IOMUXC_IPU2_SENS1_DATA16_SELECT_INPUT = 0x20E08C8,
  IOMUXC_IPU2_SENS1_DATA17_SELECT_INPUT = 0x20E08CC,
  IOMUXC_IPU2_SENS1_DATA18_SELECT_INPUT = 0x20E08D0,
  IOMUXC_IPU2_SENS1_DATA19_SELECT_INPUT = 0x20E08D4,
  IOMUXC_IPU2_SENS1_DATA_EN_SELECT_INPUT = 0x20E08D8,
  IOMUXC_IPU2_SENS1_HSYNC_SELECT_INPUT = 0x20E08DC,
  IOMUXC_IPU2_SENS1_PIX_CLK_SELECT_INPUT = 0x20E08E0,
  IOMUXC_IPU2_SENS1_VSYNC_SELECT_INPUT = 0x20E08E4,
  IOMUXC_KEY_COL5_SELECT_INPUT = 0x20E08E8,
  IOMUXC_KEY_COL6_SELECT_INPUT = 0x20E08EC,
  IOMUXC_KEY_COL7_SELECT_INPUT = 0x20E08F0,
  IOMUXC_KEY_ROW5_SELECT_INPUT = 0x20E08F4,
  IOMUXC_KEY_ROW6_SELECT_INPUT = 0x20E08F8,
  IOMUXC_KEY_ROW7_SELECT_INPUT = 0x20E08FC,
  IOMUXC_MLB_MLB_CLK_IN_SELECT_INPUT = 0x20E0900,
  IOMUXC_MLB_MLB_DATA_IN_SELECT_INPUT = 0x20E0904,
  IOMUXC_MLB_MLB_SIG_IN_SELECT_INPUT = 0x20E0908,
  IOMUXC_SDMA_EVENTS14_SELECT_INPUT = 0x20E090C,
  IOMUXC_SDMA_EVENTS47_SELECT_INPUT = 0x20E0910,
  IOMUXC_SPDIF_SPDIF_IN1_SELECT_INPUT = 0x20E0914,
  IOMUXC_SPDIF_TX_CLK2_SELECT_INPUT = 0x20E0918,
  IOMUXC_UART1_UART_RTS_B_SELECT_INPUT = 0x20E091C,
  IOMUXC_UART1_UART_RX_DATA_SELECT_INPUT = 0x20E0920,
  IOMUXC_UART2_UART_RTS_B_SELECT_INPUT = 0x20E0924,
  IOMUXC_UART2_UART_RX_DATA_SELECT_INPUT = 0x20E0928,
  IOMUXC_UART3_UART_RTS_B_SELECT_INPUT = 0x20E092C,
  IOMUXC_UART3_UART_RX_DATA_SELECT_INPUT = 0x20E0930,
  IOMUXC_UART4_UART_RTS_B_SELECT_INPUT = 0x20E0934,
  IOMUXC_UART4_UART_RX_DATA_SELECT_INPUT = 0x20E0938,
  IOMUXC_UART5_UART_RTS_B_SELECT_INPUT = 0x20E093C,
  IOMUXC_UART5_UART_RX_DATA_SELECT_INPUT = 0x20E0940,
  IOMUXC_USB_OTG_OC_SELECT_INPUT = 0x20E0944,
  IOMUXC_USB_H1_OC_SELECT_INPUT = 0x20E0948,
  IOMUXC_USDHC1_WP_ON_SELECT_INPUT = 0x20E094C,
  IOMUXC_SELECT_INPUT_UPPER_BOUND = IOMUXC_USDHC1_WP_ON_SELECT_INPUT,
} IMX_INPUT_SELECT;

typedef struct {
  UINT32 GPR0;                          // 0x00 IOMUXC_GPR0
  UINT32 GPR1;                          // 0x04 IOMUXC_GPR1
  UINT32 GPR2;                          // 0x08 IOMUXC_GPR2
  UINT32 GPR3;                          // 0x0C IOMUXC_GPR3
  UINT32 GPR4;                          // 0x10 IOMUXC_GPR4
  UINT32 GPR5;                          // 0x14 IOMUXC_GPR5
  UINT32 GPR6;                          // 0x18 IOMUXC_GPR6
  UINT32 GPR7;                          // 0x1C IOMUXC_GPR7
  UINT32 GPR8;                          // 0x20 IOMUXC_GPR8
  UINT32 GPR9;                          // 0x24 IOMUXC_GPR9
  UINT32 GPR10;                         // 0x28 IOMUXC_GPR10
  UINT32 GPR11;                         // 0x2c IOMUXC_GPR11
  UINT32 GPR12;                         // 0x30 IOMUXC_GPR12
  UINT32 GPR13;                         // 0x34 IOMUXC_GPR13
  UINT32 Reserved;                      // 0x38 Reserved
  UINT32 GPR14;                         // 0x3C IOMUXC_GPR14; see ERR006687
} IMX_IOMUXC_GPR_REGISTERS;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 ACT_CS0 : 1;                 // 0
    UINT32 ADDRS0_10 : 2;               // 1-2
    UINT32 ACT_CS1 : 1;                 // 3
    UINT32 ADDRS1_10 : 2;               // 4-5
    UINT32 ACT_CS2 : 1;                 // 6
    UINT32 ADDRS2_10 : 2;               // 7-8
    UINT32 ACT_CS3 : 1;                 // 9
    UINT32 ADDRS3_10 : 2;               // 10-11 Active Chip Select and Address Space
    UINT32 GINT : 1;                    // 12 Global interrupt "0" bit
                                        //   (connected to ARM IRQ#0 and GPC)
    UINT32 USB_OTG_ID_SEL : 1;          // 13 ''usb_otg_id' pin iomux select control.
    UINT32 SYS_INT : 1;                 // 14 PCIe_CTL
    UINT32 USB_EXP_MODE : 1;            // 15 USB Exposure mode
    UINT32 REF_SSP_EN : 1;              // 16 PCIe_PHY - Reference Clock Enable
                                        //   for SS function.
    UINT32 PU_VPU_MUX : 1;              // 17 IPU-1/IPU-2 to VPU signals control.
    UINT32 TEST_POWERDOWN : 1;          // 18 PCIe_PHY - All Circuits Power-Down
                                        //   Control Function.
    UINT32 MIPI_IPU1_MUX : 1;           // 19 MIPI sensor to IPU-1 mux control.
    UINT32 MIPI_IPU2_MUX : 1;           // 20 MIPI sensor to IPU-2 mux control
    UINT32 ENET_CLK_SEL : 1;            // 21 ENET TX reference clock
    UINT32 EXC_MON : 1;                 // 22 Exclusive monitor response select
                                        //   of illegal command
    UINT32 reserved1 : 1;               // 23
    UINT32 MIPI_DPI_OFF : 1;            // 24 MIPI DPI shutdown request
    UINT32 MIPI_COLOR_SW : 1;           // 25 MIPI color switch control
    UINT32 APP_REQ_ENTR_L1 : 1;         // 26 PCIe_CTL - Application Request to Enter L1
    UINT32 APP_READY_ENTR_L23 : 1;      // 27 PCIe_CTL - Application Ready to Enter L23
    UINT32 APP_REQ_EXIT_L1 : 1;         // 28 PCIe_CTL - Application Request to Exit L1
    UINT32 PCIE_SW_RST : 1;             // 29 PCIe Software Reset - active high
    UINT32 APP_CLK_REQ_N : 1;           // 30 PCIe_CTL (CLK LOGIC CONTROLLER GLUE)
                                        //   Indicates that application logic is
                                        //   ready to have reference clock removed.
    UINT32 CFG_L1_CLK_REMOVAL_EN : 1;   // 31 PCIe_CTL (CLK LOGIC CONTROLLER GLUE)
                                        //   Enable the reference clock removal
                                        //   in L1 state.
  };
} IMX_IOMUXC_GPR1_REG;

//
// USB PHY
//

#define IMX_USBPHY1_BASE 0x020C9000
#define IMX_USBPHY2_BASE 0x020CA000
#define IMX_USBPHY_LENGTH 0x1000

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 ENOTG_ID_CHG_IRQ : 1;      // 0 Enable OTG_ID_CHG_IRQ.
    UINT32 ENHOSTDISCONDETECT : 1;    // 1 For host mode, enables high-speed
                                      //   disconnect detector.
    UINT32 ENIRQHOSTDISCON : 1;       // 2 Enables interrupt for detection of
                                      //   disconnection to Device when in
                                      //   high-speed host mode.
    UINT32 HOSTDISCONDETECT_IRQ : 1;  // 3 Indicates that the device has
                                      //   disconnected in high-speed mode.
    UINT32 ENDEVPLUGINDETECT : 1;     // 4 For device mode, enables 200-KOhm
                                      //   pullups for detecting connectivity
                                      //   to the host.
    UINT32 DEVPLUGIN_POLARITY : 1;    // 5 For device mode interrupt generation polarity
    UINT32 OTG_ID_CHG_IRQ : 1;        // 6 OTG ID change interrupt. Indicates
                                      //   the value of ID pin changed.
    UINT32 ENOTGIDDETECT : 1;         // 7 Enables circuit to detect resistance
                                      //   of MiniAB ID pin.
    UINT32 RESUMEIRQSTICKY : 1;       // 8 1 makes RESUME_IRQ bit a sticky bit.
    UINT32 ENIRQRESUMEDETECT : 1;     // 9 Enables interrupt for detection of a
                                      //   non-J state on the USB line.
    UINT32 RESUME_IRQ : 1;            // 10 Indicates that the host is sending
                                      //   a wake-up after suspend
    UINT32 ENIRQDEVPLUGIN : 1;        // 11 Enables interrupt for the detection
                                      //   of connectivity to the USB line.
    UINT32 DEVPLUGIN_IRQ : 1;         // 12 Indicates that the device is connected
    UINT32 DATA_ON_LRADC : 1;         // 13 Enables the LRADC to monitor USB_DP
                                      //   and USB_DM.
    UINT32 ENUTMILEVEL2 : 1;          // 14 Enables UTMI+ Level2.
    UINT32 ENUTMILEVEL3 : 1;          // 15 Enables UTMI+ Level3.
    UINT32 ENIRQWAKEUP : 1;           // 16 Enables interrupt for the wakeup events
    UINT32 WAKEUP_IRQ : 1;            // 17 Indicates that there is a wakeup event.
    UINT32 ENAUTO_PWRON_PLL : 1;      // 18
    UINT32 ENAUTOCLR_CLKGATE : 1;     // 19 Enables the feature to auto-clear the
                                      //   CLKGATE bit if there is wakeup event
                                      //   while USB is suspended.
    UINT32 ENAUTOCLR_PHY_PWD : 1;     // 20 Enables the feature to auto-clear the
                                      //   PWD register bits in USBPHYx_PWD if
                                      //   there is wakeup event while USB is
                                      //   suspended
    UINT32 ENDPDMCHG_WKUP : 1;        // 21 Enables the feature to wakeup USB
                                      //   if DP/DM is toggled when USB is suspended
    UINT32 ENIDCHG_WKUP : 1;          // 22 Enables the feature to wakeup USB
                                      //   if ID is toggled when USB is suspended
    UINT32 ENVBUSCHG_WKUP : 1;        // 23 Enables the feature to wakeup USB
                                      //   if VBUS is toggled when USB is suspended.
    UINT32 FSDLL_RST_EN : 1;          // 24 Enables the feature to reset the FSDLL
                                      //   lock detection logic at the end of
                                      //   each TX packet.
    UINT32 ENAUTOCLR_USBCLKGATE : 1;  // 25
    UINT32 ENAUTOSET_USBCLKS : 1;     // 26
    UINT32 OTG_ID_VALUE : 1;          // 27
    UINT32 HOST_FORCE_LS_SE0 : 1;     // 28 Forces the next FS packet that is
                                      //   transmitted to have a EOP with LS timing.
    UINT32 UTMI_SUSPENDM : 1;         // 29 Used by the PHY to indicate a
                                      //   powered-down state.
    UINT32 CLKGATE : 1;               // 30 Gate UTMI Clocks. Clear to 0 to run clocks.
    UINT32 SFTRST : 1;                // 31 Soft-reset the USBPHYx_PWD, USBPHYx_TX,
                                      //   USBPHYx_RX, Set to 0 to release the
                                      //   PHY from reset.
  };
} USBPHYx_CTRL_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 STEP : 16;   // 0-15 Read-only value of the stepping of the RTL version.
    UINT32 MINOR : 8;   // 16-23 Read-only value of the MINOR field of the RTL version.
    UINT32 MAJOR : 8;   // 24-31 Read-only value of the MAJOR field of the RTL version
  };
} USBPHYx_VERSION_REG;

typedef struct {
  UINT32 USBPHY_PWD;           // 0x00 USB PHY Power-Down Register
  UINT32 USBPHY_PWD_SET;       // 0x04 USB PHY Power-Down Register
  UINT32 USBPHY_PWD_CLR;       // 0x08 USB PHY Power-Down Register
  UINT32 USBPHY_PWD_TOG;       // 0x0C USB PHY Power-Down Register
  UINT32 USBPHY_TX;            // 0x10 USB PHY Transmitter Control Register
  UINT32 USBPHY_TX_SET;        // 0x14 USB PHY Transmitter Control Register
  UINT32 USBPHY_TX_CLR;        // 0x18 USB PHY Transmitter Control Register
  UINT32 USBPHY_TX_TOG;        // 0x1C USB PHY Transmitter Control Register
  UINT32 USBPHY_RX;            // 0x20 USB PHY Receiver Control Register
  UINT32 USBPHY_RX_SET;        // 0x24 USB PHY Receiver Control Register
  UINT32 USBPHY_RX_CLR;        // 0x28 USB PHY Receiver Control Register
  UINT32 USBPHY_RX_TOG;        // 0x2C USB PHY Receiver Control Register
  UINT32 USBPHY_CTRL;          // 0x30 USB PHY General Control Register
  UINT32 USBPHY_CTRL_SET;      // 0x34 USB PHY General Control Register
  UINT32 USBPHY_CTRL_CLR;      // 0x38 USB PHY General Control Register
  UINT32 USBPHY_CTRL_TOG;      // 0x3C USB PHY General Control Register
  UINT32 USBPHY_STATUS;        // 0x40 USB PHY Status Register
  UINT32 reserved1[3];
  UINT32 USBPHY_DEBUG;         // 0x50 USB PHY Debug Register
  UINT32 USBPHY_DEBUG_SET;     // 0x54 USB PHY Debug Register
  UINT32 USBPHY_DEBUG_CLR;     // 0x58 USB PHY Debug Register
  UINT32 USBPHY_DEBUG_TOG;     // 0x5C USB PHY Debug Register
  UINT32 USBPHY_DEBUG0_STATUS; // 0x60 UTMI Debug Status Register 0
  UINT32 reserved2[3];
  UINT32 USBPHY_DEBUG1;        // 0x70 UTMI Debug Status Register 1
  UINT32 USBPHY_DEBUG1_SET;    // 0x74 UTMI Debug Status Register 1
  UINT32 USBPHY_DEBUG1_CLR;    // 0x78 UTMI Debug Status Register 1
  UINT32 USBPHY_DEBUG1_TOG;    // 0x7C UTMI Debug Status Register 1
  UINT32 USBPHY_VERSION;       // 0x80 UTMI RTL Version
  UINT32 reserved3[3];
  UINT32 USBPHY_IP;            // 0x90
  UINT32 USBPHY_IP_SET;        // 0x94
  UINT32 USBPHY_IP_CLR;        // 0x98
  UINT32 USBPHY_IP_TOG;        // 0x9C
} IMX_USBPHY_REGISTERS;

#define IMX_USBPHY_IP_FIX   ((1 << 17) | (1 << 18))

#pragma pack(pop)

#endif // __IMX6_DQP_H__
