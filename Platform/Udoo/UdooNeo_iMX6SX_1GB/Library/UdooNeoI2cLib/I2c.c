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

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>

#include <iMX6.h>
#include <iMX6ClkPwr.h>
#include <UdooNeoI2cLib.h>

#include <imx6sxdef.inc>

#define DEBUG_TX            0
#define DEBUG_RX            0
#define DEBUG_FAILURES      1

#define I2C_NUMBER_ATTEMPTS 10

IMX6SX_I2C_TARGET const * gpI2CTarget[4] = { 0, };

static UINT32 sgI2CUnitClockRate = 0;

static const UINT16 sgClockDividers[] = {
    // #s are in pairs;
    // first # is divider;  
    // second # is byte to use in divider register for that divider
    22,    0x20 ,  
    24,    0x21 ,  
    26,    0x22 ,  
    28,    0x23 ,
    30,    0x00 ,  
    32,    0x24 ,  
    36,    0x25 ,  
    40,    0x26 ,
    42,    0x03 ,  
    44,    0x27 ,  
    48,    0x28 ,  
    52,    0x05 ,
    56,    0x29 ,  
    60,    0x06 ,  
    64,    0x2A ,  
    72,    0x2B ,
    80,    0x2C ,  
    88,    0x09 ,  
    96,    0x2D ,  
    104,   0x0A ,
    112,   0x2E ,  
    128,   0x2F ,  
    144,   0x0C ,  
    160,   0x30 ,
    192,   0x31 ,  
    224,   0x32 ,  
    240,   0x0F ,  
    256,   0x33 ,
    288,   0x10 ,  
    320,   0x34 ,  
    384,   0x35 ,  
    448,   0x36 ,
    480,   0x13 ,  
    512,   0x37 ,  
    576,   0x14 ,  
    640,   0x38 ,
    768,   0x39 ,  
    896,   0x3A ,  
    960,   0x17 ,  
    1024,  0x3B ,
    1152,  0x18 ,  
    1280,  0x3C ,  
    1536,  0x3D ,  
    1792,  0x3E ,
    1920,  0x1B ,  
    2048,  0x3F ,  
    2304,  0x1C ,  
    2560,  0x1D ,
    3072,  0x1E ,  
    3840,  0x1F  
};

INT_I2C_UNIT    gI2CUnit[4];

#define CLOCK_DIVIDERS_COUNT    (sizeof(sgClockDividers) / sizeof(UINT16))

static
VOID
UdooNeoI2cInit(
    VOID
    )
{
    gI2CUnit[0].Regs = IMX6SX_PHYSADDR_I2C1;
    gI2CUnit[1].Regs = IMX6SX_PHYSADDR_I2C2;
    gI2CUnit[2].Regs = IMX6SX_PHYSADDR_I2C3;
    gI2CUnit[3].Regs = IMX6SX_PHYSADDR_I2C4;
}

static
BOOLEAN
UdooNeoI2cIsInitialized(
    VOID
    )
{
    return gI2CUnit[0].Regs == IMX6SX_PHYSADDR_I2C1;
}

static
UINT8
sI2C_GetClockDivider(
    UINT32 TargetRate
    )
{
    UINT32 divVal;
    UINT32 divIx;

    ASSERT(TargetRate != 0);

    // bound at upper edge of divider
    // i.e. a divider less than the one we find is too fast
    divVal = (sgI2CUnitClockRate + TargetRate - 1) / TargetRate;

    if (divVal < sgClockDividers[0])
        return 0;

    if (divVal > sgClockDividers[CLOCK_DIVIDERS_COUNT - 2])
        return (UINT8)sgClockDividers[CLOCK_DIVIDERS_COUNT - 1];

    divIx = 0;
    do
    {
        if (sgClockDividers[divIx] >= divVal)
            break;
        divIx += 2;
    } while (TRUE);

    return (UINT8)sgClockDividers[divIx + 1];
}

