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
#include <Library/DebugLib.h>
#include <Library/IoLib.h>

#include <iMXDisplay.h>
#include <iMXI2cLib.h>
#include <iMXGpio.h>
#include <iMX7IoMux.h>

//
// SiI 164 PanelLink Transmitter
//
IMX_I2C_CONTEXT i2c4SIL164Config = {
    0x30A50000,     // I2C4 ControllerAddress;
    0x7F,           // ControllerSlaveAddress
    64000000,       // ReferenceFreq;
    100000,         // TargetFreq;
    0x39,           // SIL164 DeviceAddr;
    1000,           // TimeoutInUs
    };

#define SIL164_REG_VND_IDL                  0x00
#define SIL164_REG_VND_IDH                  0x01
#define SIL164_REG_DEV_IDL                  0x02
#define SIL164_REG_DEV_IDH                  0x03
#define SIL164_REG_DEV_REV                  0x04
#define SIL164_REG_RSVD                     0x05
#define SIL164_REG_FRQ_LOW                  0x06
#define SIL164_REG_FRQ_HIGH                 0x07

#define SIL164_REG_CONTROL0                 0x08
#define SIL164_REG_DETECT                   0x09
#define SIL164_REG_CONTROL1                 0x0A
#define SIL164_REG_GPIO                     0x0B
#define SIL164_REG_CONTROL2                 0x0C

#define SIL164_CONTROL0_POWER_OFF           0x00
#define SIL164_CONTROL0_POWER_ON            0x01
#define SIL164_CONTROL0_EDGE_FALLING        0x00
#define SIL164_CONTROL0_EDGE_RISING         0x02
#define SIL164_CONTROL0_INPUT_16BIT         0x00
#define SIL164_CONTROL0_INPUT_24BIT         0x04
#define SIL164_CONTROL0_SINGLE_EDGE         0x00
#define SIL164_CONTROL0_DUAL_EDGE           0x08
#define SIL164_CONTROL0_HSYNC_LOW           0x00
#define SIL164_CONTROL0_HSYNC_ON            0x10
#define SIL164_CONTROL0_VSYNC_LOW           0x00
#define SIL164_CONTROL0_VSYNC_ON            0x20

#define SIL164_REG_DETECT                   0x09
#define SIL164_DETECT_MDI                   0x01
#define SIL164_DETECT_HTPLG                 0x02    // RO
#define SIL164_DETECT_RSEN                  0x04    // RO
#define SIL164_DETECT_INTR_MODE_RECEIVER    0x00
#define SIL164_DETECT_INTR_MODE_HOTPLUG     0x08
#define SIL164_DETECT_OUT_MODE_HIGH         0x00
#define SIL164_DETECT_OUT_MODE_INTR         0x10
#define SIL164_DETECT_OUT_MODE_RECEIVER     0x20
#define SIL164_DETECT_OUT_MODE_HOTPLUG      0x30
#define SIL164_DETECT_VSWING_STAT           0x80

#define SIL164_REG_CONTROL2_PFEN_ENABLE     0x01
#define SIL164_REG_CONTROL2_SCNT_ENABLE     0x80

//
// PCA9555 16-bit I2C-bus and SMBus I/O port with interrupt
//
IMX_I2C_CONTEXT i2c4PCA9555Config = {
    0x30A50000,     // I2C4 ControllerAddress;
    0x7F,           // ControllerSlaveAddress
    64000000,       // ReferenceFreq;
    100000,         // TargetFreq;
    0x20,           // PCA9555 DeviceAddr;
    1000,           // TimeoutInUs
    };

IMX_I2C_CONTEXT i2c2EDIDConfig = {
    0x30A40000,     // I2C3 ControllerAddress;
    0x7F,           // ControllerSlaveAddress
    64000000,       // ReferenceFreq;
    100000,         // TargetFreq;
    IMX_EDID_I2C_ADDRESS,
    10000,          // TimeoutInUs
    };

//
// EDID read buffer
//
UINT8 edidBuffer[IMX_EDID_MIN_SIZE];

#define PCA955_REG_INPUT_PORT0      0x00
#define PCA955_REG_INPUT_PORT1      0x01
#define PCA955_REG_OUTPUT_PORT0     0x02
#define PCA955_REG_OUTPUT_PORT1     0x03
#define PCA955_REG_POL_INV_PORT0    0x04
#define PCA955_REG_POL_INV_PORT1    0x05
#define PCA955_REG_CFG_PORT0        0x06
#define PCA955_REG_CFG_PORT1        0x07

#define GPIO_DVI_EN_3V3             0x20 // 1.5
#define GPIO_LCD_NTSBY_3V3          0x40 // 1.6

