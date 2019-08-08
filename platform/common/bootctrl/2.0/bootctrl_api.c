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
 * MediaTek Inc. (C) 2016. All rights reserved.
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
#include "bootctrl.h"
#include "platform.h"
#include "avb_util.h"
#include "mmc_core.h"
#include "storage_api.h"
#include <partition_api.h>
#include <partition_active.h>

#define BOOTCTR_PARTITION "para"
#define MOD "bootctrl"
/******************************************************************************
 * DEBUG
 ******************************************************************************/
#define SMSG                        print


static const char* suffix[2] = {BOOTCTRL_SUFFIX_A, BOOTCTRL_SUFFIX_B};
static AvbABData metadata_saved;
static int metadata_read = 0;

int check_suffix_with_slot(const char *suffix)
{
    int slot = -1;

    if(suffix == NULL) {
        SMSG("input suffix is NULL\n");
        return -1;
    }

    if(strcmp(suffix, BOOTCTRL_SUFFIX_A) == 0) {
        slot = 0;
    }
    else if(strcmp(suffix, BOOTCTRL_SUFFIX_B) == 0) {
        slot = 1;
    }
    else {
        SMSG("unknow slot suffix\n");
    }

   return slot;
}

static int read_write_partition_info(AvbABData *bctrl ,int mode)
{
    U32 boot_ctrl_size;
    blkdev_t *bootdev   = NULL;
    part_t *part        = NULL;
    U64 src;
    u32 part_id = 0;


    int ret = -1, storage_type = 0;
    boot_ctrl_size = sizeof(AvbABData);

    storage_type = ab_get_storage_type();
    if(storage_type == -1) {
        print("unknown device type\n");
        return -1;
    }

    if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
        SMSG("[%s] can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
    }

    if(NULL == (part = part_get(BOOTCTR_PARTITION))) {
#if CFG_DRAM_CALIB_OPTIMIZATION
        SMSG("[%s] Try to query by sram\n", MOD);
        /* The fail might caused by dram not init yet, using sram query then */
        src = get_part_addr(BOOTCTR_PARTITION);
        if (src == 0)
            SMSG("[%s] get_part_addr fail\n", MOD);

        part_id = storage_get_part_id(STORAGE_PHYS_PART_USER);
#else
        SMSG("[%s] part_get fail\n", MOD);
#endif
    } else {
        src = part->start_sect * bootdev->blksz;
        part_id = part->part_id;
    }
    src += OFFSETOF_SLOT_SUFFIX;

    if(bctrl == NULL) {
        SMSG("read_write_partition_info failed, bctrl is NULL\n");
        return ret;
    }

    if(mode == READ_PARTITION) {
      if ((metadata_read)  && (!memcmp(metadata_saved.magic ,AVB_AB_MAGIC, AVB_AB_MAGIC_LEN)))  {
        memcpy(bctrl, &metadata_saved, sizeof(AvbABData));
      }
      else {
        if (-1 == blkdev_read(bootdev, src, boot_ctrl_size, (char *)bctrl, part_id)) {
            SMSG("[%s] part_load fail\n", MOD);
            return ret;
        }
        else {
          memcpy(&metadata_saved, bctrl, sizeof(AvbABData));
          metadata_read = 1;
        }
      }
    }
    else if(mode == WRITE_PARTITION) {
        bctrl->crc32 = avb_htobe32(
             avb_crc32((const uint8_t*)bctrl, sizeof(AvbABData) - sizeof(uint32_t)));

        if (-1 == blkdev_write(bootdev, src, boot_ctrl_size, (char *)bctrl, part_id)) {
            SMSG("[%s] part_load fail\n", MOD);
            return ret;
        }
        else {
            metadata_read = 0;  //force to read from partition after successful blkdev_write()
        }
    }
    else {
        SMSG(" unknown mode, ret: 0x%x\n", ret);
        return ret;
    }
    ret = 0;
    return ret;
}

