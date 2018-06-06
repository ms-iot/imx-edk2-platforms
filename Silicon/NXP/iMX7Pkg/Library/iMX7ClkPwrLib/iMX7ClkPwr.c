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

#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>

#include <iMX7.h>
#include <iMX7ClkPwrReg.h>

//
// List of Low Power Clock Gate to enable that has not yet been enabled
//
UINT32 ImxActiveClock[] = {
    1,  // ARM_M4
    // 19, // DRAM_ALT
    75, // LCDIF
    // 96, // PCIE
    // 116, // CAN1
    // 117, // CAN2
    // 120, // ECSPI1
    // 121, // ECSPI2
    // 122, // ECSPI3
    // 123, // ECSPI4
    // 124, // GPT1
    // 125, // GPT2
    // 126, // GPT3
    // 127, // GPT4
    // 128, // FLEXTIMER1
    // 129, // FLEXTIMER2
    // 132, // PWM1
    // 133, // PWM2
    // 134, // PWM3
    // 135, // PWM4
    // 136, // I2C1
    // 137, // I2C2
    138, // I2C3
    139, // I2C4
    // 144, // SIM1
    // 145, // SIM2
    // 148, // UART1
    // 149, // UART2
    // 150, // UART3
    // 151, // UART4
    // 152, // UART5
    // 153, // UART6
    // 154, // UART7
};

VOID ImxUngateActiveClock (
    )
{
    UINT32 index;

    // TODO: determine correct clock domain settings
    // ungate the clocks across all RDC domains
    UINT32 val = IMX_CCM_CCGR_GATE_ALL_THE_TIME |
		 (IMX_CCM_CCGR_GATE_ALL_THE_TIME << 4) |
		 (IMX_CCM_CCGR_GATE_ALL_THE_TIME << 8) |
		 (IMX_CCM_CCGR_GATE_ALL_THE_TIME << 12);


    for (index = 0; index < ARRAYSIZE(ImxActiveClock); ++index) {

	DEBUG ((EFI_D_INFO, "Ungating %08x %04x\n",
            IMX_CCM_CCGR(ImxActiveClock[index]),
            val));

        MmioWrite32(
            IMX_CCM_CCGR(ImxActiveClock[index]),
            val);

	DEBUG ((EFI_D_INFO, "value after ungating %08x\n",
            MmioRead32(IMX_CCM_CCGR(ImxActiveClock[index]))));
    }
}