VOID LcdifBoardInitialize ()
{
    UINT8 i2cData;
    RETURN_STATUS status;

    //
    // Dummy read since first read returns garbage
    //
    iMXI2cRead(&i2c4SIL164Config, SIL164_REG_VND_IDL, &i2cData, 1);

    //
    // Sanity check to make sure we are attached to a SIL164 chip.
    // Reading one byte at a time as SIL164 support only byte read and write.
    //
    status = iMXI2cRead(&i2c4SIL164Config, SIL164_REG_VND_IDL, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));
    ASSERT (i2cData == 0x01);
    status = iMXI2cRead(&i2c4SIL164Config, SIL164_REG_VND_IDH, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));
    ASSERT (i2cData == 0x00);
    status = iMXI2cRead(&i2c4SIL164Config, SIL164_REG_DEV_IDL, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));
    ASSERT (i2cData == 0x06);
    status = iMXI2cRead(&i2c4SIL164Config, SIL164_REG_DEV_IDH, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));
    ASSERT (i2cData == 0x00);

    //
    // Set lcd_nstby_3v3 and dvi_en_3v3 as output and high
    //
    status = iMXI2cRead(&i2c4PCA9555Config, PCA955_REG_CFG_PORT1, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));
    i2cData &= ~(GPIO_DVI_EN_3V3 | GPIO_LCD_NTSBY_3V3);
    status = iMXI2cWrite(&i2c4PCA9555Config, PCA955_REG_CFG_PORT1, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));

    status = iMXI2cRead(&i2c4PCA9555Config, PCA955_REG_OUTPUT_PORT1, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));
    i2cData |= (GPIO_DVI_EN_3V3 | GPIO_LCD_NTSBY_3V3);
    status = iMXI2cWrite(&i2c4PCA9555Config, PCA955_REG_OUTPUT_PORT1, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));

    DEBUG_CODE_BEGIN();
    status = iMXI2cRead(&i2c4PCA9555Config, PCA955_REG_INPUT_PORT0, &i2cData, 1);
    DEBUG((DEBUG_INIT, "PCA955_REG_INPUT_PORT0: 0x%04x\n", i2cData));
    status = iMXI2cRead(&i2c4PCA9555Config, PCA955_REG_INPUT_PORT1, &i2cData, 1);
    DEBUG((DEBUG_INIT, "PCA955_REG_INPUT_PORT1: 0x%04x\n", i2cData));
    status = iMXI2cRead(&i2c4PCA9555Config, PCA955_REG_OUTPUT_PORT0, &i2cData, 1);
    DEBUG((DEBUG_INIT, "PCA955_REG_OUTPUT_PORT0: 0x%04x\n", i2cData));
    status = iMXI2cRead(&i2c4PCA9555Config, PCA955_REG_OUTPUT_PORT1, &i2cData, 1);
    DEBUG((DEBUG_INIT, "PCA955_REG_OUTPUT_PORT1: 0x%04x\n", i2cData));
    status = iMXI2cRead(&i2c4PCA9555Config, PCA955_REG_POL_INV_PORT0, &i2cData, 1);
    DEBUG((DEBUG_INIT, "PCA955_REG_POL_INV_PORT0: 0x%04x\n", i2cData));
    status = iMXI2cRead(&i2c4PCA9555Config, PCA955_REG_POL_INV_PORT1, &i2cData, 1);
    DEBUG((DEBUG_INIT, "PCA955_REG_POL_INV_PORT1: 0x%04x\n", i2cData));
    status = iMXI2cRead(&i2c4PCA9555Config, PCA955_REG_CFG_PORT0, &i2cData, 1);
    DEBUG((DEBUG_INIT, "PCA955_REG_CFG_PORT0: 0x%04x\n", i2cData));
    status = iMXI2cRead(&i2c4PCA9555Config, PCA955_REG_CFG_PORT1, &i2cData, 1);
    DEBUG((DEBUG_INIT, "PCA955_REG_CFG_PORT1: 0x%04x\n", i2cData));
    DEBUG_CODE_END();

}

