/******************************************************************************
 *	Written by Yilmaz Eyidogan
 *	utils
 *	Created date: 2025.02.02
 *******************************************************************************/
#include "util.h"
#include <stdbool.h>

union union_u32
{
  uint32_t dword;
  uint8_t u8[4];
  struct
  {
    uint8_t ll;
    uint8_t lh;
    uint8_t hl;
    uint8_t hh;
  } byte;
};

/**
 *******************************************************************************
 * @brief       endian changer
 * @param[in]   None
 * @retval      None
 * @par Description little endian to big endian
 *******************************************************************************
 */
uint16_t word_endianer(uint16_t wordIn)
{
  uint16_t tmp;

  tmp = wordIn;
  tmp <<= 8;
  wordIn >>= 8;
  tmp += wordIn;
  return tmp;
}

uint32_t dword_endianer(uint32_t dword)
{
  union union_u32 u32_1, u32_2;

  u32_1.dword = dword;
  u32_2.u8[0] = u32_1.u8[3];
  u32_2.u8[1] = u32_1.u8[2];
  u32_2.u8[2] = u32_1.u8[1];
  u32_2.u8[3] = u32_1.u8[0];
  return u32_2.dword;
}

/**
 *******************************************************************************
 * @brief       instead of labs() func
 * @param[in]   ulVal1 and ulVal2
 * @retval      Diff
 * @par Description:
 *******************************************************************************
 */
uint32_t subtraction_abs32(uint32_t ulVal1, uint32_t ulVal2)
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
 * @param[in]   val1 and val2
 * @retval      Diff
 * @par Description:
 *******************************************************************************
 */
uint32_t subtraction_abs32_signed(int32_t val1, int32_t val2)
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
void convert_hex_to_6byte_array(uint32_t val, uint8_t *ptr)
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
uint32_t convert_6byte_array_to_hex(uint8_t *ptr)
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
int32_t divisor32(int32_t val, int32_t divisor)
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
int32_t round32(int32_t val, int32_t rounder)
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

/**
 *******************************************************************************
 * @brief byte to ascii converter
 * @param[in] byte
 * @param[out] ascii code
 *******************************************************************************
 */
uint8_t uint8_to_ascii(uint8_t val)
{
  if (val <= 9)
  {
    return val + '0';
  }
  if (val <= 0x0F)
  {
    return val + 'A';
  }
  return 'x';
}

/**
 *******************************************************************************
 * @brief char hexadecimal ascii to uint8_t converter
 * @param[in] pointer of ascii string
 * @param[out] uint8_t
 *******************************************************************************
 */
uint8_t get_ascii_hex_byte(uint8_t *ptr)
{
  uint8_t u8t = 0x00;
  uint8_t length = 2;

  while (length--)
  {
    u8t *= 0x10;
    if ((*ptr >= '0') && (*ptr <= '9'))
    {
      u8t += (*ptr - '0');
    }
    else if ((*ptr >= 'A') && (*ptr <= 'F'))
    {
      u8t += (*ptr - 'A' + 10);
    }
    ++ptr;
  }
  return u8t;
}

/**
 *******************************************************************************
 * @brief char hexadecimal ascii to uint32_t converter
 * @param[in] pointer of ascii string
 * @param[in] size_t: 1 for uint8_t, 2 for uint16_t, 4 for uint32_t
 * @param[out] uint32_t
 *******************************************************************************
 */
uint32_t get_u32_from_string(uint8_t *ptr, uint8_t size_t)
{
  uint32_t u32 = 0;

  while (size_t--)
  {
    u32 *= 0x100;
    u32 += get_ascii_hex_byte (ptr);
    ptr += 2;
  }
  return u32;
}

/**
 *******************************************************************************
 * @brief parse intel hex format
 * @param[in] pointer of ascii string
 * @param[in]
 * @param[out]
 *******************************************************************************
 */
#pragma GCC push_options
#pragma GCC optimize ("O0")
uint8_t intel_hex_line_validate(uint8_t *ptr)
{
  uint16_t byte_count, i;
  uint16_t sum16;
  uint16_t checksum;

  if (ptr[0] != ':')
  {
    return false;
  }
  byte_count = get_ascii_hex_byte (&ptr[1]);
  sum16 = 0;
  for (i = 0; i < (byte_count + 4); i++)
  {
    sum16 += get_ascii_hex_byte (&ptr[1 + (i * 2)]);
  }
  //sum16 &= 0x00FF;
  checksum = get_ascii_hex_byte (&ptr[1 + (i * 2)]);
  if (((sum16 + checksum) & 0x00FF) == 0x00)
  {
    return true;
  }
  return false;
}
#pragma GCC pop_options
/* * * END OF FILE * * */
