/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 * 
 * MediaTek Inc. (C) 2010. All rights reserved.
 * 
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#ifndef __EMI_H__
#define __EMI_H__

/*EMI PSRAM (NOR) and DRAM control registers*/
#define CAL_EN                   (1 << 8)
#define EMI_GENA                 ((P_U32)(EMI_BASE+0x0070))
#define EMI_REMAP                 EMI_GENA
#define EMI_DRCT                 ((P_U32)(EMI_BASE+0x0078))
#define EMI_DDRV                 ((P_U32)(EMI_BASE+0x0080))
#define EMI_GEND                 ((P_U32)(EMI_BASE+0x0088))
#define EMI_PPCT                 ((P_U32)(EMI_BASE+0x0090)) /* EMI Performance and Power Control Register */

#define EMI_DLLV                 ((P_U32)(EMI_BASE+0x00A0))

#define EMI_DFTC                 ((P_U32)(EMI_BASE+0x00F0))
#define EMI_DFTD                 ((P_U32)(EMI_BASE+0x00F8))

/* EMI bandwith filter and MPU control registers */
#define EMI_ABCT	             ((P_U32)(EMI_BASE+0x0158))

/* EMI Memory Protect Unit */
#define EMI_MPUA                 ((P_U32)(EMI_BASE+0x0160))
#define EMI_MPUB                 ((P_U32)(EMI_BASE+0x0168))
#define EMI_MPUC                 ((P_U32)(EMI_BASE+0x0170))
#define EMI_MPUD                 ((P_U32)(EMI_BASE+0x0178))
#define EMI_MPUE                ((P_U32)(EMI_BASE+0x0180))
#define EMI_MPUF	        ((P_U32)(EMI_BASE+0x0188))
#define EMI_MPUG	        ((P_U32)(EMI_BASE+0x0190))
#define EMI_MPUH	        ((P_U32)(EMI_BASE+0x0198))

#define EMI_MPUI	        ((P_U32)(EMI_BASE+0x01A0))
#define EMI_MPUJ	        ((P_U32)(EMI_BASE+0x01A8))
#define EMI_MPUK            ((P_U32)(EMI_BASE+0x01B0))
#define EMI_MPUL            ((P_U32)(EMI_BASE+0x01B8))
#define EMI_MPUM            ((P_U32)(EMI_BASE+0x01C0))
#define EMI_MPUN            ((P_U32)(EMI_BASE+0x01C8))
#define EMI_MPUO            ((P_U32)(EMI_BASE+0x01D0))
#define EMI_MPUP            ((P_U32)(EMI_BASE+0x01D8))
#define EMI_MPUQ            ((P_U32)(EMI_BASE+0x01E0))
#define EMI_MPUR            ((P_U32)(EMI_BASE+0x01E8))

/* EMI IO delay, driving and MISC control registers */
#define EMI_IDLA            ((P_U32)(EMI_BASE+0x0200))
#define EMI_IDLB            ((P_U32)(EMI_BASE+0x0208))
#define EMI_IDLC            ((P_U32)(EMI_BASE+0x0210))
#define EMI_IDLD            ((P_U32)(EMI_BASE+0x0218))
#define EMI_IDLE            ((P_U32)(EMI_BASE+0x0220))
#define EMI_IDLF            ((P_U32)(EMI_BASE+0x0228))
#define EMI_IDLG            ((P_U32)(EMI_BASE+0x0230))
#define EMI_IDLH            ((P_U32)(EMI_BASE+0x0238))
#define EMI_IDLI            ((P_U32)(EMI_BASE+0x0240)) // IO input delay (DQS0 ~ DQS4)
#define EMI_IDLJ            ((P_U32)(EMI_BASE+0x0248))
#define EMI_IDLK            ((P_U32)(EMI_BASE+0x0250))

#define EMI_ODLA           ((P_U32)(EMI_BASE+0x0258))
#define EMI_ODLB           ((P_U32)(EMI_BASE+0x0260))
#define EMI_ODLC           ((P_U32)(EMI_BASE+0x0268))
#define EMI_ODLD           ((P_U32)(EMI_BASE+0x0270))
#define EMI_ODLE           ((P_U32)(EMI_BASE+0x0278))
#define EMI_ODLF           ((P_U32)(EMI_BASE+0x0280))
#define EMI_ODLG           ((P_U32)(EMI_BASE+0x0288))