VOID LcdifBoardConfigureDisplay (
    IMX_DISPLAY_TIMING* Timing,
    VOID* FrameBuffer
    )
{
    UINT8 i2cData;
    RETURN_STATUS status;

    //
    // 24 bit and enable HSYNC and VSYNC
    //
    i2cData = SIL164_CONTROL0_INPUT_24BIT | SIL164_CONTROL0_HSYNC_ON | SIL164_CONTROL0_VSYNC_ON;
    status = iMXI2cWrite(&i2c4SIL164Config, SIL164_REG_CONTROL0, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));

    //
    // Do we actually need to setup interrupts?
    //
    i2cData = 0x20 | 0x01;
    status = iMXI2cWrite(&i2c4SIL164Config, SIL164_REG_CONTROL1, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));

    //
    // DK = 100 - 5 step -> default (recommended setting)
    //
    i2cData = (0x04 << 5);
    status = iMXI2cWrite(&i2c4SIL164Config, SIL164_REG_CONTROL1, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));

    //
    // PFEN - Enable
    // PLFF = 100 - Recommended value
    // SCNT - Enable
    //
    i2cData = SIL164_REG_CONTROL2_PFEN_ENABLE | 0x04 << 1 | SIL164_REG_CONTROL2_SCNT_ENABLE;
    status = iMXI2cWrite(&i2c4SIL164Config, SIL164_REG_CONTROL2, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));

    //
    // Power on the chip
    //
    status = iMXI2cRead(&i2c4SIL164Config, SIL164_REG_CONTROL0, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));

    i2cData |= SIL164_CONTROL0_POWER_ON;

    status = iMXI2cWrite(&i2c4SIL164Config, SIL164_REG_CONTROL0, &i2cData, 1);
    ASSERT (!RETURN_ERROR(status));

    DEBUG_CODE_BEGIN();
    status = iMXI2cRead(&i2c4SIL164Config, SIL164_REG_CONTROL0, &i2cData, 1);
    DEBUG((DEBUG_INIT, "SIL164_REG_CONTROL0: 0x%04x\n", i2cData));
    status = iMXI2cRead(&i2c4SIL164Config, SIL164_REG_DETECT, &i2cData, 1);
    DEBUG((DEBUG_INIT, "SIL164_REG_DETECT: 0x%04x\n", i2cData));
    status = iMXI2cRead(&i2c4SIL164Config, SIL164_REG_CONTROL1, &i2cData, 1);
    DEBUG((DEBUG_INIT, "SIL164_REG_CONTROL1: 0x%04x\n", i2cData));
    status = iMXI2cRead(&i2c4SIL164Config, SIL164_REG_GPIO, &i2cData, 1);
    DEBUG((DEBUG_INIT, "SIL164_REG_GPIO: 0x%04x\n", i2cData));
    status = iMXI2cRead(&i2c4SIL164Config, SIL164_REG_CONTROL2, &i2cData, 1);
    DEBUG((DEBUG_INIT, "SIL164_REG_CONTROL2: 0x%04x\n", i2cData));
    DEBUG_CODE_END();
}

typedef enum {

    IMX_PAD_GPIO1_IO08_I2C3_SCL = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_3_X6,
        IMX_SRE_SLOW,
        IMX_HYS_ENABLED,
        IMX_PE_0_Pull_Disabled,
        IMX_PS_3_100K_PU,
        IMX_SION_ENABLED,
        IMX_IOMUXC_GPIO1_IO08_ALT4_I2C3_SCL),

    IMX_PAD_GPIO1_IO09_I2C3_SDA = _IMX_MAKE_PADCFG(
        IMX_DSE_DSE_3_X6,
        IMX_SRE_SLOW,
        IMX_HYS_ENABLED,
        IMX_PE_0_Pull_Disabled,
        IMX_PS_3_100K_PU,
        IMX_SION_ENABLED,
        IMX_IOMUXC_GPIO1_IO09_ALT4_I2C3_SDA),

} IMX_I2C3_PADCFG;

RETURN_STATUS LcdifBoardQueryEdidTiming (
    OUT IMX_DISPLAY_TIMING* PreferredTiming
    )
{
    RETURN_STATUS status;
    UINT32 index;
    UINT8* edidDataReadPtr = &edidBuffer[0];

    ImxPadConfig(IMX_PAD_GPIO1_IO08, IMX_PAD_GPIO1_IO08_I2C3_SCL);
    ImxPadConfig(IMX_PAD_GPIO1_IO09, IMX_PAD_GPIO1_IO09_I2C3_SDA);

    status = iMXI2cRead(
        &i2c2EDIDConfig,
        0,
        (UINT8*)(edidDataReadPtr),
        ARRAY_SIZE(edidBuffer));
    ASSERT(!RETURN_ERROR(status));

    DEBUG_CODE_BEGIN();
    ImxPadDumpConfig("IMX_PAD_GPIO1_IO08", IMX_PAD_GPIO1_IO08);
    ImxPadDumpConfig("IMX_PAD_GPIO1_IO09", IMX_PAD_GPIO1_IO09);

    DEBUG((DEBUG_INIT, "EDID dump\n"));
    DEBUG((DEBUG_INIT, "=================================================\n"));

    for (index = 0; index < IMX_EDID_MIN_SIZE; ++index) {
        DEBUG((
            DEBUG_INIT,
            "EDID 0x%02x: 0x%02x\n",
            index, *(edidDataReadPtr + index)));
    }

    DEBUG((DEBUG_INIT, "=================================================\n"));
    DEBUG_CODE_END();

    status = ImxValidateEdidData(edidDataReadPtr);
    if (EFI_ERROR(status)) {
        DEBUG((DEBUG_INIT, "EDID data not valid\n"));
        goto End;
    }

    {
        IMX_DETAILED_TIMING_DESCRIPTOR* edidPreferredTiming;

        edidPreferredTiming =
            (IMX_DETAILED_TIMING_DESCRIPTOR*)&edidDataReadPtr[IMX_EDID_DTD_1_OFFSET];

        status = ImxConvertDTDToDisplayTiming(
            edidPreferredTiming,
            PreferredTiming);
        if (status != EFI_SUCCESS) {
            DEBUG((DEBUG_ERROR, "Conversion to display timing failed\n"));
            goto End;
        }
    }

End:
    return status;
}

