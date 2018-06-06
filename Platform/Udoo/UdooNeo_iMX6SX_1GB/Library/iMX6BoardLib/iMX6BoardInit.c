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

#include <Library/IoLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>

#include <iMX6.h>
#include <iMX6ClkPwr.h>
#include <iMX6IoMux.h>

#include <UdooNeoI2cLib.h>

#include <UdooNeo.inc>
#include <imx6sxdef.inc>

#define SerialPrint(txt)  SerialPortWrite ((UINT8*)txt, AsciiStrLen(txt)+1);

static void MmioSetBits32(UINT32 Addr, UINT32 Bits)
{
    MmioWrite32(Addr, MmioRead32(Addr) | Bits);
}

static void MmioClrBits32(UINT32 Addr, UINT32 Bits)
{
    MmioWrite32(Addr, MmioRead32(Addr) & ~Bits);
}

static void MmioClrBits16(UINT32 Addr, UINT16 Bits)
{
    MmioWrite16(Addr, MmioRead16(Addr) & ~Bits);
}

/**
  Initialize clock and power for modules on the SOC.
**/
VOID ImxClkPwrInit ()
{
}

/**
  Initialize SDHC modules on the SOC and perform required pin-muxing
**/
VOID SdhcInit ()
{
}

/**
  Initialize EHCI modules on the SOC and perform required pin-muxing
**/
VOID EhciInit ()
{
}

/**
  Initialize SPI modules on the SOC and perform required pin-muxing
**/
VOID SpiInit ()
{
}

/**
  Initialize PCI Express module on the SOC and perform required pin-muxing
**/
VOID PcieInit ()
{
}

VOID
ConfigurePMIC(
    VOID
    );

static 
VOID 
sClearI2C(
    UINT32 gpioBase,
    UINT32 bitSCL,
    UINT32 bitSDA
    )
{
    UINT32 reg32Val;
    UINT32 ix;

    bitSCL = (1 << bitSCL);
    bitSDA = (1 << bitSDA);

    // push SCL high for 1 ms
    MmioClrBits16((gpioBase + IMX6SX_GPIO_OFFSET_GDIR), bitSCL);
    MmioClrBits16((gpioBase + IMX6SX_GPIO_OFFSET_DR), bitSCL);
    MicroSecondDelay(1000);
    // send high and low on SCL for 9 cycles
    for (ix = 0; ix < 9; ix++)
    {
        MmioClrBits16((gpioBase + IMX6SX_GPIO_OFFSET_DR), bitSCL);
        MicroSecondDelay(50);
        MmioClrBits32((gpioBase + IMX6SX_GPIO_OFFSET_DR), bitSCL);
        MicroSecondDelay(50);
    }
    // simulate NACK
    MmioClrBits16((gpioBase + IMX6SX_GPIO_OFFSET_GDIR), bitSDA);
    MmioClrBits16((gpioBase + IMX6SX_GPIO_OFFSET_DR), bitSDA);
    MicroSecondDelay(50);
    MmioClrBits16((gpioBase + IMX6SX_GPIO_OFFSET_DR), bitSCL);
    MicroSecondDelay(50);
    MmioClrBits32((gpioBase + IMX6SX_GPIO_OFFSET_DR), bitSCL);
    MicroSecondDelay(50);

    // simulate STOP
    MmioClrBits32((gpioBase + IMX6SX_GPIO_OFFSET_DR), bitSDA);
    MicroSecondDelay(50);
    MmioClrBits16((gpioBase + IMX6SX_GPIO_OFFSET_DR), bitSCL);
    MicroSecondDelay(50);
    MmioClrBits16((gpioBase + IMX6SX_GPIO_OFFSET_DR), bitSDA);
    MicroSecondDelay(50);

    // SCL, SDA back to inputs
    MmioClrBits32((gpioBase + IMX6SX_GPIO_OFFSET_GDIR), bitSCL | bitSDA);
    do
    {
        reg32Val = MmioRead32(gpioBase + IMX6SX_GPIO_OFFSET_DR);
    } while ((reg32Val & (bitSCL | bitSDA)) != (bitSCL | bitSDA));

}