static int ab_metadata_init(const char *suffix) {
    int slot = 0 ,slot1 = 0;
    int ret = -1 ,storage_type = -1;
    AvbABSlotData *slotp;

    AvbABData metadata;

    slot = check_suffix_with_slot(suffix);
    storage_type = ab_get_storage_type();
    if(storage_type == -1) {
        print("unknown device type\n");
        return -1;
    }
    if(slot == -1) {
        SMSG("ab_metadata_init failed, slot: 0x%x\n", slot);
        return -1;
    }

    if(suffix == NULL) {
        SMSG("input suffix is NULL\n");
        return -1;
    }

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        SMSG("partition_read failed, ret: 0x%x\n", ret);
        return -1;
    }

    memcpy(metadata.magic ,AVB_AB_MAGIC, AVB_AB_MAGIC_LEN);

    /* Set highest priority and reset retry count */
    slotp = &metadata.slots[slot];
    slotp->successful_boot = 0;
    slotp->priority = AVB_AB_MAX_PRIORITY;
    slotp->tries_remaining = AVB_AB_MAX_TRIES_REMAINING;

    /* Re-set arg to another slot */
    slot1 = (slot == 0) ? 1 : 0;
    slotp = &metadata.slots[slot1];
    slotp->successful_boot = 0;
    slotp->priority = AVB_AB_MAX_PRIORITY - 1;
    slotp->tries_remaining = AVB_AB_MAX_TRIES_REMAINING;

    ret = read_write_partition_info(&metadata, WRITE_PARTITION);
    if (ret < 0) {
        SMSG("partition_write failed, ret: 0x%x\n", ret);
        return -1;
    }

    return 0;
}

const char *get_suffix(void)
{
    int slot = 0, ret = -1;

    AvbABData metadata;

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if (ret < 0) {
        SMSG("get_suffix failed, ret: 0x%x\n", ret);
        return NULL;
    }

    if(memcmp(metadata.magic , AVB_AB_MAGIC, AVB_AB_MAGIC_LEN) != 0) {
        SMSG("booctrl magic not match init default value\n");
        slot = 0;
        ab_metadata_init(BOOTCTRL_SUFFIX_A);
    }
    else {

        if(metadata.slots[0].priority >= metadata.slots[1].priority)
           slot = 0;
        else if (metadata.slots[0].priority < metadata.slots[1].priority)
           slot = 1;
    }

    return suffix[slot];
}


int rollback_slot(const char *suffix) {
    int slot = 0 ,slot1 = 0;
    int ret = -1;
    AvbABSlotData *slotp;

    AvbABData metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        SMSG("rollback_slot failed, slot: 0x%x\n", slot);
        return -1;
    }

    if(suffix == NULL) {
        SMSG("input suffix is NULL\n");
        return -1;
    }

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        SMSG("rollback_slot failed, ret: 0x%x\n", ret);
        return -1;
    }

    memcpy(metadata.magic ,AVB_AB_MAGIC, AVB_AB_MAGIC_LEN);

    /* Set highest priority and reset retry count */
    slotp = &metadata.slots[slot];
    slotp->priority = AVB_AB_MAX_PRIORITY;

    /* Ensure other slot doesn't have as high a priority. */
    slot1 = (slot == 0) ? 1 : 0;
    slotp = &metadata.slots[slot1];
    if(slotp->priority == AVB_AB_MAX_PRIORITY)
        slotp->priority = AVB_AB_MAX_PRIORITY - 1;

    ret = read_write_partition_info(&metadata, WRITE_PARTITION);
    if (ret < 0) {
        SMSG("partition_write failed, ret: 0x%x\n", ret);
        return -1;
    }

    return 0;
}

uint8_t get_retry_count(const char *suffix)
{
    int slot = 0;
    int ret = -1;
    AvbABSlotData *slotp;
    AvbABData metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        SMSG("get_tries_remaining failed, slot: 0x%x\n", slot);
        return -1;
    }

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if (ret < 0) {
        SMSG("partition_read failed, ret: 0x%x\n", ret);
        return -1;
    }

    slotp = &metadata.slots[slot];
    return slotp->tries_remaining;
}

int reduce_retry_count(const char *suffix)
{
    int slot = 0, ret = -1;
    AvbABSlotData *slotp;
    AvbABData metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        SMSG("set_not_boot_mode failed, slot: 0x%x\n", slot);
        return -1;
    }

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        SMSG("partition_read failed, ret: 0x%x\n", ret);
        return -1;
    }

    slotp = &metadata.slots[slot];
    if(slotp->tries_remaining > 0)
        slotp->tries_remaining--;

    ret = read_write_partition_info(&metadata, WRITE_PARTITION);
    if(ret < 0) {
        SMSG("partition_write failed, ret: 0x%x\n", ret);
        return -1;
    }

    return 0;
}

