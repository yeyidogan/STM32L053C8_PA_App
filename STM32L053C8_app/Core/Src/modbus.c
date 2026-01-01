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
struct struct_modbus_data st_modbus_data;
uint8_t modbus_rx_buf[64];
uint8_t modbus_tx_buf[64];

const UINT16_TYPE crc_table[] = { { 0x0000 }, { 0xC1C0 }, { 0x81C1 }, { 0x4001 }, { 0x01C3 }, {
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

/**
 *******************************************************************************
 * @brief       calculate crc
 * @param[in]   ptrCell: pointer of data
 * @param[in]   length: length of data without CRC
 * @param[out]  crc
 *******************************************************************************
 */
void crc16(uint8_t *ptr_data, uint8_t length)
{
  uint8_t ucIndex;
  UINT16_TYPE crc = { 0xFFFF };

  while (length--)
  {
    ucIndex = crc.bytes.high_byte ^ *ptr_data++;
    crc.bytes.high_byte = crc.bytes.low_byte ^ crc_table[ucIndex].bytes.high_byte;
    crc.bytes.low_byte = crc_table[ucIndex].bytes.low_byte;
  }
  *ptr_data++ = crc.bytes.high_byte;
  *ptr_data = crc.bytes.low_byte;
}

/**
 *******************************************************************************
 * @brief       check crc
 * @param[in]   ptrCell: pointer of data
 * @param[in]   length: length of data without CRC
 * @param[out]  true or false
 *******************************************************************************
 */
uint8_t check_crc16(uint8_t *ptr_data, uint16_t length)
{
  uint8_t ucIndex;
  UINT16_TYPE crc = { 0xFFFF };

  while (length--)
  {
    ucIndex = crc.bytes.high_byte ^ *ptr_data++;
    crc.bytes.high_byte = crc.bytes.low_byte ^ crc_table[ucIndex].bytes.high_byte;
    crc.bytes.low_byte = crc_table[ucIndex].bytes.low_byte;
  }
  if (crc.bytes.high_byte == *ptr_data)
  {
    ++ptr_data;
    if (crc.bytes.low_byte == *ptr_data)
    {
      return true;
    }
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
 * @brief      func 0x01, read coils, outputs
 * @param[in]
 * @param[in]
 * @param[out] application status true or false
 *******************************************************************************
 */
uint8_t read_coils_0x01(void)
{
  struct st_modbus_0x01_to_04_req *ptr_req;
  struct st_modbus_io_status_resp *ptr_resp;
  uint16_t start_addr;
  uint32_t outputs;
  uint16_t qty_coils;

  uint32_t resp_mask = 1;
  uint16_t resp_byte_count;
  uint16_t i;

  ptr_req = (struct st_modbus_0x01_to_04_req*) modbus_rx_buf;
  ptr_resp = (struct st_modbus_io_status_resp*) modbus_tx_buf;

  ptr_resp->slave_add = st_modbus_data.slave_add;
  ptr_resp->function_code = ptr_req->function_code;

  qty_coils = ptr_req->quantity;
  start_addr = word_endianer (ptr_req->start_address);
  if (start_addr + qty_coils > MODBUS_COILS_QTY)
  {
    return OUT_OF_DATA_REGION;
  }
  outputs = read_gpio_outputs (0, 32);
  outputs >>= start_addr;
  resp_mask <<= qty_coils;
  resp_mask -= 1;

  outputs &= resp_mask;
  resp_byte_count = (qty_coils + 7) / 8;

  for (i = 0; i < resp_byte_count; i++)
  {
    ptr_resp->data[i] = (uint8_t) (outputs & 0x00FF);
  }
  ptr_resp->byte_count = i;
  crc16 (modbus_tx_buf, 3 + i);
  st_modbus_data.tx_length = 5 + i;

//  ptr_u8 = (uint8_t*) &(ptr_resp->byte); //coil data is stored to this and consecutive field
//
//  while (qty_coils) //uiWordQty = quantity of coils
//  {
//    if (qty_coils > 8)
//    {
//      *ptr_u8 = (uint8_t) read_gpio_outputs (start_addr, 8);
//      qty_coils -= 8;
//    }
//    else
//    {
//      *ptr_u8 = (uint8_t) read_gpio_outputs (start_addr, qty_coils);
//      qty_coils = 0;
//    }
//    start_add += 8;
//    ++ptr_u8;
//    ++ptr_resp->byte_count;
//  }
//
//  crc16 (modbus_tx_buf, sizeof(struct st_modbus_read_byte_resp) + ptr_resp->byte_count - 1);
//  mbTxRxData.txLength = sizeof(struct st_modbus_read_byte_resp) + ptr_resp->byte_count + 1;
  return EXCEPTION_NONE;
}

/**
 *******************************************************************************
 * @brief      func 0x02, read inputs
 * @param[in]
 * @param[in]
 * @param[out] application status true or false
 *******************************************************************************
 */
uint8_t read_discrete_inputs_0x02(void)
{
  struct st_modbus_0x01_to_04_req *ptr_req;
  struct st_modbus_io_status_resp *ptr_resp;
  uint16_t start_addr;
  uint32_t inputs;
  uint16_t qty_inputs;

  uint32_t resp_mask = 1;
  uint16_t resp_byte_count;
  uint16_t i;

  ptr_req = (struct st_modbus_0x01_to_04_req*) modbus_rx_buf;
  ptr_resp = (struct st_modbus_io_status_resp*) modbus_tx_buf;

  ptr_resp->slave_add = st_modbus_data.slave_add;
  ptr_resp->function_code = FUNC_READ_DISCRETE_INPUTS;

  qty_inputs = ptr_req->quantity;
  start_addr = word_endianer (ptr_req->start_address);
  if (start_addr + qty_inputs > MODBUS_INPUTS_QTY)
  {
    return OUT_OF_DATA_REGION;
  }
  inputs = read_gpio_inputs ();
  inputs >>= start_addr;
  resp_mask <<= qty_inputs;
  resp_mask -= 1;
//  for (i = 0; i < qty_inputs; i++)
//  {
//    resp_mask <<= 1;
//    resp_mask |= 1;
//  }
  inputs &= resp_mask;
  resp_byte_count = (qty_inputs + 7) / 8;

  for (i = 0; i < resp_byte_count; i++)
  {
    ptr_resp->data[i] = (uint8_t) (inputs & 0x00FF);
  }
  ptr_resp->byte_count = i;
  crc16 (modbus_tx_buf, 3 + i);
  st_modbus_data.tx_length = 5 + i;

//  u32_mf_tmp = read_gpio_inputs ();
//  u32_mf_tmp >>= PTR_MODBUS_READ_REQ->quantity.word;
//  PTR_READ_BYTE_RESP->function_code = FUNC_READ_DISCRETE_INPUTS;
//  PTR_READ_BYTE_RESP->byte_count = 0x01;
//  PTR_READ_BYTE_RESP->byte = u32_mf_tmp;
//  crc16 (mbTxRxData.ptrTxData, sizeof(MODBUS_READ_BYTE_RESPONSE_FRAME) - 2);
//  mbTxRxData.txLength = sizeof(MODBUS_READ_BYTE_RESPONSE_FRAME);
  return EXCEPTION_NONE;
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
uint8_t read_holding_register_0x03(void)
{
  struct st_modbus_0x01_to_04_req *ptr_req;
  struct st_modbus_io_status_resp *ptr_resp;
  uint16_t reg_addr;
  uint16_t qty_registers;
  uint16_t byte_count;
  UINT16_TYPE register_value;
  uint16_t index;
  uint16_t i;

  ptr_req = (struct st_modbus_0x01_to_04_req*) modbus_rx_buf;
  ptr_resp = (struct st_modbus_io_status_resp*) modbus_tx_buf;

  ptr_resp->slave_add = st_modbus_data.slave_add;
  ptr_resp->function_code = FUNC_READ_HOLDING_REGISTERS;

  reg_addr = word_endianer (ptr_req->start_address);
  qty_registers = ptr_req->quantity;
  byte_count = 2 * qty_registers;

  i = 0;
  if (qty_registers == 0)
  {
    return OUT_OF_DATA_REGION;
  }
  while (qty_registers--)
  {
    index = find_holding_register (reg_addr);
    if (index >= size_of_holding_register)
    {
      return OUT_OF_DATA_REGION;
    }
    register_value.word = st_holding_reg_array[index].mb_read_function (reg_addr);
    ptr_resp->data[i++] = register_value.bytes.high_byte;
    ptr_resp->data[i++] = register_value.bytes.low_byte;
    ++reg_addr;
  }
  ptr_resp->byte_count = i;
  crc16 (modbus_tx_buf, 0x03 + byte_count);
  st_modbus_data.tx_length = 0x05 + byte_count;

//  PTR_READ_HOLDING_REQ->function_code = FUNC_READ_HOLDING_REGISTERS;
//  PTR_READ_HOLDING_REQ->byte_count = uiWordQty * 2;
//
//  ptr_u8 = (uint8_t*) &(PTR_READ_HOLDING_REQ->word.word);
//
//  while (uiWordQty--)
//  {
//    index = find_holding_register (address);
//    if (index >= size_of_holding_register)
//    {
//      return false;
//    }
//
//    u16_type_tmp.word = *st_holding_reg_array[index].ptr_u16;
//    *ptr_u8++ = u16_type_tmp.bytes.high_byte;
//    *ptr_u8++ = u16_type_tmp.bytes.low_byte;
//    u16_type_tmp.word = st_holding_reg_array[index].mb_readFuncPtr ();
//    ++address;
//  }
//  crc16 (mbTxRxData.ptrTxData, 0x03 + PTR_READ_HOLDING_REQ->byte_count);
//  mbTxRxData.txLength = 0x05 + PTR_READ_HOLDING_REQ->byte_count;
  return EXCEPTION_NONE;
}

/**
 *******************************************************************************
 * @brief       func 0x04, read input registers
 * @param[in]
 * @param[in]
 * @param[out] application status true or false
 *******************************************************************************
 */
uint8_t read_input_registers_0x04(void)
{
  struct st_modbus_0x01_to_04_req *ptr_req;
  struct st_modbus_io_status_resp *ptr_resp;
  uint16_t start_addr;
  uint16_t qty_registers;
  uint16_t byte_count;
  uint32_t inputs;
  UINT16_TYPE register_value;
  uint16_t i;

  ptr_req = (struct st_modbus_0x01_to_04_req*) modbus_rx_buf;
  ptr_resp = (struct st_modbus_io_status_resp*) modbus_tx_buf;

  ptr_resp->slave_add = st_modbus_data.slave_add;
  ptr_resp->function_code = FUNC_READ_INPUT_REGISTERS;

  start_addr = word_endianer (ptr_req->start_address);
  qty_registers = ptr_req->quantity;
  if (start_addr + qty_registers > 1)
    return OUT_OF_DATA_REGION;
  if (qty_registers == 0)
    return OUT_OF_DATA_REGION;

  byte_count = 2 * qty_registers;

  inputs = read_gpio_inputs ();
  inputs >>= (16 * start_addr);

  while (qty_registers--)
  {
    register_value.word = inputs & 0x00FF;
    ptr_resp->data[i++] = register_value.bytes.high_byte;
    ptr_resp->data[i++] = register_value.bytes.low_byte;
  }
  ptr_resp->byte_count = i;
  crc16 (modbus_tx_buf, 0x03 + byte_count);
  st_modbus_data.tx_length = 0x05 + byte_count;
  return EXCEPTION_NONE;
}
/**
 *******************************************************************************
 * @brief       func 0x05, write single coil
 * @param[in]
 * @param[in]
 * @param[out] application status true or false
 *******************************************************************************
 */
uint8_t write_single_coil_0x05(void)
{
  struct st_modbus_write_single_req_resp *ptr_req;
  struct st_modbus_write_single_req_resp *ptr_resp;
  uint16_t output_addr;
  uint32_t outputs;
  uint32_t resp_mask = 1;

  ptr_req = (struct st_modbus_write_single_req_resp*) modbus_rx_buf;
  ptr_resp = (struct st_modbus_write_single_req_resp*) modbus_tx_buf;

  ptr_resp->slave_add = st_modbus_data.slave_add;
  ptr_resp->function_code = FUNC_WRITE_SINGLE_COIL;
  ptr_resp->value = ptr_req->value;

  output_addr = word_endianer (ptr_req->address);
  if (output_addr >= MODBUS_INPUTS_QTY)
  {
    return OUT_OF_DATA_REGION;
  }
  outputs = read_gpio_outputs (0, 32);
  resp_mask = 1;
  resp_mask <<= output_addr;
  if (ptr_req->value == 0x00FF)
  { // COIL=ON FF00 changed to 00FF, endian problem
    outputs |= resp_mask;
    write_gpio_outputs (outputs);
  }
  else if (ptr_req->value == 0x0000)
  {
    outputs &= (0xFFFF - resp_mask);
    write_gpio_outputs (outputs);
  }
  else
  {
    return PROCESS_ERROR;
  }
  crc16 (modbus_tx_buf, sizeof(struct st_modbus_write_single_req_resp));
  st_modbus_data.tx_length = sizeof(struct st_modbus_write_single_req_resp);

//  PTR_WRITE_SINGLE_COIL_RESP->function_code = FUNC_WRITE_SINGLE_COIL;
//  PTR_WRITE_SINGLE_COIL_RESP->address.word = PTR_MODBUS_WRITE_SINGLE_REQ->address.word;
//  PTR_WRITE_SINGLE_COIL_RESP->value.word = PTR_MODBUS_WRITE_SINGLE_REQ->value.word;
//  if (PTR_MODBUS_WRITE_SINGLE_REQ->value.word == 0x00FF)
//  { //COIL=ON FF00 chanded to 00FF, endian problem
//    u32_mf_tmp = 1UL;
//    u32_mf_tmp <<= word_endianer (PTR_MODBUS_WRITE_SINGLE_REQ->address.word);
//    out = read_gpio_outputs (0, 8);
//    out |= u32_mf_tmp;
//    write_gpio_outputs (out);
//  }
//  else if (PTR_MODBUS_WRITE_SINGLE_REQ->value.word == 0x0000)
//  { //COIL=OFF
//    u32_mf_tmp = 1UL;
//    u32_mf_tmp <<= word_endianer (PTR_MODBUS_WRITE_SINGLE_REQ->address.word);
//    out = read_gpio_outputs (0, 8);
//    out &= ~u32_mf_tmp;
//    write_gpio_outputs (out);
//  }
//  else
//  {
//    return false;
//  }
//
//  crc16 (mbTxRxData.ptrTxData, sizeof(MODBUS_WRITE_SINGLE_REQUEST_FRAME) - 2);
//  mbTxRxData.txLength = sizeof(MODBUS_WRITE_SINGLE_REQUEST_FRAME);
  return EXCEPTION_NONE;
}

/**
 *******************************************************************************
 * @brief       func 0x06, write single holding registers
 * @param[in]
 * @param[in]
 * @param[out] application status true or false
 *******************************************************************************
 */
uint8_t write_single_register_0x06(void)
{
  struct st_modbus_write_single_req_resp *ptr_req;
  struct st_modbus_write_single_req_resp *ptr_resp;
  uint16_t reg_addr;
  uint16_t reg_value;
  uint16_t index;

  ptr_req = (struct st_modbus_write_single_req_resp*) modbus_rx_buf;
  ptr_resp = (struct st_modbus_write_single_req_resp*) modbus_tx_buf;

  ptr_resp->slave_add = st_modbus_data.slave_add;
  ptr_resp->function_code = FUNC_WRITE_SINGLE_REGISTER; //0x06
  ptr_resp->address = ptr_req->address;
  ptr_resp->value = ptr_req->value;
  reg_addr = word_endianer (ptr_req->address);
  reg_value = word_endianer (ptr_req->value);

  index = find_holding_register (reg_addr);
  if (index >= size_of_holding_register)
  {
    return OUT_OF_DATA_REGION;
  }
  //*st_holding_reg_array[index].ptr_u16 = reg_value;
  st_holding_reg_array[index].mb_write_function (reg_value);

  crc16 (modbus_tx_buf, 0x06);
  st_modbus_data.tx_length = 0x08;

  return EXCEPTION_NONE;
}

/**
 *******************************************************************************
 * @brief       func 0x0F, write multiple coils
 * @param[in]
 * @param[in]
 * @param[out] application status true or false
 *******************************************************************************
 */
uint8_t write_multiple_coils_0x0F(void)
{
  struct st_modbus_0x0F_req *ptr_req;
  struct st_modbus_0x0F_resp *ptr_resp;
  uint16_t start_addr;
  uint16_t qty_outputs;
  uint16_t byte_count;
  uint32_t mask = 1;
  uint32_t outputs;
  uint16_t i;

  ptr_req = (struct st_modbus_0x0F_req*) modbus_rx_buf;
  ptr_resp = (struct st_modbus_0x0F_resp*) modbus_tx_buf;

  ptr_resp->slave_add = st_modbus_data.slave_add;
  ptr_resp->function_code = FUNC_WRITE_MULTIPLE_COILS; //0x0F
  ptr_resp->start_address = ptr_req->start_address;
  ptr_resp->quantity = ptr_req->quantity;

  start_addr = word_endianer (ptr_req->start_address);
  qty_outputs = word_endianer (ptr_req->quantity);
  byte_count = ptr_req->byte_count;

  if (byte_count == 0)
  {
    return OUT_OF_DATA_REGION;
  }
  if (start_addr + qty_outputs > MODBUS_COILS_QTY)
  {
    return OUT_OF_DATA_REGION;
  }

  outputs = read_gpio_outputs (0, 32);
  for (i = 0; i < qty_outputs; i++)
  {
    mask <<= 1;
  }
  --mask;
  mask <<= start_addr;
  mask = ~mask;
  outputs &= mask;

  mask = 0;
  for (i = 0; i < byte_count; i++)
  {
    mask <<= 8;
    mask &= 0xFFFFFF00;
    mask |= ptr_req->outputs_value[i];
  }
  mask <<= start_addr;
  outputs |= mask;
  write_gpio_outputs (outputs);

  crc16 (modbus_tx_buf, 6);
  st_modbus_data.tx_length = 8;
  return EXCEPTION_NONE;
}

/**
 *******************************************************************************
 * @brief       func 0x10, write holding registers
 * @param[in]
 * @param[in]
 * @param[out] application status true or false
 *******************************************************************************
 */
uint8_t write_holding_register_0x10(void)
{
  struct st_modbus_0x10_req *ptr_req;
  struct st_modbus_0x10_resp *ptr_resp;
  uint16_t start_addr;
  uint16_t qty_registers;
  UINT16_TYPE u16_type_tmp;
  uint16_t index;
  uint8_t *ptr_u8;

  ptr_req = (struct st_modbus_0x10_req*) modbus_rx_buf;
  ptr_resp = (struct st_modbus_0x10_resp*) modbus_tx_buf;

  ptr_resp->slave_add = st_modbus_data.slave_add;
  ptr_resp->function_code = FUNC_WRITE_MULTIPLE_REGISTERS; //0x10
  ptr_resp->start_address = ptr_req->start_address;
  ptr_resp->quantity = ptr_req->quantity;

  ptr_u8 = (uint8_t*) &(ptr_req->value);

  start_addr = word_endianer (ptr_req->start_address);
  qty_registers = ptr_req->quantity;

  if (qty_registers == 0)
  {
    return OUT_OF_DATA_REGION;
  }
  while (qty_registers--)
  {
    index = find_holding_register (start_addr);
    if (index >= size_of_holding_register)
    {
      return OUT_OF_DATA_REGION;
    }
    u16_type_tmp.bytes.high_byte = *ptr_u8++;
    u16_type_tmp.bytes.low_byte = *ptr_u8++;
    st_holding_reg_array[index].mb_write_function (u16_type_tmp.word);
    ++start_addr;
  }

  crc16 (modbus_tx_buf, 0x06);
  st_modbus_data.tx_length = 0x08;
  return EXCEPTION_NONE;
}

/**
 *******************************************************************************
 * @brief       prepare exception error message
 * @param[in]   exceptionCode
 * @param[out]  none
 *******************************************************************************
 */
void mb_return_exception(uint8_t exception_code)
{
  struct st_modbus_0x01_to_04_req *ptr_req;
  struct st_modbus_exc_resp *ptr_resp;

  ptr_req = (struct st_modbus_0x01_to_04_req*) modbus_rx_buf;
  ptr_resp = (struct st_modbus_exc_resp*) modbus_tx_buf;

  ptr_resp->slave_add = ptr_req->slave_add;
  ptr_resp->function_code = ptr_req->function_code + 0x80;
  ptr_resp->exceptionCode = exception_code;

  crc16 (modbus_tx_buf, 3);
  st_modbus_data.tx_length = 5;
}

/**
 *******************************************************************************
 * @brief       modbusRTU main code
 * @param[in]   none
 * @param[out]  none
 * @details     none.
 *******************************************************************************
 */
void modbus_rtu_app(void)
{
  struct st_modbus_0x01_to_04_req *ptr_req;
  uint16_t tmp;
  uint8_t return_code;

  st_modbus_data.ptr_rx = modbus_rx_buf;
  st_modbus_data.ptr_tx = modbus_tx_buf;
  st_modbus_data.tx_length = 0x00;

  ptr_req = (struct st_modbus_0x01_to_04_req*) modbus_rx_buf;
  if (st_modbus_data.rx_length < MB_MIN_REQUEST_FRAME_SIZE)
  { //return if there is not enough data
    return;
  }

  st_modbus_data.slave_add = 1;
  if (*st_modbus_data.ptr_rx == st_modbus_data.slave_add)
  {
    if (check_crc16 (st_modbus_data.ptr_rx, st_modbus_data.rx_length - 2) == false)
    {
      return;
    }

    //check function code and quantity
    switch (ptr_req->function_code) {
      case FUNC_READ_COILS:
      case FUNC_READ_DISCRETE_INPUTS:
	tmp = word_endianer (ptr_req->quantity);
	if (tmp == 0 || tmp > 0x07D0)
	{
	  mb_return_exception (OUT_OF_MB_LIMIT);
	  return;
	}
	break;
    }

    switch (ptr_req->function_code) {
      case FUNC_READ_COILS:
	return_code = read_coils_0x01 ();
	if (return_code > EXCEPTION_NONE)
	{
	  mb_return_exception (return_code);
	}
	break;
      case FUNC_READ_DISCRETE_INPUTS:
	return_code = read_discrete_inputs_0x02 ();
	if (return_code > EXCEPTION_NONE)
	{
	  mb_return_exception (return_code);
	}
	break;
      case FUNC_READ_HOLDING_REGISTERS:
	return_code = read_holding_register_0x03 ();
	if (return_code > EXCEPTION_NONE)
	{
	  mb_return_exception (return_code);
	}
	break;
      case FUNC_READ_INPUT_REGISTERS:
	return_code = read_input_registers_0x04 ();
	if (return_code > EXCEPTION_NONE)
	{
	  mb_return_exception (return_code);
	}
	break;
      case FUNC_WRITE_SINGLE_COIL:
	return_code = write_single_coil_0x05 ();
	if (return_code > EXCEPTION_NONE)
	{
	  mb_return_exception (return_code);
	}
	break;
      case FUNC_WRITE_SINGLE_REGISTER:
	return_code = write_single_register_0x06 ();
	if (return_code > EXCEPTION_NONE)
	{
	  mb_return_exception (return_code);
	}
	break;
      case FUNC_WRITE_MULTIPLE_COILS:
	return_code = write_multiple_coils_0x0F ();
	if (return_code > EXCEPTION_NONE)
	{
	  mb_return_exception (return_code);
	}
	break;
      case FUNC_WRITE_MULTIPLE_REGISTERS:
	return_code = write_holding_register_0x10 ();
	if (return_code > EXCEPTION_NONE)
	{
	  mb_return_exception (return_code);
	}
	break;
      default:
	mb_return_exception (FUNCTION_UNSUPPORTED);
	break;
    }
  }
}
/* * * END OF FILE * * */