/**
  Initialize I2C modules on the SOC and perform required pin-muxing
**/
VOID
I2cInit(
  VOID
  )
{
    UINT32 reg32Val;
    UINT16 reg16Val;

    //------------------------------------------------------------------------
    // I2C1 is PMIC, TOUCH(CN3), and J11
    // SCL GPIO1-0 pad IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO00
    // SDA GPIO1-1 pad IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO01

    // disable I2C1, then whack the whole register
    MmioClrBits16(IMX6SX_PHYSADDR_I2C1_I2CR, IMX6SX_I2C_I2CR_IEN);
    MmioWrite16(IMX6SX_PHYSADDR_I2C1_I2CR, 0);

    // configure I2C1 pads
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_GPIO1_IO00,
        IMX6SX_MUX_PADCTL_HYS              |
        IMX6SX_MUX_PADCTL_PUS_100KOHM_PU   |
        IMX6SX_MUX_PADCTL_PUE              |
        IMX6SX_MUX_PADCTL_PKE              |
        IMX6SX_MUX_PADCTL_ODE              |
        IMX6SX_MUX_PADCTL_SPEED_100MHZ     |
        IMX6SX_MUX_PADCTL_DSE_43_OHM       |
        IMX6SX_MUX_PADCTL_SRE_SLOW);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_GPIO1_IO01,
        IMX6SX_MUX_PADCTL_HYS              |
        IMX6SX_MUX_PADCTL_PUS_100KOHM_PU   |
        IMX6SX_MUX_PADCTL_PUE              |
        IMX6SX_MUX_PADCTL_PKE              |
        IMX6SX_MUX_PADCTL_ODE              |
        IMX6SX_MUX_PADCTL_SPEED_100MHZ     |
        IMX6SX_MUX_PADCTL_DSE_43_OHM       |
        IMX6SX_MUX_PADCTL_SRE_SLOW);

    // set SCL and SDA as GPIO, both as inputs to check bus is idle
    MmioClrBits32(IMX6SX_PHYSADDR_GPIO1_GDIR, (1 << 0) | (1 << 1));
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO00, 5);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO01, 5);
    reg32Val = MmioRead32(IMX6SX_PHYSADDR_GPIO1_DR);
    if ((reg32Val & ((1 << 0) | (1 << 1))) != ((1 << 0) | (1 << 1)))
    {
        // I2C1 bus is not idle. manually force it idle
        sClearI2C(IMX6SX_PHYSADDR_GPIO1, 0, 1);
    }
    // I2C1 bus is idle. now configure lines as I2C
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_I2C1_IPP_SCL_IN_SELECT_INPUT, 1);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO00, IMX6SX_MUX_MODE_SION);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_I2C1_IPP_SDA_IN_SELECT_INPUT, 1);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO01, IMX6SX_MUX_MODE_SION);
    //------------------------------------------------------------------------


    //------------------------------------------------------------------------
    // I2C2 is J6 and J10
    // SCL GPIO1-2 pad IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO02
    // SDA GPIO1-3 pad IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO03

    // disable I2C2
    MmioClrBits16(IMX6SX_PHYSADDR_I2C2_I2CR, IMX6SX_I2C_I2CR_IEN);
    MmioWrite16(IMX6SX_PHYSADDR_I2C2_I2CR, 0);

    // configure I2C2 pads
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_GPIO1_IO02,
        IMX6SX_MUX_PADCTL_HYS              |
        IMX6SX_MUX_PADCTL_PUS_100KOHM_PU   |
        IMX6SX_MUX_PADCTL_PUE              |
        IMX6SX_MUX_PADCTL_PKE              |
        IMX6SX_MUX_PADCTL_ODE              |
        IMX6SX_MUX_PADCTL_SPEED_100MHZ     |
        IMX6SX_MUX_PADCTL_DSE_43_OHM       |
        IMX6SX_MUX_PADCTL_SRE_SLOW);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_GPIO1_IO03,
        IMX6SX_MUX_PADCTL_HYS              |
        IMX6SX_MUX_PADCTL_PUS_100KOHM_PU   |
        IMX6SX_MUX_PADCTL_PUE              |
        IMX6SX_MUX_PADCTL_PKE              |
        IMX6SX_MUX_PADCTL_ODE              |
        IMX6SX_MUX_PADCTL_SPEED_100MHZ     |
        IMX6SX_MUX_PADCTL_DSE_43_OHM       |
        IMX6SX_MUX_PADCTL_SRE_SLOW);

    // set SCL and SDA as GPIO, both as inputs to check bus is idle
    MmioClrBits32(IMX6SX_PHYSADDR_GPIO1_GDIR, (1 << 2) | (1 << 3));
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO02, 5);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO03, 5);
    reg32Val = MmioRead32(IMX6SX_PHYSADDR_GPIO1_DR);
    if ((reg32Val & ((1 << 2) | (1 << 3))) != ((1 << 2) | (1 << 3)))
    {
        // I2C2 bus is not idle. manually force it idle
        sClearI2C(IMX6SX_PHYSADDR_GPIO1, 2, 3);
    }
    // I2C2 bus is idle. now configure lines as I2C
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_I2C2_IPP_SCL_IN_SELECT_INPUT, 1);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO02, IMX6SX_MUX_MODE_SION);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_I2C2_IPP_SDA_IN_SELECT_INPUT, 1);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO03, IMX6SX_MUX_MODE_SION);
    //------------------------------------------------------------------------


    //------------------------------------------------------------------------
    // I2C3 is HDMI(TDA19988BHN)
    // SCL GPIO2-14 pad IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_KEY_COL4
    // SDA GPIO2-19 pad IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_KEY_ROW4

    // disable I2C3
    reg16Val = MmioRead16(IMX6SX_PHYSADDR_I2C3_I2CR);
    reg16Val &= ~IMX6SX_I2C_I2CR_IEN;
    MmioWrite16(IMX6SX_PHYSADDR_I2C3_I2CR, reg16Val);
    MmioWrite16(IMX6SX_PHYSADDR_I2C3_I2CR, 0);

    // configure I2C3 pads
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_KEY_COL4,
        IMX6SX_MUX_PADCTL_HYS              |
        IMX6SX_MUX_PADCTL_PUS_100KOHM_PU   |
        IMX6SX_MUX_PADCTL_PUE              |
        IMX6SX_MUX_PADCTL_PKE              |
        IMX6SX_MUX_PADCTL_ODE              |
        IMX6SX_MUX_PADCTL_SPEED_100MHZ     |
        IMX6SX_MUX_PADCTL_DSE_43_OHM       |
        IMX6SX_MUX_PADCTL_SRE_SLOW);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_KEY_ROW4,
        IMX6SX_MUX_PADCTL_HYS              |
        IMX6SX_MUX_PADCTL_PUS_100KOHM_PU   |
        IMX6SX_MUX_PADCTL_PUE              |
        IMX6SX_MUX_PADCTL_PKE              |
        IMX6SX_MUX_PADCTL_ODE              |
        IMX6SX_MUX_PADCTL_SPEED_100MHZ     |
        IMX6SX_MUX_PADCTL_DSE_43_OHM       |
        IMX6SX_MUX_PADCTL_SRE_SLOW);

    // set SCL and SDA as GPIO, both as inputs to check bus is idle
    MmioClrBits32(IMX6SX_PHYSADDR_GPIO2_GDIR, (1 << 14) | (1 << 19));
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_KEY_COL4, 5);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_KEY_ROW4, 5);
    reg32Val = MmioRead32(IMX6SX_PHYSADDR_GPIO2_DR);
    if ((reg32Val & ((1 << 14) | (1 << 19))) != ((1 << 14) | (1 << 19)))
    {
        // I2C3 bus is not idle. manually force it idle
        sClearI2C(IMX6SX_PHYSADDR_GPIO2, 14, 19);
    }

    // I2C3 bus is idle. now configure lines as I2C
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_I2C3_IPP_SCL_IN_SELECT_INPUT, 2);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_KEY_COL4, 2 | IMX6SX_MUX_MODE_SION);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_I2C3_IPP_SDA_IN_SELECT_INPUT, 2);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_KEY_ROW4, 2 | IMX6SX_MUX_MODE_SION);
    //------------------------------------------------------------------------

    //------------------------------------------------------------------------
    // I2C4 is Accelerometer(FXOS8700CQ) and Gyroscope(FXAS21002CQR1)
    // SCL USB_H_STROBE pad IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_USB_H_STROBE -- GPIO7_IO11
    // SDA USB_H_DATA pad   IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_USB_H_DATA -- GPIO7_IO10

    // disable I2C4
    reg16Val = MmioRead16(IMX6SX_PHYSADDR_I2C4_I2CR);
    reg16Val &= ~IMX6SX_I2C_I2CR_IEN;
    MmioWrite16(IMX6SX_PHYSADDR_I2C4_I2CR, reg16Val);
    MmioWrite16(IMX6SX_PHYSADDR_I2C4_I2CR, 0);

    // configure I2C4 pads
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_USB_H_STROBE,
        IMX6SX_MUX_PADCTL_HYS              |
        IMX6SX_MUX_PADCTL_PUS_100KOHM_PU   |
        IMX6SX_MUX_PADCTL_PUE              |
        IMX6SX_MUX_PADCTL_PKE              |
        IMX6SX_MUX_PADCTL_ODE              |
        IMX6SX_MUX_PADCTL_SPEED_100MHZ     |
        IMX6SX_MUX_PADCTL_DSE_43_OHM       |
        IMX6SX_MUX_PADCTL_SRE_SLOW);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_USB_H_DATA,
        IMX6SX_MUX_PADCTL_HYS              |
        IMX6SX_MUX_PADCTL_PUS_100KOHM_PU   |
        IMX6SX_MUX_PADCTL_PUE              |
        IMX6SX_MUX_PADCTL_PKE              |
        IMX6SX_MUX_PADCTL_ODE              |
        IMX6SX_MUX_PADCTL_SPEED_100MHZ     |
        IMX6SX_MUX_PADCTL_DSE_43_OHM       |
        IMX6SX_MUX_PADCTL_SRE_SLOW);

    // set SCL and SDA as GPIO, both as inputs to check bus is idle
    MmioClrBits32(IMX6SX_PHYSADDR_GPIO7_GDIR, (1 << 11) | (1 << 10));
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_USB_H_STROBE, 5);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_USB_H_DATA, 5);
    reg32Val = MmioRead32(IMX6SX_PHYSADDR_GPIO7_DR);
    if ((reg32Val & ((1 << 11) | (1 << 10))) != ((1 << 11) | (1 << 10)))
    {
        // I2C3 bus is not idle. manually force it idle
        sClearI2C(IMX6SX_PHYSADDR_GPIO7, 11, 10);
    }
    // I2C4 bus is idle. now configure lines as I2C
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_I2C4_IPP_SCL_IN_SELECT_INPUT, 1);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_USB_H_STROBE, 3 | IMX6SX_MUX_MODE_SION);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_I2C4_IPP_SDA_IN_SELECT_INPUT, 1);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_USB_H_DATA, 3 | IMX6SX_MUX_MODE_SION);
    //------------------------------------------------------------------------

    DebugPrint(0xFFFFFFFF, "I2C init done\n");
}

