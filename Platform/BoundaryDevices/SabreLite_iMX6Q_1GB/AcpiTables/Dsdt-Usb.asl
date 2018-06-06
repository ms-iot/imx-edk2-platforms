/*
* Description: iMX6 Quad EHCI USB Controllers
*
*  iMX6 Quad EHCI USB Controllers
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
*/

Device(URS0)
{
   Name(_HID, "PNP0C90")
   Name(_UID, 0x0)

   // URS requires device to at least be wake-able from D2 state
   // WDF also requires that _DSW (enable & disable wake ability) to be present
   Name(_S0W, 0x3)
   Name(_PRW, Package() {0,0})
   Method(_DSW, 0x3, NotSerialized) {
   }

   Method (_STA)
   {
       Return(0xf)
   }

   Method (_CRS, 0x0, NotSerialized) {
       Name (RBUF, ResourceTemplate () {
           // Controller register address space. URS driver would add 0x0100
           // offset for host mode
           MEMORY32FIXED(ReadWrite, 0x02184000, 0x200, )

           // USB_OTG_ID pin, needs to be declared as *Wake as this device is
           // expected to be wakable. The USB PHY is capable to detect
           // USB ID changes but the interrupt cannot be acknowledge
           // and the behaviour is undefined based on NXP feedback. So
           // the the only way to reliably detect USB ID changed is to
           // either to share interrupts or assign a GPIO to detect.
           // The URS driver does not properly handle level sensitive
           // interrupts which can lead to an interrupt storm. Therefore we use
           // an edge sensitive GPIO interrupt.
           //
           // USB_OTG_ID connected to GPIO_1 (GPIO1_IO01). Use 1ms debounce.
           GpioInt (Edge, ActiveBoth, SharedAndWake, PullDefault, 100, "\\_SB.GPIO",) { 1 }
       })
       Return(RBUF)
   }

   Name (OTGR, ResourceTemplate()
   {
       GpioIO (Shared, PullDefault, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer,,) { 1 }
   })
  Field (\_SB_.GPIO.OTGP, ByteAcc, NoLock, Preserve) {
    Connection (OTGR),
    OTGF, 1
  }


    // Device Specific Method takes 4 args:
    //  Arg0 : Buffer containing a UUID [16 bytes]
    //  Arg1 : Integer containing the Revision ID
    //  Arg2 : Integer containing the Function Index
    //  Arg3 : Package that contains function-specific arguments (Unused?)
    Method (_DSM, 0x4, NotSerialized) {
      Name (RET, 0x0) // Declare return variable
      Name (PVAL, 0x0) // Declare pin value variable

      // Check UUID
      if (LEqual(Arg0,ToUUID("14EB0A6A-79ED-4B37-A8C7-84604B55C5C3"))) {
        // Function index
        switch (ToInteger(Arg2)) {
          // Function 0: Return supported functions, based on revision
          // Return value and revision ID lack documentation
          case(0) {
            switch (ToInteger(Arg1)) {
              // Revision 0: function {1,2} supported
              case(0) {
                Return(0x03)
              }
              default {
                Return(0x0)
              }
            }
          }

          // Function 1: Read USB_OTG_ID pin value
          //
          // Return value
          // 0 = UrsHardwareEventIdFloat (Function)
          // 1 = UrsHardwareEventIdGround (Host)
          case(1) {
            Store(OTGF, PVAL)  // Read value of OTG_ID Pin
            Store(LEqual(PVAL, 0), RET)   // Complement value
            Return(RET)
          }

          // Function 2: Read USB_OTG_ID pin value
          //
          // Return value
          // 0 = UrsHardwareEventIdFloat (Function)
          // 1 = UrsHardwareEventIdGround (Host)
          case(2) {
            Store(OTGF, PVAL)  // Read value of OTG_ID Pin
            Store(LEqual(PVAL, 0), RET)   // Complement value
            Return(RET)
          }

          // Unknown function index
          default {
            Return(0x0)
          }
        } // Function index
      } // Check UUID
      Return (0);
    } // _DSM

   Device (USB0)
   {
       // The host controller device node needs to have an address of '0'
       Name(_ADR, 0x0)
       Name(_UID, 0x0)
       Name (_S0W, 0x0) // D0 is the lowest supported state to wake itself up
       Method (_STA)
       {
           Return(0xf)
       }
       Method (_CRS, 0x0, NotSerialized) {
           Name (RBUF, ResourceTemplate () {
               Interrupt(ResourceConsumer, Level, ActiveHigh, SharedAndWake) { 75 }
           })
           Return(RBUF)
       }

       OperationRegion (OTGM, SystemMemory, 0x02184100, 0x100)
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

   Device(UFN0)
   {
       // The function controller device node needs to have an address of '1'
       Name(_ADR, 0x1)

       Method (_CRS, 0x0, NotSerialized) {
           Name (RBUF, ResourceTemplate () {
               Interrupt(ResourceConsumer, Level, ActiveHigh, SharedAndWake) { 75 }
           })
           Return(RBUF)
       }

       OperationRegion (OTGM, SystemMemory, 0x02184100, 0x100)
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
        if (LEqual(Arg0,ToUUID("732b85d5-b7a7-4a1b-9ba0-4bbd00ffd511"))) {
          // Function selector
          switch (ToInteger(Arg2)) {
            // Function 0: Query support
            //   Bit  Description
            //   ---  -------------------------------
            //     0  Get property
            //     1  Get properties (Function 1)
            //     2  Set USB device state
            case(0) {
              switch (ToInteger(Arg1)) {
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

            //
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
        if (LEqual(Arg0,ToUUID("FE56CFEB-49D5-4378-A8A2-2978DBE54AD2"))) {
          // Function selector
          switch (ToInteger(Arg2)) {
            // Function 1: Return number of supported USB PHYSICAL endpoints
            // Up to 8 bidirectional endpoints
            case(1) {
              Return(8)
            }
            default {
              Return (Buffer(){0x0})
            }
          }
        } // UUID
        Return (0x0);
      } // _DSM
   }
}

Device (USB1)
{
    Name (_HID, "NXP010C")
    Name (_CID, "PNP0D20")
    Name (_UID, 0x1)
    Name (_S0W, 0x0)

    // USB Host controller registers
    OperationRegion (USBH, SystemMemory, 0x02184000, 0x1000)
    Field (USBH, DWordAcc, NoLock, Preserve)
    {
        Offset(0x00000344),
        USTS, 32,             // USB_UH1_USBSTS
        Offset(0x00000348),
        INTR, 32,             // USB_UH1_USBINTR
        Offset(0x00000384),   // skip to register 0x384
        PSC1, 32,             // USB_UH1_PORTSC1
        Offset(0x00000804),   // skip to register 0x804
        NCTL, 32,             // USBNC_USB_UH1_CTRL
    }

    // USBPHY2 Registers
    OperationRegion (PHY2, SystemMemory, 0x020CA000, 0x1000)
    Field (PHY2, DWordAcc, NoLock, Preserve)
    {
        Offset(0x0000000),      // skip to register 0
        PPWD, 32,               // USBPHY2_PWD
        Offset(0x00000030),     // skip to register 0x30
        PCTL, 32,               // USBPHY2_CTRL
        Offset(0x00000034),     // skip to register 0x34
        PCTS, 32,               // USBPHY2_CTRL_SET
        Offset(0x00000038),     // skip to register 0x38
        PCTC, 32,               // USBPHY2_CTRL_CLR
        Offset(0x00000050),     // skip to register 0x50
        PDBG, 32,               // USBPHY2_DEBUG
        Offset(0x00000054),     // skip to register 0x54
        PDBS, 32,               // USBPHY2_DEBUG_SET
        Offset(0x00000058),     // skip to register 0x58
        PDBC, 32,               // USBPHY2_DEBUG_CLR
    }

    // Anatop Registers
    OperationRegion (ANAT, SystemMemory, 0x020C8000, 0x1000)
    Field (ANAT, DWordAcc, NoLock, Preserve)
    {
        Offset(0x0000244),      // skip to register 0x244
        LPBS, 32,               // ANADIG_USB2_LOOPBACK_SET
        Offset(0x0000248),      // skip to register 0x248
        LPBC, 32,               // ANADIG_USB2_LOOPBACK_CLR
    }

    Method (_STA)
    {
        Return(0xf)
    }
    Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF, ResourceTemplate () {
            MEMORY32FIXED(ReadWrite, 0x02184300, 0x100, )
            Interrupt(ResourceConsumer, Level, ActiveHigh, ExclusiveAndWake) { 72 }
        })
        Return(RBUF)
    }

    Method (_UBF, 0x0, NotSerialized)
    {

    }
}
