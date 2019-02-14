/*
* Description: iMX6 SabreLite UART Controllers
*
*  Copyright (c) Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
*/

Device (UAR1)
{
    Name (_HID, "NXP0106")
    Name (_UID, 0x1)
    Name (_DDN, "UART1")
    Method (_STA)
    {
        Return(0xf)
    }
    Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF, ResourceTemplate () {
            MEMORY32FIXED(ReadWrite, 0x02020000, 0x4000, )
            Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) { 58 }

            // UART1_TX_DATA - SD3_DAT7 - GPIO6_IO17 - 177
            // UART1_RX_DATA - SD3_DAT6 - GPIO6_IO18 - 178
            // MsftFunctionConfig (Exclusive, PullUp, IMX_ALT1, "\\_SB.GPIO", 0,
            //                     ResourceConsumer, ) { 177, 178 }
            //
            // MsftFunctionConfig (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) { Pin List }
            VendorLong () {
            MSFT_UUID,            // Vendor UUID (MSFT UUID)
            MSFT_FUNCTION_CONFIG, // Resource Identifier (MSFT Function Config)
            0x1d,0x00,            // Length (0xF + sizeof(PinList) + sizeof(ResourceName))
            0x01,                 // Revision (0x1)
            RESOURCECONSUMER_EXCLUSIVE, // Flags (Arg5 | Arg0: ResourceConsumer | Exclusive)
            PULL_UP,              // Pin configuration (Arg1: PullUp)
            IMX_ALT1,0x00,        // Function Number (Arg2: IMX_ALT1)
            PIN_TABLE_OFFSET,     // Pin Table Offset (0x12)
            0x00,                 // Resource Source Index (Arg4: 0)
            0x16,0x00,            // Resource Source Name Offset (0x12 + sizeof(PinList))
            0x20,0x00,            // Vendor Data Offset (0x12 + sizeof(PinList) + sizeof(ResourceName))
            0x00,0x00,            // Vendor Data Length (sizeof(Arg6) = 0)
            0xb1,0x00,0xb2,0x00,  // Pin List (177, 178)
            SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
            }

            // DMA channel 2, SDMA_REQ_UART1_RX for UART1 RX DMA
            FixedDMA(SDMA_REQ_UART1_RX, 2, Width8Bit, )           
            // DMA channel 1, SDMA_REQ_UART1_TX for UART1 TX DMA
            FixedDMA(SDMA_REQ_UART1_TX, 1, Width8Bit, )           
            
            UARTSerialBus(
                115200,
                DataBitsEight,
                StopBitsOne,
                0,                  // LinesInUse
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
        Return(RBUF)
    }

    Name (_DSD, Package () {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
            Package () {
                Package (2) {"SerCx-FriendlyName", "UART1"}
            }
    })
}

Device (UAR2)
{
    Name (_HID, "NXP0107")
    Name (_UID, 0x2)
    Name (_DDN, "UART2")
    Method (_STA)
    {
        Return(0xf)
    }
    Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF, ResourceTemplate () {
            MEMORY32FIXED(ReadWrite, 0x021E8000, 0x4000, )
            Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 59 }
            
            // UART2_TX_DATA - EIM_D26 - GPIO3_IO26 - 90
            // UART2_RX_DATA - EIM_D27 - GPIO3_IO27 - 91
            // MsftFunctionConfig (Exclusive, PullUp, IMX_ALT4, "\\_SB.GPIO", 0,
            //                     ResourceConsumer, ) { 90, 91 }
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
            0x5a,0x00,0x5b,0x00,  // Pin List (90, 91)
            SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
            }

            // DMA channel 4, SDMA_REQ_UART2_RX for UART2 RX DMA
            FixedDMA(SDMA_REQ_UART2_RX, 4, Width8Bit, )           
            // DMA channel 3, SDMA_REQ_UART2_TX for UART2 TX DMA
            FixedDMA(SDMA_REQ_UART2_TX, 3, Width8Bit, )           
            
            UARTSerialBus(
                115200,
                DataBitsEight,
                StopBitsOne,
                0,                  // LinesInUse
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
        Return(RBUF)
    }

    Name (_DSD, Package () {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
            Package () {
                Package (2) {"SerCx-FriendlyName", "UART2"}
            }
    })
}

