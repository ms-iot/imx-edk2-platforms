//// 
//  Microsoft Corporation
//  SiArch
//
//  Portions Copyright (c) Microsoft Corporation
//  Portions Copyright (c) Intel Corporation
//  Portions Copyright (c) Apple
//  Portions Copyright (c) ARM Ltd.
//  Portions Copyright (c) Freescale
////

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>

#include <iMX6.h>
#include <iMXDisplay.h>
#include <UdooNeoI2cLib.h>
#include "TDA19988.h"

#include "UdooNeo.inc"

// #define DEBUG_TDA_SELECT

#define sTDAReadBlock(aRegId, ReadByteCount, ValueOutPtr) ReadI2CBlock(UDOONEO_HDMI_I2C_UNIT, aRegId, ReadByteCount, ValueOutPtr, sTDASelect)
#define sTDARead(aRegId, ValueOutPtr) ReadI2CByte(UDOONEO_HDMI_I2C_UNIT, (aRegId), (ValueOutPtr), sTDASelect)
#define sTDAWrite(aRegId, ValueIn) WriteI2CByte(UDOONEO_HDMI_I2C_UNIT, (aRegId), (ValueIn), sTDASelect)
#define sTDAWriteNoInit(aRegId, ValueIn) WriteI2CByte(UDOONEO_HDMI_I2C_UNIT, (aRegId), (ValueIn), NULL)
   
#define MAX_NUM_ATTEMPTS                    5
#define INTERRUPT_TIMEOUT_MS                1000

#define CHECK_RESULT(result, errorMsg, failLabel) \
    if((result) == FALSE) { \
        DebugPrint(DEBUG_ERROR, errorMsg); \
        goto failLabel; \
    } \


static IMX6SX_I2C_TARGET    sgCEC;
static IMX6SX_I2C_TARGET    sgHDMI;

//
// I2C to HDMI Transmitter Wrappers
//

static
BOOLEAN
sTDASelect(
    UINT32 aRegId
    )
{
    UINT32  comp;
    BOOLEAN result;
    UINT8   page;

    comp = aRegId >> 16;
    page = (UINT8)((aRegId >> 8) & 0xFF);

    switch(comp)
    {
        case TDA19988_COMPONENT_CEC:
            DebugPrint(DEBUG_INFO, "Switch to CEC\n");
            result = IMX6SX_I2C_SetTarget(UDOONEO_HDMI_I2C_UNIT, &sgCEC);
            CHECK_RESULT(result, "Fail set CEC target", Exit);

            break;
        case TDA19988_COMPONENT_HDMI:
            DebugPrint(DEBUG_INFO, "Switch to HDMI\n");
            result = IMX6SX_I2C_SetTarget(UDOONEO_HDMI_I2C_UNIT, &sgHDMI);
            CHECK_RESULT(result, "Fail to set HDMI target", Exit);

            DebugPrint(DEBUG_INFO, "Switch to page 0x%02X\n", page);
            result = sTDAWriteNoInit(TDA19988_REG_HDMI_PAGESEL, (UINT8)page);
            CHECK_RESULT(result, "Fail to switch page", Exit);
            break;
        default:
            result = FALSE;
            CHECK_RESULT(result, "Unknown mode", Exit);
    }

Exit:
    DebugPrint(
        DEBUG_INFO,
        "sTDASelect result %s.\n",
        (result ? L"success" : L"fail"));

    return result;
}

static
BOOLEAN
sTDAWrite16(
    UINT32 aRegId,
    UINT16 aValue
    )
{
    BOOLEAN  result = TRUE;
    UINT8    firstByteValue = (UINT8)(aValue >> 8);
    UINT8    secondByteValue = (UINT8)(aValue & 0xFF);

    DebugPrint(DEBUG_INFO, "Reg(%02X,%02X) <- 0x%02X,0x%02X\n", (aRegId >> 8) & 0xFF, aRegId & 0xFF, aValue >> 8, aValue & 0xFF);

    result = sTDAWrite(aRegId, firstByteValue);
    CHECK_RESULT(result, "Fail to write first byte", Exit);

    result = sTDAWrite(aRegId + 1, secondByteValue);
    CHECK_RESULT(result, "Fail to write second byte", Exit);

Exit:
    return result;
}

