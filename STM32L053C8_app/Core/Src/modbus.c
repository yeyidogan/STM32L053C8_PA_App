/******************************************************************************
 *	Written by Yilmaz Eyidogan
 *	Modbus core
 *	Created date: 2025.11.17
 *******************************************************************************/
#include "modbus.h"
#include <stdbool.h>
#include "stm32l0xx_hal.h"

/* Private typedef */
/* Private define */

/* Private macro */
/* Private variables */
MODBUS_TX_RX_DATA mbTxRxData;
uint8_t modbus_rx_buf[64];
uint8_t modbus_tx_buf[64];

const UINT16_TYPE stCrcTable[] = { { 0x0000 }, { 0xC1C0 }, { 0x81C1 }, { 0x4001 }, { 0x01C3 }, {
    0xC003 }, { 0x8002 }, { 0x41C2 }, { 0x01C6 }, { 0xC006 }, { 0x8007 }, { 0x41C7 }, { 0x0005 }, {
    0xC1C5 }, { 0x81C4 }, { 0x4004 }, { 0x01CC }, { 0xC00C }, { 0x800D }, { 0x41CD }, { 0x000F }, {
    0xC1CF }, { 0x81CE }, { 0x400E }, { 0x000A }, { 0xC1CA }, { 0x81CB }, { 0x400B }, { 0x01C9 }, {
    0xC009 }, { 0x8008 }, { 0x41C8 }, { 0x01D8 }, { 0xC018 }, { 0x8019 }, { 0x41D9 }, { 0x001B }, {
    0xC1DB }, { 0x81DA }, { 0x401A }, { 0x001E }, { 0xC1DE }, { 0x81DF }, { 0x401F }, { 0x01DD }, {
    0xC01D }, { 0x801C }, { 0x41DC }, { 0x0014 }, { 0xC1D4 }, { 0x81D5 }, { 0x4015 }, { 0x01D7 }, {
    0xC017 }, { 0x8016 }, { 0x41D6 }, { 0x01D2 }, { 0xC012 }, { 0x8013 }, { 0x41D3 }, { 0x0011 }, {
    0xC1D1 }, { 0x81D0 }, { 0x4010 }, { 0x01F0 }, { 0xC030 }, { 0x8031 }, { 0x41F1 }, { 0x0033 }, {
    0xC1F3 }, { 0x81F2 }, { 0x4032 }, { 0x0036 }, { 0xC1F6 }, { 0x81F7 }, { 0x4037 }, { 0x01F5 }, {
    0xC035 }, { 0x8034 }, { 0x41F4 }, { 0x003C }, { 0xC1FC }, { 0x81FD }, { 0x403D }, { 0x01FF }, {
    0xC03F }, { 0x803E }, { 0x41FE }, { 0x01FA }, { 0xC03A }, { 0x803B }, { 0x41FB }, { 0x0039 }, {
    0xC1F9 }, { 0x81F8 }, { 0x4038 }, { 0x0028 }, { 0xC1E8 }, { 0x81E9 }, { 0x4029 }, { 0x01EB }, {
    0xC02B }, { 0x802A }, { 0x41EA }, { 0x01EE }, { 0xC02E }, { 0x802F }, { 0x41EF }, { 0x002D }, {
    0xC1ED }, { 0x81EC }, { 0x402C }, { 0x01E4 }, { 0xC024 }, { 0x8025 }, { 0x41E5 }, { 0x0027 }, {
    0xC1E7 }, { 0x81E6 }, { 0x4026 }, { 0x0022 }, { 0xC1E2 }, { 0x81E3 }, { 0x4023 }, { 0x01E1 }, {
    0xC021 }, { 0x8020 }, { 0x41E0 }, { 0x01A0 }, { 0xC060 }, { 0x8061 }, { 0x41A1 }, { 0x0063 }, {
    0xC1A3 }, { 0x81A2 }, { 0x4062 }, { 0x0066 }, { 0xC1A6 }, { 0x81A7 }, { 0x4067 }, { 0x01A5 }, {
    0xC065 }, { 0x8064 }, { 0x41A4 }, { 0x006C }, { 0xC1AC }, { 0x81AD }, { 0x406D }, { 0x01AF }, {
    0xC06F }, { 0x806E }, { 0x41AE }, { 0x01AA }, { 0xC06A }, { 0x806B }, { 0x41AB }, { 0x0069 }, {
    0xC1A9 }, { 0x81A8 }, { 0x4068 }, { 0x0078 }, { 0xC1B8 }, { 0x81B9 }, { 0x4079 }, { 0x01BB }, {
    0xC07B }, { 0x807A }, { 0x41BA }, { 0x01BE }, { 0xC07E }, { 0x807F }, { 0x41BF }, { 0x007D }, {
    0xC1BD }, { 0x81BC }, { 0x407C }, { 0x01B4 }, { 0xC074 }, { 0x8075 }, { 0x41B5 }, { 0x0077 }, {
    0xC1B7 }, { 0x81B6 }, { 0x4076 }, { 0x0072 }, { 0xC1B2 }, { 0x81B3 }, { 0x4073 }, { 0x01B1 }, {
    0xC071 }, { 0x8070 }, { 0x41B0 }, { 0x0050 }, { 0xC190 }, { 0x8191 }, { 0x4051 }, { 0x0193 }, {
    0xC053 }, { 0x8052 }, { 0x4192 }, { 0x0196 }, { 0xC056 }, { 0x8057 }, { 0x4197 }, { 0x0055 }, {
    0xC195 }, { 0x8194 }, { 0x4054 }, { 0x019C }, { 0xC05C }, { 0x805D }, { 0x419D }, { 0x005F }, {
    0xC19F }, { 0x819E }, { 0x405E }, { 0x005A }, { 0xC19A }, { 0x819B }, { 0x405B }, { 0x0199 }, {
    0xC059 }, { 0x8058 }, { 0x4198 }, { 0x0188 }, { 0xC048 }, { 0x8049 }, { 0x4189 }, { 0x004B }, {
    0xC18B }, { 0x818A }, { 0x404A }, { 0x004E }, { 0xC18E }, { 0x818F }, { 0x404F }, { 0x018D }, {
    0xC04D }, { 0x804C }, { 0x418C }, { 0x0044 }, { 0xC184 }, { 0x8185 }, { 0x4045 }, { 0x0187 }, {
    0xC047 }, { 0x8046 }, { 0x4186 }, { 0x0182 }, { 0xC042 }, { 0x8043 }, { 0x4183 }, { 0x0041 }, {
    0xC181 }, { 0x8180 }, { 0x4040 } };

