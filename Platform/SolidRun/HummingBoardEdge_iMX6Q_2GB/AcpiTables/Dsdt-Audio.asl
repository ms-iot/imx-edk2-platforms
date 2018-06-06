/** @file
*
*  iMX6 Quad Synchronous Serial Interface (SSI)
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

// Note: Only enable one SSI device. Audio driver only supports one render
// target and cannot dynamically detect the proper SSI device

// SSI1 - Disabled
Device (SSI1)
{
  Name (_HID, "NXP010A")
  Name (_UID, 0x1)

  Method (_STA) {
    Return (0x0)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x02028000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 78 }
  })
}

// SSI2 - Disabled
Device (SSI2)
{
  Name (_HID, "NXP010A")
  Name (_UID, 0x2)

  Method (_STA) {
    Return (0x0)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x0202C000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 79 }
  })
}

// SSI3 - Attached to Codec
Device (SSI3)
{
  Name (_HID, "NXP010A")
  Name (_UID, 0x3)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x02030000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 80 }
  })
}
