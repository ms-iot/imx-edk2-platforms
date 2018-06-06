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

#ifndef _CPMEM_H_
#define _CPMEM_H_

// IDMAC channel definition
#define IDMAC_CHANNEL_CSI_0                                 0
#define IDMAC_CHANNEL_CSI_1                                 1
#define IDMAC_CHANNEL_CSI_2                                 2
#define IDMAC_CHANNEL_CSI_3                                 3
#define IDMAC_CHANNEL_VDIC_VF1_VF2                          5
#define IDMAC_CHANNEL_VDIC_PREVIOUS_FIELD                   8
#define IDMAC_CHANNEL_VDIC_CURRENT_FIELD                    9
#define IDMAC_CHANNEL_VDIC_NEXT_FIELD                       10
#define IDMAC_CHANNEL_IC_VIDEO_POST_PROCESSSING             11
#define IDMAC_CHANNEL_IC_VIDEO_PRP_TASK                     12

#define IDMAC_CHANNEL_DP_PRIMARY_FLOW_MAIN_PLANE            23

typedef enum {
  CPMEM_SO_PROGRESSIVE,
  CPMEM_SO_INTERLACE,
} CPMEM_SO;

typedef enum {
  CPMEM_BNDM_DISABLE,
  CPMEM_BNDM_BAND_HEIGHT_4,
  CPMEM_BNDM_BAND_HEIGHT_8,
  CPMEM_BNDM_BAND_HEIGHT_16,
  CPMEM_BNDM_BAND_HEIGHT_32,
  CPMEM_BNDM_BAND_HEIGHT_64,
  CPMEM_BNDM_BAND_HEIGHT_128,
  CPMEM_BNDM_BAND_HEIGHT_256,
} CPMEM_BNDM;

typedef enum {
  CPMEM_BM_DISABLE,
  CPMEM_BM_BW_BH_8,
  CPMEM_BM_BW_BH_16,
  CPMEM_BM_UNUSED,
} CPMEM_BM;

typedef enum {
  CPMEM_ROT_NO_ROTATION,
  CPMEM_ROT_90_ROTATION,
} CPMEM_ROT;

typedef enum {
  CPMEM_HF_NO_HFLIP,
  CPMEM_HF_HFLIP_ENABLE,
} CPMEM_HF;

typedef enum {
  CPMEM_HF_NO_VFLIP,
  CPMEM_HF_VFLIP_ENABLE,
} CPMEM_VF;

typedef enum {
  CPMEM_THE_DISABLE,
  CPMEM_THE_ENABLE,
} CPMEM_THE;

typedef enum {
  CPMEM_CAP_SKIP_LOW,
  CPMEM_CAP_SKIP_HIGH,
} CPMEM_CAP;

typedef enum {
  CPMEM_CAE_DISABLE,
  CPMEM_CAE_ENABLE,
} CPMEM_CAE;

typedef enum {
  CPMEM_EOLI_DISABLE,
  CPMEM_EOLI_ENABLE,
} CPMEM_EOLI;

typedef enum {
  CPMEM_PFS_NON_INT_444,
  CPMEM_PFS_NON_INT_422,
  CPMEM_PFS_NON_INT_420,
  CPMEM_PFS_PAR_INT_422,
  CPMEM_PFS_PAR_INT_420,
} CPMEM_PFS_PLANAR;

typedef enum {
  CPMEM_DEC_SEL_0_15,
  CPMEM_DEC_SEL_64_79,
  CPMEM_DEC_SEL_128_143,
  CPMEM_DEC_SEL_192_207,
} CPMEM_DEC_SEL;

typedef enum {
  CPMEM_PFS_CODE = 5,
  CPMEM_PFS_GENERIC_DATA,
  CPMEM_PFS_RGB,
  CPMEM_PFS_INT_422_Y1U1Y2V1,
  CPMEM_PFS_INT_422_Y2U1Y1V1,
  CPMEM_PFS_INT_422_U1Y1V1Y2,
  CPMEM_PFS_INT_422_U1Y2V1Y1,
} CPMEM_PFS_PACKED;

typedef enum {
  CPMEM_ALU_SAME_BUFFER,
  CPMEM_ALU_SEPERATE_BUFFER,
} CPMEM_ALU;

typedef enum {
  CPMEM_ID_ID_0,
  CPMEM_ID_ID_1,
  CPMEM_ID_ID_2,
  CPMEM_ID_ID_3,
} CPMEM_ID;

typedef enum {
  CPMEM_CRE_DISABLE,
  CPMEM_CRE_ENABLE,
} CPMEM_CRE;

#pragma pack(push, 1)

// CPMEM_WORD0_PLANAR - Non interlaved
typedef union {
  struct {
    UINT32 XVVirtualCoordinate : 10;
    UINT32 YVVirtualCoordinate : 9;
    UINT32 XBinnerBlockCoordinate : 13;
    UINT32 YBinnerBlockCoordinate : 12;
    UINT32 NewSubBlock : 1;
    UINT32 CurrentField : 1;
    UINT32 MemUBufferOffset : 22;
    UINT32 MemVBufferOffset : 22;
    UINT32 InitialOffsetX : 4;
    UINT32 ReduceDoubleReadOrWrites : 1;
    UINT32 Reserved1 : 18;
    UINT32 ScanOrder : 1;
    UINT32 BandMode : 3;
    UINT32 BlockMode : 2;
    UINT32 Rotation : 1;
    UINT32 HorizontalFlip : 1;
    UINT32 VerticalFlip : 1;
    UINT32 ThresholdEnable : 1;
    UINT32 ConditionalAccessPolarity : 1;
    UINT32 ConditionalAccessEnable : 1;
    UINT32 FrameWidth : 13; // Max 8192 Pixel
    UINT32 FrameHeight : 12; // Max 4096 Pixel
    UINT32 EndOfLineInterrupt : 1;
    UINT32 Reserved2 : 9;
  };
  UINT32 Word0[5];
} CPMEM_WORD0_PLANAR_REG;

