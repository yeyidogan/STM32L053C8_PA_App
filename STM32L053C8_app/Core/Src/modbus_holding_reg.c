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
uint16_t read_dummy_func (uint16_t val)
{
  return val;
}
uint16_t write_dummy_func (uint16_t val)
{
  return val;
}

uint16_t read_adc_val (uint16_t channel)
{
  return (0x1000 + channel); //sample value
}
/**
 *******************************************************************************
 * @brief       modbus holding register list
 *******************************************************************************
 */
const struct struct_modbus_holding_registers st_holding_reg_array[] = {
//40001
    { 0x0000, read_adc_val, write_dummy_func },
    //40002
    { 0x0001, read_adc_val, write_dummy_func },
    //40003
    { 0x0002, read_adc_val, write_dummy_func },
    //40004
    { 0x0003, read_adc_val, write_dummy_func },
    //40005
    { 0x0004, read_adc_val, write_dummy_func },
    //40006
    { 0x0005, read_adc_val, write_dummy_func },
    //40006
    { 0x0006, read_adc_val, write_dummy_func },
    //40007
    { 0x0007, read_adc_val, write_dummy_func },
    //40008
    { 0x0008, read_adc_val, write_dummy_func },
    //40009
    { 0x0009, read_adc_val, write_dummy_func },
    //40010
    { 0x000A, read_adc_val, write_dummy_func },
    //40011
    { 0x000B, read_adc_val, write_dummy_func, },
    //40012
    { 0x000C, read_adc_val, write_dummy_func, }, };

const uint16_t size_of_holding_register = sizeof(st_holding_reg_array)
    / sizeof(struct struct_modbus_holding_registers);
/* * * END OF FILE * * */
