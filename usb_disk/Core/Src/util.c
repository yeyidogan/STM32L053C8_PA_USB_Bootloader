/******************************************************************************
 *	Written by Yilmaz Eyidogan
 *	util functions
 *	Created date: 2016.02.22
 *******************************************************************************/
#include "util.h"

/**
 *******************************************************************************
 * @brief       endian changer
 * @param[in]   None
 * @retval      None
 * @par Description little endian to big endian
 *******************************************************************************
 */
uint16_t
wordEndianer (uint16_t wordIn)
{
  uint16_t tmp;

  tmp = wordIn;
  tmp <<= 8;
  wordIn >>= 8;
  tmp += wordIn;
  return tmp;
}

/**
 *******************************************************************************
 * @brief       string copier
 * @param[in]   ptr_source, ptr_destination
 * @retval      length of string
 * @par Description: copy string to any buffer
 *******************************************************************************
 */
uint8_t
copy_string (uint8_t *ptrTarget, uint8_t *ptrSource, uint8_t ucMaxByte)
{
  uint8_t ucCount = 0x00;
  while (ucMaxByte--)
    {
      if (*ptrSource == 0x00)
	{
	  *ptrTarget = 0x00;
	  break;
	}
      *ptrTarget = *ptrSource;
      ++ptrTarget;
      ++ptrSource;
      ++ucCount;
    }
  return ucCount;
}

/**
 *******************************************************************************
 * @brief       instead of labs() func
 * @param[in]   ulData1 and ulData2
 * @retval      Diff
 * @par Description:
 *******************************************************************************
 */
uint32_t
subtraction_abs32 (uint32_t ulVal1, uint32_t ulVal2)
{
  if (ulVal1 > ulVal2)
    {
      return (ulVal1 - ulVal2);
    }
  else
    {
      return (ulVal2 - ulVal1);
    }
}

/**
 *******************************************************************************
 * @brief       instead of labs() func
 * @param[in]   data1 and data2
 * @retval      Diff
 * @par Description:
 *******************************************************************************
 */
uint32_t
subtraction_abs32_signed (int32_t val1, int32_t val2)
{
  if (val1 > val2)
    {
      return (uint32_t) (val1 - val2);
    }
  else
    {
      return (uint32_t) (val2 - val1);
    }
}

/**
 *******************************************************************************
 * @brief convert uint32 data to buffer
 * @param[in]
 * @param[out]
 *******************************************************************************
 */
void
convert_hex_to_6byte_array (uint32_t val, uint8_t *ptr)
{
  uint8_t i;

  ptr += 5;
  for (i = 0; i < 6; i++)
    {
      *ptr = val % 10;
      val /= 10;
      --ptr;
    }
}

/**
 *******************************************************************************
 * @brief convert uint32 data to buffer
 * @param[in]
 * @param[out]
 *******************************************************************************
 */
uint32_t
convert_6byte_array_to_hex (uint8_t *ptr)
{
  uint32_t val = 0x00;

  val = *ptr * 100000;
  ++ptr;
  val += *ptr * 10000;
  ++ptr;
  val += *ptr * 1000;
  ++ptr;
  val += *ptr * 100;
  ++ptr;
  val += *ptr * 10;
  ++ptr;
  val += *ptr;

  return val;
}

/**
 *******************************************************************************
 * @brief int32 divider
 * @param[in] val is divided to divisor. below divisor/2 is rounded floor
 * @param[out]
 *******************************************************************************
 */
int32_t
divisor32 (int32_t val, int32_t divisor)
{
  if (val < 0)
    {
      return ((val - (divisor / 2)) / divisor);
    }
  else
    {
      return ((val + (divisor / 2)) / divisor);
    }
}

/**
 *******************************************************************************
 * @brief int32 round
 * @param[in] val to be rounded. below rounder/2 is rounded floor
 * @param[out]
 *******************************************************************************
 */
int32_t
round32 (int32_t val, int32_t rounder)
{
  if (val < 0)
    {
      return (((val - (rounder / 2)) / rounder) * rounder);
    }
  else
    {
      return (((val + (rounder / 2)) / rounder) * rounder);
    }
}

/* * * END OF FILE * * */