static
BOOLEAN
sI2C_WaitStatusMaskAny(
    INT_I2C_UNIT *  pUnit,
    UINT16          FlagsMask,
    UINT32          TimeoutUs,
    UINT16 *        apRetStatus
    )
{
    UINT16  reg16Val;
    UINT32  delays;
    UINT32  maxdelays;

    delays = 0;
    maxdelays = TimeoutUs / 100;
    do
    {
        reg16Val = MmioRead16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2SR);
        if ((reg16Val & FlagsMask) != 0)
        {
            if (apRetStatus != NULL)
                *apRetStatus = reg16Val;

            return TRUE;
        }

        MicroSecondDelay(100);
        delays++;
    } while (delays < maxdelays);

    if (apRetStatus != NULL)
        *apRetStatus = reg16Val;

    return FALSE;
}

static
BOOLEAN
sI2C_WaitStatusMaskMatch(
    INT_I2C_UNIT *  pUnit,
    UINT16          FlagsMask,
    UINT16          FlagsMatch,
    UINT32          TimeoutUs,
    UINT16 *        apRetStatus
    )
{
    UINT16  reg16Val;
    UINT32  delays;
    UINT32  maxdelays;

    delays = 0;
    maxdelays = TimeoutUs / 100;
    do
    {
        reg16Val = MmioRead16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2SR);
        if ((reg16Val & FlagsMask) == FlagsMatch)
        {
            if (apRetStatus != NULL)
                *apRetStatus = reg16Val;

            return TRUE;
        }

        MicroSecondDelay(100);
        delays++;
    } while (delays < maxdelays);

    if (apRetStatus != NULL)
        *apRetStatus = reg16Val;

#if DEBUG_FAILURES
    DebugPrint(DEBUG_ERROR, "TIMEOUT\n");
#endif

    return FALSE;
}

static
void
sI2C_Stop(
    INT_I2C_UNIT * pUnit
    )
{
    UINT16  reg16Val;

    // generate stop
    reg16Val = MmioRead16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR);
    reg16Val &= ~(IMX6SX_I2C_I2CR_MSTA | IMX6SX_I2C_I2CR_MTX);
    MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR, reg16Val);

    // clear bits in I2SR
    MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2SR, 0);

    // wait for bus not busy (IBB == 0)
    if (!sI2C_WaitStatusMaskMatch(pUnit,
        IMX6SX_I2C_I2SR_IBB, 0,
        10 * 1000,
        NULL)) {
#if DEBUG_FAILURES
        DebugPrint(DEBUG_ERROR, "STOP IBB FAIL\n");
#endif
    }
}

static
BOOLEAN
sI2C_Tx(
    INT_I2C_UNIT *  pUnit,
    UINT8           DataByte
    )
{
    UINT16 reg16Val;

    // Clear bits in I2SR and let I2C Controller Settle
    MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2SR, 0);
    MicroSecondDelay(50);

#if DEBUG_TX
    DebugPrint(0xFFFFFFFF, "TX %02X\n", DataByte);
#endif

    // Out you go
    MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2DR, DataByte);

    if (!sI2C_WaitStatusMaskAny(pUnit,
        IMX6SX_I2C_I2SR_IAL | IMX6SX_I2C_I2SR_IIF | IMX6SX_I2C_I2SR_ICF,
        pUnit->Target.XferTimeoutUs,
        &reg16Val))
    {
#if DEBUG_FAILURES
        DebugPrint(0xFFFFFFFF, "TX TIMEOUT\n");
#endif
        return FALSE;   // timed out
    }

    if (reg16Val & IMX6SX_I2C_I2SR_ICF) {
        return TRUE;
    }

#if DEBUG_FAILURES
    if (reg16Val & IMX6SX_I2C_I2SR_IAL) {
        DebugPrint(0xFFFFFFFF, "TX ERROR: ARB LOST\n");
    }
    else if (reg16Val & IMX6SX_I2C_I2SR_RXAK) {
        DebugPrint(0xFFFFFFFF, "TX ERROR: NO ACK SIGNAL DETECTED AT NINETH CLK\n");
    }
    else {
        DebugPrint(0xFFFFFFFF, "TX ERROR: UNKNOWN ERROR\n");
    }
