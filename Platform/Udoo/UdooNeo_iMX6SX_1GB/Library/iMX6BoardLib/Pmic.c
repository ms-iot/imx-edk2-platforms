//// 
//  Microsoft Coporation
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
#include <Library/TimerLib.h>

#include <iMX6.h>
#include <UdooNeoI2cLib.h>
#include "PF3000.h"

#include <UdooNeo.inc>

#define sReadPMIC(aRegId, ValueOutPtr) ReadI2CByte(UDOONEO_PMIC_I2C_UNIT, (aRegId), (ValueOutPtr), NULL)
#define sWritePMIC(aRegId, ValueIn) WriteI2CByte(UDOONEO_PMIC_I2C_UNIT, (aRegId), (ValueIn), NULL)

typedef union {
    UINT8 AsUint8;
    struct {
        // LSB
        UINT32 OutputVoltage : 5;       // 0-4 SW2 output voltage (SW2_OUTPUT_VOLTAGE_*)
        UINT32 OutputVoltageRange : 1;  // 5 SW2 Output Voltage (SW2_OUTPUT_VOLTAGE_RANGE)
        UINT32 Reserved : 2;
        // MSB
    };
} PF3000_SW2VOLT_REG;

typedef enum {
    SW2_OUTPUT_VOLTAGE_RANGE_LOW_VOLTAGE = 0,  // use SW2_OUTPUT_VOLTAGE_LOW
    SW2_OUTPUT_VOLTAGE_RANGE_HIGH_VOLTAGE = 1  // use SW2_OUTPUT_VOLTAGE_HIGH
} SW2_OUTPUT_VOLTAGE_RANGE;

typedef enum {
    SW2_OUTPUT_VOLTAGE_LOW_1V5 = 0,    // 1.500 V
    SW2_OUTPUT_VOLTAGE_LOW_1V55 = 1,   // 1.550 V
    SW2_OUTPUT_VOLTAGE_LOW_1V6= 2,     // 1.600 V
    SW2_OUTPUT_VOLTAGE_LOW_1V65 = 3,   // 1.650 V
    SW2_OUTPUT_VOLTAGE_LOW_1V7 = 4,    // 1.700 V
    SW2_OUTPUT_VOLTAGE_LOW_1V75 = 5,   // 1.750 V
    SW2_OUTPUT_VOLTAGE_LOW_1V8 = 6,    // 1.800 V
    SW2_OUTPUT_VOLTAGE_LOW_1V85 = 7    // 1.850 V
} SW2_OUTPUT_VOLTAGE_LOW;

typedef enum {
    SW2_OUTPUT_VOLTAGE_HIGH_2V5 = 0,    // 2.500 V
    SW2_OUTPUT_VOLTAGE_HIGH_2V8 = 1,    // 2.800 V
    SW2_OUTPUT_VOLTAGE_HIGH_2V85 = 2,   // 2.850 V
    SW2_OUTPUT_VOLTAGE_HIGH_3V0 = 3,    // 3.000 V
    SW2_OUTPUT_VOLTAGE_HIGH_3V1 = 4,    // 3.100 V
    SW2_OUTPUT_VOLTAGE_HIGH_3V15 = 5,   // 3.150 V
    SW2_OUTPUT_VOLTAGE_HIGH_3V2 = 6,    // 3.200 V
    SW2_OUTPUT_VOLTAGE_HIGH_3V3 = 7     // 3.300 V
} SW2_OUTPUT_VOLTAGE_HIGH;

VOID
ConfigurePMIC(
    VOID
    )
{
    BOOLEAN             result;
    UINT8               value8;
    IMX6SX_I2C_TARGET   PMIC;
    PF3000_SW2VOLT_REG  sw2Volt;

    PMIC.SlaveAddr = UDOONEO_PF3000_I2C1_ADDR;
    PMIC.SlaveRegSizeBytes = 1;
    PMIC.SlaveSpeed = 400000;
    PMIC.XferTimeoutUs = 1000;  // chip transmit is about 106 Microseconds. we do 9x that

    result = IMX6SX_I2C_SetTarget(1, &PMIC);
    ASSERT(result);

    // delay 5ms to let PMIC stabilize
    MicroSecondDelay(5000);

    result = sWritePMIC(PF3000_PAGE_SEL, 0);
    if (!result) {
        goto Failed;
    }

    result = sReadPMIC(PF3000_DEVICEID, &value8);
    if (!result) {
        goto Failed;
    }
    ASSERT(value8 == 0x30);

    result = sReadPMIC(PF3000_SW2VOLT, &sw2Volt.AsUint8);
    if (!result) {
        goto Failed;
    }
    sw2Volt.OutputVoltageRange = SW2_OUTPUT_VOLTAGE_RANGE_LOW_VOLTAGE;
    sw2Volt.OutputVoltage = SW2_OUTPUT_VOLTAGE_LOW_1V8;
    result = sWritePMIC(PF3000_SW2VOLT, sw2Volt.AsUint8);
    if (!result) {
        goto Failed;
    }

    // give it a millisecond to settle
    MicroSecondDelay(1000);

    DebugPrint(0xFFFFFFFF, "PMIC config done\n");
    return;

Failed:
    DebugPrint(0xFFFFFFFF, "PMIC config failed\n");
}