/** @file
*
*  iMX6 Quad UART Controllers
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
  Name (_HID, "NXP0106")
  Name (_UID, 0x1)
  Name (_DDN, "UART1")

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x02020000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Shared) { 58 }

    // DMA channel 2, SDMA_REQ_UART1_RX for UART1 RX DMA
    FixedDMA (SDMA_REQ_UART1_RX, 2, Width8Bit, )
    // DMA channel 1, SDMA_REQ_UART1_TX for UART1 TX DMA
    FixedDMA (SDMA_REQ_UART1_TX, 1, Width8Bit, )

    UARTSerialBus (
      115200,
      DataBitsEight,
      StopBitsOne,
      0xC0,                // LinesInUse
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

  Name (_DSD, Package() {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package (2) { "dte-mode", 1 },
      Package (2) {"SerCx-FriendlyName", "UART1"}
    }
  })
}

Device (UAR2)
{
  Name (_HID, "NXP0107")
  Name (_UID, 0x2)
  Name (_DDN, "UART2")

  Method (_STA) {
    Return (0x0)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x021E8000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 59 }

    // DMA channel 4, SDMA_REQ_UART2_RX for UART2 RX DMA
    FixedDMA (SDMA_REQ_UART2_RX, 4, Width8Bit, )
    // DMA channel 3, SDMA_REQ_UART2_TX for UART2 TX DMA
    FixedDMA (SDMA_REQ_UART2_TX, 3, Width8Bit, )
    })
}
