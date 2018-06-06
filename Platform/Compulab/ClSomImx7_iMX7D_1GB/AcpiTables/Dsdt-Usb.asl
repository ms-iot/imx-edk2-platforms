/** @file
*
*  Compulab iMX7 USB OTG and EHCI Controllers
*
*  CL-SOM-iMX7 USB port 1 (i.MX7 Controller Core 0) supports for OTG signaling,
*  session request protocol (SRP), host negotiation protocol (HNP), and attach
*  detection protocol (ADP). ADP support includes dedicated timer hardware and
*  register interface.
*  CL-SOM-iMX7 USB port 2 (i.MX7 Controller Core 1) is an OTG capable port but
*  is configured to operate in host only mode onboard CL-SOM-iMX7.
*  CL-SOM-iMX7 USB port 3 (i.MX7 Controller Core 2) is an HSIC capable port
*  onboard CL-SOM-iMX7.
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

Device (USB1)
{
  Name (_HID, "NXP010C")
  Name (_CID, "PNP0D20")
  Name (_UID, 0x1)
  // Hardware revision 1 supports integrated Transaction Translator (TT) in PortSC register
  Name (_HRV, 0x1)
  // D0 is the lowest supported state to wake itself up
  Name (_S0W, 0x0)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30B10100, 0x100, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, SharedAndWake) { 75 }
  })

  OperationRegion (OTGM, SystemMemory, 0x30B10100, 0x100)
  Field (OTGM, WordAcc, NoLock, Preserve) {
    Offset (0x84),  // skip to register 84h
    PTSC, 32,       // port status control
    Offset (0xA8),  // skip to register A8h
    DSBM, 32,       // UOG_USBMOD
  }

  Name (REG, 0x0)
  Method (_UBF, 0x0, NotSerialized) {
    // Reset handled by driver so no reset required here
    Store (0x03, DSBM)          // set host mode & little endian
    Store (PTSC, REG)           // read PORTSC status
    Store (OR (REG, 0x2), PTSC) // clear current PORTSC status
  }
}

Device (USB2)
{
  Name (_HID, "NXP010C")
  Name (_CID, "PNP0D20")
  Name (_UID, 0x2)
  // Hardware revision 1 supports integrated Transaction Translator (TT) in PortSC register
  Name (_HRV, 0x1)
  // D0 is the lowest supported state to wake itself up
  Name (_S0W, 0x0)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30B20100, 0x100, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, SharedAndWake) { 74 }
  })

  OperationRegion (OTGM, SystemMemory, 0x30B20100, 0x100)
  Field (OTGM, WordAcc, NoLock, Preserve) {
    Offset (0x84),  // skip to register 84h
    PTSC, 32,       // port status control
    Offset (0xA8),  // skip to register A8h
    DSBM, 32,       // UOG_USBMOD
  }

  Name (REG, 0x0)
  Method (_UBF, 0x0, NotSerialized) {
    // Reset handled by driver so no reset required here
    Store (0x03, DSBM)          // set host mode & little endian
    Store (PTSC, REG)           // read PORTSC status
    Store (OR (REG, 0x2), PTSC) // clear current PORTSC status
  }
}
