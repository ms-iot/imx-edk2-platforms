/** @file
*
*  iMX7 UART Controllers
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

Device (UAR1)
{
  Name (_HID, "NXP0113")
  Name (_UID, 0x1)
  Name (_DDN, "UART1")
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30860000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Shared) { 58 }
  })
}

Device (UAR2)
{
  Name (_HID, "NXP0113")
  Name (_UID, 0x2)
  Name (_DDN, "UART2")
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30890000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 59 }
  })
}

Device (UAR3)
{
  Name (_HID, "NXP0113")
  Name (_UID, 0x3)
  Name (_DDN, "UART3")
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30880000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 60 }
  })
}

Device (UAR4)
{
  Name (_HID, "NXP0113")
  Name (_UID, 0x4)
  Name (_DDN, "UART4")
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30A60000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 61 }
  })
}

Device (UAR5)
{
  Name (_HID, "NXP0113")
  Name (_UID, 0x5)
  Name (_DDN, "UART5")
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30A70000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 62 }
  })
}

Device (UAR6)
{
  Name (_HID, "NXP0113")
  Name (_UID, 0x6)
  Name (_DDN, "UART6")
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30A80000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 48 }
  })
}

Device (UAR7)
{
  Name (_HID, "NXP0113")
  Name (_UID, 0x7)
  Name (_DDN, "UART7")
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30A90000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 158 }
  })
}
