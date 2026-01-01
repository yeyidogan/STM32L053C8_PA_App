/******************************************************************************
 *	Written by Yilmaz Eyidogan
 *	Modbus application functions
 *	Created date: 2016.02.20
 *******************************************************************************/
#include "modbus_holding_reg.h"

/* Private typedef */
/* Private define */

/* Private macro */
/* Private variables */

uint32_t u32_par1 = 0x01234567;
uint16_t u16_par1 = 0xABCD;

//extern const uint16_t sizeOfHoldingRegister; //defined at the end of this file
/**
 *******************************************************************************
 * @brief       r+w dummy function used if there is no r+w function
 * @param[in]
 * @param[out] application status true
 *******************************************************************************
 */
uint16_t read_dummy_func(uint16_t val)
{
  return val;
}
uint16_t write_dummy_func(uint16_t val)
{
  return val;
}

uint16_t read_adc_val(uint16_t channel)
{
  return (0x1000 + channel);
}
/**
 *******************************************************************************
 * @brief       modbus holding register list
 *******************************************************************************
 */
const struct struct_modbus_holding_registers st_holding_reg_array[3] = { ////////////////////////
    { 0x0000, //40001
	read_adc_val, //dummy func
	write_dummy_func, //dummy func
	},//

	{ 0x0001, //40002
	    read_adc_val, //
	    write_dummy_func, //
	},//

	{ 0x0002, //40003
	    read_adc_val, //
	    write_dummy_func, //
	},//
	{ 0x0003, //40004
	    read_adc_val, //
	    write_dummy_func, //
	},//
	{ 0x0004, //40005
	    read_adc_val, //
	    write_dummy_func, //
	},//
	{ 0x0005, //40006
	    read_adc_val, //
	    write_dummy_func, //
	},//
	{ 0x0006, //40006
	    read_adc_val, //
	    write_dummy_func, //
	},//
	{ 0x0007, //40007
	    read_adc_val, //
	    write_dummy_func, //
	},//
	{ 0x0008, //40008
	    read_adc_val, //
	    write_dummy_func, //
	},//
	{ 0x0009, //40009
	    read_adc_val, //
	    write_dummy_func, //
	},//
	{ 0x000A, //40010
	    read_adc_val, //
	    write_dummy_func, //
	},//
	{ 0x000B, //40011
	    read_adc_val, //
	    write_dummy_func, //
	},//
	{ 0x000C, //40012
	    read_adc_val, //
	    write_dummy_func, //
	}, //
    };

const uint16_t size_of_holding_register = sizeof(st_holding_reg_array)
    / sizeof(struct struct_modbus_holding_registers);
/* * * END OF FILE * * */
