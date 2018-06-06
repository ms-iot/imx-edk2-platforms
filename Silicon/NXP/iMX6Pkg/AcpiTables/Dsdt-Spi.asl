/** @file
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
  })
}

Device (SPI2)
{
  Name (_HID, "NXP0105")
  Name (_UID, 0x2)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x0200C000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 64 }
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
  })
}

Device (SPI4)
{
  Name (_HID, "NXP0105")
  Name (_UID, 0x4)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x02014000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 66 }
  })
}

Device (SPI5)
{
  Name (_HID, "NXP0105")
  Name (_UID, 0x5)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x02018000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 67 }
  })
}
