/** @file
*
*  Compulab iMX7 Ultra Secured Digital Host Controllers (uSDHC)
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
**/

// uSDHC1
Device (SDH1)
{
  Name (_HID, "NXP0108")
  Name (_UID, 0x1)
  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30B40000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 54 }
  })

  // Child node to represent the only SD/MMC slot on this SD/MMC bus
  // In theory an SDHC can be connected to multiple SD/MMC slots at
  // the same time, but only 1 device will be selected and active at
  // a time
  Device (SD0) {
    Method (_ADR) {
      Return (0)
    }

    Method (_RMV) {
      Return (0)
    }
  }
}

// uSDHC2
Device (SDH2)
{
  Name (_HID, "NXP0108")
  Name (_UID, 0x2)
  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30B50000, 0x4000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 55 }
  })

  // Child node to represent the only SD/MMC slot on this SD/MMC bus
  // In theory an SDHC can be connected to multiple SD/MMC slots at
  // the same time, but only 1 device will be selected and active at
  // a time
  Device (SD0) {
    Method (_ADR) {
      Return (0)
    }

    Method (_RMV) {
      Return (0)
    }
  }
}

// uSDHC3
Device (SDH3)
{
  Name (_HID, "NXP0108")
  Name (_UID, 0x3)
  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
      MEMORY32FIXED (ReadWrite, 0x30B60000, 0x4000, )
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 56 }
  })

  // Child node to represent the only SD/MMC slot on this SD/MMC bus
  // In theory an SDHC can be connected to multiple SD/MMC slots at
  // the same time, but only 1 device will be selected and active at
  // a time
  Device (SD0) {
    Method (_ADR) {
      Return (0)
    }

    Method (_RMV) {
      Return (0)
    }
  }
}
