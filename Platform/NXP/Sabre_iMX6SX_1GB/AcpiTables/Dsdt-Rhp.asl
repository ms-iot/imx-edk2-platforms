/** @file
*
*  iMX6 Sabre Resource Hub Proxy
*
*  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
*  Copyright 2019 NXP
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
  Name(_CRS, ResourceTemplate() {
    // Index 0 - 3
    I2CSerialBus(0xFFFF,, 0,, "\\_SB.I2C1",,,,)
    I2CSerialBus(0xFFFF,, 0,, "\\_SB.I2C2",,,,)
    I2CSerialBus(0xFFFF,, 0,, "\\_SB.I2C3",,,,)
    I2CSerialBus(0xFFFF,, 0,, "\\_SB.I2C4",,,,)
    // Index 4
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
      "\\_SB.SPI5",         // ResourceSource: SPI bus controller name
      0,                    // ResourceSourceIndex
                            // Resource usage
                            // DescriptorName: creates name for offset of resource descriptor
    )                       // Vendor Data

    // GPIO1_IO18 PAD_CSI_DATA04
    GpioIO(Shared, PullDown, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 18 }
    GpioInt(Edge, ActiveBoth, Shared, PullDown, 0, "\\_SB.GPIO",) { 18 }

    // GPIO1_IO19 PAD_CSI_DATA05
    GpioIO(Shared, PullDown, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 19 }
    GpioInt(Edge, ActiveBoth, Shared, PullDown, 0, "\\_SB.GPIO",) { 19 }
  })

  Name(_DSD, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package() {
      // I2C1, I2C2, I2C3, I2C4
      Package(2) { "bus-I2C-I2C1", Package() { 0 }},                        // Index 0
      Package(2) { "bus-I2C-I2C2", Package() { 1 }},                        // Index 1
      Package(2) { "bus-I2C-I2C3", Package() { 2 }},                        // Index 2
      Package(2) { "bus-I2C-I2C4", Package() { 3 }},                        // Index 3
      // SPI 5
      Package(2) { "bus-SPI-SPI5", Package() { 4 }},                        // Index 4
      Package(2) { "SPI5-MinClockInHz", 115 },                              // 115 Hz
      Package(2) { "SPI5-MaxClockInHz", 8000000 },                          // 8 MHz
      Package(2) { "SPI5-SupportedDataBitLengths", Package() { 8,16,32 }},  // Data bit length
      // GPIO Pin Count and supported drive modes
      Package (2) { "GPIO-PinCount", 204 },
      Package (2) { "GPIO-UseDescriptorPinNumbers", 1 },
      // Supported Drive Modes: InputHighImpedance, InputPullUp, InputPullDown, OutputCmos
      Package (2) { "GPIO-SupportedDriveModes", 0xf },
    }
  })
}
