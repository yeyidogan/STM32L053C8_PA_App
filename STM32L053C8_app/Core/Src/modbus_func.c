/******************************************************************************
 *	Written by Yilmaz Eyidogan
 *	Modbus application functions
 *	Created date: 2016.02.20
 *******************************************************************************/
#include "modbus_func.h"


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
uint16_t rwDummyFunc(void)
{
  return true;
}
/**
 *******************************************************************************
 * @brief       modbus holding register list
 *******************************************************************************
 */
const ST_MODBUS_HOLDING_REGISTER_TYPE st_holding_reg_array[3] = { ////////////////////////
    { (uint16_t*) &u32_par1 + 1, // pointer 1
    rwDummyFunc, //dummy func
	rwDummyFunc, //dummy func
	START_OF_WORD_REGISTER, //start address
	RW_U16_PTR_VAL //
	},//end of word

	{ ((uint16_t*) &u32_par1), //
	    rwDummyFunc, //
	    rwDummyFunc, //
	    END_OF_DWORD_REGISTER, //
	    RW_U16_PTR_VAL //
	},//
	{ ((uint16_t*) &u16_par1), //
	    rwDummyFunc, //
	    rwDummyFunc, //
	    END_OF_DWORD_REGISTER, //
	    RW_U16_PTR_VAL //
	}, //
    };

const uint16_t size_of_holding_register = sizeof(st_holding_reg_array)
    / sizeof(ST_MODBUS_HOLDING_REGISTER_TYPE);
/* * * END OF FILE * * */