#define EMI_DUTA           ((P_U32)(EMI_BASE+0x0290))
#define EMI_DUTB           ((P_U32)(EMI_BASE+0x0298))
#define EMI_DUTC           ((P_U32)(EMI_BASE+0x02A0))

#define EMI_DRVA           ((P_U32)(EMI_BASE+0x02A8))
#define EMI_DRVB           ((P_U32)(EMI_BASE+0x02B0))

#define EMI_IODC           ((P_U32)(EMI_BASE+0x02C8))

#define EMI_ODTA           ((P_U32)(EMI_BASE+0x02D0))
#define EMI_ODTB           ((P_U32)(EMI_BASE+0x02D8))

/* EMI auto-tracking control registers */
#define EMI_DQSA           ((P_U32)(EMI_BASE+0x0300))
#define EMI_DQSB           ((P_U32)(EMI_BASE+0x0308))
#define EMI_DQSC           ((P_U32)(EMI_BASE+0x0310))
#define EMI_DQSD           ((P_U32)(EMI_BASE+0x0318))


#define EMI_DQSE           ((P_U32)(EMI_BASE+0x0320))
#define EMI_DQSV           ((P_U32)(EMI_BASE+0x0328))

#define EMI_CALA           ((P_U32)(EMI_BASE+0x0330))
#define EMI_CALB           ((P_U32)(EMI_BASE+0x0338))
#define EMI_CALC           ((P_U32)(EMI_BASE+0x0340))
#define EMI_CALD           ((P_U32)(EMI_BASE+0x0348))


#define EMI_CALE           ((P_U32)(EMI_BASE+0x0350)) //DDR data auto tracking control
#define EMI_CALF           ((P_U32)(EMI_BASE+0x0358))
#define EMI_CALG           ((P_U32)(EMI_BASE+0x0360)) //DDR data auto tracking control
#define EMI_CALH           ((P_U32)(EMI_BASE+0x0368))

#define EMI_CALI           ((P_U32)(EMI_BASE+0x0370))
#define EMI_CALJ           ((P_U32)(EMI_BASE+0x0378))
#define EMI_CALK           ((P_U32)(EMI_BASE+0x0380))
#define EMI_CALL           ((P_U32)(EMI_BASE+0x0388))


#define EMI_CALM           ((P_U32)(EMI_BASE+0x0390))
#define EMI_CALN           ((P_U32)(EMI_BASE+0x0398))

#define EMI_CALO           ((P_U32)(EMI_BASE+0x03A0))
#define EMI_CALP           ((P_U32)(EMI_BASE+0x03A8))

#define EMI_DUCA           ((P_U32)(EMI_BASE+0x03B0))
#define EMI_DUCB           ((P_U32)(EMI_BASE+0x03B8))
#define EMI_DUCC           ((P_U32)(EMI_BASE+0x03C0))
#define EMI_DUCD           ((P_U32)(EMI_BASE+0x03C8))
#define EMI_DUCE           ((P_U32)(EMI_BASE+0x03D0))

/* EMI MBIST control registers*/
#define EMI_MBISTA            ((P_U32)(EMI_BASE+0x0600))
#define EMI_MBISTB            ((P_U32)(EMI_BASE+0x0608))
#define EMI_MBISTC            ((P_U32)(EMI_BASE+0x0610))
#define EMI_MBISTD            ((P_U32)(EMI_BASE+0x0618))
#define EMI_MBISTE            ((P_U32)(EMI_BASE+0x0620)) /* EMI MBIST status register */


/* EMI Flow control register A */
#define EMI_RFCA            ((P_U32)(EMI_BASE+0x0630))
#define EMI_RFCB            ((P_U32)(EMI_BASE+0x0638))
#define EMI_RFCC            ((P_U32)(EMI_BASE+0x0640))
#define EMI_RFCD            ((P_U32)(EMI_BASE+0x0648))

#endif // __EMI_H__