#endif

    return FALSE;
}

static
BOOLEAN
sI2C_StartTransfer(
    INT_I2C_UNIT *  pUnit,
    UINT8 const *   SlaveReg
    )
{
    UINT16  reg16Val;
    UINT32  regBytes;
    BOOLEAN result;

    // Clear bits in I2SR
    MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2SR, 0);

    // wait for bus not busy
    if (!sI2C_WaitStatusMaskMatch(pUnit,
        IMX6SX_I2C_I2SR_IBB, 0,
        10 * 1000,
        &reg16Val)) {
#if DEBUG_FAILURES
        DebugPrint(DEBUG_ERROR, "START XFER - BUS BUSY\n");
#endif
        return FALSE;
    }

    // set master mode bit
    reg16Val = MmioRead16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR);
    reg16Val |= IMX6SX_I2C_I2CR_MSTA;
    MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR, reg16Val);

    // wait for bus to go busy
    if (!sI2C_WaitStatusMaskMatch(pUnit,
        IMX6SX_I2C_I2SR_IBB, IMX6SX_I2C_I2SR_IBB,
        100 * 1000,
        &reg16Val))
    {
#if DEBUG_FAILURES
        DebugPrint(0xFFFFFFFF, "MASTER SET TIMEOUT(0x%04X)\n", reg16Val);
#endif
        return FALSE;
    }

    result = FALSE;

    do
    {
        // set TX and NTXAK bit
        reg16Val |= IMX6SX_I2C_I2CR_MTX | IMX6SX_I2C_I2CR_TXAK;
        MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR, reg16Val);

        // send slave address with R/nW bit set to zero
        if (!sI2C_Tx(pUnit, (UINT8)(pUnit->Target.SlaveAddr << 1)))
        {
#if DEBUG_FAILURES
            DebugPrint(0xFFFFFFFF, "SLAVEADDR TX FAIL\n");
#endif
            break;
        }

        // send register address bytes
        regBytes = pUnit->Target.SlaveRegSizeBytes;
        do
        {
            if (!sI2C_Tx(pUnit, *SlaveReg))
            {
#if DEBUG_FAILURES
                DebugPrint(0xFFFFFFFF, "SLAVEREG TX FAIL\n");
#endif
                break;
            }
            SlaveReg++;
        } while (--regBytes > 0);

        result = TRUE;

    } while (FALSE);

    if (!result)
        sI2C_Stop(pUnit);

    return result;
}

BOOLEAN
IMX6SX_I2C_Read(
    UINT32  UnitNum,
    UINT32  SlaveReg,
    UINT32  ReadByteCount,
    UINT8 * ReadBuffer
    )
{
    UINT16          reg16Val;
    BOOLEAN         result;
    INT_I2C_UNIT *  pUnit;

    ASSERT(ReadByteCount != 0);
    ASSERT(ReadBuffer != NULL);
    ASSERT((UnitNum > 0) && (UnitNum < 5));
    pUnit = &gI2CUnit[UnitNum - 1];

    if (!sI2C_StartTransfer(pUnit, (UINT8 const *)&SlaveReg)) {
#if DEBUG_FAILURES
        DebugPrint(DEBUG_ERROR, "READ XFER START FAIL\n");
#endif
        return FALSE;
    }

    result = FALSE;

    do
    {
        // repeated start
        reg16Val = MmioRead16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR);
        reg16Val |= IMX6SX_I2C_I2CR_RSTA;
        MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR, reg16Val);

        // transmit slave address + R/nW bit set to 1
        if (!sI2C_Tx(pUnit, (UINT8)(pUnit->Target.SlaveAddr << 1) | 1)) {
#if DEBUG_FAILURES
            DebugPrint(DEBUG_ERROR, "SLAVEADDR RX FAIL\n");
#endif
            break;
        }

        // clear the master transmit and no-transmit-acknowledge
        reg16Val = MmioRead16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR);
        reg16Val &= ~(IMX6SX_I2C_I2CR_MTX | IMX6SX_I2C_I2CR_TXAK);
        if (ReadByteCount == 1)
        {
            // first byte read is last byte, set no-transmit-acknowledge.
            // unfortunately named bit. should be NTXAK
            reg16Val |= IMX6SX_I2C_I2CR_TXAK;
        }
        MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR, reg16Val);
    
        // Clear bits in I2SR
        MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2SR, 0);

        // Clear out ICF flag of I2SR by reading from I2DR
        reg16Val = MmioRead16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2DR);

        do
        {
            // wait for data (IIF == 1)
            if (!sI2C_WaitStatusMaskMatch(pUnit,
                IMX6SX_I2C_I2SR_ICF, IMX6SX_I2C_I2SR_ICF,
                pUnit->Target.XferTimeoutUs,
                &reg16Val)) {
#if DEBUG_FAILURES
                DebugPrint(DEBUG_ERROR, "DATA RX TIMEOUT\n");
#endif
                break;
            }

            if (ReadByteCount == 1)
            {
                // last byte came in
                result = TRUE;
                reg16Val = MmioRead16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR);
                reg16Val &= ~IMX6SX_I2C_I2CR_MSTA;
                MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR, reg16Val);
            }
            else if (ReadByteCount == 2)
            {
                // last byte about to come in so set the NTXAK bit
                reg16Val = MmioRead16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR);
                reg16Val |= IMX6SX_I2C_I2CR_TXAK;
                MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR, reg16Val);
            }

            // Clear bits in I2SR
            MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2SR, 0);

            // Get the data that came in
            *ReadBuffer = (UINT8)MmioRead16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2DR);
