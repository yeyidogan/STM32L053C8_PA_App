/******************************************************************************
 *	Written by Yilmaz Eyidogan
 *	Modbus application function declerations
 *	Created date: 2025.11.17
 *******************************************************************************/
#ifndef __MODBUS_FUNC_H
#define __MODBUS_FUNC_H

#include <stdint.h>
#include <stdbool.h>
#include "util.h"

/* Private macro */

enum
{
  START_OF_WORD_REGISTER = (uint8_t) 0x00,
  END_OF_WORD_REGISTER,
  START_OF_DWORD_REGISTER,
  END_OF_DWORD_REGISTER
};

enum MODBUS_REGISTER_LIST
{
  LAST_DUMMY_REGITER = (uint8_t) 0x00,
  W_U16_PTR_VAL,
  R_U16_PTR_VAL,
  RW_U16_PTR_VAL,
  RW_U16_FROM_PTR_FUNC
};

typedef struct
{
  uint16_t register_address;
  uint16_t *ptr_u16; //r+w to same pointer
  uint16_t (*mb_readFuncPtr)(void); //modbus read function pointer
  uint16_t (*mb_writeFuncPtr)(void); //modbus write function pointer
} ST_MODBUS_HOLDING_REGISTER_TYPE;

/* Private variables */
extern const ST_MODBUS_HOLDING_REGISTER_TYPE st_holding_reg_array[];
extern const uint16_t size_of_holding_register;
/* Private functions */

#endif
/* * * END OF FILE * * */