uint16_t uiWordQty = 0x00;

uint32_t u32_mf_tmp;
uint16_t u16_mf_tmp, *ptr_u16;
UINT16_TYPE u16_type_tmp;
uint8_t *ptr_u8;

/**
 *******************************************************************************
 * @brief       calculate crc
 * @param[in]   ptrCell: pointer of data
 * @param[in]   length: length of data without CRC
 * @param[out]  crc
 *******************************************************************************
 */
void crc16(uint8_t *ptrCell, uint8_t length)
{
  uint8_t ucIndex;
  UINT16_TYPE crcWord = { 0xFFFF };

  while (length--)
  {
    ucIndex = crcWord.bytes.high_byte ^ *ptrCell++;
    crcWord.bytes.high_byte = crcWord.bytes.low_byte ^ stCrcTable[ucIndex].bytes.high_byte;
    crcWord.bytes.low_byte = stCrcTable[ucIndex].bytes.low_byte;
  }
  *ptrCell++ = crcWord.bytes.high_byte;
  *ptrCell = crcWord.bytes.low_byte;
}

/**
 *******************************************************************************
 * @brief       check crc
 * @param[in]   ptrCell: pointer of data
 * @param[in]   length: length of data without CRC
 * @param[out]  true or false
 *******************************************************************************
 */