int check_valid_slot(void)
{
    int slot = 0, ret = -1;
    AvbABData metadata;

    ret = read_write_partition_info(&metadata, READ_PARTITION);

    if (ret < 0) {
        SMSG("check_valid_slot read_partition_info failed, ret: 0x%x\n", ret);
        return -1;
    }

    if(metadata.slots[0].priority > 0)
           return 0;
    else if (metadata.slots[1].priority > 0)
           return 0;

    return -1;
}

int mark_slot_invalid(const char *suffix)
{
    int slot = 0, slot2 = 0, ret = -1;
    AvbABSlotData *slotp;
    AvbABData metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        SMSG("set_not_boot_mode failed, slot: 0x%x\n", slot);
        return -1;
    }

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        SMSG("partition_read failed, ret: 0x%x\n", ret);
        return -1;
    }

    slotp = &metadata.slots[slot];
    slotp->successful_boot = 0;
    slotp->priority = 0;

    ret = read_write_partition_info(&metadata, WRITE_PARTITION);
    if(ret < 0) {
        SMSG("partition_write failed, ret: 0x%x\n", ret);
        return -1;
    }

    return 0;
}

int get_bootup_status(const char *suffix)
{
    int slot = 0, ret = -1;
    AvbABSlotData *slotp;
    AvbABData metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        SMSG("set_not_boot_mode failed, slot: 0x%x\n", slot);
        return -1;
    }

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        SMSG("partition_read failed, ret: 0x%x\n", ret);
        return -1;
    }

    slotp = &metadata.slots[slot];
    return slotp->successful_boot;

    return 0;
}

int ab_get_storage_type(void)
{
    blkdev_t *bootdev = NULL;
    bootdev = blkdev_get(CFG_BOOT_DEV);

    if (bootdev == NULL) {
        print("%s can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
        return -1;
    }

    if (bootdev->type == BOOTDEV_SDMMC) {
        print("%s device is EMMC\n");
        return BOOTDEV_SDMMC;
    } else if (bootdev->type == BOOTDEV_UFS) {
        print("%s device is UFS\n");
        return BOOTDEV_UFS;
    }

    print("unknown device type\n");
    return -1;
}

int ab_get_boot_part(u32 *bootpart)
{
    int ret = 0, storage_type = 0;
    u32 boot_part = 0;

    storage_type = ab_get_storage_type();
    if(storage_type == -1) {
        print("unknown device type\n");
        return -1;
    }

    if (storage_type == BOOTDEV_SDMMC) {
        ret = mmc_get_boot_part(&boot_part);

        if(boot_part == EMMC_PART_BOOT1)
            *bootpart = BOOT_PART_A;
        else if(boot_part == EMMC_PART_BOOT2)
            *bootpart = BOOT_PART_B;
    } else if (storage_type == BOOTDEV_UFS) {
        extern struct ufs_hba g_ufs_hba;
        struct ufs_hba *hba = &g_ufs_hba;

        ret = ufs_aio_get_boot_lu(hba, &boot_part);
        if(boot_part == ATTR_B_BOOT_LUN_EN_BOOT_LU_A)
            *bootpart = BOOT_PART_A;
        else if(boot_part == ATTR_B_BOOT_LUN_EN_BOOT_LU_B)
            *bootpart = BOOT_PART_B;
    }
    return ret;
}

int ab_set_boot_part(u32 bootpart)
{
    int storage_type = 0;

    storage_type = ab_get_storage_type();
    if(storage_type == -1) {
        print("unknown device type\n");
        return -1;
    }

    if (storage_type == BOOTDEV_SDMMC) {
        return mmc_set_boot_part(bootpart);
    } else if (storage_type == BOOTDEV_UFS) {
        extern struct ufs_hba g_ufs_hba;
        struct ufs_hba *hba = &g_ufs_hba;

        return ufs_aio_set_boot_lu(hba, bootpart);
    }
}
