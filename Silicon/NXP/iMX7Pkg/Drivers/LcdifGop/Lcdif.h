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

#ifndef _LCDIF_H_
#define _LCDIF_H_

EFI_STATUS
LcdifConfigureDisplay (
    IMX_DISPLAY_TIMING* Timing,
    VOID* FrameBuffer
    );

#define IMX_LCDIF_OFFSET_RL                 0x0000
#define IMX_LCDIF_OFFSET_RL_SET             0x0004
#define IMX_LCDIF_OFFSET_RL_CLR             0x0008
#define IMX_LCDIF_OFFSET_RL_TOG             0x000C
#define IMX_LCDIF_OFFSET_CTRL1              0x0010
#define IMX_LCDIF_OFFSET_CTRL1_SET          0x0014
#define IMX_LCDIF_OFFSET_CTRL1_CLR          0x0018
#define IMX_LCDIF_OFFSET_CTRL1_TOG          0x001C
#define IMX_LCDIF_OFFSET_CTRL2              0x0020
#define IMX_LCDIF_OFFSET_CTRL2_SET          0x0024
#define IMX_LCDIF_OFFSET_CTRL2_CLR          0x0028
#define IMX_LCDIF_OFFSET_CTRL2_TOG          0x002C
#define IMX_LCDIF_OFFSET_TRANSFER_COUNT     0x0030
#define IMX_LCDIF_OFFSET_CUR_BUF            0x0040
#define IMX_LCDIF_OFFSET_NEXT_BUF           0x0050
#define IMX_LCDIF_OFFSET_TIMING             0x0060
#define IMX_LCDIF_OFFSET_VDCTRL0            0x0070
#define IMX_LCDIF_OFFSET_VDCTRL0_SET        0x0074
#define IMX_LCDIF_OFFSET_VDCTRL0_CLR        0x0078
#define IMX_LCDIF_OFFSET_VDCTRL0_TOG        0x007C
#define IMX_LCDIF_OFFSET_VDCTRL1            0x0080
#define IMX_LCDIF_OFFSET_VDCTRL2            0x0090
#define IMX_LCDIF_OFFSET_VDCTRL3            0x00A0
#define IMX_LCDIF_OFFSET_VDCTRL4            0x00B0
#define IMX_LCDIF_OFFSET_DVICTRL0           0x00C0
#define IMX_LCDIF_OFFSET_DVICTRL1           0x00D0
#define IMX_LCDIF_OFFSET_DVICTRL2           0x00E0
#define IMX_LCDIF_OFFSET_DVICTRL3           0x00F0
#define IMX_LCDIF_OFFSET_DVICTRL4           0x0100
#define IMX_LCDIF_OFFSET_CSC_COEFF0         0x0110
#define IMX_LCDIF_OFFSET_CSC_COEFF1         0x0120
#define IMX_LCDIF_OFFSET_CSC_COEFF2         0x0130
#define IMX_LCDIF_OFFSET_CSC_COEFF3         0x0140
#define IMX_LCDIF_OFFSET_CSC_COEFF4         0x0150
#define IMX_LCDIF_OFFSET_CSC_OFFSET         0x0160
#define IMX_LCDIF_OFFSET_CSC_LIMIT          0x0170
#define IMX_LCDIF_OFFSET_DATA               0x0180
#define IMX_LCDIF_OFFSET_BM_ERROR_STAT      0x0190
#define IMX_LCDIF_OFFSET_CRC_STAT           0x01A0
#define IMX_LCDIF_OFFSET_STAT               0x01B0
#define IMX_LCDIF_OFFSET_VERSION            0x01C0
#define IMX_LCDIF_OFFSET_DEBUG0             0x01D0
#define IMX_LCDIF_OFFSET_DEBUG1             0x01E0
#define IMX_LCDIF_OFFSET_DEBUG2             0x01F0
#define IMX_LCDIF_OFFSET_THRES              0x0200
#define IMX_LCDIF_OFFSET_AS_CTRL            0x0210
#define IMX_LCDIF_OFFSET_AS_BUF             0x0220
#define IMX_LCDIF_OFFSET_AS_NEXT_BUF        0x0230
#define IMX_LCDIF_OFFSET_AS_CLRKEYLOW       0x0240
#define IMX_LCDIF_OFFSET_AS_CLRKEYHIGH      0x0250
#define IMX_LCDIF_OFFSET_SYNC_DELAY         0x0260
#define IMX_LCDIF_OFFSET_DEBUG3             0x0270
#define IMX_LCDIF_OFFSET_DEBUG4             0x0280
#define IMX_LCDIF_OFFSET_DEBUG5             0x0290

