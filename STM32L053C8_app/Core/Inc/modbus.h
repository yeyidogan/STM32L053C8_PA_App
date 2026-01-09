/******************************************************************************
 *	Written by Yilmaz Eyidogan
 *	Modbus declarations
 *	Created date: 2025.11.16
 *******************************************************************************/
#ifndef __MODBUS_H
#define __MODBUS_H

#include <stdint.h>
#include <stdbool.h>
#include "util.h"
#include "modbus_holding_reg.h"

/* Private typedef */
#define MODBUS_INPUTS_QTY 18 //18 input //UP TO 32
#define MODBUS_COILS_QTY 30 //30 output
/* Private typedef */

typedef union
{
  uint16_t word;
  struct
  {
    uint8_t low_byte;
    uint8_t high_byte;
  } bytes;
} UINT16_TYPE;

struct struct_modbus_data
{
  uint8_t *ptr_tx;
  uint8_t *ptr_rx;
  uint8_t tx_length;
  uint8_t rx_length;
  uint8_t slave_add;
};

struct st_modbus_0x01_to_04_req
{
  uint8_t slave_add;
  uint8_t function_code;
  uint16_t start_address;
  uint16_t quantity;
} __attribute__((packed));
//0x01, 0x02, 0x03, 0x04 functions

struct st_modbus_io_status_resp
{
  uint8_t slave_add;
  uint8_t function_code;
  uint8_t byte_count;
  uint8_t data[60];
} __attribute__((packed));

struct st_modbus_write_single_req_resp
{
  uint8_t slave_add;
  uint8_t function_code;
  uint16_t address;
  uint16_t value;
} __attribute__((packed));
//0x05 and 0x06 functions

struct st_modbus_write_multiple_coils_req
{
  uint8_t slave_add;
  uint8_t function_code;
  uint16_t start_address;
  uint16_t quantity;
  uint8_t byte_count;
  uint8_t data[60];
} __attribute__((packed));
//0x0F function

struct st_modbus_0x10_req
{
  uint8_t slave_add;
  uint8_t function_code;
  uint16_t start_address;
  uint16_t quantity;
  uint8_t byte_count;
  uint8_t value[60];
} __attribute__((packed));

struct st_modbus_0x10_resp
{
  uint8_t slave_add;
  uint8_t function_code;
  uint16_t start_address;
  uint16_t quantity;
  uint16_t crc;
} __attribute__((packed));
//0x10 function

struct st_modbus_0x0F_req
{
  uint8_t slave_add;
  uint8_t function_code;
  uint16_t start_address;
  uint16_t quantity;
  uint8_t byte_count;
  uint8_t outputs_value[60];
} __attribute__((packed));
//0x0F function
struct st_modbus_0x0F_resp
{
  uint8_t slave_add;
  uint8_t function_code;
  uint16_t start_address;
  uint16_t quantity;
} __attribute__((packed));
//0x0F function

struct st_modbus_exc_resp
{
  uint8_t slave_add;
  uint8_t function_code;
  uint8_t exceptionCode;
  //uint16_t crc;
} __attribute__((packed));

struct st_modbus_read_byte_resp
{
  uint8_t slave_add;
  uint8_t function_code;
  uint8_t byte_count;
  uint8_t byte;
} __attribute__((packed));

/* Private define */
#define MB_MIN_REQUEST_FRAME_SIZE 0x08

enum
{
  FUNC_READ_COILS = 1, //8 bytes request frame
  FUNC_READ_DISCRETE_INPUTS, //8 bytes request frame
  FUNC_READ_HOLDING_REGISTERS, //8 bytes request frame
  FUNC_READ_INPUT_REGISTERS, //8 bytes request frame
  FUNC_WRITE_SINGLE_COIL, //8 bytes request frame
  FUNC_WRITE_SINGLE_REGISTER, //8 bytes request frame
  FUNC_WRITE_MULTIPLE_COILS = 15, //11 bytes request frame
  FUNC_WRITE_MULTIPLE_REGISTERS, //13 bytes request frame
  FUNC_READ_WRITE_MULTIPLE_REGISTERS = 0x17
};
//MODBUS_FUNCTION_CODE

enum
{
  EXCEPTION_NONE = 0x00, FUNCTION_UNSUPPORTED, OUT_OF_DATA_REGION, OUT_OF_MB_LIMIT, PROCESS_ERROR
};
//MODBUS_EXCEPTION
/* Private macro */
/* Private variables */

extern struct struct_modbus_data st_modbus_data;

/* Private functions */
extern void modbus_rtu_app(void);
extern void crc16(uint8_t *ptr_Data, uint8_t length);

#endif
/* * * END OF FILE * * */
