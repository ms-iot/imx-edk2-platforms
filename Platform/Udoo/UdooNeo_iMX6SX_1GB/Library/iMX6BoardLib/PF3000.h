#ifndef __PF3000_H__
#define __PF3000_H__

//// 
//  Microsoft Coporation
//  SiArch
//
//  Portions Copyright (c) Microsoft Corporation
//  Portions Copyright (c) Intel Corporation
//  Portions Copyright (c) Apple
//  Portions Copyright (c) ARM Ltd.
//  Portions Copyright (c) Freescale
////

// section 6.6.6.1 PMIC Register Map
#define PF3000_DEVICEID                             0x00 
#define PF3000_SILICONREVID                         0x03 
#define PF3000_FABID                                0x04 
#define PF3000_INTSTAT0                             0x05 
#define PF3000_INTMASK0                             0x06 
#define PF3000_INTSENSE0                            0x07 
#define PF3000_INTSTAT1                             0x08 
#define PF3000_INTMASK1                             0x09 
#define PF3000_INTSENSE1                            0x0A 
#define PF3000_INTSTAT3                             0x0E 
#define PF3000_INTMASK3                             0x0F 
#define PF3000_INTSENSE3                            0x10 
#define PF3000_INTSTAT4                             0x11 
#define PF3000_INTMASK4                             0x12 
#define PF3000_INTSENSE4                            0x13 
#define PF3000_COINCTL                              0x1A 
#define PF3000_PWRCTL                               0x1B 
#define PF3000_MEMA                                 0x1C 
#define PF3000_MEMB                                 0x1D 
#define PF3000_MEMC                                 0x1E 
#define PF3000_MEMD                                 0x1F 
#define PF3000_SW1AVOLT                             0x20 
#define PF3000_SW1ASTBY                             0x21 
#define PF3000_SW1AOFF                              0x22 
#define PF3000_SW1AMODE                             0x23 
#define PF3000_SW1ACONF                             0x24 
#define PF3000_SW1BVOLT                             0x2E 
#define PF3000_SW1BSTBY                             0x2F 
#define PF3000_SW1BOFF                              0x30 
#define PF3000_SW1BMODE                             0x31 
#define PF3000_SW1BCONF                             0x32 
#define PF3000_SW2VOLT                              0x35 
#define PF3000_SW2STBY                              0x36 
#define PF3000_SW2OFF                               0x37 
#define PF3000_SW2MODE                              0x38 
#define PF3000_SW2CONF                              0x39 
#define PF3000_SW3VOLT                              0x3C 
#define PF3000_SW3STBY                              0x3D 
#define PF3000_SW3OFF                               0x3E 
#define PF3000_SW3MODE                              0x3F 
#define PF3000_SW3CONF                              0x40 
#define PF3000_SWBSTCTL                             0x66 
#define PF3000_LDOGCTL                              0x69 
#define PF3000_VREFDDRCTL                           0x6A 
#define PF3000_VSNVSCTL                             0x6B 
#define PF3000_VLDO1CTL                             0x6C 
#define PF3000_VLDO2CTL                             0x6D 
#define PF3000_VCC_SDCTL                            0x6E 
#define PF3000_V33CTL                               0x6F 
#define PF3000_VLDO3CTL                             0x70 
#define PF3000_VLDO4CTL                             0x71 
#define PF3000_PAGE_SEL                             0x7F 