// CPMEM_WORD1_PLANAR - Non interleaved
typedef union {
  struct {
    UINT32 ExtMemBuffer0Address : 29;
    UINT32 ExtMemBuffer1Address : 29;
    UINT32 InterlaceOffset : 20;
    UINT32 NumberOfPixelsInWholeBurstAccess : 7;
    UINT32 PixelFormatSelect : 4;
    UINT32 AlphaUsed : 1;
    UINT32 AlphaChannelMapping : 3;
    UINT32 AxiId : 2;
    UINT32 Threshold : 7;
    UINT32 StrideLine : 14;
    UINT32 Reserved1 : 9;
    UINT32 Width3 : 3;
    UINT32 StrideLineUV : 14;
    UINT32 Reserved2 : 7;
    UINT32 ConditionalReadEnable : 1;
    UINT32 Reserved3 : 10;
  };
  UINT32 Word1[5];
} CPMEM_WORD1_PLANAR_REG;

// CPMEM_WORD0_PACKED - Interlaved
typedef union {
  struct {
    UINT32 XVVirtualCoordinate : 10;
    UINT32 YVVirtualCoordinate : 9;
    UINT32 XBinnerBlockCoordinate : 13;
    UINT32 YBinnerBlockCoordinate : 12;
    UINT32 NewSubBlock : 1;
    UINT32 CurrentField : 1;
    UINT32 ScrollXCounter : 12;
    UINT32 ScrollYCounter : 11;
    UINT32 NumberOfScroll : 10;
    UINT32 ScrollDeltaX : 7;
    UINT32 ScrollMax : 10;
    UINT32 ScrollingConfiguration : 1;
    UINT32 ScrollingEnable : 1;
    UINT32 ScrollDeltaY : 7;
    UINT32 ScrollHorizontalDirection : 1;
    UINT32 ScrollVerticalDirection : 1;
    UINT32 BitsPerPixel : 3;
    UINT32 DecodeAddressSelect : 2;
    UINT32 AccessDimension : 1;
    UINT32 ScanOrder : 1;
    UINT32 BandMode : 3;
    UINT32 BlockMode : 2;
    UINT32 Rotation : 1;
    UINT32 HorizontalFlip : 1;
    UINT32 VerticalFlip : 1;
    UINT32 ThresholdEnable : 1;
    UINT32 ConditionalAccessPolarity : 1;
    UINT32 ConditionalAccessEnable : 1;
    UINT32 FrameWidth : 13; // Max 8192 Pixel
    UINT32 FrameHeight : 12; // Max 4096 Pixel
    UINT32 EndOfLineInterrupt : 1;
    UINT32 Reserved2 : 9;
  };
  UINT32 Word0[5];
} CPMEM_WORD0_PACKED_REG;

// CPMEM_WORD1_PACKED - Non interleaved
typedef union {
  struct {
    UINT32 ExtMemBuffer0Address : 29;
    UINT32 ExtMemBuffer1Address : 29;
    UINT32 InterlaceOffset : 20;
    UINT32 NumberOfPixelsInWholeBurstAccess : 7;
    UINT32 PixelFormatSelect : 4;
    UINT32 AlphaUsed : 1;
    UINT32 AlphaChannelMapping : 3;
    UINT32 AxiId : 2;
    UINT32 Threshold : 7;
    UINT32 StrideLine : 14;
    UINT32 Width0 : 3;
    UINT32 Width1 : 3;
    UINT32 Width2 : 3;
    UINT32 Width3 : 3;
    UINT32 Offset0 : 5;
    UINT32 Offset1 : 5;
    UINT32 Offset2 : 5;
    UINT32 Offset3 : 5;
    UINT32 SelectSXSYSet : 1;
    UINT32 ConditionalReadEnable : 1;
    UINT32 Reserved1 : 10;
  };
  UINT32 Word1[5];
} CPMEM_WORD1_PACKED_REG;

typedef struct _CPMEM_PARAM {
  union {
    CPMEM_WORD0_PLANAR_REG Word0Planar;
    CPMEM_WORD0_PACKED_REG Word0Pack;
  };
  UINT32 Reserved1[3];
  union {
    CPMEM_WORD1_PLANAR_REG Word1Planar;
    CPMEM_WORD1_PACKED_REG Word1Pack;
  };
  UINT32 Reserved2[3];
} CPMEM_PARAM, *PCPMEM_PARAM;

#pragma pack(pop)

EFI_STATUS
ConfigureCpmemFrameBuffer (
  IN  DISPLAY_INTERFACE_CONTEXT   *DisplayInterfaceContextPtr,
  IN  UINT32                      Channel,
  IN  SURFACE_INFO                *FrameBufferPtr
  );

#endif  /* _CPMEM_H_ */