uint8_t check_crc16(uint8_t *ptrCell, uint16_t length)
{
  uint8_t ucIndex;
  UINT16_TYPE crcWord = { 0xFFFF };

  while (length--)
  {
    ucIndex = crcWord.bytes.high_byte ^ *ptrCell++;
    crcWord.bytes.high_byte = crcWord.bytes.low_byte ^ stCrcTable[ucIndex].bytes.high_byte;
    crcWord.bytes.low_byte = stCrcTable[ucIndex].bytes.low_byte;
  }
  if (crcWord.bytes.high_byte == *ptrCell)
  {
    ++ptrCell;
    if (crcWord.bytes.low_byte == *ptrCell)
      return true;
  }
  return false;
}

uint32_t inputs = 0, outputs = 0;

__weak uint32_t read_gpio_outputs(uint16_t start_address, uint16_t qty_coils)
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
  struct st_modbus_0x01_to_04_req *ptr_req;
  struct st_modbus_read_byte_resp *ptr_resp;
  uint16_t start_add;

  ptr_req = (struct st_modbus_0x01_to_04_req*) modbus_rx_buf;
  ptr_resp = (struct st_modbus_read_byte_resp*) modbus_tx_buf;

  ptr_resp->slave_add = ptr_req->slave_add;
  ptr_resp->function_code = ptr_req->function_code;
  ptr_resp->byte_count = 0;

  start_add = word_endianer (ptr_req->start_address.word);
  ptr_u8 = (uint8_t*) &(ptr_resp->byte); //coil data is stored to this and consecutive field

  while (uiWordQty) //uiWordQty = quantity of coils
  {
    if (uiWordQty > 8)
    {
      *ptr_u8 = (uint8_t) read_gpio_outputs (start_add, 8);
      uiWordQty -= 8;
    }
    else
    {
      *ptr_u8 = (uint8_t) read_gpio_outputs (start_add, uiWordQty);
      uiWordQty = 0;
    }
    start_add += 8;
    ++ptr_u8;
    ++ptr_resp->byte_count;
  }

  crc16 (modbus_tx_buf, sizeof(struct st_modbus_read_byte_resp) + ptr_resp->byte_count - 1);
  mbTxRxData.txLength = sizeof(struct st_modbus_read_byte_resp) + ptr_resp->byte_count + 1;
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
  PTR_READ_BYTE_RESP->function_code = FUNC_READ_DISCRETE_INPUTS;
  PTR_READ_BYTE_RESP->byte_count = 0x01;
  PTR_READ_BYTE_RESP->byte = u32_mf_tmp;
  crc16 (mbTxRxData.ptrTxData, sizeof(MODBUS_READ_BYTE_RESPONSE_FRAME) - 2);
  mbTxRxData.txLength = sizeof(MODBUS_READ_BYTE_RESPONSE_FRAME);
  return true;
}
/**
 *******************************************************************************
 * @brief       find index of holding register
 * @param[in]
 * @param[in]
 * @param[out]
 *******************************************************************************
 */
uint16_t find_holding_register(uint16_t start_address)
{
  uint16_t i = 0;

  for (i = 0; i < size_of_holding_register; i++)
  {
    if (st_holding_reg_array[i].register_address == start_address)
    {
      return i;
    }
  }
  return 0xFFFF;
}
/**
 *******************************************************************************
 * @brief       func 0x03, read holding registers
 * @param[in]
 * @param[in]
 * @param[out] application status true or false
 *******************************************************************************
 */
