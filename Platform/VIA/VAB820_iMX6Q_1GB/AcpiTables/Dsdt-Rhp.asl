/** @file
*
*  iMX6 Resource Hub Proxy
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
    SPISerialBus (            // SCLK - 
                              // MOSI - 
                              // MISO - 
                              // CE0  - 
      0,                      // Device selection (CE0)
      PolarityLow,            // Device selection polarity
      FourWireMode,           // wiremode
      8,                      // databit len
      ControllerInitiated,    // slave mode
      4000000,                // connection speed
      ClockPolarityLow,       // clock polarity
      ClockPhaseFirst,        // clock phase
      "\\_SB.SPI1",           // ResourceSource: SPI bus controller name
      0,                      // ResourceSourceIndex
                              // Resource usage
                              // DescriptorName: creates name for
                              //   offset of resource descriptor
    )                         // Vendor Data

    // Index 2
    SPISerialBus (            // SCLK - 
                              // MOSI - 
                              // MISO - 
                              // CE0  - 
      0,                      // Device selection (CE0)
      PolarityLow,            // Device selection polarity
      FourWireMode,           // wiremode
      8,                      // databit len
      ControllerInitiated,    // slave mode
      4000000,                // connection speed
      ClockPolarityLow,       // clock polarity
      ClockPhaseFirst,        // clock phase
      "\\_SB.SPI3",           // ResourceSource: SPI bus controller name
      0,                      // ResourceSourceIndex
                              // Resource usage
                              // DescriptorName: creates name for
                              //   offset of resource descriptor
    )                         // Vendor Data

    // Index 3
    SPISerialBus (            // SCLK - 
                              // MOSI - 
                              // MISO - 
                              // CE1  - 
      1,                      // Device selection (CE1)
      PolarityLow,            // Device selection polarity
      FourWireMode,           // wiremode
      8,                      // databit len
      ControllerInitiated,    // slave mode
      4000000,                // connection speed
      ClockPolarityLow,       // clock polarity
      ClockPhaseFirst,        // clock phase
      "\\_SB.SPI3",           // ResourceSource: SPI bus controller name
      0,                      // ResourceSourceIndex
                              // Resource usage
                              // DescriptorName: creates name for
                              //   offset of resource descriptor
    )                         // Vendor Data

    // GPIO1_IO01 PAD_GPIO01
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 1 }
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 1 }

    // GPIO1_IO02 PAD_GPIO02
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 2 }
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 2 }

    // GPIO1_IO04 PAD_GPIO04
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 4 }
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 4 }

    // GPIO1_IO05 PAD_GPIO05
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 5 }
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 5 }

    // GPIO1_IO07 PAD_GPIO07
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 7 }
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 7 }

    // GPIO1_IO08 PAD_GPIO08
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 8 }
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 8 }

    // GPIO1_IO09 PAD_GPIO09
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 9 }
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 9 }

    // GPIO4_IO05 PAD_GPIO19
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 101 } // 3 * 32 + 5
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 101 }

    // GPIO7_IO11 PAD_GPIO16
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0,
            ResourceConsumer,, ) { 203 } // 6 * 32 + 11
    GpioInt (Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 203 }
  })

  Name (_DSD, Package() {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package() {
      // I2C3
      Package (2) { "bus-I2C-I2C3", Package() { 0 } },

      // SPI 1
      Package (2) { "bus-SPI-SPI1", Package() { 1 } }, // Index 1
      Package (2) { "SPI1-MinClockInHz", 115 },        // 115 Hz
      Package (2) { "SPI1-MaxClockInHz", 20000000 },   // 20 MHz
      // Data bit length
      Package (2) { "SPI1-SupportedDataBitLengths", Package() { 8, 16, 32 } },
      // SPI 3
      Package (2) { "bus-SPI-SPI3", Package() { 2, 3 } }, // Index 2, 3
      Package (2) { "SPI3-MinClockInHz", 115 },        // 115 Hz
      Package (2) { "SPI3-MaxClockInHz", 20000000 },   // 20 MHz
      // Data bit length
      Package (2) { "SPI3-SupportedDataBitLengths", Package() { 8, 16, 32 } },

      // GPIO Pin Count and supported drive modes
      Package (2) { "GPIO-PinCount", 206 },
      Package (2) { "GPIO-UseDescriptorPinNumbers", 1 },

      // InputHighImpedance, InputPullUp, InputPullDown, OutputCmos
      Package (2) { "GPIO-SupportedDriveModes", 0xf },
    }
  })
}
