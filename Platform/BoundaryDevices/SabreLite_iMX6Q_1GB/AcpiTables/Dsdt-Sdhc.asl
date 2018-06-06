/*
* Description: iMX6 Sabre Ultra Secured Digital Host Controller (uSDHC)
*
*  Copyright (c) Microsoft Corporation. All rights reserved.
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


//
// uSDHC3: SDCard Slot
//
Device (SDH3)
{
   Name (_HID, "NXP0108")
   Name (_UID, 0x3)

   Method (_STA) // Status
   {
       Return(0xf) // Enabled
   }

   Name (_S1D, 0x1)
   Name (_S2D, 0x1)
   Name (_S3D, 0x1)
   Name (_S4D, 0x1)

   Method (_CRS, 0x0, NotSerialized) {
       Name (RBUF, ResourceTemplate () {
           MEMORY32FIXED(ReadWrite, 0x02198000, 0x4000, )
           Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 56 }
       })
       Return(RBUF)
   }

   Name (_DSD, Package()
   {
       ToUUID ("DAFFD814-6EBA-4D8C-8A91-BC9BBF4AA301"),
       Package ()
       {
           Package (2) { "BaseClockFrequencyHz", 198000000 },  // SDHC Base/Input Clock: 198MHz
           Package (2) { "Regulator1V8Exist", 0 },             // 1.8V Switching External Circuitry: Not-Implemented
           Package (2) { "SlotCount", 1 },                     // Number of SD/MMC slots connected on the bus: 1
           Package (2) { "RegisterBasePA", 0x02198000 }        // Register base physical address
       }
   })

   Device (SD0)
   {
      Method (_ADR) // Address
      {
         Return (0) // SD/MMC Slot0
      }

      //
      // Despite the SDCard is a removal device, UWF requires the
      // boot device to be non-removable. This is required for the
      // Windows SDCard boot scenario with UWF enabled.
      //
      Method (_RMV) // Remove
      {
         Return (0) // Fixed
      }
   }
}

//
// uSDHC4: uSDCard Slot
//
Device (SDH4)
{
   Name (_HID, "NXP0108")
   Name (_UID, 0x4)

   Method (_STA)
   {
       Return(0xf)
   }

   Name (_S1D, 0x1)
   Name (_S2D, 0x1)
   Name (_S3D, 0x1)
   Name (_S4D, 0x1)

   Method (_CRS, 0x0, NotSerialized) {
       Name (RBUF, ResourceTemplate () {
           MEMORY32FIXED(ReadWrite, 0x0219C000, 0x4000, )
           Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 57 }
       })
       Return(RBUF)
   }

   Name (_DSD, Package()
   {
       ToUUID ("DAFFD814-6EBA-4D8C-8A91-BC9BBF4AA301"),
       Package ()
       {
           Package (2) { "BaseClockFrequencyHz", 198000000 },  // SDHC Base/Input Clock: 198MHz
           Package (2) { "Regulator1V8Exist", 0 },             // 1.8V Switching External Circuitry: Not-Implemented
           Package (2) { "SlotCount", 1 },                     // Number of SD/MMC slots connected on the bus: 1
           Package (2) { "RegisterBasePA", 0x0219C000 }        // Register base physical address
       }
   })

   Device (SD0)
   {
      Method (_ADR) // Address
      {
         Return (0) // SD/MMC Slot 0
      }

      Method (_RMV) // Remove
      {
         Return (1) // Removable
      }
   }
}
