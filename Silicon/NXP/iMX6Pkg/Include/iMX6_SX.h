/** @file
*
*  Header defining the iMX6 SoloX constants (Base addresses, sizes, flags)
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

#ifndef __IMX6_SX_H__
#define __IMX6_SX_H__

#pragma pack(push, 1)

// DDR attributes
#define DDR_ATTRIBUTES_CACHED               ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define DDR_ATTRIBUTES_UNCACHED             ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED

// Boot DRAM region (kernel.img & boot working DRAM)
#define FRAME_BUFFER_BASE                   0x821D4000
#define FRAME_BUFFER_SIZE                   0x00800000 // 8MB

#define BOOT_IMAGE_PHYSICAL_BASE            0x82004000
#define BOOT_IMAGE_PHYSICAL_LENGTH          0x001D0000 // 1MB
#define BOOT_IMAGE_ATTRIBUTES               CacheAttributes

// The region of registers from 0x00100000 to 0x02300000
#define SOC_REGISTERS_PHYSICAL_BASE1        0x00100000
#define SOC_REGISTERS_PHYSICAL_LENGTH1      0x02200000
#define SOC_REGISTERS_ATTRIBUTES            ARM_MEMORY_REGION_ATTRIBUTE_DEVICE

// PCIE registers and configuration space (0x08000000 - 0x09000000)
#define PCIE_REGISTERS_PHYSICAL_BASE        0x08000000
#define PCIE_REGISTERS_PHYSICAL_LENGTH      0x01000000

// The region of registers from 0x0C000000 to 0x80000000
#define SOC_REGISTERS_PHYSICAL_BASE2        0x0C000000
#define SOC_REGISTERS_PHYSICAL_LENGTH2      0x74000000

// Main system DRAM as defined by the PCD definitions of system memory.

// MPPP definitions
#define CPU0_MPPP_PHYSICAL_BASE            0x8080F000

// Interrupt controller
#define CSP_BASE_REG_PA_IC_IFC              0x00A00100
#define CSP_BASE_REG_PA_IC_DIST             0x00A01000

// L2 cache controller
#define CSP_BASE_REG_PA_PL310               0x00A02000

// Timers
#define CSP_BASE_REG_PA_GPT                 0x02098000
#define CSP_BASE_REG_PA_EPIT1               0x020D0000
#define CSP_BASE_REG_PA_EPIT2               0x020D4000

// Timers IRQs
#define IC_DIST_VECTOR_BASE 0
#define IRQ_EPIT1           88
#define IRQ_EPIT2           89

// SDMA (Smart DMA) controller
#define CSP_BASE_REG_PA_SDMA                0x020EC000
#define IRQ_SDMA 34

// SOC peripherals
#define CSP_BASE_REG_PA_UART1               0x02020000
#define CSP_BASE_REG_PA_UART2               0x021e8000
#define CSP_BASE_REG_PA_UART3               0x021EC000
#define CSP_BASE_REG_PA_ESDHC2              0x02194000
#define CSP_BASE_REG_PA_ESDHC3              0x02198000

#define DBG_PORT_SUBTYPE_IMX6   0x000C

// Timers clock sources
#define SOC_OSC_FREQUENCY_REF_HZ  24000000  // Oscillator frequency 24Mhz
#define SOC_HIGH_FREQUENCY_REF_HZ 66000000  // High Frequency reference clock 66Mhz
#define SOC_LOW_FREQ_REF_HZ       32768     // SNVS RTC frequency 32kHz

// IOMUX Controller (IOMUXC)
#define IMX_IOMUXC_BASE 0x020E0000
#define IMX_IOMUXC_LENGTH 0x4000

// Secure Nonvolatile Storage (SNVS)
#define IMX_SNVS_BASE 0x020CC000
#define IMX_SNVS_LENGTH 0x4000
#define IMX_SNVS_IP_ID 0x3E
#define IMX_SNVS_IRQ 51         // SNVS consolidated interrupt

// IOMUXC Registers
#define IOMUXC_SW_MUX_CTL_PAD_KEY_COL0      0x020E00A4
#define IOMUXC_SW_MUX_CTL_PAD_KEY_COL1      0x020E00A8
#define IOMUXC_SW_MUX_CTL_PAD_KEY_COL2      0x020E00AC
#define IOMUXC_SW_MUX_CTL_PAD_KEY_COL3      0x020E00B0
#define IOMUXC_SW_MUX_CTL_PAD_KEY_COL4      0x020E00B4
#define IOMUXC_SW_MUX_CTL_PAD_KEY_ROW0      0x020E00B8
#define IOMUXC_SW_MUX_CTL_PAD_KEY_ROW1      0x020E00BC
#define IOMUXC_SW_MUX_CTL_PAD_KEY_ROW2      0x020E00C0
#define IOMUXC_SW_MUX_CTL_PAD_KEY_ROW3      0x020E00C4
#define IOMUXC_SW_MUX_CTL_PAD_KEY_ROW4      0x020E00C8
#define IOMUXC_SW_PAD_CTL_PAD_KEY_COL0      0x020E03EC
#define IOMUXC_SW_PAD_CTL_PAD_KEY_COL1      0x020E03F0
#define IOMUXC_SW_PAD_CTL_PAD_KEY_COL2      0x020E03F4
#define IOMUXC_SW_PAD_CTL_PAD_KEY_COL3      0x020E03F8
#define IOMUXC_SW_PAD_CTL_PAD_KEY_COL4      0x020E03FC
#define IOMUXC_SW_PAD_CTL_PAD_KEY_ROW0      0x020E0400
#define IOMUXC_SW_PAD_CTL_PAD_KEY_ROW1      0x020E0404
#define IOMUXC_SW_PAD_CTL_PAD_KEY_ROW2      0x020E0408
#define IOMUXC_SW_PAD_CTL_PAD_KEY_ROW3      0x020E040C
#define IOMUXC_SW_PAD_CTL_PAD_KEY_ROW4      0x020E0410

// define base address of Select Input registers to be one word
// less than the minimum value so that a valid Select Input value
// is non-zero. (IOMUXC_SW_PAD_CTL_GRP_B3DS)
#define IOMUXC_SELECT_INPUT_BASE_ADDRESS 0x20E0620

typedef enum {
  IOMUXC_ANATOP_USB_OTG_ID_SELECT_INPUT = 0x20E0624,
  IOMUXC_ANATOP_USB_UH1_ID_SELECT_INPUT = 0x20E0628,
  IOMUXC_AUDMUX_P3_INPUT_DA_AMX_SELECT_INPUT = 0x20E062C,
  IOMUXC_AUDMUX_P3_INPUT_DB_AMX_SELECT_INPUT = 0x20E0630,
  IOMUXC_AUDMUX_P3_INPUT_RXCLK_AMX_SELECT_INPUT = 0x20E0634,
  IOMUXC_AUDMUX_P3_INPUT_RXFS_AMX_SELECT_INPUT = 0x20E0638,
  IOMUXC_AUDMUX_P3_INPUT_TXCLK_AMX_SELECT_INPUT = 0x20E063C,
  IOMUXC_AUDMUX_P3_INPUT_TXFS_AMX_SELECT_INPUT = 0x20E0640,
  IOMUXC_AUDMUX_P4_INPUT_DA_AMX_SELECT_INPUT = 0x20E0644,
  IOMUXC_AUDMUX_P4_INPUT_DB_AMX_SELECT_INPUT = 0x20E0648,
  IOMUXC_AUDMUX_P4_INPUT_RXCLK_AMX_SELECT_INPUT = 0x20E064C,
  IOMUXC_AUDMUX_P4_INPUT_RXFS_AMX_SELECT_INPUT = 0x20E0650,
  IOMUXC_AUDMUX_P4_INPUT_TXCLK_AMX_SELECT_INPUT = 0x20E0654,
  IOMUXC_AUDMUX_P4_INPUT_TXFS_AMX_SELECT_INPUT = 0x20E0658,
  IOMUXC_AUDMUX_P5_INPUT_DA_AMX_SELECT_INPUT = 0x20E065C,
  IOMUXC_AUDMUX_P5_INPUT_DB_AMX_SELECT_INPUT = 0x20E0660,
  IOMUXC_AUDMUX_P5_INPUT_RXCLK_AMX_SELECT_INPUT = 0x20E0664,
  IOMUXC_AUDMUX_P5_INPUT_RXFS_AMX_SELECT_INPUT = 0x20E0668,
  IOMUXC_AUDMUX_P5_INPUT_TXCLK_AMX_SELECT_INPUT = 0x20E066C,
  IOMUXC_AUDMUX_P5_INPUT_TXFS_AMX_SELECT_INPUT = 0x20E0670,
  IOMUXC_AUDMUX_P6_INPUT_DA_AMX_SELECT_INPUT = 0x20E0674,
  IOMUXC_AUDMUX_P6_INPUT_DB_AMX_SELECT_INPUT = 0x20E0678,
  IOMUXC_AUDMUX_P6_INPUT_RXCLK_AMX_SELECT_INPUT = 0x20E067C,
  IOMUXC_AUDMUX_P6_INPUT_RXFS_AMX_SELECT_INPUT = 0x20E0680,
  IOMUXC_AUDMUX_P6_INPUT_TXCLK_AMX_SELECT_INPUT = 0x20E0684,
  IOMUXC_AUDMUX_P6_INPUT_TXFS_AMX_SELECT_INPUT = 0x20E0688,
  IOMUXC_CAN1_IPP_IND_CANRX_SELECT_INPUT = 0x20E068C,
  IOMUXC_CAN2_IPP_IND_CANRX_SELECT_INPUT = 0x20E0690,
  IOMUXC_CCM_PMIC_VFUNCIONAL_READY_SELECT_INPUT = 0x20E069C,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_0 = 0x20E06A0,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_1 = 0x20E06A4,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_2 = 0x20E06A8,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_3 = 0x20E06AC,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_4 = 0x20E06B0,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_5 = 0x20E06B4,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_6 = 0x20E06B8,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_7 = 0x20E06BC,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_8 = 0x20E06C0,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_9 = 0x20E06C4,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_11 = 0x20E06C8,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_12 = 0x20E06CC,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_13 = 0x20E06D0,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_14 = 0x20E06D4,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_15 = 0x20E06D8,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_16 = 0x20E06DC,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_17 = 0x20E06E0,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_18 = 0x20E06E4,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_19 = 0x20E06E8,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_20 = 0x20E06EC,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_21 = 0x20E06F0,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_22 = 0x20E06F4,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_23 = 0x20E06F8,
  IOMUXC_CSI1_IPP_CSI_D_SELECT_INPUT_10 = 0x20E06FC,
  IOMUXC_CSI1_IPP_CSI_HSYNC_SELECT_INPUT = 0x20E0700,
  IOMUXC_CSI1_IPP_CSI_PIXCLK_SELECT_INPUT = 0x20E0704,
  IOMUXC_CSI1_IPP_CSI_VSYNC_SELECT_INPUT = 0x20E0708,
  IOMUXC_CSI1_TVDECODER_IN_FIELD_SELECT_INPUT = 0x20E070C,
  IOMUXC_ECSPI1_IPP_CSPI_CLK_IN_SELECT_INPUT = 0x20E0710,
  IOMUXC_ECSPI1_IPP_IND_MISO_SELECT_INPUT = 0x20E0714,
  IOMUXC_ECSPI1_IPP_IND_MOSI_SELECT_INPUT = 0x20E0718,
  IOMUXC_ECSPI1_IPP_IND_SS_B_SELECT_INPUT_0 = 0x20E071C,
  IOMUXC_ECSPI2_IPP_CSPI_CLK_IN_SELECT_INPUT = 0x20E0720,
  IOMUXC_ECSPI2_IPP_IND_MISO_SELECT_INPUT = 0x20E0724,
  IOMUXC_ECSPI2_IPP_IND_MOSI_SELECT_INPUT = 0x20E0728,
  IOMUXC_ECSPI2_IPP_IND_SS_B_SELECT_INPUT_0 = 0x20E072C,
  IOMUXC_ECSPI3_IPP_CSPI_CLK_IN_SELECT_INPUT = 0x20E0730,
  IOMUXC_ECSPI3_IPP_IND_MISO_SELECT_INPUT = 0x20E0734,
  IOMUXC_ECSPI3_IPP_IND_MOSI_SELECT_INPUT = 0x20E0738,
  IOMUXC_ECSPI3_IPP_IND_SS_B_SELECT_INPUT_0 = 0x20E073C,
  IOMUXC_ECSPI4_IPP_CSPI_CLK_IN_SELECT_INPUT = 0x20E0740,
  IOMUXC_ECSPI4_IPP_IND_MISO_SELECT_INPUT = 0x20E0744,
  IOMUXC_ECSPI4_IPP_IND_MOSI_SELECT_INPUT = 0x20E0748,
  IOMUXC_ECSPI4_IPP_IND_SS_B_SELECT_INPUT_0 = 0x20E074C,
  IOMUXC_ECSPI5_IPP_CSPI_CLK_IN_SELECT_INPUT = 0x20E0750,
  IOMUXC_ECSPI5_IPP_IND_MISO_SELECT_INPUT = 0x20E0754,
  IOMUXC_ECSPI5_IPP_IND_MOSI_SELECT_INPUT = 0x20E0758,
  IOMUXC_ECSPI5_IPP_IND_SS_B_SELECT_INPUT_0 = 0x20E075C,
  IOMUXC_ENET1_IPG_CLK_RMII_SELECT_INPUT = 0x20E0760,
  IOMUXC_ENET1_IPP_IND_MAC0_MDIO_SELECT_INPUT = 0x20E0764,
  IOMUXC_ENET1_IPP_IND_MAC0_RXCLK_SELECT_INPUT = 0x20E0768,
  IOMUXC_ENET2_IPG_CLK_RMII_SELECT_INPUT = 0x20E076C,
  IOMUXC_ENET2_IPP_IND_MAC0_MDIO_SELECT_INPUT = 0x20E0770,
  IOMUXC_ENET2_IPP_IND_MAC0_RXCLK_SELECT_INPUT = 0x20E0774,
  IOMUXC_ESAI_IPP_IND_FSR_SELECT_INPUT = 0x20E0778,
  IOMUXC_ESAI_IPP_IND_FST_SELECT_INPUT = 0x20E077C,
  IOMUXC_ESAI_IPP_IND_HCKR_SELECT_INPUT = 0x20E0780,
  IOMUXC_ESAI_IPP_IND_HCKT_SELECT_INPUT = 0x20E0784,
  IOMUXC_ESAI_IPP_IND_SCKR_SELECT_INPUT = 0x20E0788,
  IOMUXC_ESAI_IPP_IND_SCKT_SELECT_INPUT = 0x20E078C,
  IOMUXC_ESAI_IPP_IND_SDO0_SELECT_INPUT = 0x20E0790,
  IOMUXC_ESAI_IPP_IND_SDO1_SELECT_INPUT = 0x20E0794,
  IOMUXC_ESAI_IPP_IND_SDO2_SDI3_SELECT_INPUT = 0x20E0798,
  IOMUXC_ESAI_IPP_IND_SDO3_SDI2_SELECT_INPUT = 0x20E079C,
  IOMUXC_ESAI_IPP_IND_SDO4_SDI1_SELECT_INPUT = 0x20E07A0,
  IOMUXC_ESAI_IPP_IND_SDO5_SDI0_SELECT_INPUT = 0x20E07A4,
  IOMUXC_I2C1_IPP_SCL_IN_SELECT_INPUT = 0x20E07A8,
  IOMUXC_I2C1_IPP_SDA_IN_SELECT_INPUT = 0x20E07AC,
  IOMUXC_I2C2_IPP_SCL_IN_SELECT_INPUT = 0x20E07B0,
  IOMUXC_I2C2_IPP_SDA_IN_SELECT_INPUT = 0x20E07B4,
  IOMUXC_I2C3_IPP_SCL_IN_SELECT_INPUT = 0x20E07B8,
  IOMUXC_I2C3_IPP_SDA_IN_SELECT_INPUT = 0x20E07BC,
  IOMUXC_I2C4_IPP_SCL_IN_SELECT_INPUT = 0x20E07C0,
  IOMUXC_I2C4_IPP_SDA_IN_SELECT_INPUT = 0x20E07C4,
  IOMUXC_KPP_IPP_IND_COL_SELECT_INPUT_5 = 0x20E07C8,
  IOMUXC_KPP_IPP_IND_COL_SELECT_INPUT_6 = 0x20E07CC,
  IOMUXC_KPP_IPP_IND_COL_SELECT_INPUT_7 = 0x20E07D0,
  IOMUXC_KPP_IPP_IND_ROW_SELECT_INPUT_5 = 0x20E07D4,
  IOMUXC_KPP_IPP_IND_ROW_SELECT_INPUT_6 = 0x20E07D8,
  IOMUXC_KPP_IPP_IND_ROW_SELECT_INPUT_7 = 0x20E07DC,
  IOMUXC_LCD1_BUSY_SELECT_INPUT = 0x20E07E0,
  IOMUXC_LCD2_BUSY_SELECT_INPUT = 0x20E07E4,
  IOMUXC_MLB_MLB_CLK_IN_SELECT_INPUT = 0x20E07E8,
  IOMUXC_MLB_MLB_DATA_IN_SELECT_INPUT = 0x20E07EC,
  IOMUXC_MLB_MLB_SIG_IN_SELECT_INPUT = 0x20E07F0,
  IOMUXC_SAI1_IPP_IND_SAI_RXBCLK_SELECT_INPUT = 0x20E07F4,
  IOMUXC_SAI1_IPP_IND_SAI_RXDATA_SELECT_INPUT_0 = 0x20E07F8,
  IOMUXC_SAI1_IPP_IND_SAI_RXSYNC_SELECT_INPUT = 0x20E07FC,
  IOMUXC_SAI1_IPP_IND_SAI_TXBCLK_SELECT_INPUT = 0x20E0800,
  IOMUXC_SAI1_IPP_IND_SAI_TXSYNC_SELECT_INPUT = 0x20E0804,
  IOMUXC_SAI2_IPP_IND_SAI_RXBCLK_SELECT_INPUT = 0x20E0808,
  IOMUXC_SAI2_IPP_IND_SAI_RXDATA_SELECT_INPUT_0 = 0x20E080C,
  IOMUXC_SAI2_IPP_IND_SAI_RXSYNC_SELECT_INPUT = 0x20E0810,
  IOMUXC_SAI2_IPP_IND_SAI_TXBCLK_SELECT_INPUT = 0x20E0814,
  IOMUXC_SAI2_IPP_IND_SAI_TXSYNC_SELECT_INPUT = 0x20E0818,
  IOMUXC_SDMA_EVENTS_SELECT_INPUT_14 = 0x20E081C,
  IOMUXC_SDMA_EVENTS_SELECT_INPUT_15 = 0x20E0820,
  IOMUXC_SPDIF_SPDIF_IN1_SELECT_INPUT = 0x20E0824,
  IOMUXC_SPDIF_TX_CLK2_SELECT_INPUT = 0x20E0828,
  IOMUXC_UART1_IPP_UART_RTS_B_SELECT_INPUT = 0x20E082C,
  IOMUXC_UART1_IPP_UART_RXD_MUX_SELECT_INPUT = 0x20E0830,
  IOMUXC_UART2_IPP_UART_RTS_B_SELECT_INPUT = 0x20E0834,
  IOMUXC_UART2_IPP_UART_RXD_MUX_SELECT_INPUT = 0x20E0838,
  IOMUXC_UART3_IPP_UART_RTS_B_SELECT_INPUT = 0x20E083C,
  IOMUXC_UART3_IPP_UART_RXD_MUX_SELECT_INPUT = 0x20E0840,
  IOMUXC_UART4_IPP_UART_RTS_B_SELECT_INPUT = 0x20E0844,
  IOMUXC_UART4_IPP_UART_RXD_MUX_SELECT_INPUT = 0x20E0848,
  IOMUXC_UART5_IPP_UART_RTS_B_SELECT_INPUT = 0x20E084C,
  IOMUXC_UART5_IPP_UART_RXD_MUX_SELECT_INPUT = 0x20E0850,
  IOMUXC_UART6_IPP_UART_RTS_B_SELECT_INPUT = 0x20E0854,
  IOMUXC_UART6_IPP_UART_RXD_MUX_SELECT_INPUT = 0x20E0858,
  IOMUXC_USB_IPP_IND_OTG2_OC_SELECT_INPUT = 0x20E085C,
  IOMUXC_USB_IPP_IND_OTG_OC_SELECT_INPUT = 0x20E0860,
  IOMUXC_USDHC1_IPP_CARD_DET_SELECT_INPUT = 0x20E0864,
  IOMUXC_USDHC1_IPP_WP_ON_SELECT_INPUT = 0x20E0868,
  IOMUXC_USDHC2_IPP_CARD_DET_SELECT_INPUT = 0x20E086C,
  IOMUXC_USDHC2_IPP_WP_ON_SELECT_INPUT = 0x20E0870,
  IOMUXC_USDHC4_IPP_CARD_DET_SELECT_INPUT = 0x20E0874,
  IOMUXC_USDHC4_IPP_WP_ON_SELECT_INPUT = 0x20E0878,
  IOMUXC_SELECT_INPUT_UPPER_BOUND = IOMUXC_USDHC4_IPP_WP_ON_SELECT_INPUT,
} IMX_INPUT_SELECT;

#define IOMUXC_GPR_BASE_ADDRESS             0x020E4000

typedef struct{
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
} IMX_IOMUXC_GPR_REGISTERS;

typedef enum {
  IMX_IOMUXC_GPR1_USB_OTG_ID_SEL_ENET_RX_ER,
  IMX_IOMUXC_GPR1_USB_OTG_ID_SEL_GPIO_1,
} IMX_IOMUXC_GPR1_USB_OTG_ID_SEL;

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
    UINT32 reserved2 : 1;               // 29
    UINT32 APP_CLK_REQ_N : 1;           // 30 PCIe_CTL (CLK LOGIC CONTROLLER GLUE)
                                        //   Indicates that application logic is
                                        //   ready to have reference clock removed.
    UINT32 CFG_L1_CLK_REMOVAL_EN : 1;   // 31 PCIe_CTL (CLK LOGIC CONTROLLER GLUE)
                                        //   Enable the reference clock removal
                                        //   in L1 state.
  };
} IMX_IOMUXC_GPR1_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 PCS_TX_DEEMPH_GEN1 : 6;        // 0-5 PCIe_PHY - This static value sets
                                          //   the launch amplitude of the transmitter
                                          //   when pipe0_tx_swing is set to
                                          //   1'b0 (default state).
    UINT32 PCS_TX_DEEMPH_GEN2_3P5DB : 6;  // 6-11 PCIe_PHY - This static value
                                          //   sets the Tx driver SWING_FULL value.
    UINT32 PCS_TX_DEEMPH_GEN2_6DB : 6;    // 12-17 PCIe_PHY - This static value
                                          //   sets the Tx driver de-emphasis
                                          //   value in the case where pipe0_tx_deemph
                                          //   is set to 1'b0 and the PHY is running
                                          //   at the Gen2 (6db) rate.
    UINT32 PCS_TX_SWING_FULL : 7;         // 18-24 PCIe_PHY - This static value
                                          //   sets the Tx driver de-emphasis value
                                          //   in the case where pipe0_tx_deemph
                                          //   is set to 1'b1 (the default setting)
                                          //   and the PHY is running at the
                                          //   Gen2 (3p5db) rate.
    UINT32 PCS_TX_SWING_LOW : 7;          // 25-31 PCIe_PHY - This static value
                                          //   sets the Tx driver de-emphasis
                                          //   value in the case where pipe0_tx_deemph
                                          //   is set to 1'b1 (the default setting)
                                          //   and the PHY is running at the
                                          //   Gen1 rate.
  };
} IMX_IOMUXC_GPR8_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 reserved0 : 2;               // 0-1
    UINT32 uSDHC_DBG_MUX : 2;           // 2-3 uSDHC debug bus IO mux control
    UINT32 LOS_LEVEL : 5;               // 4-8 PCIe_PHY - Loss-of-Signal Detector
                                        //   Sensitivity Level Control Function:
                                        //   Sets the sensitivity level for the
                                        //   Loss-of-Signal detector. This signal
                                        //   must be set to 0x9
    UINT32 APPS_PM_XMT_PME : 1;         // 9 PCIe_CTL - Wake Up. Used by application
                                        //   logic to wake up the PMC state machine
                                        //   from a D1, D2 or D3 power state. Upon
                                        //   wake up, the core sends a PM_PME Message
    UINT32 APP_LTSSM_ENABLE : 1;        // 10 PCIe_CTL Driven low by the application
                                        //   after reset to hold the LTSSM in the
                                        //   Detect state until the application
                                        //   is ready.When the application has
                                        //   finished initializing the core
                                        //   configuration registers, it asserts
                                        //   app_ltssm_enable to allow the LTSSM
                                        //   to continue Link establishment.
    UINT32 APP_INIT_RST : 1;            // 11 PCIe_PHY - PCIe_CTL - Request from
                                        //   the application to send a Hot Reset
                                        //   to the downstream device.
    UINT32 DEVICE_TYPE : 4;             // 12-15 PCIe_CTL - Device/Port Type.
                                        //   0000 PCIE_EP - EP Mode
                                        //   0010 PCIE_RC - RC Mode
    UINT32 APPS_PM_XMT_TURNOFF : 1;     // 16 PCIe_CTL - Request from the application
                                        //   to generate a PM_Turn_Off Message.
    UINT32 DIA_STATUS_BUS_SELECT : 4;   // 17-20 PCIe_CTL - used for debug to select
                                        //   what part of diag_status_bus will be
                                        //   reflected on the 32 bits of the iomux
    UINT32 PCIe_CTL_7 : 3;              // 21-23 PCIe control of diagnostic bus select
    UINT32 ARMP_APB_CLK_EN : 1;         // 24 ARM platform APB clock enable
    UINT32 ARMP_ATB_CLK_EN : 1;         // 25 ARM platform ATB clock enable
    UINT32 ARMP_AHB_CLK_EN : 1;         // 26 ARM platform AHB clock enable
    UINT32 ARMP_IPG_CLK_EN : 1;         // 27 ARM platform IPG clock enable
    UINT32 reserved1 : 4;               // 28-31
  };
} IMX_IOMUXC_GPR12_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 reserved1 : 1;       // 0
    UINT32 reserved2 : 1;       // 1
    UINT32 MC_ENV : 1;          // 2 Monotonic Counter Enable and Valid
    UINT32 reserved3 : 1;       // 3
    UINT32 reserved4 : 1;       // 4
    UINT32 DP_EN : 1;           // 5 Dumb PMIC Enabled
    UINT32 TOP : 1;             // 6 Turn off System Power
    UINT32 PWR_GLITCH_EN : 1;   // 7 Power Glitch Detection Enable
    UINT32 reserved5 : 1;       // 8
    UINT32 reserved6 : 1;       // 9
    UINT32 reserved7 : 5;       // 10-14
    UINT32 reserved8 : 1;       // 15
    UINT32 BTN_PRESS_TIME : 2;  // 16-17 Button press time out values for PMIC Logic.
    UINT32 DEBOUNCE : 2;        // 18-19 debounce time for the BTN input signal
    UINT32 ON_TIME : 2;         // 20-21 Time after BTN is asserted before pmic_en_b
                                //   is asserted
    UINT32 PK_EN : 1;           // 22 PMIC On Request Enable
    UINT32 PK_OVERRIDE : 1;     // 23 PMIC On Request Override
    UINT32 reserved9 : 8;       // 24-31
  };
} IMX_SNVS_LPCR_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 MINOR_REV : 8;       // 0-7 SNVS block minor version number
    UINT32 MAJOR_REV : 8;       // 8-15 SNVS block major version number
    UINT32 IP_ID : 16;          // 16-31 SNVS block ID (IMX_SNVS_IP_ID)
  };
} IMX_SNVS_HPVIDR1_REG;

typedef struct {
  UINT32 HPLR;            // 0x000 SNVS_HP Lock Register
  UINT32 HPCOMR;          // 0x004 SNVS_HP Command Register
  UINT32 HPCR;            // 0x008 SNVS_HP Control Register
  UINT32 reserved1[2];
  UINT32 HPSR;            // 0x014 SNVS_HP Status Register
  UINT32 reserved2[3];
  UINT32 HPRTCMR;         // 0x024 SNVS_HP Real Time Counter MSB Register
  UINT32 HPRTCLR;         // 0x028 SNVS_HP Real Time Counter LSB Register
  UINT32 HPTAMR;          // 0x02C SNVS_HP Time Alarm MSB Register
  UINT32 HPTALR;          // 0x030 SNVS_HP Time Alarm LSB Register
  UINT32 LPLR;            // 0x034 SNVS_LP Lock Register
  UINT32 LPCR;            // 0x038 SNVS_LP Control Register
  UINT32 reserved3[4];
  UINT32 LPSR;            // 0x04C SNVS_LP Status Register
  UINT32 reserved4[3];
  UINT32 LPSMCMR;         // 0x05C SNVS_LP Secure Monotonic Counter MSB Register
  UINT32 LPSMCLR;         // 0x060 SNVS_LP Secure Monotonic Counter LSB Register
  UINT32 reserved5[1];
  UINT32 LPGPR;           // 0x068 SNVS_LP General Purpose Register
  UINT32 reserved6[739];
  UINT32 HPVIDR1;         // 0xBF8 SNVS_HP Version ID Register 1
  UINT32 HPVIDR2;         // 0xBFC SNVS_HP Version ID Register 2
} IMX_SNVS_REGISTERS;

// System Reset Controller (SRC)
#define IMX_SRC_BASE 0x020D8000
#define IMX_SRC_LENGTH 0x4000

// SCR Register Definition
// SRC_SCR_REG.warm_rst_bypass_count
typedef enum {
  IMX_SCR_WARM_RST_BYPASS_COUNT_DISABLED,
  IMX_SCR_WARM_RST_BYPASS_COUNT_16,
  IMX_SCR_WARM_RST_BYPASS_COUNT_32,
  IMX_SCR_WARM_RST_BYPASS_COUNT_64,
} IMX_SCR_WARM_RST_BYPASS_COUNT;

// SRC_SCR_REG.mask_wdog_rst
typedef enum {
  IMX_SRC_MASK_WDOG_RST_B_MASKED = 0x5,
  IMX_SRC_MASK_WDOG_RST_B_NOT_MASKED = 0xA,
} IMX_SRC_MASK_WDOG_RST;

// SRC_SCR_REG.mask_tempsense_reset
typedef enum {
  IMX_SRC_MASK_TEMPSENSE_RESET_NOT_MASKED = 0x2,
  IMX_SRC_MASK_TEMPSENSE_RESET_MASKED = 0x5,
} IMX_SRC_MASK_TEMPSENSE_RESET;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 warm_reset_enable : 1;     // 0 WARM reset enable bit
    UINT32 sw_gpu_rst : 1;            // 1 Software reset for GPU
    UINT32 reserved1 : 1;             // 2
    UINT32 m4c_rst : 1;               // 3 Self-clearing SW reset for M4 core
    UINT32 m4c_non_sclr_rst : 1;      // 4 Non-self-clearing SW reset for M4 core
    UINT32 warm_rst_bypass_count : 2; // 5-6 Defines the XTALI cycles to count
                                      //   before bypassing the MMDC acknowledge
                                      //   for WARM reset.
    UINT32 mask_wdog_rst : 4;         // 7-10 Mask wdog_rst_b source
    UINT32 eim_rst : 1;               // 11 EIM reset is needed in order to
                                      //   reconfigure the eim chip select.
    UINT32 m4p_rst : 1;               // 12 Self-clearing SW reset for M4 platform
    UINT32 core0_rst : 1;             // 13 Software reset for core0 only
    UINT32 reserved2 : 3;             // 14-16
    UINT32 core0_dbg_rst : 1;         // 17 Software reset for core0 debug only.
    UINT32 mask_tempsense_reset : 3;  // 18-20 Mask tempsense_reset source
    UINT32 cores_dbg_rst : 1;         // 21 Software reset for debug of arm
                                      //   platform only
    UINT32 m4_enable : 1;             // 22 Enable M4 core
    UINT32 wdog3_rst_optn_m4 : 1;     // 23 wdog3_rst_b option for M4. This bit
                                      //   is only effective when wdog3_rst_option
                                      //   is set to 1.
    UINT32 wdog3_rst_optn : 1;        // 24 Wdog3_rst_b option
    UINT32 dbg_rst_msk_pg : 1;        // 25 Do not assert debug resets after
                                      //   power gating event of core
    UINT32 mix_rst_strch : 2;         // 26-27 SoC mix (Audio, ENET, uSDHC,
                                      //   EIM, QSPI, OCRAM, MMDC, etc) power up
                                      //   reset stretch mix reset
                                      //   width = (mix_rst_strtch +1)* 88 ipg_clk
                                      //   cycles
    UINT32 mask_wdog3_rst : 4;        // 28-31 Mask wdog3_rst_b source
  };
} IMX_SRC_SCR_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 BOOT_CFG1 : 8;   // 0-7
    UINT32 BOOT_CFG2 : 8;   // 8-15
    UINT32 BOOT_CFG3 : 8;   // 16-23
    UINT32 BOOT_CFG4 : 8;   // 24-31
  };
} IMX_SRC_SBMR1_REG;


typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 SEC_COFNIG : 2;    // 0-1
    UINT32 reserved1 : 1;     // 2
    UINT32 DIR_BT_DIS : 1;    // 3
    UINT32 BT_FUSE_SEL : 1;   // 4
    UINT32 reserved2 : 19;    // 5-23
    UINT32 BMOD : 2;          // 24-25
    UINT32 reserved3 : 6;     // 26-31
  };
} IMX_SRC_SBMR2_REG;

typedef struct {
  UINT32 SCR;           // 0x00 SRC Control Register (SRC_SCR)
  UINT32 SBMR1;         // 0x04 SRC Boot Mode Register 1 (SRC_SBMR1)
  UINT32 SRSR;          // 0x08 SRC Reset Status Register (SRC_SRSR)
  UINT32 reserved1[2];
  UINT32 SISR;          // 0x14 SRC Interrupt Status Register (SRC_SISR)
  UINT32 SIMR;          // 0x18 SRC Interrupt Mask Register (SRC_SIMR)
  UINT32 SBMR2;         // 0x1C SRC Boot Mode Register 2 (SRC_SBMR2)
  UINT32 GPR1;          // 0x20 SRC General Purpose Register 1 (SRC_GPR1)
  UINT32 GPR2;          // 0x24 SRC General Purpose Register 2 (SRC_GPR2)
  UINT32 GPR3;          // 0x28 SRC General Purpose Register 3 (SRC_GPR3)
  UINT32 GPR4;          // 0x2C SRC General Purpose Register 4 (SRC_GPR4)
  UINT32 GPR5;          // 0x30 SRC General Purpose Register 4 (SRC_GPR5)
  UINT32 GPR6;          // 0x34 SRC General Purpose Register 4 (SRC_GPR6)
  UINT32 GPR7;          // 0x38 SRC General Purpose Register 4 (SRC_GPR7)
  UINT32 GPR8;          // 0x3C SRC General Purpose Register 4 (SRC_GPR8)
  UINT32 GPR9;          // 0x40 SRC General Purpose Register 4 (SRC_GPR9)
  UINT32 GPR10;         // 0x44 SRC General Purpose Register 4 (SRC_GPR10)
} IMX_SRC_REGISTERS;

// Watchdog (WDOG)
#define IMX_WDOG1_BASE 0x020BC000
#define IMX_WDOG2_BASE 0x020C0000
#define IMX_WDOG3_BASE 0x02288000
#define IMX_WDOG_LENGTH 0x4000
#define IMX_WDOG_WSR_FEED1 0x5555
#define IMX_WDOG_WSR_FEED2 0xAAAA

typedef union {
  UINT16 AsUint16;
  struct {
    UINT16 WDZST : 1;       // 0 Watchdog Low Power
    UINT16 WDBG : 1;        // 1 Watchdog DEBUG Enable
    UINT16 WDE : 1;         // 2 Watchdog Enable
    UINT16 WDT : 1;         // 3 WDOG_B Time-out assertion.
    UINT16 SRS : 1;         // 4 Software Reset Signal
    UINT16 WDA : 1;         // 5 WDOG_B assertion
    UINT16 reserved1 : 1;   // 6
    UINT16 WDW : 1;         // 7 Watchdog Disable for Wait
    UINT16 WT : 8;          // 8-15 Watchdog Time-out Field
  };
} IMX_WDOG_WCR_REG;

typedef struct {
  UINT16 WCR;   // 0x0 Watchdog Control Register (WDOG1_WCR)
  UINT16 WSR;   // 0x2 Watchdog Service Register (WDOG1_WSR)
  UINT16 WRSR;  // 0x4 Watchdog Reset Status Register (WDOG1_WRSR)
  UINT16 WICR;  // 0x6 Watchdog Interrupt Control Register (WDOG1_WICR)
  UINT16 WMCR;  // 0x8 Watchdog Miscellaneous Control Register (WDOG1_WMCR)
} IMX_WDOG_REGISTERS;

// Clock Control Module (CCM)
#define IMX6SX_CCM_CLOCK_OFF    0
#define IMX6SX_RUN_ONLY         1
#define IMX6SX_RUN_AND_WAIT     3
#define IMX_CCM_BASE 0x020C4000
#define IMX_CCM_LENGTH 0x4000
#define IMX_CCM_ANALOG_BASE  0x020C8000
#define IMX_CCM_ANALOG_LENGTH 0x1000
#define IMX_REF_CLK_24M_FREQ 24000000

typedef enum {
  IMX_CCM_PLL3_SW_CLK_SEL_PLL3_MAIN_CLK,
  IMX_CCM_PLL3_SW_CLK_SEL_PLL3_BYPASS_CLK,
} IMX_CCM_PLL3_SW_CLK_SEL;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 pll3_sw_clk_sel : 1; // 0 Selects source to generate pll3_sw_clk
    UINT32 reserved1 : 1;       // 1
    UINT32 pll1_sw_clk_sel : 1; // 2 Selects source to generate pll1_sw_clk.
    UINT32 reserved2 : 5;       // 3-7
    UINT32 step_sel : 1;        // 8 Selects the option for step frequency
    UINT32 reserved3 : 23;      // 9-31
  };
} IMX_CCM_CCSR_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 arm_podf : 3;  // 0-3 Divider for ARM clock root
    UINT32 reserved : 29; // 3-31
  };
} IMX_CCM_CACRR_REG;

// CBCMR.gpu_core_sel
typedef enum {
  IMX_CCM_GPU_CLK_SEL_PLL3_PFD1,
  IMX_CCM_GPU_CLK_SEL_PLL3_PFD0,
  IMX_CCM_GPU_CLK_SEL_PLL2,
  IMX_CCM_GPU_CLK_SEL_PLL2_PFD2
} IMX_CCM_GPU_CLK_SEL;

// CBCMR.gpu_axi_sel
typedef enum {
  IMX_CCM_GPU_AXI_SEL_PLL2_PFD2,
  IMX_CCM_GPU_AXI_SEL_PLL3_PFD0,
  IMX_CCM_GPU_AXI_SEL_PLL3_PFD1,
  IMX_CCM_GPU_AXI_SEL_PLL2
} IMX_CCM_GPU_AXI_SEL;

// CBCMR.pcie_axi_clock_sel
typedef enum {
  IMX_CCM_PCIE_AXI_CLOCK_SEL_AXI,
  IMX_CCM_PCIE_AXI_CLOCK_SEL_AHB,
} IMX_CCM_PCIE_AXI_CLOCK_SEL;

// CBCMR.periph_clk2_sel
typedef enum {
  IMX_CCM_PERIPH_CLK2_SEL_PLL3_SW_CLK,
  IMX_CCM_PERIPH_CLK2_SEL_OSC_CLK,
  IMX_CCM_PERIPH_CLK2_SEL_PLL2_BYPASS_CLK
} IMX_CCM_PERIPH_CLK2_SEL;

// CBCMR.pre_periph_clk_sel
typedef enum {
  IMX_CCM_PRE_PERIPH_CLK_SEL_PLL2,
  IMX_CCM_PRE_PERIPH_CLK_SEL_PLL2_PFD2,
  IMX_CCM_PRE_PERIPH_CLK_SEL_PLL2_PFD0,
  IMX_CCM_PRE_PERIPH_CLK_SEL_PLL2_PFD2_DIV2,
} IMX_CCM_PRE_PERIPH_CLK_SEL;

// CBCMR.periph2_clk2_sel
typedef enum {
  IMX_CCM_PERIPH2_CLK2_SEL_PLL3_SW_CLK,
  IMX_CCM_PERIPH2_CLK2_SEL_OSC
} IMX_CCM_PERIPH2_CLK2_SEL;

// CBCMR.pre_periph2_clk_sel
typedef enum {
  IMX_CCM_PRE_PERIPH2_CLK_SEL_PLL2,
  IMX_CCM_PRE_PERIPH2_CLK_SEL_PLL2_PFD2,
  IMX_CCM_PRE_PERIPH2_CLK_SEL_PLL2_PFD0,
  IMX_CCM_PRE_PERIPH2_CLK_SEL_PLL4
} IMX_CCM_PRE_PERIPH2_CLK_SEL;

// CBCMR.podf (divider for lcdif1_podf, gpu_axi_podf, gpu_core_podf)
typedef enum {
  IMX_CCM_PODF_DIV1,
  IMX_CCM_PODF_DIV2,
  IMX_CCM_PODF_DIV3,
  IMX_CCM_PODF_DIV4,
  IMX_CCM_PODF_DIV5,
  IMX_CCM_PODF_DIV6,
  IMX_CCM_PODF_DIV7,
  IMX_CCM_PODF_DIV8
} IMX_CCM_PODF;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 reserved1 : 4;             // 0-3
    UINT32 gpu_core_sel : 2;          // 4-5 Selector for gpu_core clock multiplexer
    UINT32 reserved2 : 2;             // 6-7
    UINT32 gpu_axi_sel : 2;           // 8-9 Selector for gpu_axi clock multiplexer
    UINT32 pcie_axi_clock_sel : 1;    // 10 Selector for pcie_axi_clock multiplexier
    UINT32 reserved3 : 1;             // 11
    UINT32 periph_clk2_sel : 2;       // 12-13 Selector for peripheral clk2 clock
                                      //   multiplexer
    UINT32 reserved4 : 4;             // 14-17
    UINT32 pre_periph_clk_sel : 2;    // 18-19 Selector for pre_periph clock multiplexer
    UINT32 periph2_clk2_sel : 1;      // 20 Selector for periph2_clk2 clock multiplexer
    UINT32 pre_periph2_clk_sel : 2;   // 21-22 Selector for pre_periph2 clock multiplexer
    UINT32 lcdif1_podf : 3;           // 23-25 Post-divider for lcdif1 clock
    UINT32 gpu_axi_podf : 3;          // 26-28 Divider for gpu_axi_podf
    UINT32 gpu_core_podf : 3;         // 29-31 Post divider for gpu_core clock
  };
} IMX_CCM_CBCMR_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 ssi1_clk_podf : 6;   // 0-5 Divider for ssi1 clock podf
    UINT32 ssi1_clk_pred : 3;   // 6-8 Divider for ssi1 clock pred
    UINT32 esai_clk_pred : 3;   // 9-11 Divider for esai clock pred
    UINT32 reserved1 : 4;       // 12-15 Reserved
    UINT32 ssi3_clk_podf : 6;   // 16-21 Divider for ssi3 clock podf
    UINT32 ssi3_clk_pred : 3;   // 22-24 Divider for ssi3 clock pred
    UINT32 esai_clk_podf : 3;   // 25-27 Divider for esai clock podf
    UINT32 reserved2 : 4;       // 28-31 Reserved
  };
} IMX_CCM_CS1CDR_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 ssi2_clk_podf : 6;     // 0-5 Divider for ssi2 clock podf
    UINT32 ssi2_clk_pred : 3;     // 6-8 Divider for ssi2 clock pred
    UINT32 ldb_di0_clk_sel : 3;   // 9-11 Selector for ldb_di0 clock multiplexer
    UINT32 ldb_di1_clk_sel : 3;   // 12-14 Selector for ldb_di1 clock multiplexer
    UINT32 qspi2_clk_sel : 3;     // 15-17 Selector for QSPI2 clock multiplexer
    UINT32 qspi2_clk_pred : 3;    // 18-20 Divider for QSPI1 clock pred divider
    UINT32 qspi2_clk_podf : 6;    // 21-26 Divider for QSPI2 clock divider
    UINT32 reserved : 5;          // 27-31
  };
} IMX_CCM_CS2CDR_REG;

typedef enum {
  IMX_CCM_CCGR_OFF = 0x0,     // Clock is off during all modes. Stop enter
                              //   hardware handshake is disabled.
  IMX_CCM_CCGR_ON_RUN = 0x1,  // Clock is on in run mode,
                              //   but off in WAIT and STOP modes
  IMX_CCM_CCGR_ON = 0x3,      // Clock is on during all modes, except STOP mode.
} IMX_CCM_CCGR;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 aips_tz1_clk_enable : 2;     // 0-1 aips_tz1 clocks
    UINT32 aips_tz2_clk_enable : 2;     // 2-3 aips_tz2 clocks
    UINT32 apbhdma_hclk_enable : 2;     // 4-5 apbhdma hclk clock
    UINT32 asrc_clk_enable : 2;         // 6-7 asrc clock
    UINT32 caam_secure_mem_clk_enable : 2; // 8-9 caam_secure_mem clock
    UINT32 caam_wrapper_aclk_enable : 2; // 10-11 caam_wrapper_aclk clock
    UINT32 caam_wrapper_ipg_enable : 2; // 12-13 caam_wrapper_ipg clock
    UINT32 can1_clk_enable : 2;         // 14-15 can1 clock
    UINT32 can1_serial_clk_enable : 2;  // 16-17 can1_serial clock
    UINT32 can2_clk_enable : 2;         // 18-19 can2 clock
    UINT32 can2_serial_clk_enable : 2;  // 20-21 can2_serial clock
    UINT32 arm_dbg_clk_enable : 2;      // 22-23 CPU debug clocks
    UINT32 dcic1_clk_enable : 2;        // 24-25 dcic 1 clocks
    UINT32 dcic2_clk_enable : 2;        // 26-27 dcic2 clocks
    UINT32 reserved : 2;                // 28-29
    UINT32 aips_tz3_clk_enable : 2;     // 30-31 aips_tz3 clocks
  };
} IMX_CCM_CCGR0_REG;

// CHSCCDR.m4_clk_sel
typedef enum {
  IMX_CHSCCDR_M4_CLK_SEL_DIVIDED_PRE_MUXED_M4_CLOCK,
  IMX_CHSCCDR_M4_CLK_SEL_IPP_DI0_CLK,
  IMX_CHSCCDR_M4_CLK_SEL_IPP_DI1_CLK,
  IMX_CHSCCDR_M4_CLK_SEL_LDB_DI0_CLK,
  IMX_CHSCCDR_M4_CLK_SEL_LDB_DI1_CLK
} IMX_CHSCCDR_M4_CLK_SEL;

// CHSCCDR.m4_pre_clk_sel
typedef enum {
  IMX_M4_PRE_CLK_SEL_PLL2,
  IMX_M4_PRE_CLK_SEL_PLL3_SW_CLK,
  IMX_M4_PRE_CLK_SEL_OSC_CLK_24M,
  IMX_M4_PRE_CLK_SEL_PLL2_PFD0,
  IMX_M4_PRE_CLK_SEL_PLL2_PFD2,
  IMX_M4_PRE_CLK_SEL_PLL3_PFD3
} IMX_M4_PRE_CLK_SEL;

// CHSCCDR.enet_clk_sel
typedef enum {
  IMX_ENET_CLK_SEL_DIVIDED_PREMUXED_ENET_CLK,
  IMX_ENET_CLK_SEL_IPP_DI0_CLK,
  IMX_ENET_CLK_SEL_IPP_DI1_CLK,
  IMX_ENET_CLK_SEL_LDB_DI0_CLK,
  IMX_ENET_CLK_SEL_LDB_DI1_CLK
} IMX_ENET_CLK_SEL;

// CHSCCDR.enet_pre_clk_sel
typedef enum {
  IMX_ENET_PRE_CLK_SEL_PLL2,
  IMX_ENET_PRE_CLK_SEL_PLL3_SW_CLK,
  IMX_ENET_PRE_CLK_SEL_PLL5,
  IMX_ENET_PRE_CLK_SEL_PLL2_PFD0,
  IMX_ENET_PRE_CLK_SEL_PLL2_PFD2,
  IMX_ENET_PRE_CLK_SEL_PLL3_PFD2
} IMX_ENET_PRE_CLK_SEL;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 m4_clk_sel : 3;        // 0-2 Selector for M4 root clock multiplexer
    UINT32 m4_podf : 3;           // 3-5 Divider for M4 clock divider
    UINT32 m4_pre_clk_sel : 3;    // 6-8 Selector for M4 root clock pre-multiplexer
    UINT32 enet_clk_sel : 3;      // 9-11 Selector for ENET root clock multiplexer
    UINT32 enet_podf : 3;         // 12-14 Divider for ENET clock divider
    UINT32 enet_pre_clk_sel : 3;  // 15-17 Selector for ENET root clock pre-multiplexer
    UINT32 reserved : 14;         // 18-31
  };
} IMX_CCM_CHSCCDR_REG;

// NOTE: OPENVG clock cannot be gated without gating GPU2D clock as well.
//       Configure both CG bits (CCM_ANALOG_CCGR1[CG12] and
//       CCM_ANALOG_CCGR3[CG15]) to gate OPENVG.
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 ecspi1_clk_enable : 2;       // 0-1 ecspi1 clocks
    UINT32 ecspi2_clk_enable : 2;       // 2-3 ecspi2 clocks
    UINT32 ecspi3_clk_enable : 2;       // 4-5 ecspi3 clocks
    UINT32 ecspi4_clk_enable : 2;       // 6-7 ecspi4 clocks
    UINT32 ecspi5_clk_enable : 2;       // 8-9 ecspi5 clocks
    UINT32 reserved1 : 2;               // 10-11
    UINT32 epit1_clk_enable : 2;        // 12-13 epit1 clocks
    UINT32 epit2_clk_enable : 2;        // 14-15 epit2 clocks
    UINT32 esai_clk_enable : 2;         // 16-17 esai clocks
    UINT32 wakeup_clk_enable : 2;       // 18-19 wakeup clock
    UINT32 gpt_clk_enable : 2;          // 20-21 gpt bus clock
    UINT32 gpt_serial_clk_enable : 2;   // 22-23 gpt serial clock
    UINT32 reserved2 : 2;               // 24-25
    UINT32 gpu_clk_enable : 2;          // 26-27 gpu clock
    UINT32 ocram_s_clk_enable : 2;      // 28-29 ocram_s clock
    UINT32 canfd_clk_enable : 2;        // 30-31 canfd clock
  };
} IMX_CCM_CCGR1_REG;

// CBCDR.ocram_clk_sel
typedef enum {
  IMX_CCM_OCRAM_CLK_SEL_PERIPH_CLK,
  IMX_CCM_OCRAM_CLK_SEL_AXI_ALT_CLK
} IMX_CCM_OCRAM_CLK_SEL;

// CBCDR.ocram_alt_clk_sel
typedef enum {
  IMX_CCM_OCRAM_ALT_CLK_SEL_PLL2_PFD2,
  IMX_CCM_OCRAM_ALT_CLK_SEL_PLL3_PFD1
} IMX_CCM_OCRAM_ALT_CLK_SEL;

// CBCDR.periph_clk_sel
typedef enum {
  IMX_CCM_PERIPH_CLK_SEL_PRE_PERIPH_CLK_SEL,
  IMX_CCM_PERIPH_CLK_SEL_PERIPH_CLK2_CLK_DIVIDED
} IMX_CCM_PERIPH_CLK_SEL;

// CBCDR.periph2_clk_sel
typedef enum {
  IMX_CCM_PERIPH2_CLK_SEL_PRE_PERIPH2_CLK,
  IMX_CCM_PERIPH2_CLK_SEL_PERIPH2_CLK2_CLK_DIVIDED
} IMX_CCM_PERIPH2_CLK_SEL;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 periph2_clk2_podf : 3;   // 0-2 Divider for periph2_clk2 podf
    UINT32 fabric_mmdc_podf : 3;    // 3-5 Post divider for fabric / mmdc clock
    UINT32 ocram_clk_sel : 1;       // 6 OCRAM clock source select
    UINT32 ocram_alt_clk_sel : 1;   // 7 OCRAM alternative clock select
    UINT32 ipg_podf : 2;            // 8-9 Divider for ipg podf
    UINT32 ahb_podf : 3;            // 10-12 Divider for AHB PODF
    UINT32 reserved1 : 3;           // 13-15
    UINT32 ocram_podf : 3;          // 16-18 Post divider for ocram clock
    UINT32 reserved2 : 6;           // 19-24
    UINT32 periph_clk_sel : 1;      // 25 Selector for peripheral main clock
    UINT32 periph2_clk_sel : 1;     // 26 Selector for peripheral2 main clock
                                    //   (source of mmdc_clk_root )
    UINT32 periph_clk2_podf : 3;    // 27-29 Divider for periph_clk2_podf
    UINT32 reserved3 : 2;           // 30-31
  };
} IMX_CCM_CBCDR_REG;

// CCOSR.CLKO1_SEL
typedef enum {
  IMX_CCM_CLKO1_SEL_VID_CLK_ROOT = 4,
  IMX_CCM_CLKO1_SEL_OCRAM_CLK_ROOT,
  IMX_CCM_CLKO1_SEL_QSPI2_CLK_ROOT,
  IMX_CCM_CLKO1_SEL_M4_CLK_ROOT,
  IMX_CCM_CLKO1_SEL_ENET_AXI_CLK_ROOT,
  IMX_CCM_CLKO1_SEL_LCDIF2_PIX_CLK_ROOT,
  IMX_CCM_CLKO1_SEL_LCDIF1_PIX_CLK_ROOT,
  IMX_CCM_CLKO1_SEL_AHB_CLK_ROOT,
  IMX_CCM_CLKO1_SEL_IPG_CLK_ROOT,
  IMX_CCM_CLKO1_SEL_PERCLK_ROOT,
  IMX_CCM_CLKO1_SEL_CKIL_SYNC_CLK_ROOT,
  IMX_CCM_CLKO1_SEL_PLL4_MAIN_CLK
} IMX_CCM_CLKO1_SEL;

// CCOSR.CLKO2_SEL
typedef enum {
  IMX_CCM_CLKO2_SEL_MMDC_CLK_ROOT = 1,
  IMX_CCM_CLKO2_SEL_USDHC4_CLK_ROOT = 2,
  IMX_CCM_CLKO2_SEL_USDHC1_CLK_ROOT = 3,
  IMX_CCM_CLKO2_SEL_WRCK_CLK_ROOT = 5,
  IMX_CCM_CLKO2_SEL_ECSPI_CLK_ROOT = 6,
  IMX_CCM_CLKO2_SEL_USDHC3_CLK_ROOT = 8,
  IMX_CCM_CLKO2_SEL_PCIE_CLK_ROOT = 9,
  IMX_CCM_CLKO2_SEL_ARM_CLK_ROOT = 10,
  IMX_CCM_CLKO2_SEL_CSI_CORE = 11,
  IMX_CCM_CLKO2_SEL_DISPLAY_AXI_CLK_ROOT = 12,
  IMX_CCM_CLKO2_SEL_OSC_CLK = 14,
  IMX_CCM_CLKO2_SEL_USDHC2_CLK_ROOT = 17,
  IMX_CCM_CLKO2_SEL_SSI1_CLK_ROOT = 18,
  IMX_CCM_CLKO2_SEL_SSI2_CLK_ROOT = 19,
  IMX_CCM_CLKO2_SEL_SSI3_CLK_ROOT = 20,
  IMX_CCM_CLKO2_SEL_GPU_AXI_CLK_ROOT = 21,
  IMX_CCM_CLKO2_SEL_CAN_CLK_ROOT = 23,
  IMX_CCM_CLKO2_SEL_LVDS_CLK_ROOT = 24,
  IMX_CCM_CLKO2_SEL_QSPI1_CLK_ROOT = 25,
  IMX_CCM_CLKO2_SEL_ESAI_CLK_ROOT = 26,
  IMX_CCM_CLKO2_SEL_ACLK_EIM_SLOW_CLK_ROOT = 27,
  IMX_CCM_CLKO2_SEL_UART_CLK_ROOT = 28,
  IMX_CCM_CLKO2_SEL_SPDIF0_CLK_ROOT = 29,
  IMX_CCM_CLKO2_SEL_AUDIO_CLK_ROOT = 36
} IMX_CCM_CLKO2_SEL;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 CLKO1_SEL : 4;     // 0-3 Selection of the clock to be generated on CCM_CLKO1
    UINT32 CLKO1_DIV : 3;     // 4-6 Setting the divider of CCM_CLKO1
    UINT32 CLKO1_EN : 1;      // 7 Enable of CCM_CLKO1 clock
    UINT32 CLK_OUT_SEL : 1;   // 8 CCM_CLKO1 output to reflect CCM_CLKO1 or CCM_CLKO2 clocks
    UINT32 reserved1 : 7;     // 9-15
    UINT32 CLKO2_SEL : 5;     // 16-20 Selection of the clock to be generated on CCM_CLKO2
    UINT32 CLKO2_DIV : 3;     // 21-23 Setting the divider of CCM_CLKO2
    UINT32 CLKO2_EN : 1;      // 24 Enable of CCM_CLKO2 clock
    UINT32 reserved2 : 7;     // 25-31
  };
} IMX_CCM_CCOSR_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 reserved1 : 2;                 // 0-1
    UINT32 csi_clk_enable : 2;            // 2-3 csi clock
    UINT32 reserved2 : 2;                 // 4-5
    UINT32 i2c1_serial_clk_enable : 2;    // 6-7 i2c1_serial clock
    UINT32 i2c2_serial_clk_enable : 2;    // 8-9 i2c2_serial clock
    UINT32 i2c3_serial_clk_enable : 2;    // 10-11 i2c3_serial clock
    UINT32 iim_clk_enable : 2;            // 12-13 OCOTP_CTRL clock
    UINT32 iomux_ipt_clk_io_enable : 2;   // 14-15 iomux_ipt_clk_io clock
    UINT32 ipmux1_clk_enable : 2;         // 16-17 ipmux1 clock
    UINT32 ipmux2_clk_enable : 2;         // 18-19 ipmux2 clock
    UINT32 ipmux3_clk_enable : 2;         // 20-21 ipmux3 clock
    UINT32 ipsync_ip2apb_tzasc1_ipg_master_clk_enable : 2;  // 22-23 ipsync_ip2apb_tzasc1_ipg clocks
    UINT32 reserved3 : 2;                 // 24-25
    UINT32 reserved4 : 2;                 // 26-27
    UINT32 lcd_clk_enable : 2;            // 28-29 lcd clocks
    UINT32 pxp_clk_enable : 2;            // 30-31 pxp clocks
  };
} IMX_CCM_CCGR2_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 reserved1 : 2;                          // 0-1
    UINT32 m4_clk_enable : 2;                      // 2-3 m4 clock
    UINT32 enet_clk_enable : 2;                    // 4-5 enet clock
    UINT32 disp_axi_clk_enable : 2;                // 6-7 display axi clock
    UINT32 lcdif2_pix_clk_enable : 2;              // 8-9 lcdif2 pix clock
    UINT32 lcdif1_pix_clk_enable : 2;              // 10-11 lcdif1 pix clock
    UINT32 ldb_di0_clk_enable : 2;                 // 12-13 ldb_di0 clock
    UINT32 qspi1_clk_enable : 2;                   // 14-15 qspi1 clock
    UINT32 reserved2 : 2;                          // 16-17
    UINT32 mlb_clk_enable : 2;                     // 18-19 mlb clock
    UINT32 mmdc_core_aclk_fast_core_p0_enable : 2; // 20-21 mmdc_core_aclk_fast_core_p0 clock
    UINT32 reserved3 : 2;                          // 22-23
    UINT32 mmdc_core_ipg_clk_p0_enable : 2;        // 24-25 mmdc_core_ipg_clk_p0 clock
    UINT32 mmdc_core_ipg_clk_p1_enable : 2;        // 26-27 mmdc_core_ipg_clk_p1 clock
    UINT32 ocram_clk_enable : 2;                   // 28-29 ocram clock
    UINT32 reserved4 : 2;                          // 30-31
  };
} IMX_CCM_CCGR3_REG;

typedef enum {
  IMX_CCM_PERCLK_CLK_SEL_IPG_CLK_ROOT = 0,
  IMX_CCM_PERCLK_CLK_SEL_OSC_CLK = 1
} IMX_CCM_PERCLK_CLK_SEL;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32  perclk_podf         : 6;  // 0-5 Divider for perclk podf.
    UINT32  perclk_clk_sel      : 1;  // 6 Selector for the perclk clock multiplexor
    UINT32  qspi1_sel           : 3;  // 7-9 QSPI1 clock select
    UINT32  ssi1_clk_sel        : 2;  // 10-11 Selector for ssi1 clock multiplexer
    UINT32  ssi2_clk_sel        : 2;  // 12-13 Selector for ssi2 clock multiplexer
    UINT32  ssi3_clk_sel        : 2;  // 14-15 Selector for ssi3 clock multiplexer
    UINT32  usdhc1_clk_sel      : 1;  // 16 Selector for usdhc1 clock multiplexer
    UINT32  usdhc2_clk_sel      : 1;  // 17 Selector for usdhc2 clock multiplexer
    UINT32  usdhc3_clk_sel      : 1;  // 18 Selector for usdhc3 clock multiplexe
    UINT32  usdhc4_clk_sel      : 1;  // 19 Selector for usdhc4 clock multiplexer
    UINT32  lcdif2_podf         : 3;  // 20-22 Post-divider for lcdif2 clock.
    UINT32  aclk_eim_slow_podf  : 3;  // 23-25 Divider for aclk_eim_slow clock root.
    UINT32  qspi1_podf          : 3;  // 26-28 Divider for QSPI1 clock root
    UINT32  aclk_eim_slow_sel   : 2;  // 29-30 Selector for aclk_eim_slow root clock
                                      //   multiplexer
    UINT32  reserved1           : 1;  // 31
  };
} IMX_CCM_CSCMR1_REG;

typedef struct {
  UINT32 CCR;           // 0x00 CCM Control Register
  UINT32 CCDR;          // 0x04 CCM Control Divider Register
  UINT32 CSR;           // 0x08 CCM Status Register
  UINT32 CCSR;          // 0x0C CCM Clock Switcher Register
  UINT32 CACRR;         // 0x10 CCM Arm Clock Root Register
  UINT32 CBCDR;         // 0x14 CCM Bus Clock Divider Register
  UINT32 CBCMR;         // 0x18 CCM Bus Clock Multiplexer Register
  UINT32 CSCMR1;        // 0x1C CCM Serial Clock Multiplexer Register 1
  UINT32 CSCMR2;        // 0x20 CCM Serial Clock Multiplexer Register 2
  UINT32 CSCDR1;        // 0x24 CCM Serial Clock Divider Register 1
  UINT32 CS1CDR;        // 0x28 CCM SSI1 Clock Divider Register
  UINT32 CS2CDR;        // 0x2C CCM SSI2 Clock Divider Register
  UINT32 CDCDR;         // 0x30 CCM D1 Clock Divider Register
  UINT32 CHSCCDR;       // 0x34 CCM HSC Clock Divider Register
  UINT32 CSCDR2;        // 0x38 CCM Serial Clock Divider Register 2
  UINT32 CSCDR3;        // 0x3C CCM Serial Clock Divider Register 3
  UINT32 reserved1;
  UINT32 CWDR;          // 0x44 CCM Wakeup Detector Register
  UINT32 CDHIPR;        // 0x48 CCM Divider Handshake In-Process Register
  UINT32 reserved2[2];
  UINT32 CLPCR;         // 0x54 CCM Low Power Control Register
  UINT32 CISR;          // 0x58 CCM Interrupt Status Register
  UINT32 CIMR;          // 0x5C CCM Interrupt Mask Register
  UINT32 CCOSR;         // 0x60 CCM Clock Output Source Register
  UINT32 CGPR;          // 0x64 CCM General Purpose Register
  UINT32 CCGR[7];       // 0x68-80 CCM Clock Gating Register 0-6
  UINT32 reserved3;
  UINT32 CMEOR;         // 0x88 CCM Module Enable Override Register
} IMX_CCM_REGISTERS;

// CCM Analog
typedef enum {
  IMX_PLL_BYPASS_CLK_SRC_REF_CLK_24M,
  IMX_PLL_BYPASS_CLK_SRC_CLK1,
  IMX_PLL_BYPASS_CLK_SRC_GPANAIO,     // Only for CCM_ANALOG_PLL_SYS
                                      //   and CCM_ANALOG_PLL_USB1n
  IMX_PLL_BYPASS_CLK_SRC_CHRG_DET_B   // Only for CCM_ANALOG_PLL_SYS
                                      //   and CCM_ANALOG_PLL_USB1n
} IMX_PLL_BYPASS_CLK_SRC;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 PFD0_FRAC : 6;     // 0-5 fractional divide value. The resulting
                              //   frequency shall be 528*18/PFD0_FRAC where
                              //   PFD0_FRAC is in the range 12-35.
    UINT32 PFD0_STABLE : 1;   // 6
    UINT32 PFD0_CLKGATE : 1;  // 7 Set to 1 to gate ref_pfd0
    UINT32 PFD1_FRAC : 6;     // 8-13 fractional divide value
    UINT32 PFD1_STABLE : 1;   // 14
    UINT32 PFD1_CLKGATE : 1;  // 15 Set to 1 to gate ref_pfd1
    UINT32 PFD2_FRAC : 6;     // 16-21 fractional divide value
    UINT32 PFD2_STABLE : 1;   // 22
    UINT32 PFD2_CLKGATE : 1;  // 23 Set to 1 to gate ref_pfd2
    UINT32 PFD3_FRAC : 6;     // 24-29 fractional divide value
    UINT32 PFD3_STABLE : 1;   // 30
    UINT32 PFD3_CLKGATE : 1;  // 31 Set to 1 to gate ref_pfd3
  };
} IMX_CCM_PFD_480_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 PFD0_FRAC : 6;     // 0-5 fractional divide value. The resulting
                              //   frequency shall be 528*18/PFD0_FRAC where
                              //   PFD0_FRAC is in the range 12-35.
    UINT32 PFD0_STABLE : 1;   // 6
    UINT32 PFD0_CLKGATE : 1;  // 7 Set to 1 to gate ref_pfd0
    UINT32 PFD1_FRAC : 6;     // 8-13 fractional divide value
    UINT32 PFD1_STABLE : 1;   // 14
    UINT32 PFD1_CLKGATE : 1;  // 15 Set to 1 to gate ref_pfd1
    UINT32 PFD2_FRAC : 6;     // 16-21 fractional divide value
    UINT32 PFD2_STABLE : 1;   // 22
    UINT32 PFD2_CLKGATE : 1;  // 23 Set to 1 to gate ref_pfd2
    UINT32 PFD3_FRAC : 6;     // 24-29 fractional divide value. The resulting
                              //   frequency shall be 528*18/PFD3_FRAC where
                              //   PFD3_FRAC is in the range 12-35
    UINT32 PFD3_STABLE : 1;   // 30
    UINT32 PFD3_CLKGATE : 1;  // 31 Set to 1 to gate ref_pfd3
  };
} IMX_CCM_PFD_528_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 DIV_SELECT : 7;      // 0-6  Valid range for divider value: 54-108.
                                //   Fout = Fin * div_select/2.0
    UINT32 reserved1 : 5;       // 7-11
    UINT32 POWERDOWN : 1;       // 12 Powers down the PLL.
    UINT32 ENABLE: 1;           // 13 Enable the clock output.
    UINT32 BYPASS_CLK_SRC : 2;  // 14-15 Determines the bypass and PLL reference
                                //   clock source.
    UINT32 BYPASS : 1;          // 16 Bypass the PLL.
    UINT32 LVDS_SEL : 1;        // 17 Analog Debug Bit
    UINT32 LVDS_24MHZ_SEL : 1;  // 18 Analog Debug Bit
    UINT32 reserved2 : 1;       // 19 PLL_SEL (Reserved)
    UINT32 reserved3 : 11;      // 20-30
    UINT32 LOCK : 1;            // 31 1 - PLL is currently locked. 0 - PLL is
                                //   not currently locked.
  };
} IMX_CCM_ANALOG_PLL_ARM_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 DIV_SELECT : 1;      // 0 0 - Fout=Fref*20; 1 - Fout=Fref*22.
    UINT32 reserved1 : 11;      // 1-11
    UINT32 POWERDOWN : 1;       // 12 Powers down the PLL.
    UINT32 ENABLE : 1;          // 13 Enable PLL output
    UINT32 BYPASS_CLK_SRC : 2;  // 14-15 Determines the bypass source.
    UINT32 BYPASS : 1;          // 16 Bypass the PLL.
    UINT32 reserved2 : 1;       // 17
    UINT32 PFD_OFFSET_EN : 1;   // 18 Enables an offset in the phase frequency detector
    UINT32 reserved3 : 12;      // 19-30
    UINT32 LOCK : 1;            // 31 1 - PLL is currently locked; 0 - PLL is
                                //   not currently locked.
  };
} IMX_CCM_ANALOG_PLL_SYS_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 DIV_SELECT : 2;      // 0-1 - Fout=Fref*20; 1 - Fout=Fref*22.
    UINT32 reserved1 : 4;       // 2-5
    UINT32 EN_USB_CLKS : 1;     // 6 Powers the 9-phase PLL outputs for USBPHYn
    UINT32 reserved2 : 5;       // 7-11
    UINT32 POWER : 1;           // 12 Powers up the PLL.
    UINT32 ENABLE : 1;          // 13 Enable the PLL clock output
    UINT32 BYPASS_CLK_SRC : 2;  // 14-15 Determines the bypass source
    UINT32 BYPASS : 1;          // 16 Bypass the PLL.
    UINT32 reserved3 : 14;      // 17-30
    UINT32 LOCK : 1;            // 31 1 - PLL is currently locked
  };
} IMX_CCM_ANALOG_PLL_USB1_REG;

typedef enum {
  IMX_POST_DIV_SELECT_DIVIDE_4,
  IMX_POST_DIV_SELECT_DIVIDE_2,
  IMX_POST_DIV_SELECT_DIVIDE_1,
} IMX_CCM_PLL_VIDEO_CTRL_POST_DIV_SELECT;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 DIV_SELECT : 7;        // 0-6 This field controls the PLL loop divider.
                                  //   Valid range for DIV_SELECT divider value: 27~54
    UINT32 Reserved1 : 5;         // 7-11
    UINT32 POWERDOWN : 1;         // 12 Powers down the PLL
    UINT32 ENABLE : 1;            // 13 Enalbe PLL output
    UINT32 BYPASS_CLK_SRC : 2;    // 14-15 Determines the bypass source
    UINT32 BYPASS : 1;            // 16 Bypass the PLL
    UINT32 Reserved2 : 1;         // 17
    UINT32 PFD_OFFSET_EN : 1;     // 18 Enables an offset in the phase frequency
                                  //   detector
    UINT32 POST_DIV_SELECT : 2;   // 19-20 These bits implement a divider after
                                  //   the PLL, but before the enable and bypass mux.
    UINT32 Reserved3 : 1;         // 21
    UINT32 Reserved4 : 9;         // 22-30 Always set to zero
    UINT32 LOCK : 1;              // 31 PLL is/not currently locked
  };
} IMX_CCM_PLL_VIDEO_CTRL_REG;

// Power Management Unit
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 REG0_TARG : 5;   // 0-4 target voltage for the ARM core power domain
    UINT32 reserved1 : 4;   // 5-8
    UINT32 REG1_TARG : 5;   // 9-13 target voltage for the VPU/GPU power domain
    UINT32 reserved2 : 4;   // 14-17
    UINT32 REG2_TARG : 5;   // 18-22 target voltage for the SOC power domain
    UINT32 reserved3 : 4;   // 23-26
    UINT32 RAMP_RATE : 2;   // 27-28 Regulator voltage ramp rate
    UINT32 FET_ODRIVE : 1;  // 29 increases the gate drive on power gating FET
    UINT32 reserved4 : 2;   // 30-31
  };
} IMX_PMU_REG_CORE_REG;

typedef enum {
  PLL_ENET_DIV_SELECT_25MHZ = 0,
  PLL_ENET_DIV_SELECT_50MHZ = 1,
  PLL_ENET_DIV_SELECT_100MHZ = 2,
  PLL_ENET_DIV_SELECT_125MHZ = 3,
} CCM_ANALOG_PLL_ENET_DIV_SELECT;

// CCM ANALOG PLL Ethernet(n) register
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 DIV_SELECT : 2;       // 0-1
    UINT32 Zero1 : 5;            // 2-6
    UINT32 Reserved1 : 5;        // 7-11
    UINT32 POWERDOWN : 1;        // 12
    UINT32 ENABLE : 1;           // 13
    UINT32 BYPASS_CLK_SRC : 2;   // 14-15
    UINT32 BYPASS : 1;           // 16
    UINT32 Reserved2 : 1;        // 17
    UINT32 PFD_OFFSET_EN : 1;    // 18
    UINT32 ENABLE_125M : 1;      // 19
    UINT32 ENABLE_100M : 1;      // 20
    UINT32 Zero2 : 10;           // 21-30
    UINT32 LOCK : 1;             // 31
  };
} IMX_CCM_ANALOG_PLL_ENET_REG;

// CCM ANALOG PLL Ethernet(n) register
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 LVDS1_CLK_SEL : 5;    // 0-4
    UINT32 LVDS2_CLK_SEL : 5;    // 5-9
    UINT32 LVDSCLK1_OBEN : 1;    // 10
    UINT32 LVDSCLK2_OBEN : 1;    // 11
    UINT32 LVDSCLK1_IBEN : 1;    // 12
    UINT32 LVDSCLK2_IBEN : 1;    // 13
    UINT32 Reserved0 : 15;       // 14-28
    UINT32 IRQ_TEMPSENSE : 1;    // 29
    UINT32 IRQ_ANA_BO : 1;       // 30
    UINT32 IRQ_DIG_BO : 1;       // 31
  };
} IMX_CCM_ANALOG_MISC1_REG;

typedef struct {
  UINT32 PLL_ARM;           // 0x000 Analog ARM PLL control Register
  UINT32 PLL_ARM_SET;       // 0x004 Analog ARM PLL control Register
  UINT32 PLL_ARM_CLR;       // 0x008 Analog ARM PLL control Register
  UINT32 PLL_ARM_TOG;       // 0x00C Analog ARM PLL control Register
  UINT32 PLL_USB1;          // 0x010 Analog USB1 480MHz PLL Control Register
  UINT32 PLL_USB1_SET;      // 0x014 Analog USB1 480MHz PLL Control Register
  UINT32 PLL_USB1_CLR;      // 0x018 Analog USB1 480MHz PLL Control Register
  UINT32 PLL_USB1_TOG;      // 0x01C Analog USB1 480MHz PLL Control Register
  UINT32 PLL_USB2;          // 0x020 Analog USB2 480MHz PLL Control Register
  UINT32 PLL_USB2_SET;      // 0x024 Analog USB2 480MHz PLL Control Register
  UINT32 PLL_USB2_CLR;      // 0x028 Analog USB2 480MHz PLL Control Register
  UINT32 PLL_USB2_TOG;      // 0x02C Analog USB2 480MHz PLL Control Register
  UINT32 PLL_SYS;           // 0x030 Analog System PLL Control Register
  UINT32 PLL_SYS_SET;       // 0x034 Analog System PLL Control Register
  UINT32 PLL_SYS_CLR;       // 0x038 Analog System PLL Control Register
  UINT32 PLL_SYS_TOG;       // 0x03C Analog System PLL Control Register
  UINT32 PLL_SYS_SS;        // 0x040 528MHz System PLL Spread Spectrum Register
  UINT32 reserved1[11];
  UINT32 PLL_AUDIO;         // 0x070 Analog Audio PLL control Register
  UINT32 PLL_AUDIO_SET;     // 0x074 Analog Audio PLL control Register
  UINT32 PLL_AUDIO_CLR;     // 0x078 Analog Audio PLL control Register
  UINT32 PLL_AUDIO_TOG;     // 0x07C Analog Audio PLL control Register
  UINT32 PLL_AUDIO_NUM;     // 0x080 Numerator of Audio PLL Fractional Loop Divider
                            //   Register
  UINT32 reserved2[3];
  UINT32 PLL_AUDIO_DENOM;   // 0x090 Denominator of Audio PLL Fractional Loop
                            //   Divider Register
  UINT32 reserved3[3];
  UINT32 PLL_VIDEO;         // 0x0A0 Analog Video PLL control Register
  UINT32 PLL_VIDEO_SET;     // 0x0A4 Analog Video PLL control Register
  UINT32 PLL_VIDEO_CLR;     // 0x0A8 Analog Video PLL control Register
  UINT32 PLL_VIDEO_TOG;     // 0x0AC Analog Video PLL control Register
  UINT32 PLL_VIDEO_NUM;     // 0x0B0 Numerator of Video PLL Fractional Loop Divider
                            //   Register
  UINT32 reserved4[3];
  UINT32 PLL_VIDEO_DENOM;   // 0x0C0 Denominator of Video PLL Fractional Loop
                            //   Divider Register
  UINT32 reserved5[7];
  UINT32 PLL_ENET;          // 0x0E0 Analog ENET PLL Control Register
  UINT32 PLL_ENET_SET;      // 0x0E4 Analog ENET PLL Control Register
  UINT32 PLL_ENET_CLR;      // 0x0E8 Analog ENET PLL Control Register
  UINT32 PLL_ENET_TOG;      // 0x0EC Analog ENET PLL Control Register
  UINT32 PFD_480;           // 0x0F0 480MHz Clock (PLL3) Phase Fractional Divider
                            //   Control Register
  UINT32 PFD_480_SET;       // 0x0F4 480MHz Clock (PLL3) Phase Fractional Divider
                            //   Control Register
  UINT32 PFD_480_CLR;       // 0x0F8 480MHz Clock (PLL3) Phase Fractional Divider
                            //   Control Register
  UINT32 PFD_480_TOG;       // 0x0FC 480MHz Clock (PLL3) Phase Fractional Divider
                            //   Control Register
  UINT32 PFD_528;           // 0x100 528MHz Clock (PLL2) Phase Fractional Divider
                            //   Control Register
  UINT32 PFD_528_SET;       // 0x104 528MHz Clock (PLL2) Phase Fractional Divider
                            //   Control Register
  UINT32 PFD_528_CLR;       // 0x108 528MHz Clock (PLL2) Phase Fractional Divider
                            //   Control Register
  UINT32 PFD_528_TOG;       // 0x10C 528MHz Clock (PLL2) Phase Fractional Divider
                            //   Control Register
  UINT32 reserved6[16];
  UINT32 MISC0;             // 0x150 Miscellaneous Register 0
  UINT32 MISC0_SET;         // 0x154 Miscellaneous Register 0
  UINT32 MISC0_CLR;         // 0x158 Miscellaneous Register 0
  UINT32 MISC0_TOG;         // 0x15C Miscellaneous Register 0
  UINT32 MISC1;             // 0x160 Miscellaneous Register 1
  UINT32 MISC1_SET;         // 0x164 Miscellaneous Register 1
  UINT32 MISC1_CLR;         // 0x168 Miscellaneous Register 1
  UINT32 MISC1_TOG;         // 0x16C Miscellaneous Register 1
  UINT32 MISC2;             // 0x170 Miscellaneous Register 2
  UINT32 MISC2_SET;         // 0x174 Miscellaneous Register 2
  UINT32 MISC2_CLR;         // 0x178 Miscellaneous Register 2
  UINT32 MISC2_TOG;         // 0x17C Miscellaneous Register 2
} IMX_CCM_ANALOG_REGISTERS;

// General Power Controller (GPC)
#define IMX_GPC_BASE 0x020DC000
#define IMX_GPC_LENGTH 0x1000

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 PCR : 1;         // 0 Power Control
    UINT32 reserved : 31;   // 1-31
  };
} IMX_GPC_PGC_PGCR_REG;

#define IMX_GPC_PGC_PUPSCR_SW_DEFAULT 1
#define IMX_GPC_PGC_PUPSCR_SW2ISO_DEFAULT 0xf

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 SW : 6;            // 0-5 number of IPG clock cycles before asserting
                              //   power toggle on/off signal (switch_b)
    UINT32 reserved1 : 2;     // 6-7
    UINT32 SW2ISO : 6;        // 8-13 IPG clock cycles before negating isolation
    UINT32 reserved2 : 18;    // 14-31
  };
} IMX_GPC_PGC_PUPSCR_REG;

#define IMX_GPC_PGC_PDNSCR_ISO_DEFAULT 1
#define IMX_GPC_PGC_PDNSCR_ISO2SW_DEFAULT 1

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 ISO : 6;         // 0-5 number of IPG clocks before isolation
    UINT32 reserved1 : 2;   // 6-7
    UINT32 ISO2SW : 6;      // 8-13 number of IPG clocks before negating power
                            //   toggle on/off signal (switch_b)
    UINT32 reserved2 : 18;  // 14-31
  };
} IMX_GPC_PGC_PDNSCR_REG;

typedef struct {
  UINT32 CTRL;      // 0x0 PGC Control Register (PGC_GPU/CPU_CTRL)
  UINT32 PUPSCR;    // 0x4 Power Up Sequence Control Register (PGC_GPU/CPU_PUPSCR)
  UINT32 PDNSCR;    // 0x8 Pull Down Sequence Control Register (PGC_GPU/CPU_PDNSCR)
  UINT32 SR;        // 0xC Power Gating Controller Status Register (PGC_GPU/CPU_SR)
} IMX_GPC_PGC_REGISTERS;

// General Power Controller (GPC)
typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 gpu_vpu_pdn_req : 1;   // 0 GPU/VPU Power Down request. Self-cleared bit.
    UINT32 gpu_vpu_pup_req : 1;   // 1 GPU/VPU Power Up request. Self-cleared bit.
    UINT32 MEGA_PDN_REQ : 1;      // 2 MEGA domain power down request. Self-clear bit.
    UINT32 MEGA_PUP_REQ : 1;      // 3 MEGA domain power up request. Self-clear bit.
    UINT32 DISPLAY_PDN_REQ : 1;   // 4 Display Power Down request. Self-cleared bit.
    UINT32 DISPLAY_PUP_REQ : 1;   // 5 Display Power Up request. Self-cleared bit.
    UINT32 PCIE_PHY_PDN_REQ : 1;  // 6 PCIE PHY power down request. Self-clear bit.
    UINT32 PCIE_PHY_PUP_REQ : 1;  // 7 PCIE PHY power up request. Self-clear bit.
    UINT32 reserved1 : 8;         // 8-15
    UINT32 DVFS0CR : 1;           // 16 DVFS0 (ARM) Change request (bit is read-only)
    UINT32 VADC_ANALOG_OFF : 1;   // 17 Indication to VADC whether the analog power
                                  //   to VADC is available or not
    UINT32 VADC_EXT_PWD_N : 1;    // 18 VADC power down bit
    UINT32 reserved2 : 2;         // 19-20
    UINT32 GPCIRQM : 1;           // 21 GPC interrupt/event masking
    UINT32 L2_PGE : 1;            // 22 L2 Cache Power Gate Enable
    UINT32 reserved3 : 9;         // 23-31
  };
} IMX_GPC_CNTR_REG;

typedef struct {
  UINT32 CNTR;                    // 0x000 GPC Interface control register (GPC_CNTR)
  UINT32 PGR;                     // 0x004 GPC Power Gating Register (GPC_PGR)
  UINT32 IMR1;                    // 0x008 IRQ masking register 1 (GPC_IMR1)
  UINT32 IMR2;                    // 0x00C IRQ masking register 2 (GPC_IMR2)
  UINT32 IMR3;                    // 0x010 IRQ masking register 3 (GPC_IMR3)
  UINT32 IMR4;                    // 0x014 IRQ masking register 4 (GPC_IMR4)
  UINT32 ISR1;                    // 0x018 IRQ status resister 1 (GPC_ISR1)
  UINT32 ISR2;                    // 0x01C IRQ status resister 2 (GPC_ISR2)
  UINT32 ISR3;                    // 0x020 IRQ status resister 3 (GPC_ISR3)
  UINT32 ISR4;                    // 0x024 IRQ status resister 4 (GPC_ISR4)
  UINT32 reserved1[142];
  IMX_GPC_PGC_REGISTERS PGC_GPU;  // 0x260-0x26C GPU PGC Control
  UINT32 reserved2[12];
  IMX_GPC_PGC_REGISTERS PGC_CPU;  // 0x2A0-0x2AC CPU PGC Control
} IMX_GPC_REGISTERS;

// Ethernet controller (ENET)
#define IMX_ENET_BASE 0x02188000
#define IMX_ENET_LENGTH 0x4000

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 RESET : 1;       // 0 Ethernet MAC Reset
    UINT32 ETHEREN : 1;     // 1 Ethernet Enable
    UINT32 MAGICEN : 1;     // 2 Magic Packet Detection Enable
    UINT32 SLEEP : 1;       // 3 Sleep Mode Enable
    UINT32 EN1588 : 1;      // 4 EN1588 Enable
    UINT32 SPEED : 1;       // 5 Selects between 10/100 and 1000 Mbps modes of operation
    UINT32 DBGEN : 1;       // 6 Debug Enable
    UINT32 reserved1 : 1;   // 7
    UINT32 DBSWP : 1;       // 8 Descriptor Byte Swapping Enable
    UINT32 SVLANEN : 1;     // 9 Enable additional detection of S-VLAN tag according
                            //   to IEEE802.1Q
    UINT32 VLANUSE2ND : 1;  // 10 VLAN use second tag
    UINT32 SVLANDBL : 1;    // 11 S-VLAN double tag
    UINT32 reserved2 : 4;   // 12-15 This field must be set to 0
    UINT32 TXC_DLY : 1;     // 16 Transmit clock delay
    UINT32 RXC_DLY : 1;     // 17 Receive clock delay
    UINT32 reserved3 : 14;  // 18-31 This field must be set to 01110000000000b = 0x1C00
  };
} IMX_ENET_ECR_REG;

#define IMX_ENET_ECR_REG_SET_RESERVED(ecrReg) (ecrReg)->reserved3 = 0x1C00

typedef struct {
  UINT32 reserved0;     // 0
  UINT32 EIR;           // 4
  UINT32 EIMR;          // 8
  UINT32 reserved1;     // Ch
  UINT32 RDAR;          // 10h
  UINT32 TDAR;          // 14h
  UINT32 reserved2[3];  // 18h - 20h
  UINT32 ECR;           // 24h Ethernet Control Register (ENET_ECR)
  UINT32 reserved3[6];  // 28h - 3Ch
  UINT32 MMFR;          // 40h
  UINT32 MSCR;          // 44h
  UINT32 reserved4[7];  // 48h - 60h
  UINT32 MIBC;          // 64h
  UINT32 reserved5[7];  //
  UINT32 RCR;           // 84h
  UINT32 reserved6[15]; //
  UINT32 TCR;           // C4h
  UINT32 reserved7[7];
  UINT32 PALR;          // E4h
  UINT32 PAUR;          // E8h
  UINT32 OPD;           // ECh
  UINT32 reserved8[322];
} IMX_ENET_REGISTERS;

// GPIO Controller (GPIO)
#define IMX_GPIO_BASE 0x0209C000
#define IMX_GPIO_LENGTH (7 * 0x4000)

// USB CORE (EHCI)
#define IMX_USBCORE_BASE 0x02184000
#define IMX_USBCORE_LENGTH 0x200
#define IMX_USBCMD_OFFSET 0x140
#define IMX_USBMODE_OFFSET 0x1A8

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 RS : 1;          // 0 Run/Stop (RS) . Read/Write. Default 0b. 1=Run. 0=Stop.
    UINT32 RST : 1;         // 1 Controller Reset (RESET) - Read/Write.
    UINT32 FS_1 : 2;        // 2-3 Frame List Size (Read/Write or Read Only). Default 000b.
    UINT32 PSE : 1;         // 4 Periodic Schedule Enable- Read/Write. Default 0b.
    UINT32 ASE : 1;         // 5 Asynchronous Schedule Enable Read/Write. Default 0b.
    UINT32 IAA : 1;         // 6 Interrupt on Async Advance Doorbell Read/Write.
    UINT32 reserved1 : 1;   // 7
    UINT32 ASP : 2;         // 8-9 Asynchronous Schedule Park Mode Count (OPTIONAL)
    UINT32 reserved2 : 1;   // 10
    UINT32 ASPE : 1;        // 11 Asynchronous Schedule Park Mode Enable (OPTIONAL)
    UINT32 ATDTW : 1;       // 12 Add dTD TripWire - Read/Write. [device mode only]
    UINT32 SUTW : 1;        // 13 Setup TripWire - Read/Write. [device mode only]
    UINT32 reserved3 : 1;   // 14
    UINT32 FS2 : 1;         // 15 Frame List Size [host mode only]
    UINT32 ITC : 8;         // 16-23 Interrupt Threshold Control Read/Write. Default 08h.
    UINT32 reserved : 8;    // 24-31
  };
} USB_USBCMD_REG;

typedef enum {
  IMX_USBMODE_IDLE = 0,
  IMX_USBMODE_DEVICE = 2,
  IMX_USBMODE_HOST = 3,
} IMX_USBMODE_CM;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 CM : 2;          // 0-1 Controller Mode.
    UINT32 ES : 1;          // 1 Endian Select (0- Little, 1-Big)
    UINT32 SLOM : 1;        // 3 Setup Lockout Mode
    UINT32 SDIS : 1;        // 4 Stream Disable Mode
    UINT32 reserved : 26;   // 5-31
  };
} USB_USBMODE_REG;

// USB Non-CORE
#define IMX_USBNONCORE_BASE 0x02184800
#define IMX_USBNONCORE_LENGTH 0x20

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 reserved1 : 7;       // 0-6
    UINT32 OVER_CUR_DIS : 1;    // 7 Disable Overcurrent Detection
    UINT32 OVER_CUR_POL : 1;    // 8 Polarity of Overcurrent (1-active low, 0-active high)
    UINT32 PWR_POL : 1;         // 9 Power Polarity (1-active high, 0-active low)
    UINT32 WIE : 1;             // 10 Wake-up Interrupt Enable
    UINT32 RESET : 1;           // 11 Force Host 1 UTMI PHY Reset.
    UINT32 SUSPENDM : 1;        // 12 Force Host 1 UTMI PHY Suspend.
    UINT32 UTMI_ON_CLOCK : 1;   // 13 Force UTMI PHY clock output on even if in
                                //   low-power suspend mode.
    UINT32 WKUP_SW_EN : 1;      // 14 Software Wake-up Enable
    UINT32 WKUP_SW : 1;         // 15 Software Wake-up
    UINT32 WKUP_ID_EN : 1;      // 16 Wake-up on ID change enable
    UINT32 WKUP_VBUS_EN : 1;    // 17 wake-up on VBUS change enable
    UINT32 reserved2 : 13;      // 18-30
    UINT32 WIR : 1;             // 31 Wake-up Interrupt Request
  };
} USBNC_USB_UH_CTRL_REG;

typedef struct {
  UINT32 USBNC_USB_OTG_CTRL;          // 0x00 USB OTG Control Register
  UINT32 USBNC_USB_UH1_CTRL;          // 0x04 USB Host1 Control Register
  UINT32 USBNC_USB_UH2_CTRL;          // 0x08 USB Host2 Control Register
  UINT32 USBNC_USB_UH3_CTRL;          // 0x0C USB Host3 Control Register
  UINT32 USBNC_USB_UH2_HSIC_CTRL;     // 0x10 USB Host2 HSIC Control Register
  UINT32 USBNC_USB_UH3_HSIC_CTRL;     // 0x14 USB Host3 HSIC Control Register
  UINT32 USBNC_USB_OTG_PHY_CTRL_0;    // 0x18 OTG UTMI PHY Control 0 Register
  UINT32 USBNC_USB_UH1_PHY_CTRL_0;    // 0x1C Host1 UTMI PHY Control 0 Register
} IMX_USBNONCORE_REGISTERS;

// USB PHY
#define IMX_USBPHY1_BASE 0x020C9000
#define IMX_USBPHY2_BASE 0x020CA000
#define IMX_USBPHY_LENGTH 0x1000

typedef enum {
  IMX_USBPHY0, // OTG
  IMX_USBPHY1,
  IMX_USBPHY_COUNT
} IMX_USBPHY_ID;

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
    UINT32 DEVPLUGIN_POLARITY : 1;    // 5 For device mode interrupt generation
                                      //   polarity
    UINT32 OTG_ID_CHG_IRQ : 1;        // 6 OTG ID change interrupt. Indicates
                                      //   the value of ID pin changed.
    UINT32 ENOTGIDDETECT : 1;         // 7 Enables circuit to detect resistance
                                      //   of MiniAB ID pin.
    UINT32 RESUMEIRQSTICKY : 1;       // 8 1 makes RESUME_IRQ bit a sticky bit.
    UINT32 ENIRQRESUMEDETECT : 1;     // 9 Enables interrupt for detection of a
                                      //   non-J state on the USB line.
    UINT32 RESUME_IRQ : 1;            // 10 Indicates that the host is sending a
                                      //   wake-up after suspend
    UINT32 ENIRQDEVPLUGIN : 1;        // 11 Enables interrupt for the detection
                                      //   of connectivity to the USB line.
    UINT32 DEVPLUGIN_IRQ : 1;         // 12 Indicates that the device is connected
    UINT32 DATA_ON_LRADC : 1;         // 13 Enables the LRADC to monitor USB_DP
                                      //   and USB_DM.
    UINT32 ENUTMILEVEL2 : 1;          // 14 Enables UTMI+ Level2.
    UINT32 ENUTMILEVEL3 : 1;          // 15 Enables UTMI+ Level3.
    UINT32 ENIRQWAKEUP : 1;           // 16 Enables interrupt for the wakeup events
    UINT32 WAKEUP_IRQ : 1;            // 17 Indicates that there is a wakeup event.
    UINT32 reserved1 : 1;             // 18 reserved
    UINT32 ENAUTOCLR_CLKGATE : 1;     // 19 Enables the feature to auto-clear
                                      //   the CLKGATE bit if there is wakeup
                                      //   event while USB is suspended.
    UINT32 ENAUTOCLR_PHY_PWD : 1;     // 20 Enables the feature to auto-clear
                                      //   the PWD register bits in USBPHYx_PWD
                                      //   if there is wakeup event while USB is
                                      //   suspended
    UINT32 ENDPDMCHG_WKUP : 1;        // 21 Enables the feature to wakeup USB if
                                      //   DP/DM is toggled when USB is suspended
    UINT32 ENIDCHG_WKUP : 1;          // 22 Enables the feature to wakeup USB if
                                      //   ID is toggled when USB is suspended
    UINT32 ENVBUSCHG_WKUP : 1;        // 23 Enables the feature to wakeup USB if
                                      //   VBUS is toggled when USB is suspended.
    UINT32 FSDLL_RST_EN : 1;          // 24 Enables the feature to reset the FSDLL
                                      //   lock detection logic at the end of
                                      //   each TX packet.
    UINT32 reserved2 : 2;             // 25-26
    UINT32 OTG_ID_VALUE : 1;          // 27
    UINT32 HOST_FORCE_LS_SE0 : 1;     // 28 Forces the next FS packet that is
                                      //   transmitted to have a EOP with LS timing.
    UINT32 UTMI_SUSPENDM : 1;         // 29 Used by the PHY to indicate a
                                      //   powered-down state.
    UINT32 CLKGATE : 1;               // 30 Gate UTMI Clocks. Clear to 0 to
                                      //   run clocks.
    UINT32 SFTRST : 1;                // 31 Soft-reset the USBPHYx_PWD,
                                      //   USBPHYx_TX, USBPHYx_RX, Set to 0 to
                                      //   release the PHY from reset.
  };
} USBPHYx_CTRL_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 STEP : 16; // 0-15 Read-only value for the stepping of the RTL version.
    UINT32 MINOR : 8; // 16-23 Read-only value for the MINOR field of the RTL version.
    UINT32 MAJOR : 8; // 24-31 Read-only value for the MAJOR field of the RTL version
  };
} USBPHYx_VERSION_REG;

typedef struct {
  UINT32 USBPHY_PWD;             // 0x00 USB PHY Power-Down Register
  UINT32 USBPHY_PWD_SET;         // 0x04 USB PHY Power-Down Register
  UINT32 USBPHY_PWD_CLR;         // 0x08 USB PHY Power-Down Register
  UINT32 USBPHY_PWD_TOG;         // 0x0C USB PHY Power-Down Register
  UINT32 USBPHY_TX;              // 0x10 USB PHY Transmitter Control Register
  UINT32 USBPHY_TX_SET;          // 0x14 USB PHY Transmitter Control Register
  UINT32 USBPHY_TX_CLR;          // 0x18 USB PHY Transmitter Control Register
  UINT32 USBPHY_TX_TOG;          // 0x1C USB PHY Transmitter Control Register
  UINT32 USBPHY_RX;              // 0x20 USB PHY Receiver Control Register
  UINT32 USBPHY_RX_SET;          // 0x24 USB PHY Receiver Control Register
  UINT32 USBPHY_RX_CLR;          // 0x28 USB PHY Receiver Control Register
  UINT32 USBPHY_RX_TOG;          // 0x2C USB PHY Receiver Control Register
  UINT32 USBPHY_CTRL;            // 0x30 USB PHY General Control Register
  UINT32 USBPHY_CTRL_SET;        // 0x34 USB PHY General Control Register
  UINT32 USBPHY_CTRL_CLR;        // 0x38 USB PHY General Control Register
  UINT32 USBPHY_CTRL_TOG;        // 0x3C USB PHY General Control Register
  UINT32 USBPHY_STATUS;          // 0x40 USB PHY Status Register
  UINT32 reserved1[3];
  UINT32 USBPHY_DEBUG;           // 0x50 USB PHY Debug Register
  UINT32 USBPHY_DEBUG_SET;       // 0x54 USB PHY Debug Register
  UINT32 USBPHY_DEBUG_CLR;       // 0x58 USB PHY Debug Register
  UINT32 USBPHY_DEBUG_TOG;       // 0x5C USB PHY Debug Register
  UINT32 USBPHY_DEBUG0_STATUS;   // 0x60 UTMI Debug Status Register 0
  UINT32 reserved2[3];
  UINT32 USBPHY_DEBUG1;          // 0x70 UTMI Debug Status Register 1
  UINT32 USBPHY_DEBUG1_SET;      // 0x74 UTMI Debug Status Register 1
  UINT32 USBPHY_DEBUG1_CLR;      // 0x78 UTMI Debug Status Register 1
  UINT32 USBPHY_DEBUG1_TOG;      // 0x7C UTMI Debug Status Register 1
  UINT32 USBPHY_VERSION;         // 0x80 UTMI RTL Version
} IMX_USBPHY_REGISTERS;

// USB Analog
#define IMX_USBANA_BASE 0x020C81A0

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 reserved1 : 18;  // 0-17
    UINT32 CHK_CONTACT : 1; // 18
    UINT32 CHK_CHRG_B : 1;  // 19
    UINT32 EN_B : 1;        // 20
    UINT32 reserved2 : 11;  // 21-31
  };
} USB_ANALOG_USB_CHRG_DETECT_REG;

typedef union {
  UINT32 AsUint32;
  struct {
    UINT32 HS_USE_EXTERNAL_R : 1;   // 0 Use external resistor to generate the
                                    //   current bias for the high speed transmitter.
    UINT32 EN_DEGLITCH : 1;         // 1 Enable the deglitching circuit of the
                                    //   USB PLL output.
    UINT32 reserved1 : 28;          // 2-29
    UINT32 EN_CLK_UTMI : 1;         // 30 Enables the clk to the UTMI block.
    UINT32 reserved2 : 1;           // 31
  };
} USB_ANALOG_USB_MISC_REG;

typedef struct {
  UINT32 USB_ANALOG_USB_VBUS_DETECT;       // 0x00 USB VBUS Detect Register
  UINT32 USB_ANALOG_USB_VBUS_DETECT_SET;   // 0x04 USB VBUS Detect Register
  UINT32 USB_ANALOG_USB_VBUS_DETECT_CLR;   // 0x08 USB VBUS Detect Register
  UINT32 USB_ANALOG_USB_VBUS_DETECT_TOG;   // 0x0C USB VBUS Detect Register
  UINT32 USB_ANALOG_USB_CHRG_DETECT;       // 0x10 USB Charger Detect Register
  UINT32 USB_ANALOG_USB_CHRG_DETECT_SET;   // 0x14 USB Charger Detect Register
  UINT32 USB_ANALOG_USB_CHRG_DETECT_CLR;   // 0x18 USB Charger Detect Register
  UINT32 USB_ANALOG_USB_CHRG_DETECT_TOG;   // 0x1C USB Charger Detect Register
  UINT32 USB_ANALOG_USB_VBUS_DETECT_STAT;  // 0x20 USB VBUS Detect Status Register
  UINT32 reserved1[3];
  UINT32 USB_ANALOG_USB_CHRG_DETECT_STAT;  // 0x30 USB Charger Detect Status Register
  UINT32 reserved2[7];
  UINT32 USB_ANALOG_USB_MISC;              // 0x50 USB Misc Register
  UINT32 USB_ANALOG_USB_MISC_SET;          // 0x54 USB Misc Register
  UINT32 USB_ANALOG_USB_MISC_CLR;          // 0x58 USB Misc Register
  UINT32 USB_ANALOG_USB_MISC_TOG;          // 0x5C USB Misc Register
} IMX_USBANA_USB_REGISTERS;

typedef struct {
  IMX_USBANA_USB_REGISTERS USBANA[IMX_USBPHY_COUNT];
  UINT32 USB_ANALOG_DIGPROG;                // 0xC0 Chip Silicon Version
} IMX_USBANA_REGISTERS;

#pragma pack(pop)

#endif // __IMX6_SX_H__