static
BOOLEAN
sTDASetBits(
    UINT32  aRegId,
    UINT8   aBits
    )
{
    UINT8 val;
    BOOLEAN result;

    result = sTDARead(aRegId, &val);
    CHECK_RESULT(result, "Fail to read reg to set bit", Exit);

    val |= aBits;

    result = sTDAWrite(aRegId, val);
    CHECK_RESULT(result, "Fail to write reg to set bit", Exit);

Exit:
    return result;
}

static
BOOLEAN
sTDAClrBits(
    UINT32  aRegId,
    UINT8   aBits
    )
{
    UINT8 val;
    BOOLEAN result;

    result = sTDARead(aRegId, &val);
    CHECK_RESULT(result, "Fail to read reg to clear bit", Exit);

    val &= ~aBits;
    
    result = sTDAWrite(aRegId, val);
    CHECK_RESULT(result, "Fail to write reg to clear bit", Exit);

Exit:
    return result;
}

//
// TDA Configuration Methods
//

static
BOOLEAN
sResetTDA(
    VOID
    )
{
    BOOLEAN result = TRUE;

    result = sTDAWrite(TDA19988_REG_CEC_ENABLE, 0);
    CHECK_RESULT(result, "Write TDA19988_REG_CEC_ENABLE = 0 fail\n", Cleanup);
    MicroSecondDelay(100 * 1000);

    result = sTDAWrite(TDA19988_REG_CEC_ENABLE, TDA19988_CEC_ENABLE_ALL);
    CHECK_RESULT(
        result,
        "Write TDA19988_REG_CEC_ENABLE = TDA19988_CEC_ENABLE_ALL fail",
        Cleanup);
    MicroSecondDelay(10 * 1000);

    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_SOFTRESET, 
        TDA19988_HDMI_CTRL_SOFTRESET_AUDIO |
        TDA19988_HDMI_CTRL_SOFTRESET_I2C_MASTER);
    CHECK_RESULT(
        result,
        "Write TDA19988_REG_HDMI_CTRL_SOFTRESET = TDA19988_HDMI_CTRL_SOFTRESET_AUDIO | TDA19988_HDMI_CTRL_SOFTRESET_I2C_MASTER fail",
         Cleanup);

    MicroSecondDelay(50 * 1000);
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_SOFTRESET, 0);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_CTRL_SOFTRESET = 0 fail\n", Cleanup);
    MicroSecondDelay(50 * 1000);

    result = sTDASetBits(TDA19988_REG_HDMI_CTRL_MAIN_CNTRL0, TDA19988_HDMI_CTRL_MAIN_CNTRL0_SR);
    CHECK_RESULT(result, "Set TDA19988_HDMI_CTRL_MAIN_CNTRL0_SR fail\n", Cleanup);
    MicroSecondDelay(5 * 1000);

    result = sTDAClrBits(TDA19988_REG_HDMI_CTRL_MAIN_CNTRL0, TDA19988_HDMI_CTRL_MAIN_CNTRL0_SR);
    CHECK_RESULT(result, "Clear TDA19988_HDMI_CTRL_MAIN_CNTRL0_SR fail\n", Cleanup);
    MicroSecondDelay(50 * 1000);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SERIAL_1, 0x00);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_PLL_SERIAL_1 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SERIAL_2, 0x00);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_PLL_SERIAL_2 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SERIAL_3, 0x00);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_PLL_SERIAL_3 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SERIALIZER, 0x00);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_PLL_SERIALIZER fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_BUFFER_OUT, 0x00);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_PLL_BUFFER_OUT fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SCG1, 0x00);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_PLL_SCG1 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_AUDIO_DIV_POW2, 0x03);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_PLL_AUDIO_DIV_POW2 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SEL_CLK, TDA19988_HDMI_PLL_SEL_CLK_CLK1 | TDA19988_HDMI_PLL_SEL_CLK_ENA_SC_CLK);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_PLL_SEL_CLK fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SCGN1, 0xfa);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_PLL_SCGN1 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SCGN2, 0x00);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_PLL_SCGN2 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SCGR1, 0x5b);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_PLL_SCGR1 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SCGR2, 0x00);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_PLL_SCGR2 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SCG2, 0x10);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_PLL_SCG2 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_MUX_VP_VIP_OUT, 0x24);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_CTRL_MUX_VP_VIP_OUT fail\n", Cleanup);

    // enable input on all ports before setting muxing
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_ENA_VP_0,  0xFF);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_CTRL_ENA_VP_0 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_ENA_VP_1,  0xFF);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_CTRL_ENA_VP_1 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_ENA_VP_2,  0xFF);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_CTRL_ENA_VP_2 fail\n", Cleanup);