#define PF3000_EXTPAGE1_OTP_FUSEREAD_EN             0x80 
#define PF3000_EXTPAGE1_OTP_LOADMASK                0x84 
#define PF3000_EXTPAGE1_OTP_ECC_SE1                 0x8A 
#define PF3000_EXTPAGE1_OTP_ECC_DE1                 0x8C 
#define PF3000_EXTPAGE1_OTP_SW1AVOLT                0xA0 
#define PF3000_EXTPAGE1_OTP_SW1ASEQ                 0xA1 
#define PF3000_EXTPAGE1_OTP_SW1xCONFIG              0xA2 
#define PF3000_EXTPAGE1_OTP_SW1BVOLT                0xA8 
#define PF3000_EXTPAGE1_OTP_SW1BSEQ                 0xA9 
#define PF3000_EXTPAGE1_OTP_SW2VOLT                 0xAC 
#define PF3000_EXTPAGE1_OTP_SW2_SEQ                 0xAD 
#define PF3000_EXTPAGE1_OTP_SW2CONFIG               0xAE 
#define PF3000_EXTPAGE1_OTP_SW3VOLT                 0xB0 
#define PF3000_EXTPAGE1_OTP_SW3_SEQ                 0xB1 
#define PF3000_EXTPAGE1_OTP_SW3CONFIG               0xB2 
#define PF3000_EXTPAGE1_OTP_SWBSTVOLT               0xBC 
#define PF3000_EXTPAGE1_OTP_SWBSTSEQ                0xBD 
#define PF3000_EXTPAGE1_OTP_VSNVSVOLT               0xC0 
#define PF3000_EXTPAGE1_OTP_VLDO1VOLT               0xC8 
#define PF3000_EXTPAGE1_OTP_VLDO1SEQ                0xC9 
#define PF3000_EXTPAGE1_OTP_VLDO2VOLT               0xCC 
#define PF3000_EXTPAGE1_OTP_VLDO2SEQ                0xCD 
#define PF3000_EXTPAGE1_OTP_VCC_SDVOLT              0xD0 
#define PF3000_EXTPAGE1_OTP_VCC_SDSEQ               0xD1 
#define PF3000_EXTPAGE1_OTP_V33VOLT                 0xD4 
#define PF3000_EXTPAGE1_OTP_V33_SEQ                 0xD5 
#define PF3000_EXTPAGE1_OTP_VLDO3VOLT               0xD8 
#define PF3000_EXTPAGE1_OTP_VLDO3SEQ                0xD9 
#define PF3000_EXTPAGE1_OTP_VLDO4VOLT               0xDC 
#define PF3000_EXTPAGE1_OTP_VLDO4SEQ                0xDD 
#define PF3000_EXTPAGE1_OTP_PUCONFIG1               0xE0 
#define PF3000_EXTPAGE1_OTP_FUSEPOR1                0xE4 
#define PF3000_EXTPAGE1_OTP_PWRGDEN                 0xE8 
#define PF3000_EXTPAGE1_OTP_BLOWN                   0xF7 
#define PF3000_EXTPAGE1_OTP_I2CADDR                 0xFF 

#define PF3000_EXTPAGE2_SW1APWRSTG                  0x81 
#define PF3000_EXTPAGE2_SW1BPWRSTG                  0x83 
#define PF3000_EXTPAGE2_SW2_PWRSTG                  0x84 
#define PF3000_EXTPAGE2_SW3_PWRSTG                  0x85 
#define PF3000_EXTPAGE2_PWRCTRLOTP_CTRL             0x88 
#define PF3000_EXTPAGE2_I2C_WRITE_ADDRESS_TRAP      0x8D 
#define PF3000_EXTPAGE2_I2C_TRAP_PAGE               0x8E 
#define PF3000_EXTPAGE2_I2C_TRAP_CNTR               0x8F 
#define PF3000_EXTPAGE2_IO_DRV                      0x90 
#define PF3000_EXTPAGE2_OTP_AUTO_ECC0               0xD0 
#define PF3000_EXTPAGE2_OTP_ECC_CTRL1               0xE1 
#define PF3000_EXTPAGE2_OTP_ECC_CTRL2               0xE2 
#define PF3000_EXTPAGE2_OTP_ECC_CTRL3               0xE3 
#define PF3000_EXTPAGE2_OTP_ECC_CTRL4               0xE4 
#define PF3000_EXTPAGE2_OTP_ECC_CTRL5               0xE5 
#define PF3000_EXTPAGE2_OTP_FUSE_CTRL1              0xF1 
#define PF3000_EXTPAGE2_OTP_FUSE_CTRL2              0xF2 
#define PF3000_EXTPAGE2_OTP_FUSE_CTRL3              0xF3 
#define PF3000_EXTPAGE2_OTP_FUSE_CTRL4              0xF4 
#define PF3000_EXTPAGE2_OTP_FUSE_CTRL5              0xF5 

#endif  // __PF3000_H__