#define IMX_LCDIF_RL_SFTRST                      0x80000000
#define IMX_LCDIF_RL_CLKGATE                     0x40000000
#define IMX_LCDIF_RL_YCBCR422_INPUT              0x20000000
#define IMX_LCDIF_RL_READ_WRITEB                 0x10000000
#define IMX_LCDIF_RL_WAIT_FOR_VSYNC_EDGE         0x08000000
#define IMX_LCDIF_RL_DATA_SHIFT_DIR_LEFT         0x00000000
#define IMX_LCDIF_RL_DATA_SHIFT_DIR_RIGHT        0x04000000
#define IMX_LCDIF_RL_SHIFT_NUM_BITS_MASK         0x03E00000
#define IMX_LCDIF_RL_SHIFT_NUM_BITS_SHL          21
#define IMX_LCDIF_RL_DVI_MODE                    0x00100000
#define IMX_LCDIF_RL_BYPASS_COUNT                0x00080000
#define IMX_LCDIF_RL_VSYNC_MODE                  0x00040000
#define IMX_LCDIF_RL_DOTCLK_MODE                 0x00020000
#define IMX_LCDIF_RL_DATA_SELECT_CMD_MODE        0x00000000
#define IMX_LCDIF_RL_DATA_SELECT_DATA_MODE       0x00010000
#define IMX_LCDIF_RL_INPUT_DATA_SWIZZLE_MASK     0x0000C000
#define IMX_LCDIF_RL_INPUT_DATA_SWIZZLE_NONE     0x00000000
#define IMX_LCDIF_RL_INPUT_DATA_SWIZZLE_LE       0x00000000
#define IMX_LCDIF_RL_INPUT_DATA_SWIZZLE_BE       0x00004000
#define IMX_LCDIF_RL_INPUT_DATA_SWIZZLE_SWAP     0x00004000
#define IMX_LCDIF_RL_INPUT_DATA_SWIZZLE_HSWAP    0x00008000
#define IMX_LCDIF_RL_INPUT_DATA_SWIZZLE_HBSWAP   0x0000C000
#define IMX_LCDIF_RL_CSC_DATA_SWIZZLE_MASK       0x00003000
#define IMX_LCDIF_RL_CSC_DATA_SWIZZLE_NONE       0x00000000
#define IMX_LCDIF_RL_CSC_DATA_SWIZZLE_LE         0x00000000
#define IMX_LCDIF_RL_CSC_DATA_SWIZZLE_BE         0x00001000
#define IMX_LCDIF_RL_CSC_DATA_SWIZZLE_SWAP       0x00001000
#define IMX_LCDIF_RL_CSC_DATA_SWIZZLE_HSWAP      0x00002000
#define IMX_LCDIF_RL_CSC_DATA_SWIZZLE_HBSWAP     0x00003000
#define IMX_LCDIF_RL_LCD_DATABUS_WIDTH_MASK      0x00000C00
#define IMX_LCDIF_RL_LCD_DATABUS_WIDTH_16        0x00000000
#define IMX_LCDIF_RL_LCD_DATABUS_WIDTH_8         0x00000400
#define IMX_LCDIF_RL_LCD_DATABUS_WIDTH_18        0x00000800
#define IMX_LCDIF_RL_LCD_DATABUS_WIDTH_24        0x00000C00
#define IMX_LCDIF_RL_WORD_LENGTH_MASK            0x00000300
#define IMX_LCDIF_RL_WORD_LENGTH_16              0x00000000
#define IMX_LCDIF_RL_WORD_LENGTH_8               0x00000100
#define IMX_LCDIF_RL_WORD_LENGTH_18              0x00000200
#define IMX_LCDIF_RL_WORD_LENGTH_24              0x00000300
#define IMX_LCDIF_RL_RGB_TO_YCBCR422_CSC         0x00000080
#define IMX_LCDIF_RL_ENABLE_PXP_HANDSHAKE        0x00000040
#define IMX_LCDIF_RL_MASTER                      0x00000020
#define IMX_LCDIF_RL_DATA_FORMAT_16_BIT          0x00000008
#define IMX_LCDIF_RL_DATA_FORMAT_18_BIT          0x00000004
#define IMX_LCDIF_RL_DATA_FORMAT_24_BIT          0x00000002
#define IMX_LCDIF_RL_RUN                         0x00000001

