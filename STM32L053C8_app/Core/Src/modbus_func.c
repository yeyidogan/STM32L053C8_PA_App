/******************************************************************************
 *	Written by Yilmaz Eyidogan
 *	Modbus application functions
 *	Created date: 2016.02.20
 *******************************************************************************/
#include "modbus_func.h"
#include "stm32l0xx_hal.h"

/* Private typedef */
/* Private define */

/* Private macro */
/* Private variables */
uint32_t u32_mf_tmp;
uint16_t u16_mf_tmp, *ptr_u16;
UINT16_TYPE u16_type_tmp;
uint8_t *ptr_u8;

uint32_t u32_par1 = 0x01234567;

uint32_t inputs = 0, outputs = 0;

__weak uint32_t read_gpio_outputs(void)
{
  return outputs;
}
__weak uint32_t read_gpio_inputs(void)
{
  return inputs;
}
__weak void write_gpio_outputs(uint32_t out)
{
  outputs = out;
}

//extern const uint16_t sizeOfHoldingRegister; //defined at the end of this file
/**
 *******************************************************************************
 * @brief       func 0x01, read coils, outputs
 * @param[in]
 * @param[in]
 * @param[out] application status true or false
 *******************************************************************************
 */
uint8_t read_coils(void)
{

  u32_mf_tmp = read_gpio_outputs ();
  u32_mf_tmp >>= PTR_MODBUS_READ_REQ->quantity.word;
  PTR_READ_BYTE_RESP->functionCode = FUNC_READ_COILS;
  PTR_READ_BYTE_RESP->byteCount = 0x01;
  PTR_READ_BYTE_RESP->byte = u32_mf_tmp;
  crc16 (mbTxRxData.ptrTxData, sizeof(MODBUS_READ_BYTE_RESPONSE_FRAME) - 2);
  mbTxRxData.txLength = sizeof(MODBUS_READ_BYTE_RESPONSE_FRAME);
  return true;
}
/**
 *******************************************************************************
 * @brief       func 0x02, read inputs
 * @param[in]
 * @param[in]
 * @param[out] application status true or false
 *******************************************************************************
 */
uint8_t read_inputs(void)
{
  u32_mf_tmp = read_gpio_inputs ();
  u32_mf_tmp >>= PTR_MODBUS_READ_REQ->quantity.word;
  PTR_READ_BYTE_RESP->functionCode = FUNC_READ_DISCRETE_INPUTS;
  PTR_READ_BYTE_RESP->byteCount = 0x01;
  PTR_READ_BYTE_RESP->byte = u32_mf_tmp;
  crc16 (mbTxRxData.ptrTxData, sizeof(MODBUS_READ_BYTE_RESPONSE_FRAME) - 2);
  mbTxRxData.txLength = sizeof(MODBUS_READ_BYTE_RESPONSE_FRAME);
  return true;
}
/**
 *******************************************************************************
 * @brief       func 0x03, read holding registers
 * @param[in]
 * @param[in]
 * @param[out] application status true or false
 *******************************************************************************
 */
uint8_t read_HoldingRegister(uint16_t index_of_HoldingRegister)
{
  PTR_READ_HOLDING_REQ->functionCode = FUNC_READ_HOLDING_REGISTERS;
  PTR_READ_HOLDING_REQ->byteCount = uiWordQty * 2;

  ptr_u8 = (uint8_t*) &(PTR_READ_HOLDING_REQ->word.word);

  while (uiWordQty--)
  {
    switch (st_holding_reg_array[index_of_HoldingRegister].register_type) {
      case W_U16_PTR_VAL:
	*ptr_u8++ = 0x00;
	*ptr_u8++ = 0x00;
	break;
      case R_U16_PTR_VAL:
      case RW_U16_PTR_VAL:
	u16_type_tmp.word = *st_holding_reg_array[index_of_HoldingRegister].ptr_u16;
	*ptr_u8++ = u16_type_tmp.bytes.high_byte;
	*ptr_u8++ = u16_type_tmp.bytes.low_byte;
	break;
      case RW_U16_FROM_PTR_FUNC:
	break;
      case LAST_DUMMY_REGITER:
	uiWordQty = 0x00;
	break;
      default:
	break;
    }
    ++index_of_HoldingRegister;
  }
  crc16 (mbTxRxData.ptrTxData, 0x03 + PTR_READ_HOLDING_REQ->byteCount);
  mbTxRxData.txLength = 0x05 + PTR_READ_HOLDING_REQ->byteCount;
  return true;
}
/**
 *******************************************************************************
 * @brief       func 0x05, write single coil
 * @param[in]
 * @param[in]
 * @param[out] application status true or false
 *******************************************************************************
 */
uint8_t write_SingleCoil(void)
{
  uint32_t out;
  PTR_WRITE_SINGLE_COIL_RESP->functionCode = FUNC_WRITE_SINGLE_COIL;
  PTR_WRITE_SINGLE_COIL_RESP->address.word = PTR_MODBUS_WRITE_SINGLE_REQ->address.word;
  PTR_WRITE_SINGLE_COIL_RESP->value.word = PTR_MODBUS_WRITE_SINGLE_REQ->value.word;
  if (PTR_MODBUS_WRITE_SINGLE_REQ->value.word == 0x00FF)
  { //COIL=ON FF00 chanded to 00FF, endian problem
    u32_mf_tmp = 1UL;
    u32_mf_tmp <<= word_endianer (PTR_MODBUS_WRITE_SINGLE_REQ->address.word);
    out = read_gpio_outputs ();
    out |= u32_mf_tmp;
    write_gpio_outputs (out);
  }
  else if (PTR_MODBUS_WRITE_SINGLE_REQ->value.word == 0x0000)
  { //COIL=OFF
    u32_mf_tmp = 1UL;
    u32_mf_tmp <<= word_endianer (PTR_MODBUS_WRITE_SINGLE_REQ->address.word);
    out = read_gpio_outputs ();
    out &= ~u32_mf_tmp;
    write_gpio_outputs (out);
  }
  else
  {
    return false;
  }

  crc16 (mbTxRxData.ptrTxData, sizeof(MODBUS_WRITE_SINGLE_REQUEST_FRAME) - 2);
  mbTxRxData.txLength = sizeof(MODBUS_WRITE_SINGLE_REQUEST_FRAME);
  return true;
}
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

	{ ((uint16_t*) &u32_par1), rwDummyFunc, //
	    rwDummyFunc, //
	    END_OF_DWORD_REGISTER, //
	    RW_U16_PTR_VAL //
	}, //

    };

const uint16_t sizeOfHoldingRegister = sizeof(st_holding_reg_array)
    / sizeof(ST_MODBUS_HOLDING_REGISTER_TYPE);
/* * * END OF FILE * * */