VOID IMXSetVideoPllClockRate (
  UINT32 TargetClockRate,
  UINT32 PreDividerLcdif1Val,
  UINT32 PostDividerLcdif1Val
  )
{
    IMX_CCM_PLL_VIDEO_CTRL_REG pllVideoCtrlReg;
    IMX_CCM_TARGET_ROOT_REG ccmTargetRootLcdifReg;

    //
    // Gate the LCD pixel at CCGR75
    //
    MmioWrite32(IMX_CCM_CCGR(75), IMX_CCM_CCGR_GATE_SETTING_NOT_NEEDED);

    //
    // Disable LCDIF_PIXEL_CLK_ROOT clock root
    //
    ccmTargetRootLcdifReg.AsUint32 = 0;
    ccmTargetRootLcdifReg.ENABLE = 1;
    MmioWrite32(
        IMX_CCM_TARGET_ROOT(70) + IMX_CCM_CLR_OFFSET,
        ccmTargetRootLcdifReg.AsUint32);

    //
    // PLL output frequency = (Reference Freq) * (DIV_SELECT + NUM / DENOM)
    //
    // Use clock rate as denominator for simple fractional calculation.
    // This way we just need to figure out the target clock rate ratio
    // to the 24MHz reference.
    //
    {
        UINT32 denom = IMX_REF_CLK_24M_FREQ;
        UINT32 divSelect = TargetClockRate / IMX_REF_CLK_24M_FREQ;
        UINT32 numerator = TargetClockRate % IMX_REF_CLK_24M_FREQ;

        pllVideoCtrlReg.AsUint32 = 0;

        ASSERT (numerator < denom);
        ASSERT ((divSelect >= 27) && (divSelect <= 54));

        pllVideoCtrlReg.POWERDOWN = 0;
        pllVideoCtrlReg.DIV_SELECT = divSelect;
        pllVideoCtrlReg.POST_DIV_SEL = IMX_POST_DIV_SEL_DIVIDE_1;
        pllVideoCtrlReg.TEST_DIV_SELECT = IMX_TEST_DIV_SEL_DIVIDE_4;

        DEBUG ((
            DEBUG_INFO,
            "Video PLL divSelect (%d) numerator (%d) denominator %d\n",
            divSelect,
            numerator,
            denom
            ));

        MmioWrite32(IMX_CCM_ANALOG_PLL_VIDEO, pllVideoCtrlReg.AsUint32);
        MmioWrite32(IMX_CCM_ANALOG_PLL_VIDEO_NUM, numerator);
        MmioWrite32(IMX_CCM_ANALOG_PLL_VIDEO_DENOM, denom);
    }

    DEBUG ((DEBUG_INFO, "Waiting for Video PLL to lock\n"));
    //
    // Wait for PLL to lock
    //
    {
        UINT32 counter = 1000;

        do {
            pllVideoCtrlReg.AsUint32 = MmioRead32(IMX_CCM_ANALOG_PLL_VIDEO);
            --counter;
            MicroSecondDelay(10);
        } while (!(pllVideoCtrlReg.LOCK) && (counter != 0));

        if (counter == 0) {
            DEBUG((DEBUG_ERROR, "Time out waiting for Video PLL to lock\n"));
            ASSERT(FALSE);
        }
    }
    DEBUG ((DEBUG_INFO, "Video PLL locked\n"));

    //
    // Enable the PLL output
    //
    pllVideoCtrlReg.AsUint32 = 0;
    pllVideoCtrlReg.ENABLE = 1;
    MmioWrite32(
        IMX_CCM_ANALOG_PLL_VIDEO + IMX_CCM_SET_OFFSET,
        pllVideoCtrlReg.AsUint32);

    //
    // Set muxing, pre and post divider for LCDIF_PIXEL_CLK_ROOT
    // Freq = (clock source freq)/(pre_div+1)/(post_div+1)
    //
    ccmTargetRootLcdifReg.AsUint32 = MmioRead32(IMX_CCM_TARGET_ROOT(70));

    ccmTargetRootLcdifReg.PRE_PODF = (PreDividerLcdif1Val - 1);
    ccmTargetRootLcdifReg.POST_PODF = (PostDividerLcdif1Val - 1);
    ccmTargetRootLcdifReg.MUX = 0x06; //110b - Video PLL
    ccmTargetRootLcdifReg.ENABLE = 0x01;

    MmioWrite32(IMX_CCM_TARGET_ROOT(70), ccmTargetRootLcdifReg.AsUint32);

    //
    // Ungate the LCD pixel clock
    //
    MmioWrite32(IMX_CCM_CCGR(75), IMX_CCM_CCGR_GATE_ALL_THE_TIME);

    DEBUG_CODE_BEGIN();
    DEBUG((DEBUG_INIT, "IMX_CCM_ANALOG_PLL_VIDEO 0x%08x\n", MmioRead32(IMX_CCM_ANALOG_PLL_VIDEO)));
    DEBUG((DEBUG_INIT, "IMX_CCM_ANALOG_PLL_VIDEO_NUM 0x%08x\n", MmioRead32(IMX_CCM_ANALOG_PLL_VIDEO_NUM)));
    DEBUG((DEBUG_INIT, "IMX_CCM_ANALOG_PLL_VIDEO_DENOM 0x%08x\n", MmioRead32(IMX_CCM_ANALOG_PLL_VIDEO_DENOM)));

    DEBUG((DEBUG_INIT, "IMX_CCM_PLL_CTRL(31) 0x%08x : 0x%08x\n", IMX_CCM_PLL_CTRL(31), MmioRead32(IMX_CCM_PLL_CTRL(31))));
    DEBUG((DEBUG_INIT, "IMX_CCM_PLL_CTRL(32) 0x30380A00 : 0x%08x\n", MmioRead32(0x30380A00)));
    DEBUG((DEBUG_INIT, "IMX_CCM_CCGR(75)  0x%08x : 0x%08x\n", IMX_CCM_CCGR(75), MmioRead32(IMX_CCM_CCGR(75))));
    DEBUG((DEBUG_INIT, "IMX_CCM_CCGR(168)  0x%08x : 0x%08x\n", IMX_CCM_CCGR(168), MmioRead32(IMX_CCM_CCGR(168))));
    DEBUG((DEBUG_INIT, "IMX_CCM_CCGR(169)  0x%08x : 0x%08x\n", IMX_CCM_CCGR(169), MmioRead32(IMX_CCM_CCGR(169))));
    DEBUG_CODE_END();
}