uint8_t read_HoldingRegister(uint16_t address)
{
  PTR_READ_HOLDING_REQ->function_code = FUNC_READ_HOLDING_REGISTERS;
  PTR_READ_HOLDING_REQ->byte_count = uiWordQty * 2;

  ptr_u8 = (uint8_t*) &(PTR_READ_HOLDING_REQ->word.word);
  uint16_t index;

  while (uiWordQty--)
  {
    index = find_holding_register (address);
    if (index >= size_of_holding_register)
    {
      return false;
    }

    u16_type_tmp.word = *st_holding_reg_array[index].ptr_u16;
    *ptr_u8++ = u16_type_tmp.bytes.high_byte;
    *ptr_u8++ = u16_type_tmp.bytes.low_byte;
    u16_type_tmp.word = st_holding_reg_array[index].mb_readFuncPtr ();
    ++address;
  }
  crc16 (mbTxRxData.ptrTxData, 0x03 + PTR_READ_HOLDING_REQ->byte_count);
  mbTxRxData.txLength = 0x05 + PTR_READ_HOLDING_REQ->byte_count;
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
  PTR_WRITE_SINGLE_COIL_RESP->function_code = FUNC_WRITE_SINGLE_COIL;
  PTR_WRITE_SINGLE_COIL_RESP->address.word = PTR_MODBUS_WRITE_SINGLE_REQ->address.word;
  PTR_WRITE_SINGLE_COIL_RESP->value.word = PTR_MODBUS_WRITE_SINGLE_REQ->value.word;
  if (PTR_MODBUS_WRITE_SINGLE_REQ->value.word == 0x00FF)
  { //COIL=ON FF00 chanded to 00FF, endian problem
    u32_mf_tmp = 1UL;
    u32_mf_tmp <<= word_endianer (PTR_MODBUS_WRITE_SINGLE_REQ->address.word);
    out = read_gpio_outputs (0, 8);
    out |= u32_mf_tmp;
    write_gpio_outputs (out);
  }
  else if (PTR_MODBUS_WRITE_SINGLE_REQ->value.word == 0x0000)
  { //COIL=OFF
    u32_mf_tmp = 1UL;
    u32_mf_tmp <<= word_endianer (PTR_MODBUS_WRITE_SINGLE_REQ->address.word);
    out = read_gpio_outputs (0, 8);
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
 * @brief       func 0x10, write holding registers
 * @param[in]
 * @param[in]
 * @param[out] application status true or false
 *******************************************************************************
 */
uint8_t write_HoldingRegister(uint16_t address)
{
  struct st_modbus_0x10_req *ptr_req;
  struct st_modbus_0x10_resp *ptr_resp;
  UINT16_TYPE u16_type_tmp;
  uint16_t index;

  ptr_req = (struct st_modbus_0x10_req*) modbus_rx_buf;
  ptr_resp = (struct st_modbus_0x10_resp*) modbus_tx_buf;

  ptr_resp->slave_add = ptr_req->slave_add;
  ptr_resp->function_code = FUNC_WRITE_MULTIPLE_REGISTERS; //0x10
  ptr_resp->start_address = ptr_req->start_address;
  ptr_resp->quantity.word = ptr_req->quantity.word;

  ptr_u8 = (uint8_t*) &(ptr_req->value);

  while (uiWordQty--)
  {
    index = find_holding_register (address);
    if (index >= size_of_holding_register)
    {
      return false;
    }
    u16_type_tmp.bytes.high_byte = *ptr_u8++;
    u16_type_tmp.bytes.low_byte = *ptr_u8++;
    *st_holding_reg_array[index].ptr_u16 = u16_type_tmp.word;
    st_holding_reg_array[index].mb_writeFuncPtr ();
    ++address;
  }

  crc16 (modbus_tx_buf, 0x06);
  mbTxRxData.txLength = 0x08;
  return true;
}

/**
 *******************************************************************************
 * @brief       prepare exception error message
 * @param[in]   exceptionCode
 * @param[out]  none
 *******************************************************************************
 */
#define SIZE_OF_EXCEPTION_FRAME 0x05
void mb_return_exception(uint8_t exceptionCode)
{

  PTR_EXCEPTION->slave_add = *mbTxRxData.ptrRxData;
  PTR_EXCEPTION->function_code = PTR_MODBUS_READ_REQ->function_code + 0x80;
  PTR_EXCEPTION->exceptionCode = exceptionCode;

  crc16 (mbTxRxData.ptrTxData, SIZE_OF_EXCEPTION_FRAME - 2);
  mbTxRxData.txLength = SIZE_OF_EXCEPTION_FRAME;
}

/**
 *******************************************************************************
 * @brief       modbusRTU main code
 * @param[in]   none
 * @param[out]  none
 * @details     none.
 *******************************************************************************
 */
void modbusRTU(void)
{
  uint16_t tmp;
  mbTxRxData.txLength = 0x00;

  mbTxRxData.ptrRxData = modbus_rx_buf;
  mbTxRxData.ptrTxData = modbus_tx_buf;

  if (mbTxRxData.rxLength < MB_MIN_REQUEST_FRAME_SIZE)
  { //return if there is not enough data
    return;
  }

  mbTxRxData.slave_add = 1;
  if (*mbTxRxData.ptrRxData == mbTxRxData.slave_add)
  {
    if (check_crc16 (mbTxRxData.ptrRxData, mbTxRxData.rxLength - 2) == false)
    {
      return;
    }
    *mbTxRxData.ptrTxData = mbTxRxData.slave_add;

    //check function code and quantity
    switch (PTR_MODBUS_READ_REQ->function_code) {
      case FUNC_READ_COILS:
      case FUNC_READ_DISCRETE_INPUTS:
      case FUNC_READ_HOLDING_REGISTERS:
      case FUNC_READ_INPUT_REGISTERS:
      case FUNC_WRITE_MULTIPLE_COILS:
      case FUNC_WRITE_MULTIPLE_REGISTERS:
	tmp = word_endianer (PTR_MODBUS_READ_REQ->quantity.word);
	if (tmp == 0 || tmp > 0x07D0)
	{
	  mb_return_exception (OUT_OF_MB_LIMIT);
	  return;
	}
	break;
    }

    uiWordQty = word_endianer (PTR_MODBUS_READ_REQ->quantity.word);

    switch (PTR_MODBUS_READ_REQ->function_code) {
      case FUNC_READ_COILS:
	tmp = word_endianer (PTR_MODBUS_READ_REQ->start_address.word) + uiWordQty;
	if (tmp > MB_COIL_SIZE)
	{
	  mb_return_exception (OUT_OF_DATA_REGION);
	  break;
	}
	if (read_coils () == false)
	{
	  mb_return_exception (PROCESS_ERROR);
	}
	break;
      case FUNC_READ_DISCRETE_INPUTS:
	tmp = word_endianer (PTR_MODBUS_READ_REQ->start_address.word) + uiWordQty;
	if (tmp > MB_INPUT_SIZE)
	{
	  mb_return_exception (OUT_OF_DATA_REGION);
	  break;
	}
	if (read_inputs () == false)
	{
	  mb_return_exception (PROCESS_ERROR);
	}
	break;
      case FUNC_READ_HOLDING_REGISTERS:
	tmp = word_endianer (PTR_MODBUS_READ_REQ->start_address.word) + uiWordQty;
	if (tmp > size_of_holding_register)
	{
	  mb_return_exception (OUT_OF_DATA_REGION);
	  break;
	}
	tmp = word_endianer (PTR_MODBUS_READ_REQ->start_address.word);
	if (read_HoldingRegister (tmp) == false)
	{
	  mb_return_exception (PROCESS_ERROR);
	}
	break;
      case FUNC_READ_INPUT_REGISTERS:
	break;
      case FUNC_WRITE_SINGLE_COIL:
	tmp = word_endianer (PTR_MODBUS_WRITE_SINGLE_REQ->address.word);
	if (tmp >= MB_COIL_SIZE)
	{
	  mb_return_exception (OUT_OF_DATA_REGION);
	  break;
	}
	if (write_SingleCoil () == false)
	{
	  mb_return_exception (PROCESS_ERROR);
	}
	break;
      case FUNC_WRITE_SINGLE_REGISTER:
	break;
      case FUNC_WRITE_MULTIPLE_COILS:
	break;
      case FUNC_WRITE_MULTIPLE_REGISTERS:
	tmp = word_endianer (PTR_MODBUS_WRITE_MULTIREGI_REQ->start_address.word) + uiWordQty;
	if (tmp > size_of_holding_register)
	{
	  mb_return_exception (OUT_OF_DATA_REGION);
	  break;
	}
	tmp = word_endianer (PTR_MODBUS_WRITE_MULTIREGI_REQ->start_address.word);
	if (write_HoldingRegister (tmp) == false)
	{
	  mb_return_exception (PROCESS_ERROR);
	}
	break;
      default:
	mb_return_exception (FUNCTION_UNSUPPORTED);
	break;
    }
  }
}
/* * * END OF FILE * * */