#define IMX_LCDIF_CTRL1_COMBINE_MPU_WR_STRB      0x08000000
#define IMX_LCDIF_CTRL1_BM_ERROR_IRQ_EN          0x04000000
#define IMX_LCDIF_CTRL1_BM_ERROR_IRQ             0x02000000
#define IMX_LCDIF_CTRL1_RECOVER_ON_UNDERFLOW     0x01000000
#define IMX_LCDIF_CTRL1_INTERLACE_FIELDS         0x00800000
#define IMX_LCDIF_CTRL1_START_SECOND_FIELD       0x00400000
#define IMX_LCDIF_CTRL1_FIFO_CLEAR               0x00200000
#define IMX_LCDIF_CTRL1_IRQ_ON_ALT_FIELDS        0x00100000
#define IMX_LCDIF_CTRL1_BYTE_PACKING_FORMAT_MASK 0x000F0000
#define IMX_LCDIF_CTRL1_BYTE_PACKING_FORMAT_SHL  16
#define IMX_LCDIF_CTRL1_OVERFLOW_IRQ_EN          0x00008000
#define IMX_LCDIF_CTRL1_UNDERFLOW_IRQ_EN         0x00004000
#define IMX_LCDIF_CTRL1_CUR_FRAME_DONE_IRQ_EN    0x00002000
#define IMX_LCDIF_CTRL1_VSYNC_EDGE_IRQ_EN        0x00001000
#define IMX_LCDIF_CTRL1_OVERFLOW_IRQ             0x00000800
#define IMX_LCDIF_CTRL1_UNDERFLOW_IRQ            0x00000400
#define IMX_LCDIF_CTRL1_CUR_FRAME_DONE_IRQ       0x00000200
#define IMX_LCDIF_CTRL1_VSYNC_EDGE_IRQ           0x00000100
#define IMX_LCDIF_CTRL1_BUSY_ENABLE              0x00000004
#define IMX_LCDIF_CTRL1_MODE86                   0x00000002
#define IMX_LCDIF_CTRL1_RESET                    0x00000001

#define IMX_LCDIF_TRANSFER_COUNT_H_MASK          0x0000FFFF
#define IMX_LCDIF_TRANSFER_COUNT_V_MASK          0xFFFF0000
#define IMX_LCDIF_TRANSFER_COUNT_V_SHL           16

#define IMX_LCDIF_VDCTRL0_VSYNC_OEB              0x20000000
#define IMX_LCDIF_VDCTRL0_ENABLE_PRESENT         0x10000000
#define IMX_LCDIF_VDCTRL0_VSYNC_POL              0x08000000
#define IMX_LCDIF_VDCTRL0_HSYNC_POL              0x04000000
#define IMX_LCDIF_VDCTRL0_DOTCLK_POL             0x02000000
#define IMX_LCDIF_VDCTRL0_ENABLE_POL             0x01000000
#define IMX_LCDIF_VDCTRL0_VSYNC_PERIOD_UNIT      0x00200000
#define IMX_LCDIF_VDCTRL0_VSYNC_PULSE_UNIT       0x00100000
#define IMX_LCDIF_VDCTRL0_HALF_LINE              0x00080000
#define IMX_LCDIF_VDCTRL0_HALF_LINE_MODE         0x00040000
#define IMX_LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_MASK 0x0003FFFF

