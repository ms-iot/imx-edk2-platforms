/** @file
*
*  Compulab iMX7 UART Controllers
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

    // We do not support dynamic muxing of UART pins for the kernel
    // debugger UART. The pins should already be muxed by firmware.

    UARTSerialBus(
      115200,
      DataBitsEight,
      StopBitsOne,
      0x00,                  // LinesInUse
      LittleEndian,
      ParityTypeNone,
      FlowControlNone,
      0,
      0,
      "\\_SB.CL0.CPU0",
      0,
      ResourceConsumer,
      ,
    )
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"SerCx-FriendlyName", "UART1"}
      }
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

    // UART2_RX_DATA - LCD_CLK - GPIO3_IO0 - 64
    // UART2_TX_DATA - LCD_ENABLE - GPIO3_IO1 - 65
    // UART2_RTS_B - LCD_HSYNC - GPIO3_IO2 - 66
    // UART2_CTS_B - LCD_VSYNC - GPIO3_IO3 - 67
    // MsftFunctionConfig (Exclusive, PullUp, IMX_ALT4, "\\_SB.GPIO", 0,
    //                     ResourceConsumer, ) { 64, 65, 66, 67 }
    //
    // MsftFunctionConfig (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) { Pin List }
    VendorLong () {
      MSFT_UUID,            // Vendor UUID (MSFT UUID)
      MSFT_FUNCTION_CONFIG, // Resource Identifier (MSFT Function Config)
      0x21,0x00,            // Length (0xF + sizeof(PinList) + sizeof(ResourceName))
      0x01,                 // Revision (0x1)
      RESOURCECONSUMER_EXCLUSIVE, // Flags (Arg5 | Arg0: ResourceConsumer | Exclusive)
      PULL_UP,              // Pin configuration (Arg1: PullUp)
      IMX_ALT4,0x00,        // Function Number (Arg2: IMX_ALT4)
      PIN_TABLE_OFFSET,     // Pin Table Offset (0x12)
      0x00,                 // Resource Source Index (Arg4: 0)
      0x1a,0x00,            // Resource Source Name Offset (0x12 + sizeof(PinList))
      0x24,0x00,            // Vendor Data Offset (0x12 + sizeof(PinList) + sizeof(ResourceName))
      0x00,0x00,            // Vendor Data Length (sizeof(Arg6) = 0)
      0x40,0x00,0x41,0x00,0x42,0x00,0x43,0x00,  // Pin List (64, 65, 66, 67)
      SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
    }

    UARTSerialBus(
      115200,
      DataBitsEight,
      StopBitsOne,
      0xC0,                  // LinesInUse
      LittleEndian,
      ParityTypeNone,
      FlowControlNone,
      0,
      0,
      "\\_SB.CL0.CPU0",
      0,
      ResourceConsumer,
      ,
    )
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"SerCx-FriendlyName", "UART2"}
      }
  })
}

// UART3 reserved for bluetooth on CL-SOM-iMX7
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
    Return(0x0)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30A60000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 61 }

    // UART4_CTS_B - I2C1_SCL - GPIO4_IO8 - 104
    // UART4_RTS_B - I2C1_SDA - GPIO4_IO9 - 105
    // UART4_RX_DATA - I2C2_SCL - GPIO4_IO10 - 106
    // UART4_TX_DATA - I2C2_SDA - GPIO4_IO11 - 107
    // MsftFunctionConfig (Exclusive, PullUp, IMX_ALT1, "\\_SB.GPIO", 0,
    //                     ResourceConsumer, ) { 104, 105, 106, 107 }
    //
    // MsftFunctionConfig (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) { Pin List }
    VendorLong () {
      MSFT_UUID,            // Vendor UUID (MSFT UUID)
      MSFT_FUNCTION_CONFIG, // Resource Identifier (MSFT Function Config)
      0x21,0x00,            // Length (0xF + sizeof(PinList) + sizeof(ResourceName))
      0x01,                 // Revision (0x1)
      RESOURCECONSUMER_EXCLUSIVE, // Flags (Arg5 | Arg0: ResourceConsumer | Exclusive)
      PULL_UP,              // Pin configuration (Arg1: PullUp)
      IMX_ALT1,0x00,        // Function Number (Arg2: IMX_ALT1)
      PIN_TABLE_OFFSET,     // Pin Table Offset (0x12)
      0x00,                 // Resource Source Index (Arg4: 0)
      0x1a,0x00,            // Resource Source Name Offset (0x12 + sizeof(PinList))
      0x24,0x00,            // Vendor Data Offset (0x12 + sizeof(PinList) + sizeof(ResourceName))
      0x00,0x00,            // Vendor Data Length (sizeof(Arg6) = 0)
      0x68,0x00,0x69,0x00,0x6a,0x00,0x6b,0x00,  // Pin List (104, 105, 106, 107)
      SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
    }

    UARTSerialBus(
      115200,
      DataBitsEight,
      StopBitsOne,
      0xC0,                  // LinesInUse
      LittleEndian,
      ParityTypeNone,
      FlowControlNone,
      0,
      0,
      "\\_SB.CL0.CPU0",
      0,
      ResourceConsumer,
      ,
    )
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"SerCx-FriendlyName", "UART4"}
      }
  })
}

// UART1 in CL_SOM_iMX7 schematic
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

    // UART5_CTS_B - I2C3_SCL - GPIO4_IO12 - 108
    // UART5_RTS_B - I2C3_SDA - GPIO4_IO13 - 109
    // UART5_RX_DATA - I2C4_SCL - GPIO4_IO14 - 110
    // UART5_TX_DATA - I2C4_SDA - GPIO4_IO15 - 111
    // MsftFunctionConfig (Exclusive, PullUp, IMX_ALT1, "\\_SB.GPIO", 0,
    //                     ResourceConsumer, ) { 108, 109, 110, 111 }
    //
    // MsftFunctionConfig (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) { Pin List }
    VendorLong () {
      MSFT_UUID,            // Vendor UUID (MSFT UUID)
      MSFT_FUNCTION_CONFIG, // Resource Identifier (MSFT Function Config)
      0x21,0x00,            // Length (0xF + sizeof(PinList) + sizeof(ResourceName))
      0x01,                 // Revision (0x1)
      RESOURCECONSUMER_EXCLUSIVE, // Flags (Arg5 | Arg0: ResourceConsumer | Exclusive)
      PULL_UP,              // Pin configuration (Arg1: PullUp)
      IMX_ALT1,0x00,        // Function Number (Arg2: IMX_ALT1)
      PIN_TABLE_OFFSET,     // Pin Table Offset (0x12)
      0x00,                 // Resource Source Index (Arg4: 0)
      0x1a,0x00,            // Resource Source Name Offset (0x12 + sizeof(PinList))
      0x24,0x00,            // Vendor Data Offset (0x12 + sizeof(PinList) + sizeof(ResourceName))
      0x00,0x00,            // Vendor Data Length (sizeof(Arg6) = 0)
      0x6c,0x00,0x6d,0x00,0x6e,0x00,0x6f,0x00,  // Pin List (108, 109, 110, 111)
      SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
    }

    UARTSerialBus(
      115200,
      DataBitsEight,
      StopBitsOne,
      0xC0,                  // LinesInUse
      LittleEndian,
      ParityTypeNone,
      FlowControlNone,
      0,
      0,
      "\\_SB.CL0.CPU0",
      0,
      ResourceConsumer,
      ,
    )
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"SerCx-FriendlyName", "UART5"}
      }
  })
}

Device (UAR6)
{
  Name (_HID, "NXP0113")
  Name (_UID, 0x6)
  Name (_DDN, "UART6")
  Method (_STA) {
    Return (0x0)
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
    Return (0x0)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30A90000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 158 }

    // UART7_RX_DATA - ECSPI2_SCLK - GPIO4_IO20 - 116
    // UART7_TX_DATA - ECSPI2_MOSI - GPIO4_IO21 - 117
    // UART7_RTS_B - ECSPI2_MISO - GPIO4_IO22 - 118
    // UART7_CTS_B - ECSPI2_SS0 - GPIO4_IO23 - 119
    // MsftFunctionConfig (Exclusive, PullUp, IMX_ALT1, "\\_SB.GPIO", 0,
    //                     ResourceConsumer, ) { 116, 117, 118, 119 }
    //
    // MsftFunctionConfig (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) { Pin List }
    VendorLong () {
      MSFT_UUID,            // Vendor UUID (MSFT UUID)
      MSFT_FUNCTION_CONFIG, // Resource Identifier (MSFT Function Config)
      0x21,0x00,            // Length (0xF + sizeof(PinList) + sizeof(ResourceName))
      0x01,                 // Revision (0x1)
      RESOURCECONSUMER_EXCLUSIVE, // Flags (Arg5 | Arg0: ResourceConsumer | Exclusive)
      PULL_UP,              // Pin configuration (Arg1: PullUp)
      IMX_ALT1,0x00,        // Function Number (Arg2: IMX_ALT1)
      PIN_TABLE_OFFSET,     // Pin Table Offset (0x12)
      0x00,                 // Resource Source Index (Arg4: 0)
      0x1a,0x00,            // Resource Source Name Offset (0x12 + sizeof(PinList))
      0x24,0x00,            // Vendor Data Offset (0x12 + sizeof(PinList) + sizeof(ResourceName))
      0x00,0x00,            // Vendor Data Length (sizeof(Arg6) = 0)
      0x74,0x00,0x75,0x00,0x76,0x00,0x77,0x00,  // Pin List (116, 117, 118, 119)
      SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
    }

    UARTSerialBus(
      115200,
      DataBitsEight,
      StopBitsOne,
      0xC0,                  // LinesInUse
      LittleEndian,
      ParityTypeNone,
      FlowControlNone,
      0,
      0,
      "\\_SB.CL0.CPU0",
      0,
      ResourceConsumer,
      ,
    )
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"SerCx-FriendlyName", "UART7"}
      }
  })
}