VOID
HdmiInit(
    VOID
    )
{
    UINT32 reg32Val;

    // GPIO3-27 is HDMI interrupt GPIO in
    MmioClrBits32(IMX6SX_PHYSADDR_GPIO3_GDIR, (1 << 27));
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_RESET, 0x05);

    // GPIO1-12 is CCM_CLKO2 for HDMI
    /* Set clock CCM_CLKO2 to 12MHz (24Mhz oscillator div 2) */
    reg32Val = MmioRead32(IMX_CCM_BASE + IMX6SX_CCM_OFFSET_CCOSR);
    reg32Val &= ~(IMX6SX_CCM_CCOSR_CLKO2_DIV_MASK | IMX6SX_CCM_CCOSR_CLKO2_SEL_MASK);
    reg32Val |= (IMX6SX_CCM_CCOSR_CLKO2_EN |
                 (1 << IMX6SX_CCM_CCOSR_CLKO2_DIV_SHL) |
                 IMX6SX_CCM_CCOSR_CLKO2_SEL_OSC_CLK |
                IMX6SX_CCM_CCOSR_CLK_OUT_SEL_CLKO2);
    MmioWrite32(IMX_CCM_BASE + IMX6SX_CCM_OFFSET_CCOSR, reg32Val);

    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO12, 0x03);

    DebugPrint(0xFFFFFFFF, "HDMI init done\n");
}

