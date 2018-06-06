/** @file
*
*  iMX6 Quad SPI Controller
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
*
**/

Device (SPI1)
{
  Name (_HID, "NXP0105")
  Name (_UID, 0x1)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x02008000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 63 }

    // CS0 (PAD_EIM_D19) GPIO3_IO19
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 83 }
  })
}

Device (SPI3)
{
  Name (_HID, "NXP0105")
  Name (_UID, 0x3)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x02010000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 65 }

    // CS0 (PAD_DISP0_DAT3) GPIO4_IO24
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 120 }
    // CS1 (PAD_DISP0_DAT4) GPIO4_IO25
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 121 }
  })
}
