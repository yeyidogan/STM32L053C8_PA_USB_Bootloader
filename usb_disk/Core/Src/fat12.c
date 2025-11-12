/******************************************************************************
 * @file     fat12.c
 * @version  v1.00
 * @brief    fat12 file system
 * @author   Yilmaz Eyidogan
 *****************************************************************************/

#include "fat12.h"

const struct struct_fat12_mbr fat12_mbr =
  {
    { 0xEB, 0xFE, 0x90 }, //code to jump, BS_jmpBoot
	{ 'M', 'S', 'D', 'O', 'S', '5', '.', '0' }, //os name
      BYTES_PER_SECTOR, //bytes per sector, BPB_BytsPerSec
      0x01, //sectors per cluster, BPB_SecPerClus
      RESERVED_SECTORS, //reserved sectors, BPB_RsvdSecCnt
      NUMBER_OF_FAT_COPY, //number of fat copies, BPB_NumFATs
      NUMBER_OF_ROOT_ENTRIES, //number of possible root entries, BPB_RootEntCnt
      0x0100, //small number of sectors, BPB_TotSec16
      MEDIA_DESCRIPTOR, //media descriptor
      0x0001, //sectors per fat, BPB_FATSz16
      0x00FF, //sectors per track, BPB_SecPerTrk
      0x0001, //number of heads, BPB_NumHeads
      0x00000000, //hidden sectors
      0x00000000, //nlarge of sectors
      0x80, //drive number
      0x00, //reserved
      0x29, //extended boot signature
      0x01020304, //serial number
	{ 'U', 'P', 'D', 'A', 'T', 'E', 'I', 'S', 'T', 'E', 'L' },
	{ 'F', 'A', 'T', ' ', ' ', ' ', ' ', ' ' } };

const struct struct_fat12_dir_entry fat12_dir_entry =
  {
    { 'U', 'P', 'D', 'A', 'T', 'E', 'I', 'S', 'T', 'E', 'L' },
  ATTR_VOLUME_ID, 0x00, //ntres
      0x00, //crt_time_tenth
	{ 0x00, 0x00, 0x00 }, //crt_time
	{ 0x01, 0x01, 0x26 }, //crt_date
	{ 0x01, 0x01, 0x26 }, //last_acc_date
      0x00, //frst_clus_hi
	{ 0x00, 0x00, 0x00 }, //wrt_time
	{ 0x01, 0x01, 0x26 }, //wrt_date
      0x00, //frst_clus_lo
      0x00, //file_size
    };

/*
 void fat12_fat_entry(uint8_t * ptr_fat, uint16_t N){
 uint16_t FATOffset, ThisFATSecNum, ThisFATEntOffset;

 FATOffset = N + (N / 2);
 ThisFATSecNum = fat12_mbr.reserved_sectors + (FATOffset / fat12_mbr.nbyte_of_sector);
 ThisFATEntOffset = FATOffset % fat12_mbr.nbyte_of_sector;
 }
 */