VOID
LcdInit(
    VOID
    )
{
    UINT32 reg32Val;

    // turn off LCD module clock
    ImxClkPwrLcdClockDisable();

    // turn off lcd pixel clock and display axi clock
    MmioClrBits32(IMX_CCM_BASE + IMX6SX_CCM_OFFSET_CCGR3, 
        (IMX6SX_RUN_AND_WAIT << IMX6SX_SHL_CCM_CCGR3_LCDIF1_PIX) |
        (IMX6SX_RUN_AND_WAIT << IMX6SX_SHL_CCM_CCGR3_DISPLAY_AXI)
        );

    // almost all LCD pads have this config
    reg32Val = IMX6SX_MUX_PADCTL_HYS |
        IMX6SX_MUX_PADCTL_PUS_100KOHM_PU |
        IMX6SX_MUX_PADCTL_PUE |
        IMX6SX_MUX_PADCTL_PKE |
        IMX6SX_MUX_PADCTL_SPEED_100MHZ |
        IMX6SX_MUX_PADCTL_DSE_43_OHM |
        IMX6SX_MUX_PADCTL_SRE_SLOW;

    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_CLK, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA00, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA01, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA02, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA03, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA04, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA05, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA06, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA07, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA08, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA09, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA10, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA11, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA12, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA13, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA14, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA15, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA16, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA17, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA18, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA19, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA20, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA21, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA22, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_DATA23, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_ENABLE, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_HSYNC, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_LCD1_VSYNC, 0);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_CLK, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA00, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA01, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA02, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA03, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA04, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA05, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA06, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA07, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA08, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA09, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA10, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA11, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA12, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA13, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA14, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA15, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA16, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA17, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA18, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA19, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA20, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA21, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA22, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_DATA23, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_ENABLE, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_HSYNC, reg32Val);
    MmioWrite32(IMX_IOMUXC_BASE + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_LCD1_VSYNC, reg32Val);

    DebugPrint(0xFFFFFFFF, "LCD init done\n");
}

