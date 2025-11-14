/******************************************************************************
 *	Written by Yilmaz Eyidogan
 *	flash or internal e2 functions
 *	Created date: 2017.04.06
 *******************************************************************************/
#ifndef _MFLASH_H_
#define _MFLASH_H_

#include <stdint.h>
#include <stdbool.h>

/* define -------------------------------------------------------------*/

/* define of user applications ----------------------------------------*/
#define FLASH_APPLICATION_ADDRESS 0x08007000
#define FLASH_TOTAL_SIZE 0x08010000
#define FLASH_LAST_PAGE_ADDRESS 0x0800FF80
#define FLASH_LAST_FLASH_ADDRESS 0x0800FFFF
#define FLASH_APP_CODE_ADDRESS 0x0800FFF8
#define FLASH_APP_CODE 0xAA5500FF
//#define FLASH_PAGE_SIZE 128
#define FLASH_HALF_PAGE_DATA_CNT 16

enum
{
  FLASH_READ_PROTECTION_LEVEL_0 = (uint8_t) 0x00,
  FLASH_READ_PROTECTION_LEVEL_1,
  FLASH_READ_PROTECTION_LEVEL_2
};

enum
{
  DATA_TYPE_UINT8 = (uint8_t) 0x00,
  DATA_TYPE_UINT16
};

enum
{
  FLASH_DATA_WAITING = (uint8_t) 0x00,
  FLASH_STARTUP_CODE_DETECTED,
  FLASH_PROGRAMMED
};

struct struct_half_page
{
  uint32_t val[16];
  uint16_t id;
};
/* macro --------------------------------------------------------------*/
#define START_FLASH_TIMER flash_tmr=(uint16_t)0x00
#define CHECK_FLASH_TIMER_REACH_TO(x) flash_tmr>(uint16_t)x

/* typedef ------------------------------------------------------------*/
/* variables ----------------------------------------------------------*/
extern uint16_t flash_tmr;
extern volatile uint8_t flash_status;
extern uint32_t flash_app_prog_addr;
extern uint32_t flash_csum;
extern uint32_t flash_csum_of_bin_file;
extern struct struct_half_page st_half_page;

/* functions ----------------------------------------------------------*/
extern uint8_t flash_e2_write_sequential(uint32_t addr, uint16_t *ptr_data, uint8_t length);
extern uint8_t flash_e2_read_sequential(uint32_t addr, uint16_t *ptr_data, uint8_t length);
extern uint8_t flash_ob_get_rdp_level(void);
extern void lock_flash(void);
extern uint8_t unlock_flash(void);
extern void lock_option_byte(void);
extern uint8_t unlock_option_byte(void);
extern void write_option_byte(uint32_t index, uint32_t val);
extern void erase_flash_page(uint32_t page_addr);
extern void write_flash_program(uint32_t addr, uint32_t data);
extern uint32_t flash_decode_uint32(uint8_t *ptr);

#endif
/* * * END OF FILE * * */