#if 0
    //
    // component bit field order high..low = ABGR
    //
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_VIP_CNTRL_0, 0x45);
    if (!result) {
        goto Cleanup;
    }
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_VIP_CNTRL_1, 0x23);
    if (!result) {
        goto Cleanup;
    }
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_VIP_CNTRL_2, 0x01);
    if (!result) {
        goto Cleanup;
    }
#else
    //
    // component bit field order high..low = ARGB
    //
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_VIP_CNTRL_0, 0x23);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_CTRL_VIP_CNTRL_0 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_VIP_CNTRL_1, 0x01);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_CTRL_VIP_CNTRL_1 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_VIP_CNTRL_2, 0x45);
    CHECK_RESULT(result, "Write TDA19988_REG_HDMI_CTRL_VIP_CNTRL_2 fail\n", Cleanup);
#endif

Cleanup:
    return result;
}

static
BOOLEAN
sCheckVersionInfo(
    VOID
    )
{
    BOOLEAN  result = TRUE;
    UINT8    value8;
    UINT32   ix;

    result = sTDARead(TDA19988_REG_HDMI_CTRL_VERSION_LSB, &value8);
    CHECK_RESULT(result, "sCheckVersionInfo: Read TDA19988_REG_HDMI_CTRL_VERSION_LSB fail\n", Cleanup);
    ix = value8 & ~(TDA19988_HDMI_CTRL_VERSION_MSB_NO_HDCP | TDA19988_HDMI_CTRL_VERSION_MSB_NO_SCALER);

    result = sTDARead(TDA19988_REG_HDMI_CTRL_VERSION_MSB, &value8);
    CHECK_RESULT(result, "sCheckVersionInfo: Read TDA19988_REG_HDMI_CTRL_VERSION_MSB fail\n", Cleanup);
    ix |= (value8 << 8);
    if (ix != 0x0301) {
        DebugPrint(DEBUG_ERROR, "HDMI did not return correct version information. 0x%4x expected 0x0301\n", ix);
        result = FALSE;
        goto Cleanup;
    }

Cleanup:
    return result;
}

static
BOOLEAN
sConfigureDDC(
    VOID
    )
{
    BOOLEAN result = TRUE;

    // enable DDC
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_DDC_DISABLE, 0);
    CHECK_RESULT(result, "sConfigureDDC: Write TDA19988_REG_HDMI_CTRL_DDC_DISABLE fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_HDCP_TX3, 0x27);
    CHECK_RESULT(result, "sConfigureDDC: Write TDA19988_REG_HDMI_HDCP_TX3 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_CEC_CLK_CTRL,
        TDA19988_CEC_CLK_CTRL_IMCLK_SEL |
        TDA19988_CEC_CLK_CTRL_GHOST_DIS);
    CHECK_RESULT(result, "sConfigureDDC: Write TDA19988_REG_CEC_CLK_CTRL fail\n", Cleanup);

    // disable multi-master
    result = sTDASetBits(TDA19988_REG_HDMI_CTRL_I2C_MASTER, TDA19988_HDMI_CTRL_I2C_MASTER_DIS_MM);
    CHECK_RESULT(result, "sConfigureDDC: disable multi-master fail\n", Cleanup);

    // soft reset DDC
    result = sTDASetBits(TDA19988_REG_HDMI_CTRL_SOFTRESET, 2);
    CHECK_RESULT(result, "sConfigureDDC: soft reset fail\n", Cleanup);
    MicroSecondDelay(100 * 1000);

    sTDAClrBits(TDA19988_REG_HDMI_CTRL_SOFTRESET, 2);
    CHECK_RESULT(result, "sConfigureDDC: clear TDA19988_REG_HDMI_CTRL_SOFTRESET fail\n", Cleanup);
    MicroSecondDelay(100 * 1000);

    // turn on DDC clock
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_CCLK_ON, 1);
    CHECK_RESULT(result, "sConfigureDDC: turn on DDC clock fail\n", Cleanup);
    MicroSecondDelay(10 * 1000);

