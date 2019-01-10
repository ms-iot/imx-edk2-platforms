/** @file
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

Device (PCI0)
{
  Name (_HID, "PNP0A08")
  Name (_UID, 0)
  Name (_STR, Unicode ("PCIe 0 Device"))
  Name (_BBN, 0)
  Name (_SEG, 0)
  Name (_CCA, 0)  // Pcie is not cache coherent

  // PCIE_PL memory map
  OperationRegion (PLGC, SystemMemory, 0x01FFC700, 0x220)
  Field (PLGC, DWordAcc, NoLock, Preserve) {
    Offset (0x2C),  // Skip to register 0x01FFC72C
    DBG1, 32,       // PCIE_PL_DEBUG1
  }

  // IOMUXC memory map
  OperationRegion (IMXC, SystemMemory, 0x020E0000, 0x4000)
  Field (IMXC, DWordAcc, NoLock, Preserve) {
    Offset (0x04),  // Skip to register 0x020E0004
    GPR1, 32,       // IOMUXC_GPR1
  }

  // PCIe is only available if PCIe PHY reference clock is enabled and link is up
  Method (_STA, 0x0, Serialized) {
    Name (LNK, 0x0);
    Name (GPR, 0x0);
    Store (GPR1, GPR);  // read IOMUXC_GPR1 register

    // Check if the PCIe PHY reference clock is enabled
    If (LNotEqual (And (GPR, 0x00010000), 0x00010000)) {
      Return (0x0)
    }

    Store (DBG1, LNK);  // read PCIE_PL_DEBUG1 register

    // Check if link is already up and is not in training
    //  Bit4  : [36]: xmlh_link_up LTSSM reports PHY link up
    //  Bit29  : [61]: xmlh_link_in_training LTSSM performing link training
    If (LAnd (And (DBG1, 0x00000010), Lequal (And (DBG1, 0x20000000), 0x00))) {
      Return (0xF)
    } else {
      Return (0x0)
    }
  }

  // Declare the resources assigned to this root complex.
  Name (_CRS, ResourceTemplate () {
    WordBusNumber (
      ResourceProducer, // Specify bus ranged is passed to child devices
      MinFixed,         // Specify min address is fixed
      MaxFixed,         // Specify max address is fixed
      PosDecode,        // Positive decode of bus number
      0,                // AddressGranularity 2 power of 0
      0,                // AddressMinimum - Minimum Bus Number
      1,                // AddressMaximum - Maximum Bus Number
      0,                // AddressTranslation - Set to 0
      2)                // RangeLength - Number of Busses

    // PCI memory space
    Memory32Fixed (ReadWrite, 0x01100000, 0x00E00000, )
  })

  // Declare the PCI Routing Table.
  Name (_PRT, Package() {
    // Routing for device 0, all functions.
    // Note: ARM doesn't support LNK nodes, so the third param
    // is 0 and the fourth param is the SPI number of the interrupt
    // line.
    Package() {0x0000FFFF, 0, 0, 155}, // INTA
    Package() {0x0000FFFF, 1, 0, 154}, // INTB
    Package() {0x0000FFFF, 2, 0, 153}, // INTC
    Package() {0x0000FFFF, 3, 0, 152}, // INTD/MSI
  })

  Name (SUPP, 0) // PCI _OSC Support Field value
  Name (CTRL, 0) // PCI _OSC Control Field value

  // Declare an _OSC (OS Control Hand-off) method which takes 4 arguments.
  //
  // Arguments:
  //   Arg0  A Buffer containing a UUID
  //   Arg1  An Integer containing a Revision ID of the buffer format
  //   Arg2  An Integer containing a count of entries in Arg3
  //   Arg3  A Buffer containing a list of DWORD capabilities
  // Return Value:
  //   A Buffer containing a list of capabilities
  Method (_OSC, 4) {

    // Create DWord-addressable fields from the Capabilities Buffer
    // Create CDW1 outside the test as it's used in the else clause.
    // The first DWORD in the _OSC Capabilities Buffer contains bits
    // that are generic to _OSC
    // PCI Firmware Specification - 4.5.1.
    //
    // ACPI - 6.2.10
    // CDW return value information
    // Bit 0 - Reserved (not used)
    // Bit 1 - _OSC failure. Platform Firmware was unable to process the request or query
    // Bit 2 - Unrecognized UUID
    // Bit 3 - Unrecognized Revision
    // Bit 4 - Capabilities Masked
    // All others reserved

    CreateDWordField (Arg3, 0, CDW1)
    If (LEqual (Arg0, ToUUID ("33DB4D5B-1FF7-401C-9657-7441C03DD766"))) {
      CreateDWordField (Arg3, 4, CDW2)
      CreateDWordField (Arg3, 8, CDW3)

      // Save Capabilities DWord 2 & 3
      // The second DWORD in the _OSC Capabilities Buffer is the Support Field
      // The third DWORD in the _OSC Capabilities Buffer is the Control Field
      // PCI Firmware Specification - 4.5.1.
      // Supported feature should be set in the control field
      //
      // Support field
      // Bit 0 - Extended PCI Config operation regions supported
      // Bit 1 - Active State Power Management supported
      // Bit 2 - Clock Power Management Capability supported
      // Bit 3 - PCI Segment Groups supported
      // Bit 4 - MSI supported
      // Bit 5 - Optimized Buffer Flush and Fill supported
      // Bit 6 - ASPM Optionality supported
      //
      // Control field
      // Bit 0 - PCI Express Native Hot Plug control
      // Bit 1 - SHPC Native Hot Plug control
      // Bit 2 - PCI Express Native Power Management Events control
      // Bit 3 - PCI Express Advanced Error Reporting control
      // Bit 4 - PCI Express Capability Structure control
      // Bit 5 - Latency Tolerance Reporting control

      Store (CDW2, SUPP)
      Store (CDW3, CTRL)

      // Only allow native hot plug control if OS supports:
      //  ASPM
      //  Clock PM
      //  MSI/MSI-X
      If (LNotEqual (And (SUPP, 0x16), 0x16)) {
        And (CTRL, 0x1E, CTRL)
      }

      // Spec does not mention if standard hot plug is supported
      // so unmask the bit
      And (CTRL, 0x1D, CTRL)

      // Check for unknown revision.
      If (LNotEqual (Arg1, One)) {
        Or (CDW1, 0x08, CDW1)
      }

      // Check if capabilities bits were masked.
      If (LNotEqual (CDW3, CTRL)) {
        Or (CDW1, 0x10, CDW1)
      }

      // Update DWORD3 in the buffer.
      Store (CTRL, CDW3)
      Return (Arg3)
    } else {
      // Unrecognized UUID
      Or (CDW1, 4, CDW1)
      Return (Arg3)
    }
  }

  // Declare a _DSM method for various functions called by the OS.
  // See the APCI spec, Section 9.14.1,
  // and the PCI FW spec, Section 4.6.
  // See also:
  // http://download.microsoft.com/download/9/c/5/9c5b2167-8017-4bae-9fde-d599bac8184a/PCI-rsc.doc
  Method (_DSM, 0x4, Serialized) {
    // Match against the _DSM PCI UUID. PCI firmware spec - 4.6.1
    //     Arg0 - UUID
    //     Arg1 - Revision ID
    //     Arg2 - Function Index
    //     Arg3 - Empty Package
    //
    // Revision 1 & 2
    //     Function 1 - PCI Express Slot Information
    //     Function 2 - PCI Express Slot Number
    //     Function 3 - Vendor-specific Token ID
    //     Function 4 - PCI Bus Capabilities
    //     Function 5 - Ignore PCI Boot Configuration
    // Revision 2 only
    //     Function 6 - LTR Maximum Latency
    //     Function 7 - Express device under OS
    If (LEqual (Arg0, ToUUID ("E5C937D0-3553-4d7a-9117-EA4D19C3434D"))) {

      switch (ToInteger (Arg2)) {
      // Function 0: Return supported functions as a bit-field
      // with one bit for each supported function.
      // Bit 0 must always be set, as that represents
      // function 0 (which is what is being called here).
      // Support for different functions may depend on
      // the revision ID of the interface, passed as Arg1.

      // Function 0: Get supported functions
      case (0) {
          // Functions 0,1,5 are supported.
          if (LEqual (Arg1, 1)) {
            return (Buffer() { 0x23 })
          }

          // Functions 0,1,5,7 are supported.
          if (LEqual (Arg1, 2)) {
            return (Buffer() { 0xA3 })
          }

          // Functions 0,1,5,7 are supported for
          // future revision. Expect backward compatibility
          return (Buffer() { 0xA3 })
        }

      // Function 1: PCI Express Slot Information
      // PCI firmware spec - 4.6.1
      case (1) {
          // Package item 1:
          //    0: Failure
          //    1: Success
          // Package item 2:
          //  Integer 1:Bit Position
          //        0 Supports x1
          //        1 Supports x2
          //        2 Supports x4
          //        3 Supports x8
          //        4 Supports x12
          //        5 Supports x16
          //  Integer 2:
          //        0h Unknown
          //        1h PCI Express Card Slot
          //        2h PCI Express Server I/O Module Slot
          //        3h PCI Express ExpressCard* Slot
          //        4h PCI Express Mini Card Slot
          //        5h PCI Express Wireless Form Factor Slot
          //        Others Reserved
          //  Integer 3:
          //        0 SMBus signal
          //        1 WAKE# signal
          return ( Package (2) {
            0x01, // Status success (1)
            Package() {
              0, // Supports x1
              4, // PCI Express Mini Card Slot
              1  // Supported signals (no SMBus, WAKE#)
            }
          })
        }

      // Function 5: Ignore PCI Boot Configurations
      // PCI firmware spec - 4.6.5
      case (5) {
          // Always allow OS to rebalance
          return (0x01)
        }

      // Function 7: Naming a PCI or PCI Express Device Under
      //             Operating Systems
      // PCI firmware spec - 4.6.7
      case (7) {
          // Verify revision 2
          if (LEqual (Arg1, 2)) {
            // PCI Express Slot Parsing
            Return ( Package (2) {
              1,            // Instance of the enumeration
              "PCIe Slot 1" // String name which matches the label on the chassis
            })
          }
        }
      }
    }
    return (Buffer() { 0 })
  }

  // Root Port 0 Device within the Root Complex.
  Device (RP0) {
    Name (_ADR, 0)  // Device 0, Function 0
    Name (_S0W, 4)  // Supports wake
  }

}
