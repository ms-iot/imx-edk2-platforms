/** @file
*
*  iMX7 CPU, Timer, and Global Page Description
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

OperationRegion(GLBL,SystemMemory,0x82003000,0x18)
Field(GLBL, AnyAcc, Nolock, Preserve)
{
  Offset(0),        // Miscellaneous Dynamic Registers:
  SIGN, 32,         // Global Page Signature 'GLBL'
  REVN, 8,          // Revision
      , 8,          // Reserved
      , 8,          // Reserved
      , 8,          // Reserved
  M0ID, 8,          // MAC 0 ID
  MC0V, 8,          // MAC 0 Valid
  MC0L, 32,         // MAC Address 0 Low
  MC0H, 16,         // MAC Address 0 High
  M1ID, 8,          // MAC 1 ID
  MC1V, 8,          // MAC 1 Valid
  MC1L, 32,         // MAC Address 1 Low
  MC1H, 16,         // MAC Address 1 High
}

// CPU Cluster Device
Device (CL0)
{
  Name (_HID, "ACPI0010")
  Name (_UID, 0x10)
  Name (_LPI, Package() {
    0,            // Version
    0x01000000,   // Level ID
    1,             // Count

    // A7 WAIT mode
    Package () {
      75,     // Min residency (us)
      50,     // Wake latency (us)
      1,      // Flags, set bit0 to 1 to enable this state.
      1,      // Arch context lost flags. Even though this state
              // doesn't lose context, must set this to 1 so Windows
              // uses always-on timer
      0,      // Residency counter frequency
      0,      // Enabled parent state
      0x40,   // Integer entry method
      ResourceTemplate(){Register(SystemMemory,0,0,0,0)}, // Residency counter register
      ResourceTemplate(){Register(SystemMemory,0,0,0,0)}, // Usage counter register
      "A7 clock gated"      // Name
    },
  })

  Device (CPU0) {
    Name (_HID, "ACPI0007")
    Name (_UID, 0x0)
    Method (_STA) {
      Return (0xf)
    }

    Name (_LPI, Package () {
      0, // Version
      0, // Level ID
      2, // Count

      // Core WFI
      Package () {
        0, // Min residency (us)
        0, // Wake latency (us)
        1, // Flags, set bit0 to 1 to enable this state
        0, // Arch context last flags
        0, // Residency counter frequency
        0, // Enabled parent state
        ResourceTemplate () {
          Register (FFixedHW, 0x20, 0, 0x00000000FFFFFFFF, 3) // Register entry method <= WFI
        },
        ResourceTemplate () {
          Register (SystemMemory,0,0,0,0) // Residency counter register
        },
        ResourceTemplate () {
          Register (SystemMemory,0,0,0,0) // Usage counter register
        },
        "WFI"       // Name
      },

      // Core clock gate
      Package () {
        75, // Min residency (us)
        50, // Wake latency (us)
        1,  // Flags, set bit0 to 1 to enable this state
        1,  // Arch context lost flags. Even though this state
            // doesn't lose context, must set this to 1 so Windows
            // uses always-on timer
        0,  // Residency counter frequency
        1,  // Enabled parent state (Enables A7 clock gate)
        ResourceTemplate () {
          Register (FFixedHW, 0x20, 0,0x0000000040000004,3)
        },
        ResourceTemplate () {
          Register (SystemMemory,0,0,0,0) // Residency counter register
        },
        ResourceTemplate () {
          Register (SystemMemory,0,0,0,0) // Usage counter register
        },
        "A7 clock gated"       // Name
      },
    })
  }

  Device (CPU1) {
    Name (_HID, "ACPI0007")
    Name (_UID, 0x1)
    Method (_STA) {
        Return(0xf)
    }

    Name (_LPI, Package () {
      0, // Version
      0, // Level ID
      2, // Count

      // Core WFI
      Package () {
        0, // Min residency (us)
        0, // Wake latency (us)
        1, // Flags, set bit0 to 1 to enable this state
        0, // Arch context last flags
        0, // Residency counter frequency
        0, // Enabled parent state
        ResourceTemplate () {
          Register (FFixedHW, 0x20, 0, 0x00000000FFFFFFFF, 3) // Register entry method <= WFI
        },
        ResourceTemplate () {
          Register (SystemMemory,0,0,0,0) // Residency counter register
        },
        ResourceTemplate () {
          Register (SystemMemory,0,0,0,0) // Usage counter register
        },
        "WFI"       // Name
      },

      // Core clock gate
      Package ()
      {
        75, // Min residency (us)
        50, // Wake latency (us)
        1,  // Flags, set bit0 to 1 to enable this state
        1,  // Arch context lost flags. Even though this state
            // doesn't lose context, must set this to 1 so Windows
            // uses always-on timer
        0,  // Residency counter frequency
        1,  // Enabled parent state (Enables A7 clock gate)
        ResourceTemplate () {
          Register (FFixedHW, 0x20, 0,0x0000000040000004,3)
        },
        ResourceTemplate () {
          Register (SystemMemory,0,0,0,0) // Residency counter register
        },
        ResourceTemplate () {
          Register (SystemMemory,0,0,0,0) // Usage counter register
        },
        "A7 clock gated"       // Name
      },
    })
  }
}