Cleanup:
    return result;
}

static
BOOLEAN
sConfigureInterrupts(
    VOID
    )
{
    BOOLEAN result = TRUE;
    UINT8    value8;

    // enable connection status change interrupt
    result = sTDAWrite(TDA19988_REG_CEC_RX_INTEN, 0x02);
    CHECK_RESULT(result, "sConfigureInterrupts: enable connection status change interrupt fail\n", Cleanup);

    // back to HDMI. clear interrupt status
    result = sTDARead(TDA19988_REG_HDMI_CTRL_INT_FLAGS_0, &value8);
    CHECK_RESULT(result, "sConfigureInterrupts:  clear interrupt status fail\n", Cleanup);

    result = sTDARead(TDA19988_REG_HDMI_CTRL_INT_FLAGS_1, &value8);
    CHECK_RESULT(result, "sConfigureInterrupts:read TDA19988_REG_HDMI_CTRL_INT_FLAGS_1 fail\n", Cleanup);

    result = sTDARead(TDA19988_REG_HDMI_CTRL_INT_FLAGS_2, &value8);
    CHECK_RESULT(result, "sConfigureInterrupts: read TDA19988_REG_HDMI_CTRL_INT_FLAGS_2 fail\n", Cleanup);

Cleanup:
    return result;
}

