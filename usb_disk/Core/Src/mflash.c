/******************************************************************************
 *	Written by Yilmaz Eyidogan
 *	flash functions
 *	Created date: 2025.11.13
 *******************************************************************************/
#include "mflash.h"
#include "util.h"
#include "stm32l0xx.h"
#include <stdint.h>

/* define ------------------------------------------------------------*/
/* macro -------------------------------------------------------------*/
/* typedef -----------------------------------------------------------*/
/* variables ---------------------------------------------------------*/

volatile uint8_t flash_status = FLASH_DATA_WAITING;
uint32_t flash_app_prog_addr = FLASH_APPLICATION_ADDRESS;
uint32_t flash_csum = 0x00;
uint32_t flash_csum_of_bin_file;
struct struct_half_page st_half_page = { 0x00 };

/* functions ---------------------------------------------------------*/

/**
 *******************************************************************************
 * @brief flash eeprom read function
 * @param[in]
 * @param[out]
 *******************************************************************************
 */
uint32_t read_flash_eeprom(uint32_t addr, uint8_t data_type)
{
  uint32_t data;

  FLASH_WaitForLastOperation (FLASH_TIMEOUT_VALUE);
  switch (data_type) {
    case DATA_TYPE_UINT8:
      //read word (8-bit) at a specified address.
      data = (uint8_t) (*(__IO uint8_t*) addr);
      break;
    case DATA_TYPE_UINT16:
      //read word (16-bit) at a specified address.
      data = (uint16_t) (*(__IO uint16_t*) addr);
      break;
    default:
      data = (uint32_t) (*(__IO uint32_t*) addr);
      break;
  }
  FLASH_WaitForLastOperation (FLASH_TIMEOUT_VALUE);
  return data;
}

/**
 *******************************************************************************
 * @brief sequentional flash write function
 * @param[in]
 * @param[out]
 *******************************************************************************
 */
uint8_t flash_write_sequential(uint32_t addr, uint32_t *ptr_data, uint16_t length)
{
  if (length == 0)
  {
    return false;
  }

  HAL_FLASH_Unlock ();
  while (length >= sizeof(uint32_t))
  {
    HAL_FLASH_Program (0, addr, *ptr_data);

    length -= sizeof(uint32_t);
    addr += sizeof(uint32_t);
    ++ptr_data;
    HAL_Delay (7);
  }

  HAL_FLASH_Lock ();
  return true;
}

/**
 *******************************************************************************
 * @brief sequentional flash read function
 * @param[in]
 * @param[out]
 *******************************************************************************
 */
uint8_t flash_read_sequential(uint32_t addr, uint32_t *ptr_data, uint16_t length)
{
  if (length == 0)
  {
    return false;
  }

  while (length >= sizeof(uint32_t))
  {
    *ptr_data = *(__IO uint32_t*) addr;
    FLASH_WaitForLastOperation (FLASH_TIMEOUT_VALUE);
    length -= sizeof(uint32_t);
    addr += sizeof(uint32_t);
    ++ptr_data;
  }
  return true;
}

/* * * END OF FILE * * */
