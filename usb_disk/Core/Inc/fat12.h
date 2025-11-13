/******************************************************************************
 * @file     fat12.h
 * @version  v1.00
 * @brief    fat12 file system header file
 * @author   Yilmaz Eyidogan
 *****************************************************************************/

#ifndef _FAT12_
#define _FAT12_

#include <stdint.h>

#define BOOT_SECTOR_SIGN 0xAA55

#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define ATTR_LONG_NAME 0x0F

struct struct_fat12_mbr
{
  uint8_t code[3];
  uint8_t os_name[8];
  uint16_t nbyte_of_sector;
  uint8_t nsector_of_cluster;
  uint16_t reserved_sectors;
  uint8_t nfat_copy;
  uint16_t nroot_entries;
  uint16_t nsmall_of_sector;
  uint8_t med_desc;
  uint16_t nsector_of_fat;
  uint16_t nsector_of_track;
  uint16_t nhead;
  uint32_t nsector_of_hidden;
  uint32_t nlarge_of_sector;
  uint8_t drive_number;
  uint8_t reserved2;
  uint8_t ext_boot_sign;
  uint32_t serial_num;
  uint8_t label[11];
  uint8_t file_system_type[8];
} __attribute__((packed));

struct struct_date
{
  uint16_t day :5;
  uint16_t month :4;
  uint16_t year :7;
} __attribute__((packed));

struct struct_time
{
  uint16_t second :5;
  uint16_t minutes :6;
  uint16_t hour :5;
} __attribute__((packed));

struct struct_fat12_dir_entry
{
  uint8_t name[11];
  uint8_t attribute;
  uint8_t ntres;
  uint8_t crt_time_tenth;
  struct struct_time crt_time;
  struct struct_date crt_date;
  struct struct_date last_acc_date;
  uint16_t frst_clus_hi;
  struct struct_time wrt_time;
  struct struct_date wrt_date;
  uint16_t frst_clus_lo;
  uint32_t file_size;
} __attribute__((packed));

extern const struct struct_fat12_mbr fat12_mbr;
extern const struct struct_fat12_dir_entry fat12_dir_entry;

#define VOLUME_START 0x00
#define BYTES_PER_SECTOR 0x0200
#define RESERVED_SECTORS 0x0001
#define NUMBER_OF_ROOT_ENTRIES 0x0200
#define NUMBER_OF_FAT_COPY 0x02
#define SECTOR_PER_FAT 0x0001
#define MEDIA_DESCRIPTOR 0xF0
#define FAT_REGION (VOLUME_START + RESERVED_SECTORS)
#define ROOT_DIRECTORY_REGION (FAT_REGION + (NUMBER_OF_FAT_COPY * SECTOR_PER_FAT))
#define START_OF_DATA_REGION (ROOT_DIRECTORY_REGION + (NUMBER_OF_ROOT_ENTRIES * 32 / BYTES_PER_SECTOR))

#endif

/* * * END OF FILE * * */