static
BOOLEAN
sConfigureMisc(
    VOID    
    )
{
    BOOLEAN result = TRUE;

    // mute audio FIFO
    result = sTDASetBits(TDA19988_REG_HDMI_AUDIO_AIP_CNTRL_0, TDA19988_HDMI_AUDIO_AIP_CNTRL_0_RST_FIFO);
    CHECK_RESULT(result, "sConfigureMisc: mute audio FIFO fail\n", Cleanup);

    // turn off HDCP mode
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_TBG_CNTRL_1, TDA19988_HDMI_CTRL_TBG_CNTRL_1_DWIN_DIS);
    CHECK_RESULT(result, "sConfigureMisc: turn off HDCP mode\n", Cleanup);

    sTDAClrBits(TDA19988_REG_HDMI_HDCP_TX33, TDA19988_HDMI_HDCP_TX33_HDMI);
    CHECK_RESULT(result, "sConfigureMisc: clear TDA19988_REG_HDMI_HDCP_TX33 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_AUDIO_ENC_CNTRL, 0);
    CHECK_RESULT(result, "sConfigureMisc: write TDA19988_REG_HDMI_AUDIO_ENC_CNTRL fail\n", Cleanup);

    // misc configuration
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_HVF_CNTRL_0, 0);
    CHECK_RESULT(result, "sConfigureMisc: write TDA19988_REG_HDMI_CTRL_HVF_CNTRL_0\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_VIP_CNTRL_4, 0);
    CHECK_RESULT(result, "sConfigureMisc: write TDA19988_REG_HDMI_CTRL_VIP_CNTRL_4 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_VIP_CNTRL_5, 0);
    CHECK_RESULT(result, "sConfigureMisc: write TDA19988_REG_HDMI_CTRL_VIP_CNTRL_5 fail\n", Cleanup);

    sTDAClrBits(TDA19988_REG_HDMI_PLL_SERIAL_1, TDA19988_HDMI_PLL_SERIAL_1_MAN_IP);
    CHECK_RESULT(result, "sConfigureMisc: clear TDA19988_REG_HDMI_PLL_SERIAL_1 fail\n", Cleanup);

    sTDAClrBits(TDA19988_REG_HDMI_PLL_SERIAL_3, 
        TDA19988_HDMI_PLL_SERIAL_3_SRL_CCIR |
        TDA19988_HDMI_PLL_SERIAL_3_SRL_DE);
    CHECK_RESULT(result, "sConfigureMisc: clear TDA19988_REG_HDMI_PLL_SERIAL_3 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SERIALIZER, 0);
    CHECK_RESULT(result, "sConfigureMisc: write TDA19988_REG_HDMI_PLL_SERIALIZER fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_HVF_CNTRL_1, 0);
    CHECK_RESULT(result, "sConfigureMisc: write TDA19988_REG_HDMI_CTRL_HVF_CNTRL_1 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_RPT_CNTRL, 0);
    CHECK_RESULT(result, "sConfigureMisc: write TDA19988_REG_HDMI_CTRL_RPT_CNTRL fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SEL_CLK, 
        TDA19988_HDMI_PLL_SEL_CLK_CLK1 |
        TDA19988_HDMI_PLL_SEL_CLK_ENA_SC_CLK);
    CHECK_RESULT(result, "sConfigureMisc: write TDA19988_REG_HDMI_PLL_SEL_CLK fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_SERIAL_2, 0);
    CHECK_RESULT(result, "sConfigureMisc: write TDA19988_REG_HDMI_PLL_SERIAL_2 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_PLL_ANA_GENERAL, 0x09);
    CHECK_RESULT(result, "sConfigureMisc: write TDA19988_REG_HDMI_PLL_ANA_GENERAL fail\n", Cleanup);

    // set color matrix bypass and bias
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_MAT_CONTRL, TDA19988_HDMI_CTRL_MAT_CONTRL_BP | 1);
    CHECK_RESULT(result, "sConfigureMisc: write TDA19988_REG_HDMI_CTRL_MAT_CONTRL fail\n", Cleanup);

Cleanup:
    return result;
}

static
BOOLEAN
sConfigureVideoFormat(
    IMX_DISPLAY_TIMING* Timing
    )
{
    BOOLEAN result = TRUE;

    // configure video format
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_VIDFORMAT, 0xFF);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VIDFORMAT fail\n", Cleanup);

    // Reference pixel preset
    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_REFPIX_MSB, (Timing->HSyncOffset + 3));
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VIDFORMAT fail\n", Cleanup);

    // Reference line preset
    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_REFLINE_MSB, (Timing->VSyncOffset + 1));
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_REFLINE_MSB fail\n", Cleanup);

    // Number of pixel per line
    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_NPIX_MSB, Timing->HActive + Timing->HBlank);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_NPIX_MSB fail\n", Cleanup);

    // Number of lines per frame
    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_NLINE_MSB, Timing->VActive + Timing->VBlank);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_NLINE_MSB fail\n", Cleanup);

    // Vertical synchronization line number for start pulse in field 1
    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_VS_LINE_STRT_1_MSB, Timing->VSyncOffset);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VS_LINE_STRT_1_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_VS_PIX_STRT_1_MSB, Timing->HSyncOffset);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VS_PIX_STRT_1_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_VS_LINE_END_1_MSB, (Timing->VSyncOffset + Timing->VSync));
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VS_LINE_END_1_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_VS_PIX_END_1_MSB, Timing->HSyncOffset);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VS_PIX_END_1_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_HS_PIX_START_MSB, Timing->HSyncOffset);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_HS_PIX_START_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_HS_PIX_STOP_MSB, (Timing->HSyncOffset + Timing->HSync));
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_HS_PIX_STOP_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_VWIN_START_1_MSB, (Timing->VBlank - 1));
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VWIN_START_1_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_VWIN_END_1_MSB, ((Timing->VActive + Timing->VBlank) - 1));
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VWIN_END_1_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_DE_START_MSB, Timing->HBlank);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_DE_START_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_DE_STOP_MSB, Timing->HActive + Timing->HBlank);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_DE_STOP_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_VS_LINE_STRT_2_MSB, 0);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VS_LINE_STRT_2_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_VS_PIX_STRT_2_MSB, 0);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VS_PIX_STRT_2_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_VS_LINE_END_2_MSB, 0);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VS_LINE_END_2_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_VS_PIX_END_2_MSB, 0);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VS_PIX_END_2_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_VWIN_START_2_MSB, 0);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VWIN_START_2_MSB fail\n", Cleanup);

    result = sTDAWrite16(TDA19988_REG_HDMI_CTRL_VWIN_END_2_MSB, 0);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_VWIN_END_2_MSB fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_ENABLE_SPACE, 0);
    CHECK_RESULT(result, "sConfigureVideoFormat: write TDA19988_REG_HDMI_CTRL_ENABLE_SPACE fail\n", Cleanup);

