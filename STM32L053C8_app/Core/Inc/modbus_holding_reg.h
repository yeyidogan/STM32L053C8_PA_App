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

struct struct_modbus_holding_registers
{
  uint16_t register_address;
  uint16_t (*mb_read_function)(uint16_t x); //modbus read function pointer
  uint16_t (*mb_write_function)(uint16_t x); //modbus write function pointer
};

/* Private variables */
extern const struct struct_modbus_holding_registers st_holding_reg_array[];
extern const uint16_t size_of_holding_register;
/* Private functions */

#endif
/* * * END OF FILE * * */