#if DEBUG_RX
            DebugPrint(0xFFFFFFFF, "RX %02X\n", *ReadBuffer);
#endif
            ReadBuffer++;

        } while (--ReadByteCount > 0);

    } while (FALSE);

//     // TODO: Check with Kurt on this
//     if (result)
//     {
//         if (!sI2C_WaitStatusMaskMatch(pUnit,
//             IMX6SX_I2C_I2SR_SRW | IMX6SX_I2C_I2SR_IIF, IMX6SX_I2C_I2SR_SRW | IMX6SX_I2C_I2SR_IIF,
//             100 * 1000,
//             &reg16Val))
//         {
// #if DEBUG_FAILURES
//             DebugPrint(0xFFFFFFFF, "SRW Not Set after last recv (%04X)!\n", reg16Val);
// #endif
//         }
//     }

    sI2C_Stop(pUnit);

    return result;
}

BOOLEAN
IMX6SX_I2C_Write(
    UINT32                      UnitNum,
    UINT32                      SlaveReg,
    UINT32                      WriteByteCount,
    UINT8 const *               WriteBuffer
    )
{
    INT_I2C_UNIT *  pUnit;

    ASSERT(WriteByteCount != 0);
    ASSERT(WriteBuffer != NULL);
    ASSERT((UnitNum > 0) && (UnitNum < 5));
    pUnit = &gI2CUnit[UnitNum - 1];

    if (!sI2C_StartTransfer(pUnit, (UINT8 const *)&SlaveReg)) {
#if DEBUG_FAILURES
        DebugPrint(DEBUG_ERROR, "WRITE XFER START FAIL\n");
#endif
        return FALSE;
    }

    do
    {
        if (!sI2C_Tx(pUnit, *WriteBuffer)) {
#if DEBUG_FAILURES
            DebugPrint(DEBUG_ERROR, "WRITE FAIL\n");
#endif
            break;
        }
        WriteBuffer++;
    } while (--WriteByteCount > 0);

    sI2C_Stop(pUnit);

    return (WriteByteCount == 0) ? TRUE : FALSE;
}