Cleanup:
    return result;
}

BOOLEAN
ReadEDID(
    UINT8* EdidDataRead
    )
{
    BOOLEAN  result = TRUE;
    UINT8    value8;
    UINT32   ix;
    UINTN    elapsedTimeMilliSec = 0;

    // enable EDID block read interrupt
    result = sTDASetBits(TDA19988_REG_HDMI_CTRL_INT_FLAGS_2, 2);
    CHECK_RESULT(result, "ReadEDID : enable EDID block read interrupt fail\n", Cleanup);

    // enable global interrupts
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_INT_FLAGS_0, 4);
    CHECK_RESULT(result, "ReadEDID : enable global interrupts fail\n", Cleanup);

    // clear ram power down
    result= sTDAClrBits(TDA19988_REG_HDMI_HDCP_TX4, 2);
    CHECK_RESULT(result, "ReadEDID : clear ram power down fail\n", Cleanup);

    // set device address
    result = sTDAWrite(TDA19988_REG_HDMI_EDID_DDC_ADDR, 0xA0);
    CHECK_RESULT(result, "ReadEDID : set device address fail\n", Cleanup);

    // set address offset into 256-byte pages
    result = sTDAWrite(TDA19988_REG_HDMI_EDID_DDC_OFFS, 0);
    CHECK_RESULT(result, "ReadEDID : enable global interrupts fail\n", Cleanup);

    // hard coded value segment address?
    result = sTDAWrite(TDA19988_REG_HDMI_EDID_DDC_SEGM_ADDR, 0x60);
    CHECK_RESULT(result, "ReadEDID : set segment address fail\n", Cleanup);

    // default value for offset 0?
    result = sTDAWrite(TDA19988_REG_HDMI_EDID_DDC_SEGM, 0);
    CHECK_RESULT(result, "ReadEDID : set default offset fail\n", Cleanup);

    // toggle EDID read request bit
    result = sTDAWrite(TDA19988_REG_HDMI_EDID_CTRL, 1);
    CHECK_RESULT(result, "ReadEDID : toggle EDID 1 fail\n", Cleanup);

    result = sTDAWrite(TDA19988_REG_HDMI_EDID_CTRL, 0);
    CHECK_RESULT(result, "ReadEDID : toggle EDID 2 fail\n", Cleanup);

    DebugPrint(DEBUG_INFO, "Waiting for EDID Interrupt\n");

    elapsedTimeMilliSec = 0;
    do
    {
        MicroSecondDelay(5 * 1000);
        result = sTDARead(TDA19988_REG_HDMI_CTRL_INT_FLAGS_2, &value8);
        CHECK_RESULT(result, "ReadEDID : read TDA19988_REG_HDMI_CTRL_INT_FLAGS_2 fail\n", Cleanup);

        if (value8 & 2) {
            break;
        }

        elapsedTimeMilliSec += 5;

        if(elapsedTimeMilliSec > INTERRUPT_TIMEOUT_MS) {
            //
            // t-vivuo:
            // 1 in 5 times EDID will not trigger an interrupt. 
            //
            // return false in this case so we use the fall back timing
            //
            DebugPrint(DEBUG_ERROR, "EDID Interrupt Timeout. Continuing without EDID...\n");
            result = FALSE;
            goto Cleanup;
        }
        
    } while (TRUE);


    for (ix = 0; ix < IMX_EDID_MIN_SIZE; ix++) {
        result = sTDARead(TDA19988_REG_HDMI_EDID(ix), (EdidDataRead + ix));
        if(result == FALSE) {
            DebugPrint(DEBUG_ERROR, "ReadEDID Fail to read EDID %d\n", ix);
            goto Cleanup;
        }
    }

    // set ram power down
    result = sTDASetBits(TDA19988_REG_HDMI_HDCP_TX4, 2);
    CHECK_RESULT(result, "ReadEDID : set ram power down fail\n", Cleanup);

    {
        EFI_STATUS status;

        status = ImxValidateEdidData(
            EdidDataRead);
        if (RETURN_ERROR(status)) {
            DebugPrint(DEBUG_ERROR, "Invalid EDID data\n");
            result = FALSE;
            goto Cleanup;
        }
    }

    DebugPrint(DEBUG_INFO, "EDID Initialized\n");
