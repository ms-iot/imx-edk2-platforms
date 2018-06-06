/** @file
*
*  Compulab iMX7 I2C Controllers
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

Device (I2C1)
{
  Name (_HID, "NXP0104")
  Name (_UID, 0x1)
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30A20000, 0x14, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 67 }
  })
}

// I2C2 is used to communicate with devices on the SOM such as the codec or RTC
Device (I2C2)
{
  Name (_HID, "NXP0104")
  Name (_UID, 0x2)
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30A30000, 0x14, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 68 }
  })
}

Device (I2C3)
{
  Name (_HID, "NXP0104")
  Name (_UID, 0x3)
  Method (_STA) {
      Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED(ReadWrite, 0x30A40000, 0x14, )
    Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 69 }

    // GPIO1_IO8 ALT4
    // GPIO1_IO9 ALT4
    // MsftFunctionConfig (Exclusive, PullUp, IMX_ALT4, "\\_SB.GPIO", 0,
    //                     ResourceConsumer, ) { 8, 9 }
    //
    // MsftFunctionConfig (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) { Pin List }
    VendorLong () {
      MSFT_UUID,            // Vendor UUID (MSFT UUID)
      MSFT_FUNCTION_CONFIG, // Resource Identifier (MSFT Function Config)
      0x1d,0x00,            // Length (0xF + sizeof(PinList) + sizeof(ResourceName))
      0x01,                 // Revision (0x1)
      RESOURCECONSUMER_EXCLUSIVE, // Flags (Arg5 | Arg0: ResourceConsumer | Exclusive)
      PULL_UP,              // Pin configuration (Arg1: PullUp)
      IMX_ALT4,0x00,        // Function Number (Arg2: IMX_ALT4)
      PIN_TABLE_OFFSET,     // Pin Table Offset (0x12)
      0x00,                 // Resource Source Index (Arg4: 0)
      0x16,0x00,            // Resource Source Name Offset (0x12 + sizeof(PinList))
      0x20,0x00,            // Vendor Data Offset (0x12 + sizeof(PinList) + sizeof(ResourceName))
      0x00,0x00,            // Vendor Data Length (sizeof(Arg6) = 0)
      0x08,0x00,0x09,0x00,  // Pin List (8, 9)
      SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
    }
  })
}

Device (I2C4)
{
  Name (_HID, "NXP0104")
  Name (_UID, 0x4)
  Method (_STA) {
    Return(0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED(ReadWrite, 0x30A50000, 0x14, )
    Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 70 }
  })
}
