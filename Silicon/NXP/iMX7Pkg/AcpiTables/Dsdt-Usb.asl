/** @file
*
*  iMX7 USB OTG and EHCI Controllers
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

Device(OTG1)
{
  Name(_HID, "PNP0C90")
  Name(_UID, 0x1)

  // URS requires device to at least be wake-able from D2 state
  // WDF also requires that _DSW (enable & disable wake ability) to be present
  Name(_S0W, 0x3)
  Name(_PRW, Package() {0,0})
  Method(_DSW, 0x3, NotSerialized) {
  }

  Method (_STA) {
    Return(0xf)
  }

  Name (_CRS, ResourceTemplate () {
    // Controller register address space. URS driver would add 0x0100
    // offset for host mode
    MEMORY32FIXED(ReadWrite, 0x30B10000, 0x200, )

    // USB_OTG_ID pin, needs to be declared as *Wake as this device is
    // expected to be wakable. The USB PHY is capable to detect
    // USB ID changes but the interrupt cannot be acknowledge
    // and the behaviour is undefined based on NXP feedback. So
    // the the only way to reliably detect USB ID changed is to
    // either to share interrupts or assign a GPIO to detect.
    // NXP recommends the shared interrupts approach. Currently
    // there is no GPIO driver so use shared interrupt implementation.
    Interrupt(ResourceConsumer, Level, ActiveHigh, SharedAndWake) { 75 }
  })

  OperationRegion (USBP, SystemMemory, 0x30B10200, 0x100)
  Field (USBP, WordAcc, NoLock, Preserve)
  {
    Offset(0x04),   // skip to register 04h
    CTL2, 32,       // USBNC_OTGn_CTRL2
    Offset(0x3C),   // skip to register 3Ch
    PHYS, 32,       // USBNC_OTGn_PHY_STATUS
  }

  OperationRegion (OTGM, SystemMemory, 0x30B10100, 0x100)
  Field (OTGM, WordAcc, NoLock, Preserve)
  {
    Offset(0xA4),   // skip to register A4h
    OTGS, 32,       // OTGSC
  }

  //
  // Device Specific Method takes 4 args:
  //  Arg0 : Buffer containing a UUID [16 bytes]
  //  Arg1 : Integer containing the Revision ID
  //  Arg2 : Integer containing the Function Index
  //  Arg3 : Package that contains function-specific arguments (Unused?)
  //
  Method (_DSM, 0x4, NotSerialized) {
    Name (RET, 0x0) // Declare return variable
    Name (REG, 0x0) // Declare reg variable

    // Check UUID
    switch(ToBuffer(Arg0)) {
      // URS UUID
      case (ToUUID("14EB0A6A-79ED-4B37-A8C7-84604B55C5C3")) {
        // Function index
        switch (Arg2) {
          // Function 0: Return supported functions, based on revision
          case (0) {
            switch(Arg1)
            {
              // Revision 0: function {1,2} supported
              case(0) {
                Return (0x03)
              }
              default {
                Return (0x0)
              }
            }
          }

          // Function 1: Initialize ID pin value and enable
          //             USB ID pin change interrupt
          //
          // Return value
          // 0 = UrsHardwareEventIdFloat (Function)
          // 1 = UrsHardwareEventIdGround (Host)
          case(1) {
            Store(CTL2, REG)            // Read USBNC_OTGn_CTRL2 register
            And(REG, 0xFFEFFFFF, REG)   // Set DIG_ID_SEL to 0
            Store(REG, CTL2)            // Write USBNC_OTGn_CTRL2 register

            Store(OTGS, REG)            // Read USB_nOTGSC register
            And(REG, 0x7F007F3B, REG)   // Preserved all bits except interrupts and reserved
            Or(REG, 0x01000000, REG)    // Enable USB ID interrupt (IDIE)
            Store(REG, OTGS)            // Write USB_nOTGSC register

            Store(PHYS, REG)            // Read USBNC_OTGn_PHY_STATUS status register
            And(REG, 0x00000010, REG)   // Check bit 4 (ID_DIG)
            Store(LEqual(REG, 0x0), RET)
            Return(RET)
          }

          // Function 2: Read ID pin value. This function could also
          //             clears the interrupt for USB ID pin
          //             state change if share interrupt is used
          //
          // Return value
          // 0 = UrsHardwareEventIdFloat (Function)
          // 1 = UrsHardwareEventIdGround (Host)
          case(2) {
            Store(OTGS, REG)                // Read USB_nOTGSC register
            And(REG, 0x7F007F3B, REG)       // Preserved all bits except interrupts and reserved
            Or(REG, 0x00010000, REG)        // Clear IDIS bit
            Store(REG, OTGS)                // Write USB_nOTGSC register

            Store(PHYS, REG)                // Read USBNC_OTGn_PHY_STATUS status register
            And(REG, 0x00000010, REG)       // Check bit 4 (ID_DIG)
            Store(LEqual(REG, 0x0), RET)
            Return(RET)
          }

          // Unknown function index
          default {
            Return(0x0)
          }
        } // Function index
      }
      // Unknown UUID
      default {
        Return(0x0)
      }
    } // Check UUID
  } // _DSM

  Device (USB1) {
    // The host controller device node needs to have an address of '0'
    Name(_ADR, 0x0)
    Name(_UID, 0x1)
    Name (_S0W, 0x0) // D0 is the lowest supported state to wake itself up
    Name (_S1W, 0x0) // D0 is the lowest supported state to wake itself up
    Name (_S2W, 0x0) // D0 is the lowest supported state to wake itself up
    Name (_S3W, 0x0) // D0 is the lowest supported state to wake itself up
    Name (_S4W, 0x0) // D0 is the lowest supported state to wake itself up
    Method (_STA) {
      Return(0xf)
    }
    Name (_CRS, ResourceTemplate () {
      Interrupt(ResourceConsumer, Level, ActiveHigh, SharedAndWake) { 75 }
    })

    OperationRegion (OTGM, SystemMemory, 0x30B10100, 0x100)
    Field (OTGM, WordAcc, NoLock, Preserve)
    {
      Offset(0x84),   // skip to register 84h
      PTSC, 32,       // port status control
      Offset(0xA8),   // skip to register A8h
      DSBM, 32,       // UOG_USBMOD
    }

    Name (REG, 0x0)    // Declare register read variable
    Method (_UBF, 0x0, NotSerialized) {
      // Reset handled by driver so no reset required here
      Store(0x03, DSBM)          // set host mode & little endian
      Store(PTSC, REG)           // read PORTSC status
      Store(OR(REG,0x2),PTSC)    // clear current PORTSC status
    }
  }

  Device(UFN1) {
    // The function controller device node needs to have an address of '1'
    Name(_ADR, 0x1)
    Name(_UID, 0x1)
    Name (_CRS, ResourceTemplate () {
      Interrupt (ResourceConsumer, Level, ActiveHigh, SharedAndWake) { 75 }
    })

    OperationRegion (OTGM, SystemMemory, 0x30B10100, 0x100)
    Field (OTGM, WordAcc, NoLock, Preserve)
    {
      Offset(0x84),   // skip to register 84h
      PTSC, 32,       // port status control
      Offset(0xA8),   // skip to register A8h
      DSBM, 32,       // UOG_USBMOD
    }

    Name (REG, 0x0)    // Declare register read variable
    Method (_UBF, 0x0, NotSerialized) {
      // Reset handled by driver so no reset required here
      Store(0x02, DSBM)          // set device mode & little endian
      Store(PTSC, REG)           // read PORTSC status
      Store(OR(REG,0x2),PTSC)    // clear current PORTSC status
    }

    // Device Specific Method takes 4 args:
    //  Arg0 : Buffer containing a UUID [16 bytes]
    //  Arg1 : Integer containing the Revision ID
    //  Arg2 : Integer containing the Function Index
    //  Arg3 : Package that contains function-specific arguments
    Method (_DSM, 0x4, NotSerialized) {
      switch(ToBuffer(Arg0)) {
        // UFX Chipidea interface identifier
        case(ToUUID("732b85d5-b7a7-4a1b-9ba0-4bbd00ffd511")) {
          // Function selector
          switch(Arg2) {
            // Function 0: Query support
            //   Bit  Description
            //   ---  -------------------------------
            //     0  Get property
            //     1  Get properties (Function 1)
            //     2  Set USB device state
            case(0) {
              switch(Arg1) {
                // Revision 0: functions {0,1} supported
                case(0) {
                  Return(Buffer(){0x03})
                }
                default {
                  Return(Buffer(){0x01})
                }
              }
            }

            // Function 1: Return device capabilities bitmap
            //   Bit  Description
            //   ---  -------------------------------
            //     0  Attach detach
            //     1  Software charging
            case(1) {
              Return(0x01)
            }

            // Function 2: Get port type
            //     0x00  Unknown port
            //     0x01  Standard downstream
            //     0x02  Charging downstream
            //     0x03  Dedicated charging
            case(2) {
              Return(0x01)
            }

            // Function 3: Set device state
            case(3) {
              Return (Buffer(){0x0})
            }

            // Unknown function
            default {
              Return (Buffer(){0x0})
            }
          }
        }

        // UFX interface identifier
        case(ToUUID("FE56CFEB-49D5-4378-A8A2-2978DBE54AD2")) {
          // Function selector
          switch(Arg2) {
            // Function 1: Return number of supported USB PHYSICAL endpoints
            // Based on chapter 65.1.1 - Up to 8 bidirectional endpoints
            case(1) {
              Return(8)
            }
            default {
              Return (Buffer(){0x0})
            }
          }
        }

        // Unknown UUID
        default {
          Return(0x0)
        }
      } // UUID
    } // _DSM
  }
}

Device(OTG2)
{
  Name(_HID, "PNP0C90")
  Name(_UID, 0x2)
  // URS requires device to at least be wake-able from D2 state
  // WDF also requires that _DSW (enable & disable wake ability) to be present
  Name(_S0W, 0x3)
  Name(_PRW, Package() {0,0})
  Method(_DSW, 0x3, NotSerialized) {
  }

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    // Controller register address space. URS driver would add 0x0100
    // offset for host mode
    MEMORY32FIXED (ReadWrite, 0x30B20000, 0x200, )

    // USB_OTG_ID pin, needs to be declared as *Wake as this device is
    // expected to be wakable. The USB PHY is capable to detect
    // USB ID changes but the interrupt cannot be acknowledge
    // and the behaviour is undefined based on NXP feedback. So
    // the the only way to reliably detect USB ID changed is to
    // either to share interrupts or assign a GPIO to detect.
    // NXP recommends the shared interrupts approach. Currently
    // there is no GPIO driver so use shared interrupt implementation.
    Interrupt (ResourceConsumer, Level, ActiveHigh, SharedAndWake) { 74 }
  })

  OperationRegion (USBP, SystemMemory, 0x30B20200, 0x100)
  Field (USBP, WordAcc, NoLock, Preserve)
  {
    Offset(0x04),   // skip to register 04h
    CTL2, 32,       // USBNC_OTGn_CTRL2
    Offset(0x3C),   // skip to register 3Ch
    PHYS, 32,       // USBNC_OTGn_PHY_STATUS
  }

  OperationRegion (OTGM, SystemMemory, 0x30B20100, 0x100)
  Field (OTGM, WordAcc, NoLock, Preserve)
  {
    Offset(0xA4),   // skip to register A4h
    OTGS, 32,       // OTGSC
  }

  // Device Specific Method takes 4 args:
  //  Arg0 : Buffer containing a UUID [16 bytes]
  //  Arg1 : Integer containing the Revision ID
  //  Arg2 : Integer containing the Function Index
  //  Arg3 : Package that contains function-specific arguments (Unused?)
  Method (_DSM, 0x4, NotSerialized) {
    Name (RET, 0x0) // Declare return variable
    Name (REG, 0x0) // Declare reg variable

    // Check UUID
    switch(ToBuffer(Arg0)) {
      // URS UUID
      case(ToUUID("14EB0A6A-79ED-4B37-A8C7-84604B55C5C3")) {
        // Function index
        switch(Arg2) {
          // Function 0: Return supported functions, based on revision
          case(0) {
            switch(Arg1) {
              // Revision 0: function {1,2} supported
              case(0) {
                Return(0x03)
              }
              default {
                Return(0x0)
              }
            }
          }

          // Function 1: Initialize ID pin value and enable
          //             USB ID pin change interrupt
          //
          // Return value
          // 0 = UrsHardwareEventIdFloat (Function)
          // 1 = UrsHardwareEventIdGround (Host)
          case(1) {
            Store(CTL2, REG)                // Read USBNC_OTGn_CTRL2 register
            And(REG, 0xFFEFFFFF, REG)       // Set DIG_ID_SEL to 0
            Store(REG, CTL2)                // Write USBNC_OTGn_CTRL2 register

            Store(OTGS, REG)                // Read USB_nOTGSC register
            And(REG, 0x7F007F3B, REG)       // Preserved all bits except interrupts and reserved
            Or(REG, 0x01000000, REG)        // Enable USB ID interrupt (IDIE)
            Store(REG, OTGS)                // Write USB_nOTGSC register

            Store(PHYS, REG)                // Read USBNC_OTGn_PHY_STATUS status register
            And(REG, 0x00000010, REG)       // Check bit 4 (ID_DIG)
            Store(LEqual(REG, 0x0), RET)
            Return(RET)
          }

          // Function 2: Read ID pin value. This function could also
          //             clears the interrupt for USB ID pin
          //             state change
          //
          // Return value
          // 0 = UrsHardwareEventIdFloat (Function)
          // 1 = UrsHardwareEventIdGround (Host)
          case(2) {
            Store(OTGS, REG)                // Read USB_nOTGSC register
            And(REG, 0x7F007F3B, REG)       // Preserved all bits except interrupts and reserved
            Or(REG, 0x00010000, REG)        // Clear IDIS bit
            Store(REG, OTGS)                // Write USB_nOTGSC register

            Store(PHYS, REG)                // Read USBNC_OTGn_PHY_STATUS status register
            And(REG, 0x00000010, REG)       // Check bit 4 (ID_DIG)
            Store(LEqual(REG, 0x0), RET)
            Return(RET)
          }

          // Unknown function index
          default {
            Return(0x0)
          }
        } // Function index
      }

      // Unknown UUID
      default {
        Return(0x0)
      }
    } // Check UUID
  } // _DSM

  Device (USB2) {
    // The host controller device node needs to have an address of '0'
    Name(_ADR, 0x0)
    Name(_UID, 0x2)
    Name (_S0W, 0x0) // D0 is the lowest supported state to wake itself up
    Name (_S1W, 0x0) // D0 is the lowest supported state to wake itself up
    Name (_S2W, 0x0) // D0 is the lowest supported state to wake itself up
    Name (_S3W, 0x0) // D0 is the lowest supported state to wake itself up
    Name (_S4W, 0x0) // D0 is the lowest supported state to wake itself up
    Method (_STA) {
      Return (0xf)
    }
    Name (_CRS, ResourceTemplate () {
      Interrupt(ResourceConsumer, Level, ActiveHigh, SharedAndWake) { 74 }
    })

    OperationRegion (OTGM, SystemMemory, 0x30B20100, 0x100)
    Field (OTGM, WordAcc, NoLock, Preserve)
    {
      Offset(0x84),   // skip to register 84h
      PTSC, 32,       // port status control
      Offset(0xA8),   // skip to register A8h
      DSBM, 32,       // UOG_USBMOD
    }

    Name (REG, 0x0)    // Declare register read variable
    Method (_UBF, 0x0, NotSerialized) {
      // Reset handled by driver so no reset required here
      Store(0x03, DSBM)          // set host mode & little endian
      Store(PTSC, REG)           // read PORTSC status
      Store(OR(REG,0x2),PTSC)    // clear current PORTSC status
    }
  }

  Device(UFN2) {
    // The function controller device node needs to have an address of '1'
    Name(_ADR, 0x1)
    Name(_UID, 0x2)

    Name (_CRS, ResourceTemplate () {
      Interrupt(ResourceConsumer, Level, ActiveHigh, SharedAndWake) { 74 }
    })

    OperationRegion (OTGM, SystemMemory, 0x30B20100, 0x100)
    Field (OTGM, WordAcc, NoLock, Preserve)
    {
      Offset(0x84),   // skip to register 84h
      PTSC, 32,       // port status control
      Offset(0xA8),   // skip to register A8h
      DSBM, 32,       // UOG_USBMOD
    }

    Name (REG, 0x0)    // Declare register read variable
    Method (_UBF, 0x0, NotSerialized) {
      // Reset handled by driver so no reset required here
      Store(0x02, DSBM)          // set device mode & little endian
      Store(PTSC, REG)           // read PORTSC status
      Store(OR(REG,0x2),PTSC)    // clear current PORTSC status
    }

    // Device Specific Method takes 4 args:
    //  Arg0 : Buffer containing a UUID [16 bytes]
    //  Arg1 : Integer containing the Revision ID
    //  Arg2 : Integer containing the Function Index
    //  Arg3 : Package that contains function-specific arguments
    Method (_DSM, 0x4, NotSerialized) {
      switch(ToBuffer(Arg0)) {
        // UFX Chipidea interface identifier
        case(ToUUID("732b85d5-b7a7-4a1b-9ba0-4bbd00ffd511")) {
          // Function selector
          switch(Arg2) {
            // Function 0: Query support
            //   Bit  Description
            //   ---  -------------------------------
            //     0  Get property
            //     1  Get properties (Function 1)
            //     2  Set USB device state
            case(0) {
              switch(Arg1) {
                // Revision 0: functions {0,1} supported
                case(0) {
                  Return(Buffer(){0x03})
                }
                default {
                  Return(Buffer(){0x01})
                }
              }
            }

            // Function 1: Return device capabilities bitmap
            //   Bit  Description
            //   ---  -------------------------------
            //     0  Attach detach
            //     1  Software charging
            case(1) {
              Return(0x01)
            }

            // Function 2: Get port type
            //     0x00  Unknown port
            //     0x01  Standard downstream
            //     0x02  Charging downstream
            //     0x03  Dedicated charging
            case(2) {
              Return(0x01)
            }

            // Function 3: Set device state
            case(3) {
              Return (Buffer(){0x0})
            }

            // Unknown function
            default {
              Return (Buffer(){0x0})
            }
          }
        }

        // UFX interface identifier
        case(ToUUID("FE56CFEB-49D5-4378-A8A2-2978DBE54AD2")) {
          // Function selector
          switch(Arg2) {
            // Function 1: Return number of supported USB PHYSICAL endpoints
            // Based on chapter 65.1.1 - Up to 8 bidirectional endpoints
            case(1) {
              Return(8)
            }
            default {
              Return (Buffer(){0x0})
            }
          }
        }

        // Unknown UUID
        default {
          Return(0x0)
        }
      } // UUID
    } // _DSM
  }
}

Device (USB3)
{
  Name (_HID, "NXP010C")
  Name (_CID, "PNP0D20")
  Name (_UID, 0x3)
  Name (_S0W, 0x3)
  Method (_STA) {
    Return(0xf)
  }
  Name (_CRS, ResourceTemplate () {
      MEMORY32FIXED(ReadWrite, 0x30B30100, 0x100, )
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 72 }
  })

  OperationRegion (OTGM, SystemMemory, 0x30B30100, 0x100)
  Field (OTGM, WordAcc, NoLock, Preserve)
  {
    Offset(0x84),   // skip to register 84h
    PTSC, 32,       // port status control
    Offset(0xA8),   // skip to register A8h
    DSBM, 32,       // UOG_USBMOD
  }

  Name (REG, 0x0)    // Declare register read variable
  Method (_UBF, 0x0, NotSerialized)
  {
    // Reset handled by driver so no reset required here
    Store(0x03, DSBM)          // set host mode & little endian
    Store(PTSC, REG)           // read PORTSC status
    Store(OR(REG,0x2),PTSC)    // clear current PORTSC status
  }
}