VOID IMXSetAudioPllClockRate (
  UINT32 TargetClockRate,
  UINT32 PreDividerAudioVal,
  UINT32 PostDividerAudioVal
  )
{
    IMX_CCM_PLL_VIDEO_CTRL_REG pllAudioCtrlReg;
    IMX_CCM_TARGET_ROOT_REG ccmTargetRootAudioMclkReg;

    //
    // Gate the SAIn AUDIO devices
    //
    MmioWrite32(IMX_CCM_CCGR(140), IMX_CCM_CCGR_GATE_SETTING_NOT_NEEDED);
    MmioWrite32(IMX_CCM_CCGR(141), IMX_CCM_CCGR_GATE_SETTING_NOT_NEEDED);
    MmioWrite32(IMX_CCM_CCGR(142), IMX_CCM_CCGR_GATE_SETTING_NOT_NEEDED);

    //
    // Disable AUDIO_MCLK_CLK_ROOT clock root
    //
    ccmTargetRootAudioMclkReg.AsUint32 = 0;
    ccmTargetRootAudioMclkReg.ENABLE = 1;
    MmioWrite32(
        IMX_CCM_TARGET_ROOT(121) + IMX_CCM_CLR_OFFSET,
        ccmTargetRootAudioMclkReg.AsUint32);

    //
    // PLL output frequency = (Reference Freq) * (DIV_SELECT + NUM / DENOM)
    //
    // Use clock rate as denominator for simple fractional calculation.
    // This way we just need to figure out the target clock rate ratio
    // to the 24MHz reference.
    //
    {
        UINT32 denom = IMX_REF_CLK_24M_FREQ;
        UINT32 divSelect = TargetClockRate / IMX_REF_CLK_24M_FREQ;
        UINT32 numerator = TargetClockRate % IMX_REF_CLK_24M_FREQ;

        pllAudioCtrlReg.AsUint32 = 0;

        ASSERT (numerator < denom);
        ASSERT ((divSelect >= 27) && (divSelect <= 54));

        pllAudioCtrlReg.POWERDOWN = 0;
        pllAudioCtrlReg.DIV_SELECT = divSelect;
        pllAudioCtrlReg.POST_DIV_SEL = IMX_POST_DIV_SEL_DIVIDE_1;
        pllAudioCtrlReg.TEST_DIV_SELECT = IMX_TEST_DIV_SEL_DIVIDE_4;

        DEBUG ((
            DEBUG_INFO,
            "Audio PLL divSelect (%d) numerator (%d) denominator %d\n",
            divSelect,
            numerator,
            denom
            ));

        MmioWrite32(IMX_CCM_ANALOG_PLL_AUDIO, pllAudioCtrlReg.AsUint32);
        MmioWrite32(IMX_CCM_ANALOG_PLL_AUDIO_NUM, numerator);
        MmioWrite32(IMX_CCM_ANALOG_PLL_AUDIO_DENOM, denom);
    }

    DEBUG ((DEBUG_INFO, "Waiting for Audio PLL to lock\n"));
    //
    // Wait for PLL to lock
    //
    {
        UINT32 counter = 1000;

        do {
            pllAudioCtrlReg.AsUint32 = MmioRead32(IMX_CCM_ANALOG_PLL_AUDIO);
            --counter;
            MicroSecondDelay(10);
        } while (!(pllAudioCtrlReg.LOCK) && (counter != 0));

        if (counter == 0) {
            DEBUG((DEBUG_ERROR, "Time out waiting for Audio PLL to lock\n"));
            ASSERT(FALSE);
        }
    }
    DEBUG ((DEBUG_INFO, "Audio PLL locked\n"));

    //
    // Enable the PLL output
    //
    pllAudioCtrlReg.AsUint32 = 0;
    pllAudioCtrlReg.ENABLE = 1;
    MmioWrite32(
        IMX_CCM_ANALOG_PLL_AUDIO + IMX_CCM_SET_OFFSET,
        pllAudioCtrlReg.AsUint32);

    //
    // Set muxing, pre and post divider for AUDIO_MCLK_CLK_ROOT
    // Freq = (clock source freq)/(pre_div+1)/(post_div+1)
    //
    ccmTargetRootAudioMclkReg.AsUint32 = MmioRead32(IMX_CCM_TARGET_ROOT(121));

    ccmTargetRootAudioMclkReg.PRE_PODF = (PreDividerAudioVal - 1);
    ccmTargetRootAudioMclkReg.POST_PODF = (PostDividerAudioVal - 1);
    ccmTargetRootAudioMclkReg.MUX = 0x05; //101b - Audio PLL
    ccmTargetRootAudioMclkReg.ENABLE = 0x01;

    MmioWrite32(IMX_CCM_TARGET_ROOT(121), ccmTargetRootAudioMclkReg.AsUint32);

    //
    // Ungate the SAIn AUDIO device clocks
    //
    MmioWrite32(IMX_CCM_CCGR(140), IMX_CCM_CCGR_GATE_RUN_WAIT);
    MmioWrite32(IMX_CCM_CCGR(141), IMX_CCM_CCGR_GATE_RUN_WAIT);
    MmioWrite32(IMX_CCM_CCGR(142), IMX_CCM_CCGR_GATE_RUN_WAIT);

    DEBUG_CODE_BEGIN();
    DEBUG((DEBUG_INIT, "IMX_CCM_ANALOG_PLL_AUDIO 0x%08x\n", MmioRead32(IMX_CCM_ANALOG_PLL_AUDIO)));
    DEBUG((DEBUG_INIT, "IMX_CCM_ANALOG_PLL_AUDIO_NUM 0x%08x\n", MmioRead32(IMX_CCM_ANALOG_PLL_AUDIO_NUM)));
    DEBUG((DEBUG_INIT, "IMX_CCM_ANALOG_PLL_AUDIO_DENOM 0x%08x\n", MmioRead32(IMX_CCM_ANALOG_PLL_AUDIO_DENOM)));

    DEBUG((DEBUG_INIT, "IMX_CCM_PLL_CTRL(29) 0x%08x : 0x%08x\n", IMX_CCM_PLL_CTRL(29), MmioRead32(IMX_CCM_PLL_CTRL(29))));
    DEBUG((DEBUG_INIT, "IMX_CCM_PLL_CTRL(30) 0x%08x : 0x%08x\n", IMX_CCM_PLL_CTRL(30), MmioRead32(IMX_CCM_PLL_CTRL(30))));
    DEBUG((DEBUG_INIT, "IMX_CCM_CCGR(140)  0x%08x : 0x%08x\n", IMX_CCM_CCGR(140), MmioRead32(IMX_CCM_CCGR(140))));
    DEBUG((DEBUG_INIT, "IMX_CCM_CCGR(141)  0x%08x : 0x%08x\n", IMX_CCM_CCGR(141), MmioRead32(IMX_CCM_CCGR(141))));
    DEBUG((DEBUG_INIT, "IMX_CCM_CCGR(142)  0x%08x : 0x%08x\n", IMX_CCM_CCGR(142), MmioRead32(IMX_CCM_CCGR(142))));
    DEBUG_CODE_END();
}