#if 0
    // dump EDID
    for (ix = 0;ix < 128;ix++)
    {
        DebugPrint(DEBUG_INIT, "%02X ", *(EdidDataRead + ix));
        if ((ix & 0x7) == 7)
            DebugPrint(DEBUG_INIT, "\n");
    }
#endif

Cleanup:
    return result;
}

//
// Public Function
//

VOID
InitializeTDA(
    VOID
    )
{
    BOOLEAN  result;
    UINTN    numAttempts = 0;

    DEBUG((DEBUG_INFO, "InitalizeTDA\n"));

    sgCEC.SlaveAddr = UDOONEO_CEC_I2C2_ADDR;
    sgCEC.SlaveRegSizeBytes = 1;
    sgCEC.SlaveSpeed = 400000;
    sgCEC.XferTimeoutUs = 1000; 

    sgHDMI.SlaveAddr = UDOONEO_HDMI_I2C2_ADDR;
    sgHDMI.SlaveRegSizeBytes = 1;
    sgHDMI.SlaveSpeed = 400000;
    sgHDMI.XferTimeoutUs = 1000;

Start:
    result = sResetTDA();
    CHECK_RESULT(result, "InitializeTDA : sResetTDA fail\n", Failed);

    result = sCheckVersionInfo();
    CHECK_RESULT(result, "InitializeTDA : sCheckVersionInfo fail\n", Failed);

    result = sConfigureDDC();
    CHECK_RESULT(result, "InitializeTDA : sConfigureDDC fail\n", Failed);

    result = sConfigureInterrupts();
    CHECK_RESULT(result, "InitializeTDA : sConfigureInterrupts fail\n", Failed);

    result = sConfigureMisc();
    CHECK_RESULT(result, "InitializeTDA : sConfigureMisc fail\n", Failed);

    // pass through sync from LCD
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_TBG_CNTRL_1,
        TDA19988_HDMI_CTRL_TBG_CNTRL_1_DWIN_DIS |
        TDA19988_HDMI_CTRL_TBG_CNTRL_1_V_EXT |
        TDA19988_HDMI_CTRL_TBG_CNTRL_1_H_EXT |
        TDA19988_HDMI_CTRL_TBG_CNTRL_1_X_EXT |
        TDA19988_HDMI_CTRL_TBG_CNTRL_1_TOGGLE_EN);
    CHECK_RESULT(result, "InitializeTDA : setup pass through sync from LCD fail\n", Failed);

    return;

Failed:
    numAttempts++;
    if(numAttempts < MAX_NUM_ATTEMPTS) {    
        DebugPrint(DEBUG_INFO, "HDMI TDA configuration failed, Retrying\n");
        goto Start;
    }

     DebugPrint(DEBUG_ERROR, "HDMI TDA configuration failed after %d attempts\n", numAttempts);
}

EFI_STATUS
ConfigureTDA(
    IMX_DISPLAY_TIMING* Timing
    )
{
    BOOLEAN result;

    result = sConfigureVideoFormat(Timing);
    CHECK_RESULT(result, "ConfigureTDA : sConfigureVideoFormat fail\n", Failed);

    // enable
    result = sTDAWrite(TDA19988_REG_HDMI_CTRL_TBG_CNTRL_0, 0);
    CHECK_RESULT(result, "ConfigureTDA : write to TDA19988_REG_HDMI_CTRL_TBG_CNTRL_0 fail\n", Failed);

#if 0
    // check for display connection
    result = sTDARead(TDA19988_REG_CEC_RX_LEVEL, &value8);
    CHECK_RESULT(result, "ConfigureTDA : read to TDA19988_REG_CEC_RX_LEVEL fail\n", Failed);

    if (value8 & TDA19988_CEC_RX_HPD) {
        DebugPrint(DEBUG_INIT, "Display Connected\n");
    } else {
        DebugPrint(DEBUG_INIT, "Display not connected\n");
    }
#endif

    return EFI_SUCCESS;

Failed:
    return EFI_DEVICE_ERROR;
}