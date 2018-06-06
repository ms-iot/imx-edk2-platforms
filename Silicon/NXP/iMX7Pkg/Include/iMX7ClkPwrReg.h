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

#ifndef _IMX7_CLK_PWR_REG_H_
#define _IMX7_CLK_PWR_REG_H_

//
// Helper macro
//
#ifndef ARRAYSIZE
#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

//
// 24MHz clock from XTAL
//
#define IMX_REF_CLK_24M_FREQ 24000000

//
// Generic reg, set, clear and toggle offset
//
#define IMX_CCM_REG_OFFSET       0x00
#define IMX_CCM_SET_OFFSET       0x04
#define IMX_CCM_CLR_OFFSET       0x08
#define IMX_CCM_TOGGLE_OFFSET    0x0C

//
// It looks like CCM base address is the same for Solo and Duo so no need to
// to use PCD variable
//
#define IMX_CCM_BASE 0x30380000

#define IMX_CCM_PLL_CTRL(a)                 (a < 32) ? IMX_CCM_BASE + 0x0800 + (0x10 * a) : IMX_CCM_BASE + 0x0A00
#define IMX_CCM_CCGR(a)                     IMX_CCM_BASE + 0x4000 + (0x10 * a)
#define IMX_CCM_TARGET_ROOT(a)              IMX_CCM_BASE + 0x8000 + (0x80 * a)

typedef enum {
    IMX_CCM_CCGR_GATE_SETTING_NOT_NEEDED = 0x00,
    IMX_CCM_CCGR_GATE_RUN = 0x01,
    IMX_CCM_CCGR_GATE_RUN_WAIT = 0x02,
    IMX_CCM_CCGR_GATE_ALL_THE_TIME = 0x03,
} IMX_CCM_CCGR_GATE_SETTING;

#define IMX_CCM_ANALOG_BASE 0x30360000

#define IMX_CCM_ANALOG_PLL_ARM_OFFSET		    0x60
#define IMX_CCM_ANALOG_PLL_DDR_OFFSET		    0x70
#define IMX_CCM_ANALOG_PLL_DDR_SS_OFFSET	    0x80
#define IMX_CCM_ANALOG_PLL_DDR_NUM_OFFSET	    0x90
#define IMX_CCM_ANALOG_PLL_DDR_DENOM_OFFSET	    0xa0
#define IMX_CCM_ANALOG_PLL_480_OFFSET		    0xb0
#define IMX_CCM_ANALOG_PLL_ENET_OFFSET		    0xe0
#define IMX_CCM_ANALOG_PLL_AUDIO_OFFSET	        0xf0
#define IMX_CCM_ANALOG_PLL_AUDIO_SS_OFFSET	    0x100
#define IMX_CCM_ANALOG_PLL_AUDIO_NUM_OFFSET	    0x110
#define IMX_CCM_ANALOG_PLL_AUDIO_DENOM_OFFSET	0x120
#define IMX_CCM_ANALOG_PLL_VIDEO_OFFSET	        0x130
#define IMX_CCM_ANALOG_PLL_VIDEO_SS_OFFSET	    0x140
#define IMX_CCM_ANALOG_PLL_VIDEO_NUM_OFFSET	    0x150
#define IMX_CCM_ANALOG_PLL_VIDEO_DENOM_OFFSET	0x160

#define IMX_CCM_ANALOG_PLL_ARM          IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_ARM_OFFSET
#define IMX_CCM_ANALOG_PLL_DDR          IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_DDR_OFFSET
#define IMX_CCM_ANALOG_PLL_DDR_SS       IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_DDR_SS_OFFSET
#define IMX_CCM_ANALOG_PLL_DDR_NUM      IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_DDR_NUM_OFFSET
#define IMX_CCM_ANALOG_PLL_DDR_DENOM    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_DDR_DENOM_OFFSET
#define IMX_CCM_ANALOG_PLL_480          IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_480_OFFSET
#define IMX_CCM_ANALOG_PLL_ENET         IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_ENET_OFFSET
#define IMX_CCM_ANALOG_PLL_AUDIO        IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_AUDIO_OFFSET
#define IMX_CCM_ANALOG_PLL_AUDIO_SS     IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_AUDIO_SS_OFFSET
#define IMX_CCM_ANALOG_PLL_AUDIO_NUM    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_AUDIO_NUM_OFFSET
#define IMX_CCM_ANALOG_PLL_AUDIO_DENOM  IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_AUDIO_DENOM_OFFSET
#define IMX_CCM_ANALOG_PLL_VIDEO        IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_VIDEO_OFFSET
#define IMX_CCM_ANALOG_PLL_VIDEO_SS     IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_VIDEO_SS_OFFSET
#define IMX_CCM_ANALOG_PLL_VIDEO_NUM    IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_VIDEO_NUM_OFFSET
#define IMX_CCM_ANALOG_PLL_VIDEO_DENOM  IMX_CCM_ANALOG_BASE + IMX_CCM_ANALOG_PLL_VIDEO_DENOM_OFFSET

