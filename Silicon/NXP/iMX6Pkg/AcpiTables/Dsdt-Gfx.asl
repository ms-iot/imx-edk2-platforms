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

Device (VPU0)
{
  Name (_HID, "NXP0109")
  Name (_UID, 0x0)

  Method (_STA) {
    Return (0x0)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x02040000, 0x3C000, )

    // JPEG codec interrupt request
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 35 }

    // VPU interrupt request
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 44 }
  })
}

// Description: Graphics Processing Unit (GPU)
Device (GPU0)
{
  Name (_HID, "VERI2000")
  Name (_CID, "VERI2000")
  Name (_UID, 0)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    // GPU 3D (GC2000)
    MEMORY32FIXED (ReadWrite, 0x00130000, 0x4000, )

    // GPU 2D (GC320)
    MEMORY32FIXED (ReadWrite, 0x00134000, 0x4000, )

    // IPU Base
    MEMORY32FIXED (ReadWrite, 0x02600000, 0x400000, )

    // HDMI PHY
    MEMORY32FIXED (ReadWrite, 0x00120000, 0x9000, )

    // GC2000
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 41 }

    // GC320
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 42 }

    // IPU1 sync interrupt request
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 38 }
  })
}
