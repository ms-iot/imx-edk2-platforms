/** @file
*
*  Udoo Neo UART Controllers
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
  // Use NXP0113 UART which uses 24MHz reference clock
  Name (_HID, "NXP0113")
  Name (_UID, 0x1)
  Name (_DDN, "UART1")
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x02020000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Shared) { 58 }

    // UART1_TX_DATA - GPIO_4 - GPIO1_IO04 - 4 - Udoo UART1_TXD J7 pin 2
    // UART1_RX_DATA - GPIO_5 - GPIO1_IO05 - 5 - Udoo UART1_RXD J7 pin 4
    // MsftFunctionConfig (Exclusive, PullUp, IMX_ALT0, "\\_SB.GPIO", 0,
    //                     ResourceConsumer, ) { 4, 5 }
    //
    // MsftFunctionConfig (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) { Pin List }
    VendorLong () {
      MSFT_UUID,            // Vendor UUID (MSFT UUID)
      MSFT_FUNCTION_CONFIG, // Resource Identifier (MSFT Function Config)
      0x1d,0x00,            // Length (0xF + sizeof(PinList) + sizeof(ResourceName))
      0x01,                 // Revision (0x1)
      RESOURCECONSUMER_EXCLUSIVE, // Flags (Arg5 | Arg0: ResourceConsumer | Exclusive)
      PULL_UP,              // Pin configuration (Arg1: PullUp)
      IMX_ALT0,0x00,        // Function Number (Arg2: IMX_ALT0)
      PIN_TABLE_OFFSET,     // Pin Table Offset (0x12)
      0x00,                 // Resource Source Index (Arg4: 0)
      0x16,0x00,            // Resource Source Name Offset (0x12 + sizeof(PinList))
      0x20,0x00,            // Vendor Data Offset (0x12 + sizeof(PinList) + sizeof(ResourceName))
      0x00,0x00,            // Vendor Data Length (sizeof(Arg6) = 0)
      0x04,0x00,0x05,0x00,  // Pin List (4, 5) RXD/TXD
      SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
    }

    UARTSerialBus (
      115200,
      DataBitsEight,
      StopBitsOne,
      0,                // LinesInUse TX,RX
      LittleEndian,
      ParityTypeNone,
      FlowControlNone,
      0,
      0,
      "\\_SB.CPU0",
      0,
      ResourceConsumer,
      ,)
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
  // Use NXP0113 UART which uses 24MHz reference clock
  Name (_HID, "NXP0113")
  Name (_UID, 0x2)
  Name (_DDN, "UART2")
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x021E8000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 59 }

    // UART2_TX_DATA - GPIO_6 - GPIO1_IO06 - 6 - Udoo UART2_TXD J7 pin 6
    // UART2_RX_DATA - GPIO_7 - GPIO1_IO07 - 7 - Udoo UART2_RXD J7 pin 8
    // MsftFunctionConfig (Exclusive, PullUp, IMX_ALT0, "\\_SB.GPIO", 0,
    //                     ResourceConsumer, ) { 6, 7 }
    //
    // MsftFunctionConfig (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) { Pin List }
    VendorLong () {
      MSFT_UUID,            // Vendor UUID (MSFT UUID)
      MSFT_FUNCTION_CONFIG, // Resource Identifier (MSFT Function Config)
      0x1d,0x00,            // Length (0xF + sizeof(PinList) + sizeof(ResourceName))
      0x01,                 // Revision (0x1)
      RESOURCECONSUMER_EXCLUSIVE, // Flags (Arg5 | Arg0: ResourceConsumer | Exclusive)
      PULL_UP,              // Pin configuration (Arg1: PullUp)
      IMX_ALT0,0x00,        // Function Number (Arg2: IMX_ALT0)
      PIN_TABLE_OFFSET,     // Pin Table Offset (0x12)
      0x00,                 // Resource Source Index (Arg4: 0)
      0x16,0x00,            // Resource Source Name Offset (0x12 + sizeof(PinList))
      0x20,0x00,            // Vendor Data Offset (0x12 + sizeof(PinList) + sizeof(ResourceName))
      0x00,0x00,            // Vendor Data Length (sizeof(Arg6) = 0)
      0x06,0x00,0x07,0x00,  // Pin List (6, 7) RXD/TXD
      SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
    }

    UARTSerialBus (
      115200,
      DataBitsEight,
      StopBitsOne,
      0,                // LinesInUse TX,RX
      LittleEndian,
      ParityTypeNone,
      FlowControlNone,
      0,
      0,
      "\\_SB.CPU0",
      0,
      ResourceConsumer,
      ,)
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"SerCx-FriendlyName", "UART2"}
      }
  })
}

Device (UAR3)
{
  // Use NXP0113 UART which uses 24MHz reference clock
  Name (_HID, "NXP0113")
  Name (_UID, 0x3)
  Name (_DDN, "UART3")
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x021EC000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 60 }

    // UART3_TX - NANDF_D7 - GPIO4_IO11 - 107 - Udoo USART0_TX J4 pin 4
    // UART3_RX - NANDF_D6 - GPIO4_IO10 - 106 - Udoo USART0_RX J4 pin 2
    // MsftFunctionConfig (Exclusive, PullUp, IMX_ALT3, "\\_SB.GPIO", 0,
    //                     ResourceConsumer, ) { 106, 107 }
    //
    // MsftFunctionConfig (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) { Pin List }
    VendorLong () {
      MSFT_UUID,            // Vendor UUID (MSFT UUID)
      MSFT_FUNCTION_CONFIG, // Resource Identifier (MSFT Function Config)
      0x1d,0x00,            // Length (0xF + sizeof(PinList) + sizeof(ResourceName))
      0x01,                 // Revision (0x1)
      RESOURCECONSUMER_EXCLUSIVE, // Flags (Arg5 | Arg0: ResourceConsumer | Exclusive)
      PULL_UP,              // Pin configuration (Arg1: PullUp)
      IMX_ALT3,0x00,        // Function Number (Arg2: IMX_ALT3)
      PIN_TABLE_OFFSET,     // Pin Table Offset (0x12)
      0x00,                 // Resource Source Index (Arg4: 0)
      0x16,0x00,            // Resource Source Name Offset (0x12 + sizeof(PinList))
      0x20,0x00,            // Vendor Data Offset (0x12 + sizeof(PinList) + sizeof(ResourceName))
      0x00,0x00,            // Vendor Data Length (sizeof(Arg6) = 0)
      0x6A,0x00,0x6B,0x00,  // Pin List (106, 107) RXD/TXD
      SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
    }

    UARTSerialBus (
      115200,
      DataBitsEight,
      StopBitsOne,
      0,                // LinesInUse TX,RX
      LittleEndian,
      ParityTypeNone,
      FlowControlNone,
      0,
      0,
      "\\_SB.CPU0",
      0,
      ResourceConsumer,
      ,)
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"SerCx-FriendlyName", "UART3"}
      }
  })
}

Device (UAR4)
{
  // Use NXP0113 UART which uses 24MHz reference clock
  Name (_HID, "NXP0113")
  Name (_UID, 0x4)
  Name (_DDN, "UART4")
  Method (_STA) {
    Return (0x0)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x021F0000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 61 }

    // UART4_TX_DATA - SDIO3_CMD, CSI_PIXCLK, and SD2_DATA1
    // UART4_RX_DATA - SDIO3_DATA3, CSI_MCLK, and SD2_DATA0
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"SerCx-FriendlyName", "UART4"}
      }
  })
}


Device (UAR5)
{
  // Use NXP0113 UART which uses 24MHz reference clock
  Name (_HID, "NXP0113")
  Name (_UID, 0x5)
  Name (_DDN, "UART5")
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x021F4000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 62 }

    // UART5_TX_DATA - SD4_DAT5 - GPIO6_IO19 - 179 - 0xB3 - Udoo U1ATX J4 pin 3
    // UART5_RX_DATA - SD4_DAT4 - GPIO6_IO18 - 178 - 0xB2 - Udoo U1ARX J4 pin 1
    // MsftFunctionConfig (Exclusive, PullUp, IMX_ALT2, "\\_SB.GPIO", 0,
    //                     ResourceConsumer, ) { 178, 179 }

    // MsftFunctionConfig (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) { Pin List }
    VendorLong () {
      MSFT_UUID,            // Vendor UUID (MSFT UUID)
      MSFT_FUNCTION_CONFIG, // Resource Identifier (MSFT Function Config)
      0x1d,0x00,            // Length (0xF + sizeof(PinList) + sizeof(ResourceName))
      0x01,                 // Revision (0x1)
      RESOURCECONSUMER_EXCLUSIVE, // Flags (Arg5 | Arg0: ResourceConsumer | Exclusive)
      PULL_UP,              // Pin configuration (Arg1: PullUp)
      IMX_ALT2,0x00,        // Function Number (Arg2: IMX_ALT2)
      PIN_TABLE_OFFSET,     // Pin Table Offset (0x12)
      0x00,                 // Resource Source Index (Arg4: 0)
      0x16,0x00,            // Resource Source Name Offset (0x12 + sizeof(PinList))
      0x20,0x00,            // Vendor Data Offset (0x12 + sizeof(PinList) + sizeof(ResourceName))
      0x00,0x00,            // Vendor Data Length (sizeof(Arg6) = 0)
      0xB2,0x00,0xB3,0x00,  // Pin List (178,179) RXD/TXD
      SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
    }

    UARTSerialBus (
      115200,
      DataBitsEight,
      StopBitsOne,
      0,                // LinesInUse TX,RX
      LittleEndian,
      ParityTypeNone,
      FlowControlNone,
      0,
      0,
      "\\_SB.CPU0",
      0,
      ResourceConsumer,
      ,)
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
  // Use NXP0113 UART which uses 24MHz reference clock
  Name (_HID, "NXP0113")
  Name (_UID, 0x6)
  Name (_DDN, "UART6")
  Method (_STA) {
    Return (0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x022A0000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 49 }

    // UART5_TX_DATA - CSI_DATA05 - GPIO1_IO19 - 19 - 0x13 - Udoo CSI0_DAT5 J6 pin 16
    // UART5_RX_DATA - CSI_DATA04 - GPIO1_IO18 - 18 - 0x12 - Udoo CSI0_DAT4 J6 pin 14
    // MsftFunctionConfig (Exclusive, PullUp, IMX_ALT4, "\\_SB.GPIO", 0,
    //                     ResourceConsumer, ) { 18, 19 }
    
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
      0x12,0x00,0x13,0x00,  // Pin List (18,19) RXD/TXD
      SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
    }

    UARTSerialBus (
      115200,
      DataBitsEight,
      StopBitsOne,
      0,                // LinesInUse TX,RX
      LittleEndian,
      ParityTypeNone,
      FlowControlNone,
      0,
      0,
      "\\_SB.CPU0",
      0,
      ResourceConsumer,
      ,)
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"SerCx-FriendlyName", "UART6"}
      }
  })
}
