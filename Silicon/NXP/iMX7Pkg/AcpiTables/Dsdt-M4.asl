/** @file
*
*  iMX7 M4 Device
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
*/

Device(ACM4)
{
  Name(_HID, "NXP0004")
  Name(_UID, 0)
  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    // SRC_M4RCR Register
    MEMORY32FIXED (ReadWrite, 0x3039000C, 0x4, )
    // TCML Register
    MEMORY32FIXED (ReadWrite, 0x007F8000, 0x8000, )
    // OCRAM_S Register
    MEMORY32FIXED (ReadWrite, 0x00180000, 0x8000, )
    // MU interrupt (88 + 32)
    Interrupt (ResourceConsumer, Level, ActiveHigh, SharedAndWake) { 120 }
  })
}
