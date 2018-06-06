/** @file
*
*  Common IMX definitions
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
**/

// IMX alternate settings codes
#define IMX_ALT0 0x0
#define IMX_ALT1 0x1
#define IMX_ALT2 0x2
#define IMX_ALT3 0x3
#define IMX_ALT4 0x4
#define IMX_ALT5 0x5
#define IMX_ALT6 0x6
#define IMX_ALT7 0x7

// Vendor Macro defines for MsftFunctionConfig
#define MSFT_UUID 0x00,0x60,0x44,0xd5,0xf3,0x1f,0x11,0x60,0x4a,0xb8,0xb0,0x9c,0x2d,0x23,0x30,0xdd,0x2f
#define MSFT_FUNCTION_CONFIG 0x8d
#define RESOURCEPRODUCER_EXCLUSIVE 0x00,0x00
#define RESOURCEPRODUCER_SHARED 0x01,0x00
#define RESOURCECONSUMER_EXCLUSIVE 0x10,0x00
#define RESOURCECONSUMER_SHARED 0x11,0x00
#define PULL_DEFAULT 0x0
#define PULL_UP 0x1
#define PULL_DOWN 0x2
#define PULL_NONE 0x3
#define PIN_TABLE_OFFSET 0x12,0x00
#define SB_GPIO 0x5c,0x5f,0x53,0x42,0x2e,0x47,0x50,0x49,0x4f,0x00 // \_SB.GPIO in ASCII

// IMX SDMA request lines.
// These are logical values, the mapping to the SOC
// actual DMA request lines are done in the HAL extension.
#define IMX7_SDMA_REQ_ECSPI1_RX 0
#define IMX7_SDMA_REQ_ECSPI1_TX 1
#define IMX7_SDMA_REQ_ECSPI2_RX 2
#define IMX7_SDMA_REQ_ECSPI2_TX 3
#define IMX7_SDMA_REQ_ECSPI3_RX 4
#define IMX7_SDMA_REQ_ECSPI3_TX 5
#define IMX7_SDMA_REQ_ECSPI4_RX 6
#define IMX7_SDMA_REQ_ECSPI4_TX 7
#define IMX7_SDMA_REQ_SAI1_RX 8
#define IMX7_SDMA_REQ_SAI1_TX 9
#define IMX7_SDMA_REQ_SAI2_RX 10
#define IMX7_SDMA_REQ_SAI2_TX 11
#define IMX7_SDMA_REQ_SAI3_RX 12
#define IMX7_SDMA_REQ_SAI3_TX 13
#define IMX7_SDMA_REQ_IOMUX1 14
#define IMX7_SDMA_REQ_IOMUX2 15
#define IMX7_SDMA_REQ_ADC1 16
#define IMX7_SDMA_REQ_ADC2 17
#define IMX7_SDMA_REQ_I2C1 18
#define IMX7_SDMA_REQ_SIM1 19
#define IMX7_SDMA_REQ_I2C2 20
#define IMX7_SDMA_REQ_SIM2 21
#define IMX7_SDMA_REQ_I2C3 22
#define IMX7_SDMA_REQ_SIM3 23
#define IMX7_SDMA_REQ_I2C4 24
#define IMX7_SDMA_REQ_SIM4 25
#define IMX7_SDMA_REQ_UART1_RX 26
#define IMX7_SDMA_REQ_UART1_TX 27
#define IMX7_SDMA_REQ_UART2_RX 28
#define IMX7_SDMA_REQ_UART2_TX 29
#define IMX7_SDMA_REQ_UART3_RX 30
#define IMX7_SDMA_REQ_UART3_TX 31
#define IMX7_SDMA_REQ_UART4_RX 32
#define IMX7_SDMA_REQ_UART4_TX 33
#define IMX7_SDMA_REQ_UART5_RX 34
#define IMX7_SDMA_REQ_UART5_TX 35
#define IMX7_SDMA_REQ_UART6_RX 36
#define IMX7_SDMA_REQ_UART6_TX 37
#define IMX7_SDMA_REQ_UART7_RX 38
#define IMX7_SDMA_REQ_UART7_TX 39
#define IMX7_SDMA_REQ_QSPI1_TX 40
#define IMX7_SDMA_REQ_QSPI1_RX 41
#define IMX7_SDMA_REQ_GPT1 42
#define IMX7_SDMA_REQ_GPT2 43
#define IMX7_SDMA_REQ_GPT3 44
#define IMX7_SDMA_REQ_GPT4 45
#define IMX7_SDMA_REQ_FM1 46
#define IMX7_SDMA_REQ_FM2 47
#define IMX7_SDMA_REQ_PXP 48
#define IMX7_SDMA_REQ_CSI 49
#define IMX7_SDMA_REQ_LCDIF 50
#define IMX7_SDMA_REQ_ENET1 51
#define IMX7_SDMA_REQ_ENET2 52
#define IMX7_SDMA_REQ_ENET1_ENET2 53