VOID
TestI2CAccelerometer(
    VOID
    )
{
    /* kick accelerometer to ensure communications with it is ok */

    UINT8               regVal8;
    BOOLEAN             result = TRUE;
    IMX6SX_I2C_TARGET   Accel;

    Accel.SlaveAddr = UDOONEO_ACCEL_I2C4_ADDR;
    Accel.SlaveRegSizeBytes = 1;
    Accel.SlaveSpeed = 400000;
    Accel.XferTimeoutUs = 1000;

    result = IMX6SX_I2C_SetTarget(4, &Accel);
    ASSERT(result);

    // delay 5ms to let Accel stabilize
    MicroSecondDelay(5000);

    result = ReadI2CByte(4, 0x0D, &regVal8, NULL);
    if (!result) {
        DebugPrint(0xFFFFFFFF, "Failed to send I2c command to accelerometer.\n");
        return;
    }

    if (regVal8 == 0xC7) {
        DebugPrint(0xFFFFFFFF, "Accelerometer returned correct value.\n");
    }
    else {
        DebugPrint(0xFFFFFFFF, "Did not get expected result from accelerometer.\n");
    }
}

VOID
ConfigureM4(
    VOID
    )
{
    UINT32              regVal;

    // assert non-self-clearing M4C (core) reset
    regVal = MmioRead32(IMX6SX_PHYSADDR_SRC_SCR);
    regVal |= IMX6SX_SRC_SCR_M4C_NON_SCLR_RST;
    MmioWrite32(IMX6SX_PHYSADDR_SRC_SCR, regVal);

    MicroSecondDelay(50);

    // disable the M4 module
    regVal = MmioRead32(IMX6SX_PHYSADDR_SRC_SCR);
    regVal &= ~(IMX6SX_SRC_SCR_M4_ENABLE | IMX6SX_SRC_SCR_M4C_RST);
    MmioWrite32(IMX6SX_PHYSADDR_SRC_SCR, regVal);

    // turn off the M4 module clock
    regVal = MmioRead32(IMX6SX_PHYSADDR_CCM_CCGR3);
    regVal &= ~(IMX6SX_RUN_AND_WAIT << IMX6SX_SHL_CCM_CCGR3_M4);
    MmioWrite32(IMX6SX_PHYSADDR_CCM_CCGR3, regVal);

    // configure M4 clock - select PLL2_PFD2 (400Mhz - actually 396Mhz) divided by 2
    regVal = MmioRead32(IMX6SX_PHYSADDR_CCM_CHSCCDR);
    regVal &= ~(IMX6SX_CCM_CHSCCDR_M4_PRE_CLK_SEL_MASK |
                IMX6SX_CCM_CHSCCDR_M4_PODF_MASK |
                IMX6SX_CCM_CHSCCDR_M4_CLK_SEL_MASK);
    regVal |= IMX6SX_CCM_CHSCCDR_M4_PRE_CLK_SEL_PLL2_PFD2;
    regVal |= (1 << IMX6SX_CCM_CHSCCDR_M4_PODF_SHL);
    regVal |= IMX6SX_CCM_CHSCCDR_M4_CLK_SEL_PREMUX;
    MmioWrite32(IMX6SX_PHYSADDR_CCM_CHSCCDR, regVal);

    // turn on the M4 module clock
    regVal = MmioRead32(IMX6SX_PHYSADDR_CCM_CCGR3);
    regVal |= (IMX6SX_RUN_AND_WAIT << IMX6SX_SHL_CCM_CCGR3_M4);
    MmioWrite32(IMX6SX_PHYSADDR_CCM_CCGR3, regVal);

    MicroSecondDelay(50);

    // enable the M4 module
    regVal = MmioRead32(IMX6SX_PHYSADDR_SRC_SCR);
    regVal |= IMX6SX_SRC_SCR_M4_ENABLE;
    MmioWrite32(IMX6SX_PHYSADDR_SRC_SCR, regVal);

    MicroSecondDelay(50);

    // reset the M4 *PLATFORM* and wait for the reset to complete
    regVal = MmioRead32(IMX6SX_PHYSADDR_SRC_SCR);
    regVal |= IMX6SX_SRC_SCR_M4P_RST;
    MmioWrite32(IMX6SX_PHYSADDR_SRC_SCR, regVal);
    do
    {
        regVal = MmioRead32(IMX6SX_PHYSADDR_SRC_SCR);
    } while (regVal & IMX6SX_SRC_SCR_M4P_RST);

    DebugPrint(0xFFFFFFFF, "M4 init done\n");
}

