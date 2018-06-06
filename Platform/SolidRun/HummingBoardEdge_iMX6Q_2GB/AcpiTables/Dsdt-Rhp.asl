/** @file
*
*  iMX6 Quad Resource Hub Proxy
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

Device (RHPX)
{
  Name (_HID, "MSFT8000")
  Name (_CID, "MSFT8000")
  Name (_UID, 1)

  Name (_CRS, ResourceTemplate() {
    // Index 0
    I2CSerialBus (0xFFFF,, 0,, "\\_SB.I2C3",,,,)

    // Index 1
    SPISerialBus (            // SCKL
                              // MOSI
                              // MISO
                              // CE0
      0,                      // Device selection (CE0)
      PolarityLow,            // Device selection polarity
      FourWireMode,           // wiremode
      8,                      // databit len
      ControllerInitiated,    // slave mode
      4000000,                // connection speed
      ClockPolarityLow,       // clock polarity
      ClockPhaseFirst,        // clock phase
      "\\_SB.SPI2",           // ResourceSource: SPI bus controller name
      0,                      // ResourceSourceIndex
                              // Resource usage
                              // DescriptorName: creates name for
                              //   offset of resource descriptor
    )                         // Vendor Data

    // SPDIF_IN - GPIO1_IO24 PAD_ENET_RX_ER
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 24 } // 0 * 32 + 24
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 24 }

    // DISP1_DATA17 - GPIO2_IO16 PAD_EIM_ADDR22
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 48 } // 1 * 32 + 16
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 48 }

    // DISP1_DATA16 - GPIO2_IO17 PAD_EIM_ADDR21
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 49 } // 1 * 32 + 17
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 49 }

    // DISP1_DATA15 - GPIO2_IO18 PAD_EIM_ADDR20
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 50 } // 1 * 32 + 18
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 50 }

    // DISP1_DATA14 - GPIO2_IO19 PAD_EIM_ADDR19
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 51 } // 1 * 32 + 19
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 51 }

    // DISP1_DATA13 - GPIO2_IO20 PAD_EIM_ADDR18
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 52 } // 1 * 32 + 20
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 52 }

    // DISP1_DATA12 - GPIO2_IO21 PAD_EIM_ADDR17
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 53 } // 1 * 32 + 21
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 53 }

    // DI1_DISP_CLK - GPIO2_IO22 PAD_EIM_ADDR16
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 54 } // 1 * 32 + 22
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 54 }

    // DISP1_DATA11 - GPIO2_IO28 PAD_EIM_EB0
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 60 } // 1 * 32 + 28
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 60 }

    // DISP1_DATA10 - GPIO2_IO29 PAD_EIM_EB1
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 61 } // 1 * 32 + 29
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 61 }

    // DISP1_DATA09 - GPIO3_IO00 PAD_EIM_AD00
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 64 } // 2 * 32 + 0
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 64 }

    // DISP1_DATA08 - GPIO3_IO01 PAD_EIM_AD01
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 65 } // 2 * 32 + 1
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 65 }

    // DISP1_DATA07 - GPIO3_IO02 PAD_EIM_AD02
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 66 } // 2 * 32 + 2
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 66 }

    // DISP1_DATA06 - GPIO3_IO03 PAD_EIM_AD03
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 67 } // 2 * 32 + 3
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 67 }

    // DISP1_DATA05 - GPIO3_IO04 PAD_EIM_AD04
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 68 } // 2 * 32 + 4
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 68 }

    // DISP1_DATA04 - GPIO3_IO05 PAD_EIM_AD05
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 69 } // 2 * 32 + 5
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 69 }

    // DISP1_DATA03 - GPIO3_IO06 PAD_EIM_AD06
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 70 } // 2 * 32 + 6
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 70 }

    // DISP1_DATA02 - GPIO3_IO07 PAD_EIM_AD07
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 71 } // 2 * 32 + 7
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 71 }

    // DISP1_DATA01 - GPIO3_IO08 PAD_EIM_AD08
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 72 } // 2 * 32 + 8
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 72 }

    // DISP1_DATA00 - GPIO3_IO09 PAD_EIM_AD09
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 73 } // 2 * 32 + 9
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 73 }

    // DI1_PIN15 - GPIO3_IO10 PAD_EIM_AD10
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 74 } // 2 * 32 + 10
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 74 }

    // DI1_PIN02 - GPIO3_IO11 PAD_EIM_AD11
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 75 } // 2 * 32 + 11
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 75 }

    // DI1_PIN03 - GPIO3_IO12 PAD_EIM_AD12
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 76 } // 2 * 32 + 12
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 76 }

    // DI1_D0_CS - GPIO3_IO13 PAD_EIM_AD13
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 77 } // 2 * 32 + 13
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 77 }

    // DI1_D1_CS - GPIO3_IO14 PAD_EIM_AD14
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 78 } // 2 * 32 + 14
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 78 }

    // DI1_PIN01 - GPIO3_IO15 PAD_EIM_AD15
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 79 } // 2 * 32 + 15
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 79 }

    // DISP1_DATA22 - GPIO3_IO26 PAD_EIM_DATA26
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 90 } // 2 * 32 + 26
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 90 }

    // DISP1_DATA23 - GPIO3_IO27 PAD_EIM_DATA27
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 91 } // 2 * 32 + 27
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 91 }

    // DISP1_DATA21 - GPIO3_IO30 PAD_EIM_DATA30
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 94 } // 2 * 32 + 20
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 94 }

    // DISP1_DATA20 - GPIO3_IO31 PAD_EIM_DATA31
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 95 } // 2 * 32 + 31
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 95 }

    // DISP1_DATA19 - GPIO5_IO04 PAD_EIM_ADDR24
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 132 } // 4 * 32 + 4
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 132 }

    // DISP1_DATA18 - GPIO6_IO06 PAD_EIM_ADDR23
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 166 } // 5 * 32 + 6
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 166 }

    // SPDIF_OUT - GPIO7_IO12 PAD_GPIO17
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 204 } // 6 * 32 + 12
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 204 }
  })

  Name (_DSD, Package() {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package() {
      // I2C3
      Package (2) { "bus-I2C-I2C3", Package() { 0 } },

      // SPI 2
      Package (2) { "bus-SPI-SPI2", Package() { 1 } }, // Index 1
      Package (2) { "SPI2-MinClockInHz", 115 },        // 115 Hz
      Package (2) { "SPI2-MaxClockInHz", 12000000 },   // 12 MHz
      // Data bit length
      Package (2) { "SPI2-SupportedDataBitLengths", Package() { 8, 16, 32 } },

      // GPIO Pin Count and supported drive modes
      Package (2) { "GPIO-PinCount", 206 },
      Package (2) { "GPIO-UseDescriptorPinNumbers", 1 },

      // InputHighImpedance, InputPullUp, InputPullDown, OutputCmos
      Package (2) { "GPIO-SupportedDriveModes", 0xf },
    }
  })
}
