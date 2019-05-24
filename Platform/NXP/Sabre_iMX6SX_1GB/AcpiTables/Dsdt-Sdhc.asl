/** @file
*
*  iMX6 Sabre Ultra Secured Digital Host Controller (uSDHC)
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

// uSDHC2: SDIO Socket
Device (SDH2)
{
  Name (_HID, "NXP0108")
  Name (_UID, 0x2)

  Method (_STA) {
    Return(0xf)
  }

  Name (_S1D, 0x1)
  Name (_S2D, 0x1)
  Name (_S3D, 0x1)
  Name (_S4D, 0x1)

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED(ReadWrite, 0x02194000, 0x4000, )
    Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 55 }
  })

  // Child node to represent the only SD/MMC slot on this SD/MMC bus
  // In theory an SDHC can be connected to multiple SD/MMC slots at
  // the same time, but only 1 device will be selected and active at
  // a time
  Device (SD0) {
    Method (_ADR) {
      Return (0)
    }

    Method (_RMV) {  // Remove
      Return (0)     
    }
  }
}

// uSDHC3: SDCard Socket (8-bit)
Device (SDH3)
{
  Name (_HID, "NXP0108")
  Name (_UID, 0x3)

  Method (_STA) {
    Return(0xf)
  }

  Name (_S1D, 0x1)
  Name (_S2D, 0x1)
  Name (_S3D, 0x1)
  Name (_S4D, 0x1)

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED(ReadWrite, 0x02198000, 0x4000, )
    Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 56 }
    // CD pin - GPIO2_IO10 - 1 * 32 + 10
    GpioIO(Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 42 }
    GpioInt(Edge, ActiveBoth, Shared, PullUp, 0, "\\_SB.GPIO",) { 42 }
  })

  Device (SD0) {
    Method (_ADR) {
      Return (0)
    }

    Method (_RMV) {  // Remove
      Return (1)     // Removable
    }
  }
}

// uSDHC4: SDCard Socket (Boot)
Device (SDH4)
{
  Name (_HID, "NXP0108")
  Name (_UID, 0x4)

  Method (_STA) {
    Return(0xf)
  }

  Name (_S1D, 0x1)
  Name (_S2D, 0x1)
  Name (_S3D, 0x1)
  Name (_S4D, 0x1)

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED(ReadWrite, 0x0219C000, 0x4000, )
    Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 57 }
  })


  Device (SD0) {
    Method (_ADR) {
      Return (0)
    }

    //
    // Despite the SDCard is a removal device, UWF requires the
    // boot device to be non-removable. This is required for the
    // Windows SDCard boot scenario with UWF enabled.
    //
    Method (_RMV) {  // Remove
      Return (0)     // Fixed
    }
  }
}
