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

typedef struct
{
  uint8_t *ptrTxData;
  uint8_t *ptrRxData;
  uint8_t txLength;
  uint8_t rxLength;
  uint8_t slave_add;
} MODBUS_TX_RX_DATA;

typedef struct
{
  uint8_t slave_add;
  uint8_t function_code;
  UINT16_TYPE start_address;
  UINT16_TYPE quantity;
  UINT16_TYPE crc;
} __attribute__((packed)) MODBUS_READ_REQUEST_FRAME;
struct st_modbus_0x01_to_04_req
{
  uint8_t slave_add;
  uint8_t function_code;
  UINT16_TYPE start_address;
  UINT16_TYPE quantity;
  UINT16_TYPE crc;
} __attribute__((packed));
//0x01, 0x02, 0x03, 0x04 functions

struct st_modbus_io_status_resp{
  uint8_t slave_add;
  uint8_t function_code;
  uint8_t byte_count;
  uint8_t data[60];
} __attribute__((packed));

typedef struct
{
  uint8_t slave_add;
  uint8_t function_code;
  UINT16_TYPE address;
  UINT16_TYPE value;
  UINT16_TYPE crc;
} __attribute__((packed)) MODBUS_WRITE_SINGLE_REQUEST_FRAME;
struct st_modbus_write_single_req_resp
{
  uint8_t slave_add;
  uint8_t function_code;
  UINT16_TYPE address;
  UINT16_TYPE value;
//  UINT16_TYPE crc;
} __attribute__((packed));
//0x05 and 0x06 functions

typedef struct
{
  uint8_t slave_add;
  uint8_t function_code;
  UINT16_TYPE start_address;
  UINT16_TYPE quantity;
  uint8_t byte_count;
  uint8_t value;
} __attribute__((packed)) MODBUS_WRITE_MULTIPLE_COILS_REQUEST_FRAME;
//0x0F function

typedef struct
{
  uint8_t slave_add;
  uint8_t function_code;
  UINT16_TYPE start_address;
  UINT16_TYPE quantity;
  uint8_t byte_count;
  UINT16_TYPE value;
} __attribute__((packed)) MODBUS_WRITE_MULTIPLE_REGISTERS_REQUEST_FRAME;

struct st_modbus_0x10_req
{
  uint8_t slave_add;
  uint8_t function_code;
  UINT16_TYPE start_address;
  UINT16_TYPE quantity;
  uint8_t byte_count;
  UINT16_TYPE value;
} __attribute__((packed));

struct st_modbus_0x10_resp
{
  uint8_t slave_add;
  uint8_t function_code;
  UINT16_TYPE start_address;
  UINT16_TYPE quantity;
  UINT16_TYPE crc;
} __attribute__((packed));
//0x10 function

struct st_modbus_exc_resp
{
  uint8_t slave_add;
  uint8_t function_code;
  uint8_t exceptionCode;
  UINT16_TYPE crc;
} __attribute__((packed));

typedef struct
{
  uint8_t slave_add;
  uint8_t function_code;
  uint8_t byte_count;
  uint8_t byte;
  UINT16_TYPE crc;
} __attribute__((packed)) MODBUS_READ_BYTE_RESPONSE_FRAME;
struct st_modbus_read_byte_resp
{
  uint8_t slave_add;
  uint8_t function_code;
  uint8_t byte_count;
  uint8_t byte;
} __attribute__((packed));

typedef struct
{
  uint8_t slave_add;
  uint8_t function_code;
  uint8_t byte_count;
  UINT16_TYPE word;
  UINT16_TYPE crc;
} __attribute__((packed)) MODBUS_READ_WORD_RESPONSE_FRAME;

/* Private define */
#define MB_MIN_REQUEST_FRAME_SIZE 0x08



//#define PTR_MODBUS_READ_REQ ((MODBUS_READ_REQUEST_FRAME *)mbTxRxData.ptrRxData)
#define PTR_MODBUS_WRITE_SINGLE_REQ ((MODBUS_WRITE_SINGLE_REQUEST_FRAME *)mbTxRxData.ptrRxData)
#define PTR_MODBUS_WRITE_MULTICOIL_REQ ((MODBUS_WRITE_MULTIPLE_COILS_REQUEST_FRAME *)mbTxRxData.ptrRxData)
#define PTR_MODBUS_WRITE_MULTIREGI_REQ ((MODBUS_WRITE_MULTIPLE_REGISTERS_REQUEST_FRAME *)mbTxRxData.ptrRxData)

#define PTR_READ_BYTE_RESP ((MODBUS_READ_BYTE_RESPONSE_FRAME *)mbTxRxData.ptrTxData)

#define PTR_WRITE_SINGLE_COIL_RESP ((MODBUS_WRITE_SINGLE_REQUEST_FRAME *)mbTxRxData.ptrTxData)

#define PTR_READ_HOLDING_REQ ((MODBUS_READ_WORD_RESPONSE_FRAME *)mbTxRxData.ptrTxData)

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

extern MODBUS_TX_RX_DATA mbTxRxData;
extern uint16_t uiWordQty;

/* Private functions */
extern void modbus_rtu_app(void);
extern void crc16(uint8_t *ptrCell, uint8_t length);

#endif
/* * * END OF FILE * * */