#define IMX_LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH_MASK 0xFFFF0000
#define IMX_LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH_SHL  18
#define IMX_LCDIF_VDCTRL2_HSYNC_PERIOD_MASK      0x0000FFFF

#define IMX_LCDIF_VDCTRL3_MUX_SYNC_SIGNALS       0x20000000
#define IMX_LCDIF_VDCTRL3_VSYNC_ONLY             0x10000000
#define IMX_LCDIF_VDCTRL3_HORZ_WAIT_CNT_MASK     0x0FFF0000
#define IMX_LCDIF_VDCTRL3_HORZ_WAIT_CNT_SHL      16
#define IMX_LCDIF_VDCTRL3_VERT_WAIT_CNT_MASK     0x0000FFFF

#define IMX_LCDIF_VDCTRL4_DOTCLK_DELAY_SEL_MASK  0xE0000000
#define IMX_LCDIF_VDCTRL4_DOTCLK_DELAY_SEL_SHL   29
#define IMX_LCDIF_VDCTRL4_DOTCLK_MODE_ON         0x00020000
#define IMX_LCDIF_VDCTRL4_SYNC_SIGNALS_ON        0x00040000
#define IMX_LCDIF_VDCTRL4_WAIT_FOR_VSYNC_EDGE_ON 0x08000000
#define IMX_LCDIF_VDCTRL4_DOTCLK_HVALID_DATA_CNT_MASK 0x0003FFFF

#pragma pack(push, 1)

typedef union {
    UINT32 AsUint32;
    struct {
        UINT32 RSRVD0 : 1;                          // 0 Reserved
        UINT32 INITIAL_DUMMY_READ : 3;              // 1-3 The value in this field determines the number of dummy 8/16/18/24-bit sub words
        UINT32 READ_MODE_NUM_PACKED_SUBWORDS : 3;   // 4-6 Indicates the number of valid 8/16/18/24-bit sub words that will be packed into the 32-bit word in read mode.
        UINT32 RSRVD1 : 1;                          // 7 Reserved
        UINT32 READ_MODE_6_BIT_INPUT : 1;           // 8 Setting this bit to 1 indicates to eLCDIF that even though LCD_DATABUS_WIDTH is set to 8 bits
        UINT32 READ_MODE_OUTPUT_IN_RGB_FORMAT : 1;  // 9 Setting this bit will enable the eLCDIF to convert the incoming data to the RGB format given by WORD_LENGTH bit field.
        UINT32 READ_PACK_DIR : 1;                   // 10 The default value of 0 indicates data is stored in the little endian format.
        UINT32 RSRVD2 : 1;                          // 11 Reserved
        UINT32 EVEN_LINE_PATTERN : 3;               // 12-14 Watchdog Time-out Field
        UINT32 RSRVD3 : 1;                          // 15 Reserved
        UINT32 ODD_LINE_PATTERN : 3;                // 16-18 This field determines the order of the RGB components of each pixel in ODD lines
        UINT32 RSRVD4 : 1;                          // 19 Reserved
        UINT32 BURST_LEN_8 : 1;                     // 20 By default, when the eLCDIF is in the bus master mode, it will issue AXI bursts of length 16 (except when in packed 24 BPP mode, it will issue bursts of length 15).When this bit is set to 1, the block will issue bursts of length 8
        UINT32 OUTSTANDING_REQS : 3;                // 21-23 This bit field indicates the maximum number of outstanding transactions that
        UINT32 RSRVD5 : 8;                          // 24-31 Reserved
    };
} LCDIFx_CTRL2_REG;

#pragma pack(pop)

#endif // _LCDIF_H_

