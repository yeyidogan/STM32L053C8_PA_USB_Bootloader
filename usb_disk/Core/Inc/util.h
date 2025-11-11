/******************************************************************************
*	Written by Yilmaz Eyidogan
*	public structures
*	Created date: 2015.01.04
*******************************************************************************/
#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

/* define ------------------------------------------------------------*/
/* typedef -----------------------------------------------------------*/
/* macro -------------------------------------------------------------*/
/* variables ---------------------------------------------------------*/
/* typedef -----------------------------------------------------------*/
/* function ----------------------------------------------------------*/
extern uint16_t wordEndianer(uint16_t wordIn);
extern uint8_t copy_string(uint8_t * ptrTarget, uint8_t * ptrSource, uint8_t ucMaxByte);
extern int32_t divisor32(int32_t val, int32_t divisor);
extern int32_t round32(int32_t val, int32_t rounder);
extern void int32_to_string_buf(uint8_t * ptr_buf, int32_t num, uint8_t point, uint8_t size, uint8_t type);

#endif

/* * * END OF FILE * * */
