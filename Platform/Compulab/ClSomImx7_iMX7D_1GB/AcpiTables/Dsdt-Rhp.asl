/**
*
*  Compulab iMX7 Dual Resource Hub Proxy
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

Device(RHPX)
{
  Name(_HID, "MSFT8000")
  Name(_CID, "MSFT8000")
  Name(_UID, 1)
  Name(_CRS, ResourceTemplate () {
    // Index 0
    I2CSerialBus (0xFFFF,, 0,, "\\_SB.I2C2",,,,)

    // Index 1
    I2CSerialBus (0xFFFF,, 0,, "\\_SB.I2C3",,,,)

    // Index 2
    I2CSerialBus (0xFFFF,, 0,, "\\_SB.I2C4",,,,)

    // Index 3
    SPISerialBus (          // SCLK
                            // MOSI
                            // MISO
                            // CE0
      0,                    // Device selection (CE0)
      PolarityLow,          // Device selection polarity
      FourWireMode,         // wiremode
      8,                    // databit len
      ControllerInitiated,  // slave mode
      4000000,              // connection speed
      ClockPolarityLow,     // clock polarity
      ClockPhaseFirst,      // clock phase
      "\\_SB.SPI3",         // ResourceSource: SPI bus controller name
      0,                    // ResourceSourceIndex
                            // Resource usage
                            // DescriptorName: creates name for offset of resource descriptor
    )                       // Vendor Data

    // Index 4
    SPISerialBus (          // SCLK
                            // MOSI
                            // MISO
                            // CE0
      1,                    // Device selection (CE1)
      PolarityLow,          // Device selection polarity
      FourWireMode,         // wiremode
      8,                    // databit len
      ControllerInitiated,  // slave mode
      4000000,              // connection speed
      ClockPolarityLow,     // clock polarity
      ClockPhaseFirst,      // clock phase
      "\\_SB.SPI3",         // ResourceSource: SPI bus controller name
      0,                    // ResourceSourceIndex
                            // Resource usage
                            // DescriptorName: creates name for offset of resource descriptor
    )                       // Vendor Data


    // GPIO 6 - GPIO2_IO28 EPDC_BDR0
    // 1 * 32 + 28
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 60 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 60 }

    // GPIO 10 - GPIO2_IO29 EPDC_BDR1
    // 1 * 32 + 29
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 61 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 61 }

    // GPIO 3 - GPIO2_IO30 EPDC_PWR_COM
    // 1 * 32 + 30
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 62 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 62 }

    // GPIO 1 - GPIO2_IO31 EPDC_PWR_STAT
    // 1 * 32 + 31
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 63 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 63 }

    // GPIO 2 - GPIO3_IO12 LCD_DATA07
    // 2 * 32 + 12
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 76 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 76 }

    // GPIO 4 - GPIO3_IO13 LCD_DATA08
    // 2 * 32 + 13
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 77 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 77 }

    // GPIO 5 - GPIO3_IO14 LCD_DATA09
    // 2 * 32 + 14
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 78 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 78 }

    // GPIO 8 - GPIO3_IO15 LCD_DATA10
    // 2 * 32 + 15
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 79 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 79 }

    // GPIO 9 - GPIO3_IO16 LCD_DATA11
    // 2 * 32 + 16
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 80 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 80 }

    // GPIO 11 - GPIO7_IO08 ENET1_RGMII_TD2
    // 6 * 32 + 8
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 200 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 200 }
  })

  Name(_DSD, Package() {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      // I2C2
      Package (2) { "bus-I2C-I2C2", Package() { 0 }},

      // I2C3
      Package (2) { "bus-I2C-I2C3", Package() { 1 }},

      // I2C4
      Package (2) { "bus-I2C-I2C4", Package() { 2 }},

      // SPI3
      Package (2) { "bus-SPI-SPI3", Package() { 3, 4 }},                     // Index 2
      Package (2) { "SPI3-MinClockInHz", 115 },                              // 115 Hz
      Package (2) { "SPI3-MaxClockInHz", 12000000 },                         // 12 MHz
      Package (2) { "SPI3-SupportedDataBitLengths", Package() { 8,16,32 }},  // Data bit length

      // GPIO Pin Count and supported drive modes
      Package (2) { "GPIO-PinCount", 204 },
      Package (2) { "GPIO-UseDescriptorPinNumbers", 1 },

      // InputHighImpedance, InputPullUp, InputPullDown, OutputCmos
      Package (2) { "GPIO-SupportedDriveModes", 0xf },
    }
  })
}