EFI_STATUS
ImxSetLcdIfClockRate (
    UINT32 ClockRate
    )
{
    BOOLEAN foundConfig = FALSE;
    UINT32 targetFreq;
    UINT32 preDivSelectCount;
    UINT32 postDivSelectCount;
    UINT32 preDividerLcdif1[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    UINT32 postDividerLcdif1[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

    for (postDivSelectCount = 0;
        postDivSelectCount < ARRAYSIZE (postDividerLcdif1);
        ++postDivSelectCount) {

        for (preDivSelectCount = 0;
            preDivSelectCount < ARRAYSIZE (preDividerLcdif1);
            ++preDivSelectCount) {

            //
            // Need to verify the meaning of post div and test div values
            //
            targetFreq =
                ClockRate *
                preDividerLcdif1[preDivSelectCount] *
                postDividerLcdif1[postDivSelectCount] *
                1 * // IMX_POST_DIV_SEL_DIVIDE_1
                4; //IMX_TEST_DIV_SEL_DIVIDE_4

            //
            // The valid range for PLL loop divider is 27-54 so we
            // need to target freq need to fit within the valid range.
            //
            if ((targetFreq >= PLL5_MIN_FREQ) &&
                (targetFreq <= PLL5_MAX_FREQ)) {
                foundConfig = TRUE;
                break;
            }
        }

        if (foundConfig == TRUE) {
            break;
        }
    }

    if (foundConfig == FALSE) {
        DEBUG((DEBUG_ERROR, "No valid configuration found for clock rate %d\n", ClockRate));
        ASSERT(FALSE);
        return EFI_INVALID_PARAMETER;
    }

    DEBUG ((
        DEBUG_INFO,
        "Video PLL setting (%d) Target Freq (%d) PreDiv %d PostDiv %d\n",
        ClockRate,
        targetFreq,
        preDividerLcdif1[preDivSelectCount],
        postDividerLcdif1[postDivSelectCount]
        ));

    IMXSetVideoPllClockRate(
        targetFreq,
        preDividerLcdif1[preDivSelectCount],
        postDividerLcdif1[postDivSelectCount]);

    return EFI_SUCCESS;
}

EFI_STATUS
ImxSetAudioMclkClockRate (
    UINT32 ClockRate
    )
{
    BOOLEAN foundConfig = FALSE;
    UINT32 targetFreq;
    UINT32 preDivSelectCount;
    UINT32 postDivSelectCount;
    UINT32 preDividerAudioMclk1[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    UINT32 postDividerAudioMclk1[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

    for (postDivSelectCount = 0;
        postDivSelectCount < ARRAYSIZE (postDividerAudioMclk1);
        ++postDivSelectCount) {

        for (preDivSelectCount = 0;
            preDivSelectCount < ARRAYSIZE (preDividerAudioMclk1);
            ++preDivSelectCount) {

            //
            // Need to verify the meaning of post div and test div values
            //
            targetFreq =
                ClockRate *
                preDividerAudioMclk1[preDivSelectCount] *
                postDividerAudioMclk1[postDivSelectCount] *
                1 * // IMX_POST_DIV_SEL_DIVIDE_1
                4; //IMX_TEST_DIV_SEL_DIVIDE_4

            //
            // The valid range for PLL loop divider is 27-54 so we
            // need to target freq need to fit within the valid range.
            //
            if ((targetFreq >= PLL5_MIN_FREQ) &&
                (targetFreq <= PLL5_MAX_FREQ)) {
                foundConfig = TRUE;
                break;
            }
        }

        if (foundConfig == TRUE) {
            break;
        }
    }

    if (foundConfig == FALSE) {
        DEBUG((DEBUG_ERROR, "No valid configuration found for clock rate %d\n", ClockRate));
        ASSERT(FALSE);
        return EFI_INVALID_PARAMETER;
    }

    DEBUG ((
        DEBUG_INFO,
        "Audio PLL setting (%d) Target Freq (%d) PreDiv %d PostDiv %d\n",
        ClockRate,
        targetFreq,
        preDividerAudioMclk1[preDivSelectCount],
        postDividerAudioMclk1[postDivSelectCount]
        ));

    IMXSetAudioPllClockRate(
        targetFreq,
        preDividerAudioMclk1[preDivSelectCount],
        postDividerAudioMclk1[postDivSelectCount]);

    return EFI_SUCCESS;
}