//
// The valid value for PLL loop divider is 27-54 so define the range of valid
// frequency for PLL5 below before divider is applied.
//
#define PLL5_MIN_FREQ 650000000
#define PLL5_MAX_FREQ 1300000000

typedef union {
    UINT32 AsUint32;
    struct {
        // LSB
        UINT32 DIV_SELECT : 7;          // 0-6 This field controls the PLL loop divider. Valid range for DIV_SELECT divider value: 27~54
        UINT32 HALF_LF : 1;             // 7 Reduces the frequency of the loop filter 2x.
        UINT32 DOUBLE_LF : 1;           // 8 Increases the frequency of the loop filter 2x.
        UINT32 HALF_CP : 1;             // 9 Reduces the charge pump gain 2x.
        UINT32 DOUBLE_CP : 1;           // 10 Increases the charge pump gain 2x.
        UINT32 HOLD_RING_OFF : 1;       // 11 Status of ana_irq2 input from analog block.
        UINT32 POWERDOWN : 1;           // 12 Powers down the PLL
        UINT32 ENABLE : 1;              // 13 Enable PLL output
        UINT32 BYPASS_CLK_SRC : 2;      // 14-15 Determines the bypass source
        UINT32 BYPASS : 1;              // 16 Bypass the PLL
        UINT32 DITHER_ENABLE : 1;       // 17 Enables dither in the fractional modulator calculation.
        UINT32 PFD_OFFSET_EN : 1;       // 18 Enables an offset in the phase frequency detector
        UINT32 TEST_DIV_SELECT : 2;     // 19-20 Control bits for the divider for the PLL clk and lvds outputs: 0x0=div-by-4(default), 0x1=div-by-2, 0x2=div-by-1 1, 0x3=div-by-1. This divider is placed before the post divider.
        UINT32 Reserved0 : 1;           // 21 Always set to zero
        UINT32 POST_DIV_SEL : 2;        // 22-23 Post-divider for video PLL: 0x0=div-by-1(default), 0x1=div-by-2, 0x2=div-by-1, 0x3=div-by-4. The output clock of the video PLL should be gated prior to changing this divider to prevent glitches.
        UINT32 PLL_VIDEO_OVERRIDE : 1;  // 24 The OVERRIDE bit allows the clock control module to automatically override portions of the register.
        UINT32 Reserved1 : 6;           // 25-30 Always set to zero
        UINT32 LOCK : 1;                // 31 PLL is/not currently locked
        // MSB
    };
} IMX_CCM_PLL_VIDEO_CTRL_REG;

typedef enum {
    IMX_POST_DIV_SEL_DIVIDE_2 = 0x01,
    IMX_POST_DIV_SEL_DIVIDE_1 = 0x02,
    IMX_POST_DIV_SEL_DIVIDE_4 = 0x03,
} IMX_CCM_PLL_VIDEO_CTRL_POST_DIV_SEL;

typedef enum {
    IMX_TEST_DIV_SEL_DIVIDE_4 = 0x00,
    IMX_TEST_DIV_SEL_DIVIDE_2 = 0x01,
    IMX_TEST_DIV_SEL_DIVIDE_1 = 0x02,
} IMX_CCM_PLL_VIDEO_CTRL_TEST_DIV_SEL;

typedef union {
    UINT32 AsUint32;
    struct {
        // LSB
        UINT32 POST_PODF : 6;           // 0-5 Post divider divide number
        UINT32 Reservedd0 : 10;         // 6-15
        UINT32 PRE_PODF : 3;            // 16-18 Pre divider divide the number.
        UINT32 Reserved2 : 5;           // 19-23
        UINT32 MUX : 3;                 // 24-26 Selection of clock sources
        UINT32 Reserved3 : 1;           // 27
        UINT32 ENABLE : 1;              // 28 Enable PLL output
        UINT32 Reserved4 : 3;           // 29-31 Always set to zero
        // MSB
    };
} IMX_CCM_TARGET_ROOT_REG;

#endif

