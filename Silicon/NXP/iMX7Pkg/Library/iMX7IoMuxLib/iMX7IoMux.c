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

#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>

#include <iMX7.h>
#include <iMX7IoMux.h>

//
// Muxing functions
//

VOID
ImxPadConfig (
  IMX_PAD Pad,
  IMX_PADCFG PadConfig
  )
{
    IMX_PAD_BITS pad = { Pad };
    IMX_PADCFG_BITS padConfig = { PadConfig };

    UINTN baseAddress;

    if(IMX_IOMUX_PAD_CTL_OFFSET(Pad) < IMX_IOMUX_PAD_CTL_OFFSET(IMX_PAD_GPIO1_IO08)) {
        baseAddress = IOMUXC_LPSR_SW_MUX_PAD_BASE_ADDRESS;
    } else {
        baseAddress = IOMUXC_SW_MUX_PAD_BASE_ADDRESS;
    }

    if(pad.Fields.MuxCtlRegOffset != 0) {
        MmioWrite32 (
            baseAddress + pad.Fields.MuxCtlRegOffset,
            _IMX_PADCFG_MUX_CTL(PadConfig));
    }

    if (pad.Fields.PadCtlRegOffset != 0) {
        MmioWrite32 (
            baseAddress + pad.Fields.PadCtlRegOffset,
            _IMX_PADCFG_PAD_CTL(PadConfig));
    }

    if(padConfig.Fields.InpSelReg != 0) {
        MmioWrite32 (
            _IMX_SEL_INP_REGISTER(_IMX_PADCFG_SEL_INP(PadConfig)),
            _IMX_SEL_INP_VALUE(_IMX_PADCFG_SEL_INP(PadConfig)));
    }
}

VOID
ImxPadDumpConfig (
  char *SignalFriendlyName,
  IMX_PAD Pad
  )
{
    IMX_IOMUXC_MUX_CTL muxCtl;
    UINTN baseAddress;

    if(IMX_IOMUX_PAD_CTL_OFFSET(Pad) < IMX_IOMUX_PAD_CTL_OFFSET(IMX_PAD_GPIO1_IO08)) {
        baseAddress = IOMUXC_LPSR_SW_MUX_PAD_BASE_ADDRESS;
    } else {
        baseAddress = IOMUXC_SW_MUX_PAD_BASE_ADDRESS;
    }

    muxCtl.AsUint32 = MmioRead32 (
        baseAddress + IMX_IOMUX_PAD_MUX_OFFSET(Pad));

    DEBUG ((
        DEBUG_INIT,
        "- %a MUX_CTL(0x%p)=0x%08x: MUX_MODE:%d SION:%d | ",
        SignalFriendlyName,
        baseAddress + IMX_IOMUX_PAD_MUX_OFFSET(Pad),
        muxCtl.AsUint32,
        muxCtl.Fields.MUX_MODE,
        muxCtl.Fields.SION));

    IMX_IOMUXC_PAD_CTL padCtl;
    padCtl.AsUint32 = MmioRead32 (
        baseAddress + IMX_IOMUX_PAD_CTL_OFFSET(Pad));

    DEBUG ((
        DEBUG_INIT,
        "PAD_CTL(0x%p)=0x%08x: DSE:%d SRE:%d HYS:%d PE:%d PS:%d\n",
        baseAddress + IMX_IOMUX_PAD_CTL_OFFSET(Pad),
        padCtl.AsUint32,
        padCtl.Fields.DSE,
        padCtl.Fields.SRE,
        padCtl.Fields.HYS,
        padCtl.Fields.PE,
        padCtl.Fields.PS));
}

//
// GPIO functions
//

VOID
ImxGpioDirection (
  IMX_GPIO_BANK Bank,
  UINT32 IoNumber,
  IMX_GPIO_DIR Direction
  )
{
    volatile IMX_GPIO_REGISTERS *gpioRegisters =
        (IMX_GPIO_REGISTERS *)IMX_GPIO_BASE;

    ASSERT (IoNumber < 32);

    if (Direction == IMX_GPIO_DIR_INPUT) {
        MmioAnd32 ((UINTN)&gpioRegisters->Banks[Bank - 1].GDIR, ~(1 << IoNumber));
    } else {
        MmioOr32 ((UINTN)&gpioRegisters->Banks[Bank - 1].GDIR, 1 << IoNumber);
    }
}

VOID
ImxGpioWrite (
  IMX_GPIO_BANK Bank,
  UINT32 IoNumber,
  IMX_GPIO_VALUE Value
  )
{
    volatile IMX_GPIO_REGISTERS *gpioRegisters =
        (IMX_GPIO_REGISTERS *)IMX_GPIO_BASE;

    ASSERT (IoNumber < 32);

    if (Value == IMX_GPIO_LOW) {
        MmioAnd32 ((UINTN)&gpioRegisters->Banks[Bank - 1].DR, ~(1 << IoNumber));
    } else {
        MmioOr32 ((UINTN)&gpioRegisters->Banks[Bank - 1].DR, 1 << IoNumber);
    }
}

IMX_GPIO_VALUE
ImxGpioRead (
  IMX_GPIO_BANK Bank,
  UINT32 IoNumber
  )
{
    volatile IMX_GPIO_REGISTERS *gpioRegisters =
        (IMX_GPIO_REGISTERS *)IMX_GPIO_BASE;

    ASSERT (IoNumber < 32);

    UINT32 Mask = (1 << IoNumber);
    UINT32 Psr = MmioRead32 ((UINTN)&gpioRegisters->Banks[Bank - 1].PSR);

    if (Psr & Mask) {
        return IMX_GPIO_HIGH;
    } else {
        return IMX_GPIO_LOW;
    }
}