VOID
USBInit(
    VOID
    )
{
    UINT32 regVal;
    UINT32 gpioPadConfig;

    gpioPadConfig = IMX6SX_MUX_PADCTL_SPEED_100MHZ |
        IMX6SX_MUX_PADCTL_PKE |
        IMX6SX_MUX_PADCTL_DSE_43_OHM |
        IMX6SX_MUX_PADCTL_SRE_SLOW;

    //
    // OTG1 - this is the micro A/B socket (client/host) - has ID pin
    //
    // OC -- GPIO1_IO8
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO08, 0);
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_USB_IPP_IND_OTG_OC_SELECT_INPUT, 0);
    // PWR -- GPIO1_IO9
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO09, 0);
    // ID -- GPIO1_IO10
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_GPIO1_IO10, 0);
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_ANATOP_USB_OTG_ID_SELECT_INPUT, 0);

    //
    // OTG2 - this is the regular size A socket (host) - no ID pin
    //
    // OC -- QSPI1A_DATA0
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_QSPI1A_DATA0, 1);
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_USB_IPP_IND_OTG2_OC_SELECT_INPUT, 2);

    // PWR -- NAND_RE = GPIO4_IO12 - OUTPUT USB OTG2 power (VBUS) on GPIO4-IO12
    MmioSetBits32(IMX6SX_PHYSADDR_GPIO4_DR, (1 << 12));
    MmioSetBits32(IMX6SX_PHYSADDR_GPIO4_GDIR, (1 << 12));
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_SW_MUX_CTL_PAD_NAND_RE_B, 0x05);
    MmioWrite32(IMX6SX_PHYSADDR_IOMUXC + IMX6SX_IOMUXC_OFFSET_SW_PAD_CTL_PAD_NAND_RE_B, gpioPadConfig);

    // UOG2 power is manual
    regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_OTG2_CTRL);
    regVal |= (1 << 9);
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_OTG2_CTRL, regVal);

    MicroSecondDelay(100);

    // disable charger detection - these signals are active low
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_USB1_CHRG_DETECT_SET, IMX6SX_ANALOG_USB_CHRG_DETECT_EN_B | IMX6SX_ANALOG_USB_CHK_CHRG_B);
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_USB2_CHRG_DETECT_SET, IMX6SX_ANALOG_USB_CHRG_DETECT_EN_B | IMX6SX_ANALOG_USB_CHK_CHRG_B);

    // clear clock bypasses
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_PLL_USB1_CLR, IMX6SX_ANALOG_PLL_USB_BYPASS);
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_PLL_USB2_CLR, IMX6SX_ANALOG_PLL_USB_BYPASS);

    // ensure plls are powered up, enabled, and USB clocks are enabled
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_PLL_USB1_SET,
        IMX6SX_ANALOG_PLL_USB_ENABLE |
        IMX6SX_ANALOG_PLL_USB_POWER |
        IMX6SX_ANALOG_PLL_USB_EN_USB_CLKS);
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_PLL_USB2_SET,
        IMX6SX_ANALOG_PLL_USB_ENABLE |
        IMX6SX_ANALOG_PLL_USB_POWER |
        IMX6SX_ANALOG_PLL_USB_EN_USB_CLKS);

    // ensure PLLs are locked before we continue
    do
    {
        regVal = MmioRead32(IMX6SX_PHYSADDR_ANALOG_PLL_USB1);
    } while (!(regVal & IMX6SX_ANALOG_PLL_USB_LOCK));
    do
    {
        regVal = MmioRead32(IMX6SX_PHYSADDR_ANALOG_PLL_USB2);
    } while (!(regVal & IMX6SX_ANALOG_PLL_USB_LOCK));

    // disable overcurrent detection, low active
    MmioSetBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_OTG1_CTRL, (1 << 8) | (1 << 7));
    MmioSetBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_OTG2_CTRL, (1 << 8) | (1 << 7));

    // turn on (ungate) internal phy clocks
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY1_CTRL_CLR, 0x40000000);
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY2_CTRL_CLR, 0x40000000);

    MicroSecondDelay(100);

    // enable clocks to UTMI blocks
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_USB1_MISC_SET, 0x40000000);
    MmioWrite32(IMX6SX_PHYSADDR_ANALOG_USB2_MISC_SET, 0x40000000);
    MicroSecondDelay(10);

    // install defaults to registers
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_UH_CTRL, 0x00001000);
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_UH_HSIC_CTRL, 0x00000042);
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_OTG1_PHY_CTRL_0, 0x00000000);
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USBNC_USB_OTG2_PHY_CTRL_0, 0x00000098);

    // Stop the hosts
    regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBMODE);
    if ((regVal & 3) == 3)
    {
        MmioClrBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBCMD, 1);
        do
        {
            regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBSTS);
        } while (!(regVal & 0x00001000));
    }

    regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBMODE);
    if ((regVal & 3) == 3)
    {
        MmioClrBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBCMD, 1);
        do
        {
            regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBSTS);
        } while (!(regVal & 0x00001000));
    }

    MmioClrBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UH1_USBCMD, 1);
    do
    {
        regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UH1_USBSTS);
    } while (!(regVal & 0x00001000));

    // reset the controllers
    MmioSetBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBCMD, 2);
    MmioSetBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBCMD, 2);
    MmioSetBits32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UH1_USBCMD, 2);

    // wait for controllers to complete reset
    do
    {
        regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBCMD);
    } while (regVal & 2);
    do
    {
        regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBCMD);
    } while (regVal & 2);
    do
    {
        regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UH1_USBCMD);
    } while (regVal & 2);

    // confirm UOG1 reset (set mode to zero), then set controller UOG1 as a client
    regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBMODE);
    ASSERT((regVal & 3) == 0);
    regVal |= (2 | 8);  // Set SLOM to disable lockout
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBMODE, regVal);
    regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG1_USBMODE);
    ASSERT((regVal & 3) == 2);

    // confirm UOG2 reset (set mode to zero), then set controller UOG2 as a host
    regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBMODE);
    ASSERT((regVal & 3) == 0);
    regVal |= (3 | 8);  // Set SLOM to disable lockout
    MmioWrite32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBMODE, regVal);
    regVal = MmioRead32(IMX6SX_PHYSADDR_USBO2H_USB + IMX6SX_USBO2H_OFFSET_USB_UOG2_USBMODE);
    ASSERT((regVal & 3) == 3);

    // set USB PHY resets
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY1_CTRL_SET, IMX6SX_USBPHY_CTRL_SFTRST);
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY2_CTRL_SET, IMX6SX_USBPHY_CTRL_SFTRST);

    MicroSecondDelay(10);

    // release resets and ungate clocks
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY1_CTRL_CLR, IMX6SX_USBPHY_CTRL_SFTRST | IMX6SX_USBPHY_CTRL_CLKGATE);
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY2_CTRL_CLR, IMX6SX_USBPHY_CTRL_SFTRST | IMX6SX_USBPHY_CTRL_CLKGATE);

    MicroSecondDelay(10);

    // power up phys
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY1_PWD, 0);
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY2_PWD, 0);

    MicroSecondDelay(10);

    // enable Full and Low speed devices
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY1_CTRL_SET, IMX6SX_USBPHY_CTRL_ENUTMILEVEL3 | IMX6SX_USBPHY_CTRL_ENUTMILEVEL2);
    MmioWrite32(IMX6SX_PHYSADDR_USBPHY2_CTRL_SET, IMX6SX_USBPHY_CTRL_ENUTMILEVEL3 | IMX6SX_USBPHY_CTRL_ENUTMILEVEL2);

    MicroSecondDelay(10);

    DebugPrint(0xFFFFFFFF, "USB init done\n");
}

/**
  Initialize controllers that must setup at the early stage
**/
RETURN_STATUS
ArmPlatformInitialize (
  IN  UINTN                     MpId
  )
{
  if (!ArmPlatformIsPrimaryCore (MpId)) {
    return RETURN_SUCCESS;
  }

  ImxClkPwrInit ();

  SerialPortInitialize ();
  SerialPrint("\nSerial Port Init\n\r");
  I2cInit ();
  ConfigurePMIC ();
  HdmiInit ();
  LcdInit ();
  // TestI2CAccelerometer ();
  // ConfigureM4 ();
  USBInit ();

  return RETURN_SUCCESS;
}

/**
  Return the current Boot Mode

  This function returns the boot reason on the platform

**/
EFI_BOOT_MODE
ArmPlatformGetBootMode (
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}

/**
  UART initialization for function that is called from SEC.
  Nothing to initialize as we are dependent on primary boot loader to setup
  UART.
**/
VOID UartInit ()
{

  return;
}