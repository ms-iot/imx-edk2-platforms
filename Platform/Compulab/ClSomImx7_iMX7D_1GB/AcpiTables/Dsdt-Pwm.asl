/** @file
*
*  Compulab iMX7 Dual Pulse Width Modulator (PWM)
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

Device (PWM1)
{
  Name (_HID, "NXP010E")
  Name (_UID, 0x1)
  Method (_STA) {
    Return (0x0)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30660000, 0x18, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 113 }
  })
}

Device (PWM2)
{
  Name (_HID, "NXP010E")
  Name (_UID, 0x2)
  Method (_STA) {
    Return (0x0)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30670000, 0x18, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 114 }
  })
}

Device (PWM3)
{
  Name (_HID, "NXP010E")
  Name (_UID, 0x3)
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30680000, 0x18, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 115 }
  })
}
