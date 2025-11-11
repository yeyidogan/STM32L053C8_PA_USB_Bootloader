/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : usbd_storage_if.c
 * @version        : v2.0_Cube
 * @brief          : Memory management layer.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_storage_if.h"

/* USER CODE BEGIN INCLUDE */
#include "fat12.h"
#include "util.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
 * @brief Usb device.
 * @{
 */

/** @defgroup USBD_STORAGE
 * @brief Usb mass storage device module
 * @{
 */

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
 * @brief Private types.
 * @{
 */

/* USER CODE BEGIN PRIVATE_TYPES */
#define FAT_SECTOR_CNT 6
#define FAT_SECTOR_SIZE 512
enum
{
  BUF_ID_MBR = 0x00,
  BUF_ID_FAT1,
  BUF_ID_FAT2,
  BUF_ID_ROOT,
  BUF_ID_DATA1,
  BUF_ID_DATA2,
  BUF_ID_UNDEF,
  BUF_ID_MAX,
};

uint8_t fat_buffer[BUF_ID_MAX][FAT_SECTOR_SIZE] =
  { 0x00 };
/* USER CODE END PRIVATE_TYPES */

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Private_Defines
 * @brief Private defines.
 * @{
 */

#define STORAGE_LUN_NBR                  1
#define STORAGE_BLK_NBR                  0x10000
#define STORAGE_BLK_SIZ                  0x200

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Private_Macros
 * @brief Private macros.
 * @{
 */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Private_Variables
 * @brief Private variables.
 * @{
 */

/* USER CODE BEGIN INQUIRY_DATA_FS */
/** USB Mass storage Standard Inquiry Data. */
const int8_t STORAGE_Inquirydata_FS[] =
  {/* 36 */

  /* LUN 0 */
  0x00, 0x80, 0x02, 0x02, (STANDARD_INQUIRY_DATA_LEN - 5), 0x00, 0x00, 0x00,
      'U', 'P', 'D', 'A', 'T', 'E', 'R', ' ', /* Manufacturer : 8 bytes */
      'M', 'a', 's', 's', ' ', 'S', 't', 'o', /* Product      : 16 Bytes */
      'r', 'a', 'g', 'e', ' ', ' ', ' ', ' ', '0', '.', '0', '1', /* Version      : 4 Bytes */
  };
/* USER CODE END INQUIRY_DATA_FS */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Exported_Variables
 * @brief Public variables.
 * @{
 */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
 * @brief Private functions declaration.
 * @{
 */

static int8_t
STORAGE_Init_FS (uint8_t lun);
static int8_t
STORAGE_GetCapacity_FS (uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t
STORAGE_IsReady_FS (uint8_t lun);
static int8_t
STORAGE_IsWriteProtected_FS (uint8_t lun);
static int8_t
STORAGE_Read_FS (uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t
STORAGE_Write_FS (uint8_t lun, uint8_t *buf, uint32_t blk_addr,
		  uint16_t blk_len);
static int8_t
STORAGE_GetMaxLun_FS (void);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
void
init_boot_sector (void)
{
  if (fat_buffer[BUF_ID_MBR][0x15] == MEDIA_DESCRIPTOR)
    {
      return;
    }
  memset (fat_buffer[BUF_ID_MBR], 0x00, STORAGE_BLK_SIZ);
  memset (fat_buffer[BUF_ID_FAT1], 0x00, STORAGE_BLK_SIZ);
  memset (fat_buffer[BUF_ID_FAT2], 0x00, STORAGE_BLK_SIZ);
  *(uint32_t*) fat_buffer[BUF_ID_FAT1] = 0x00FF0FFF;
  *(uint32_t*) fat_buffer[BUF_ID_FAT2] = 0x00FF0FFF;
  memset (fat_buffer[BUF_ID_ROOT], 0x00, STORAGE_BLK_SIZ);
  memset (fat_buffer[BUF_ID_DATA1], 0x00, STORAGE_BLK_SIZ);
  memset (fat_buffer[BUF_ID_DATA2], 0x00, STORAGE_BLK_SIZ);
  memset (fat_buffer[BUF_ID_UNDEF], 0x00, STORAGE_BLK_SIZ);

  fat_buffer[BUF_ID_MBR][0x01FE] = 0x55;
  fat_buffer[BUF_ID_MBR][0x01FF] = 0xAA;

  memcpy ((char*) fat_buffer[BUF_ID_MBR], (char*) &fat12_mbr,
	  sizeof(fat12_mbr));
  strncpy ((char*) fat_buffer[BUF_ID_ROOT], (char*) &fat12_dir_entry, sizeof(struct struct_fat12_dir_entry));
  strcpy ((char*) &fat_buffer[BUF_ID_ROOT][32], "\xE5          \x10");
  strncpy ((char*) &fat_buffer[BUF_ID_ROOT][44], (char*) &fat12_dir_entry.ntres, 20);
}
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
 * @}
 */

USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
  { STORAGE_Init_FS, STORAGE_GetCapacity_FS, STORAGE_IsReady_FS,
      STORAGE_IsWriteProtected_FS, STORAGE_Read_FS, STORAGE_Write_FS,
      STORAGE_GetMaxLun_FS, (int8_t*) STORAGE_Inquirydata_FS };

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Initializes over USB FS IP
 * @param  lun:
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t
STORAGE_Init_FS (uint8_t lun)
{
  /* USER CODE BEGIN 2 */
  init_boot_sector();
  return (USBD_OK);
  /* USER CODE END 2 */
}

/**
 * @brief  .
 * @param  lun: .
 * @param  block_num: .
 * @param  block_size: .
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t
STORAGE_GetCapacity_FS (uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 3 */
  *block_num = STORAGE_BLK_NBR;
  *block_size = STORAGE_BLK_SIZ;
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
 * @brief  .
 * @param  lun: .
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t
STORAGE_IsReady_FS (uint8_t lun)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
 * @brief  .
 * @param  lun: .
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t
STORAGE_IsWriteProtected_FS (uint8_t lun)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
 * @brief  .
 * @param  lun: .
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t
STORAGE_Read_FS (uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 6 */
  switch (blk_addr)
    {
    case VOLUME_START:
      memcpy (buf, fat_buffer[BUF_ID_MBR], STORAGE_BLK_SIZ);
      break;
    case FAT_REGION:
      memcpy (buf, fat_buffer[BUF_ID_FAT1], STORAGE_BLK_SIZ);
      break;
    case FAT_REGION + 1:
      memcpy (buf, fat_buffer[BUF_ID_FAT2], STORAGE_BLK_SIZ);
      break;
    case ROOT_DIRECTORY_REGION:
      memcpy (buf, fat_buffer[BUF_ID_ROOT], STORAGE_BLK_SIZ);
      break;
    case DATA_REGION:
      memcpy (buf, fat_buffer[BUF_ID_DATA1], STORAGE_BLK_SIZ);
      break;
    case DATA_REGION + 1:
      memcpy (buf, fat_buffer[BUF_ID_DATA2], STORAGE_BLK_SIZ);
      break;
    default:
      memcpy (buf, fat_buffer[BUF_ID_UNDEF], STORAGE_BLK_SIZ);
      break;
    }
  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
 * @brief  .
 * @param  lun: .
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t
STORAGE_Write_FS (uint8_t lun, uint8_t *buf, uint32_t blk_addr,
		  uint16_t blk_len)
{
  /* USER CODE BEGIN 7 */
  switch (blk_addr)
    {
    case VOLUME_START:
//    init_boot_sector();
      memcpy (fat_buffer[BUF_ID_MBR], buf, STORAGE_BLK_SIZ);
      break;
    case FAT_REGION:
      memcpy (fat_buffer[BUF_ID_FAT1], buf, STORAGE_BLK_SIZ);
      break;
    case FAT_REGION + 1:
      memcpy (fat_buffer[BUF_ID_FAT2], buf, STORAGE_BLK_SIZ);
      break;
    case ROOT_DIRECTORY_REGION:
      memcpy (fat_buffer[BUF_ID_ROOT], buf, STORAGE_BLK_SIZ);
      break;
    case DATA_REGION:
      memcpy (fat_buffer[BUF_ID_DATA1], buf, STORAGE_BLK_SIZ);
      break;
    case DATA_REGION + 1:
      memcpy (fat_buffer[BUF_ID_DATA2], buf, STORAGE_BLK_SIZ);
      break;
    default:
      memcpy (fat_buffer[BUF_ID_UNDEF], buf, STORAGE_BLK_SIZ);
      break;
    }
  return (USBD_OK);
  /* USER CODE END 7 */
}

/**
 * @brief  .
 * @param  None
 * @retval .
 */
int8_t
STORAGE_GetMaxLun_FS (void)
{
  /* USER CODE BEGIN 8 */
  return (STORAGE_LUN_NBR - 1);
  /* USER CODE END 8 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
 * @}
 */

/**
 * @}
 */