BOOLEAN
IMX6SX_I2C_SetTarget(
    UINT32                      UnitNum,
    IMX6SX_I2C_TARGET const *   Target
    )
{
    UINT32          reg16Val;
    INT_I2C_UNIT *  pUnit;
    EFI_STATUS      status;
    IMX_CLOCK_INFO  i2cClockInfo;

    ASSERT(Target != NULL);
    ASSERT((UnitNum > 0) && (UnitNum < 5));

    if (!UdooNeoI2cIsInitialized()) {
        UdooNeoI2cInit();
    }

    CopyMem(&gI2CUnit[UnitNum-1].Target, Target, sizeof(IMX6SX_I2C_TARGET));
    pUnit = &gI2CUnit[UnitNum-1];

    status = ImxClkPwrGetClockInfo(IMX_PERCLK_CLK_ROOT, &i2cClockInfo);
    if (EFI_ERROR(status)) {
#if DEBUG_FAILURES
        DebugPrint(DEBUG_ERROR, "Unable to Get I2C Clock Info\n");
#endif
        return FALSE;
    }

    // All units have the same input clock
    sgI2CUnitClockRate = i2cClockInfo.Frequency;

    // ensure controller is disabled
    reg16Val = MmioRead16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR);
    if ((reg16Val & IMX6SX_I2C_I2CR_IEN) != 0)
    {
        MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR, 0);
        MicroSecondDelay(1000);
    }

    // clear the status register
    MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2SR, 0);

    // set our own slave address to something that is NOT who
    // we are targeting by changing the lowest bit
    MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_IADR, (UINT16)(Target->SlaveAddr ^ 1) << 1);

    // set the divider register value for the proper divider
    MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_IFDR, sI2C_GetClockDivider(Target->SlaveSpeed));

    // enable the controller and allow it to settle
    MmioWrite16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR, IMX6SX_I2C_I2CR_IEN);
    MicroSecondDelay(500);
    // confirm controller is enabled
    if (!(MmioRead16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2CR) & IMX6SX_I2C_I2CR_IEN)) {
#if DEBUG_FAILURES
        DebugPrint(DEBUG_ERROR, "I2C%d did not enable\n", UnitNum);
#endif 
        ASSERT(FALSE);
        return FALSE;
    }

    // confirm bus is not busy
    reg16Val = MmioRead16(pUnit->Regs + IMX6SX_I2C_OFFSET_I2SR);
    return ((reg16Val & IMX6SX_I2C_I2SR_IBB) == 0) ? TRUE : FALSE;
}

BOOLEAN
ReadI2CByte(
    UINTN Unit,
    UINT32 Register,
    UINT8 *Value,
    PFN_InitI2C Callback
    )
{
    BOOLEAN  result = TRUE;
    UINT8    value8 = 0;
    UINTN    i = 0;

    DebugPrint(
        DEBUG_INFO,
        "Read Unit %d Register 0x%08X\n",
        Unit,
        Register);

    for (i = 0; i < I2C_NUMBER_ATTEMPTS; i++) {
        if (Callback != NULL) {
            result = Callback(Register);
            if (!result) {
                continue;
            }
        }


        result = IMX6SX_I2C_Read(Unit, (UINT8)Register, 1, &value8);
        if (result) {
            *Value = value8;
            return TRUE;
        }
    }

    DebugPrint(
        DEBUG_ERROR,
        "Read Unit %d Register 0x%08X failed after %d Attempts\n",
        Unit,
        Register,
        I2C_NUMBER_ATTEMPTS);

    return FALSE;
}

BOOLEAN
WriteI2CByte(
    UINTN Unit,
    UINT32 Register,
    UINT8 Value8,
    PFN_InitI2C Callback
    )
{
    BOOLEAN  result = TRUE;
    UINTN    i = 0;

    DebugPrint(
        DEBUG_INFO,
        "Write Unit %d Register 0x%08X\n",
        Unit,
        Register);

    for (i = 0; i < I2C_NUMBER_ATTEMPTS; i++) {
        if (Callback != NULL) {
            result = Callback(Register);
            if (!result) {
                continue;
            }
        }

        result = IMX6SX_I2C_Write(Unit, (UINT8)Register, 1, &Value8);
        if(result) {
            return TRUE;
        }
    }

    DebugPrint(
        DEBUG_INFO, 
        "Write Unit %d Register 0x%08X failed after %d Attempts\n",
        Unit,
        Register,
        I2C_NUMBER_ATTEMPTS);

    return FALSE;
}