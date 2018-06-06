/** @file
*
*  Udoo Neo Resource Hub Proxy (Rhproxy) table
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

  Name(_CRS, ResourceTemplate() {
    // Index 0
    // LVDS CN3, JTAG J11, PMIC
    I2CSerialBus (0xFFFF,, 0,, "\\_SB.I2C1",,,,)

    // Index 1
    // J10, J6
    I2CSerialBus (0xFFFF,, 0,, "\\_SB.I2C2",,,,)

    // Index 2
    // Accelerometer, Gyroscope
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
      "\\_SB.SPI2",         // ResourceSource: SPI bus controller name
      0,                    // ResourceSourceIndex
                            // Resource usage
                            // DescriptorName: creates name for offset of resource descriptor
    )                       // Vendor Data

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
      "\\_SB.SPI3",         // ResourceSource: SPI bus controller name
      0,                    // ResourceSourceIndex
                            // Resource usage
                            // DescriptorName: creates name for offset of resource descriptor
    )                       // Vendor Data

    // Index 5
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

    // GPIO1_IO14 PAD_CSI_DATA00
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 14 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 14 }

    // GPIO1_IO15 PAD_CSI_DATA01
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 15 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 15 }

    // GPIO1_IO16 PAD_CSI_DATA02
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 16 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 16 }

    // GPIO1_IO17 PAD_CSI_DATA03
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 17 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 17 }

    // GPIO1_IO18 PAD_CSI_DATA04
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 18 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 18 }

    // GPIO1_IO19 PAD_CSI_DATA05
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 19 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 19 }

    // GPIO1_IO20 PAD_CSI_DATA06
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 20 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 20 }

    // GPIO1_IO21 PAD_CSI_DATA07
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 21 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 21 }

    // GPIO1_IO22 PAD_CSI_HSYNC
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 22 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 22 }

    // GPIO1_IO24 PAD_CSI_PIXCLK
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 24 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 24 }

    // GPIO1_IO25 PAD_CSI_VSYNC
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 25 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 25 }

    // GPIO4_IO04 PAD_NAND_DATA00 - ECSPI5_MISO
    // 3 * 32 + 4
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 100 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 100 }

    // GPIO4_IO06 PAD_NAND_DATA02 - ECSPI5_SCLK
    // 3 * 32 + 6
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 102 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 102 }

    // GPIO4_IO09 PAD_NAND_DATA04 - GPIO_3
    // 3 * 32 + 8
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 104 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 104 }

    // GPIO4_IO09 PAD_NAND_DATA05 - GPIO_8
    // 3 * 32 + 9
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 105 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 105 }

    // GPIO4_IO10 PAD_NAND_DATA06
    // 3 * 32 + 10
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 106 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 106 }

    // GPIO4_IO11 PAD_NAND_DATA07
    // 3 * 32 + 11
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 107 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 107 }

    // GPIO4_IO20 PAD_QSPI1A_DQS
    // 3 * 32 + 20
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 116 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 116 }

    // GPIO4_IO23 PAD_QSPI1A_SS1_B
    // 3 * 32 + 23
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 119 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 119 }

    // GPIO4_IO24 PAD_QSPI1B_DATA0
    // 3 * 32 + 24
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 120 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 120 }

    // GPIO4_IO25 PAD_QSPI1B_DATA1
    // 3 * 32 + 25
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 121 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 121 }

    // GPIO4_IO28 PAD_QSPI1B_DQS
    // 3 * 32 + 28
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 124 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 124 }

    // GPIO4_IO29 PAD_QSPI1B_SCLK
    // 3 * 32 + 29
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 125 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 125 }

    // GPIO4_IO30 PAD_QSPI1B_SS0
    // 3 * 32 + 30
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 126 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 126 }

    // GPIO4_IO31 PAD_QSPI1B_SS1_B
    // 3 * 32 + 31
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 127 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 127 }

    // GPIO5_IO12 PAD_RGMII2_RD0 - PWM_4
    // 4 * 32 + 12
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 140 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 140 }

    // GPIO5_IO13 PAD_RGMII2_RD0 - PWM_3
    // 4 * 32 + 13
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 141 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 141 }

    // GPIO5_IO14 PAD_RGMII2_RD2 - PWM_2
    // 4 * 32 + 14
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 142 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 142 }

    // GPIO5_IO15 PAD_RGMII2_RD3 - PWM_1
    // 4 * 32 + 15
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 143 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 143 }

    // GPIO5_IO18 PAD_RGMII2_TD0 - PWM_8
    // 4 * 32 + 18
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 146 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 146 }

    // GPIO5_IO19 PAD_RGMII2_TD1 - PWM_7
    // 4 * 32 + 19
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 147 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 147 }

    // GPIO5_IO20 PAD_RGMII2_TD2 - PWM_6
    // 4 * 32 + 20
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 148 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 148 }

    // GPIO5_IO21 PAD_RGMII2_TD2 - PWM_5
    // 4 * 32 + 21
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 149 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 149 }

    // GPIO5_IO22 PAD_RGMII2_TX_CTL
    // 4 * 32 + 22
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 150 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 150 }

    // GPIO5_IO27 PAD_RGMII2_RXC
    // 4 * 32 + 27
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 155 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 155 }

    // GPIO6_IO00 PAD_SD1_CLK
    // 5 * 32 + 0
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 160 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 160 }

    // GPIO6_IO12 PAD_SD4_CLK
    // 5 * 32 + 12
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 172 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 172 }

    // GPIO6_IO13 PAD_SD4_CMD
    // 5 * 32 + 13
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 173 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 173 }

    // GPIO6_IO14 PAD_SD4_DATA0
    // 5 * 32 + 14
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 174 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 174 }

    // GPIO6_IO15 PAD_SD4_DATA1
    // 5 * 32 + 15
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 175 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 175 }

    // GPIO6_IO16 PAD_SD4_DATA2
    // 5 * 32 + 16
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 176 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 176 }

    // GPIO6_IO17 PAD_SD4_DATA3
    // 5 * 32 + 17
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 177 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 177 }

    // GPIO6_IO19 PAD_SD4_DATA4
    // 5 * 32 + 18
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 178 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 178 }

    // GPIO6_IO19 PAD_SD4_DATA5
    // 5 * 32 + 19
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 179 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 179 }

    // GPIO6_IO20 PAD_SD4_DATA6
    // 5 * 32 + 20
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 180 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 180 }

    // GPIO6_IO21 PAD_SD4_DATA7
    // 5 * 32 + 21
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 181 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 181 }

    // GPIO6_IO22 PAD_SD4_RESET_B
    // 5 * 32 + 22
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 182 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 182 }

    // GPIO7_IO10 PAD_USB_H_DATA
    // 6 * 32 + 10
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 202 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 202 }

    // GPIO7_IO11 PAD_USB_H_STROBE
    // 6 * 32 + 11
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 203 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 203 }
  })

  Name(_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package (2) { "bus-I2C-I2C1", Package() { 0 }},
      Package (2) { "bus-I2C-I2C2", Package() { 1 }},
      Package (2) { "bus-I2C-I2C4", Package() { 2 }},

      // SPI 2
      Package (2) { "bus-SPI-SPI2", Package() { 3 }},                        // Index 3
      Package (2) { "SPI2-MinClockInHz", 115 },                              // 115 Hz
      Package (2) { "SPI2-MaxClockInHz", 8000000 },                          // 8 MHz
      Package (2) { "SPI2-SupportedDataBitLengths", Package() { 8,16,32 }},  // Data bit length

      // SPI 3
      Package (2) { "bus-SPI-SPI3", Package() { 4 }},                        // Index 4
      Package (2) { "SPI3-MinClockInHz", 115 },                              // 115 Hz
      Package (2) { "SPI3-MaxClockInHz", 8000000 },                          // 8 MHz
      Package (2) { "SPI3-SupportedDataBitLengths", Package() { 8,16,32 }},  // Data bit length

      // SPI 5
      Package (2) { "bus-SPI-SPI5", Package() { 5 }},                        // Index 5
      Package (2) { "SPI5-MinClockInHz", 115 },                              // 115 Hz
      Package (2) { "SPI5-MaxClockInHz", 8000000 },                          // 8 MHz
      Package (2) { "SPI5-SupportedDataBitLengths", Package() { 8,16,32 }},  // Data bit length

      // GPIO Pin Count and supported drive modes
      Package (2) { "GPIO-PinCount", 204 },
      Package (2) { "GPIO-UseDescriptorPinNumbers", 1 },

      // InputHighImpedance, InputPullUp, InputPullDown, OutputCmos
      Package (2) { "GPIO-SupportedDriveModes", 0xf },
    }
  })
}
