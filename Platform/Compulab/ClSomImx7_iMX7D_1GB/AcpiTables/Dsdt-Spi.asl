/** @file
*
*  Compulab iMX7 SPI Controllers
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

Device (SPI3)
{
  Name (_HID, "NXP0105")
  Name (_UID, 0x3)
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30840000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 65 }

    // CS0 (PAD_I2C2_SDA) GPIO4_IO11 (3 * 32 + 11 = 107)
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 107 }

    // CS3 (PAD_I2C2_SDA) GPIO5_IO10 (4 * 32 + 10 = 138)
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 138 }

    // MISO: PAD_I2C1_SCL (GPIO4_IO08) ALT3 (3 * 32 + 8  = 104)
    // MOSI: PAD_I2C1_SDA (GPIO4_IO09) ALT3 (3 * 32 + 9  = 105)
    // SCLK: PAD_I2C2_SCL (GPIO4_IO10) ALT3 (3 * 32 + 10 = 106)
    // MsftFunctionConfig (Exclusive, PullDown, IMX_ALT3, "\\_SB.GPIO", 0,
    //                     ResourceConsumer, ) { 104, 105, 106 }
    //
    // MsftFunctionConfig (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) { Pin List }
    VendorLong () {
      MSFT_UUID,            // Vendor UUID (MSFT UUID)
      MSFT_FUNCTION_CONFIG, // Resource Identifier (MSFT Function Config)
      0x1f,0x00,            // Length (0xF + sizeof(PinList) + sizeof(ResourceName))
      0x01,                 // Revision (0x1)
      RESOURCECONSUMER_EXCLUSIVE, // Flags (Arg5 | Arg0: ResourceConsumer | Exclusive)
      PULL_DOWN,            // Pin configuration (Arg1: PullDown)
      IMX_ALT3,0x00,        // Function Number (Arg2: IMX_ALT3)
      PIN_TABLE_OFFSET,     // Pin Table Offset (0x12)
      0x00,                 // Resource Source Index (Arg4: 0)
      0x18,0x00,            // Resource Source Name Offset (0x12 + sizeof(PinList))
      0x22,0x00,            // Vendor Data Offset (0x12 + sizeof(PinList) + sizeof(ResourceName))
      0x00,0x00,            // Vendor Data Length (sizeof(Arg6) = 0)
      0x68,0x00,0x69,0x00,0x6a,0x00,  // Pin List (104, 105, 106)
      SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
    }
  })
}
