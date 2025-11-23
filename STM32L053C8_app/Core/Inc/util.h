/******************************************************************************
*	Written by Yilmaz Eyidogan
*	utils.h
*	Created date: 2025.02.02
*******************************************************************************/
#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

/* define ------------------------------------------------------------*/
/* typedef -----------------------------------------------------------*/
/* macro -------------------------------------------------------------*/
/* variables ---------------------------------------------------------*/
/* typedef -----------------------------------------------------------*/
/* function ----------------------------------------------------------*/
extern uint16_t word_endianer(uint16_t wordIn);
extern uint32_t dword_endianer(uint32_t dword);
extern uint32_t subtraction_abs32(uint32_t ulVal1, uint32_t ulVal2);
extern uint32_t subtraction_abs32_signed(int32_t val1, int32_t val2);
extern void convert_hex_to_6byte_array(uint32_t val, uint8_t *ptr);
extern uint32_t convert_6byte_array_to_hex(uint8_t *ptr);
extern int32_t divisor32(int32_t val, int32_t divisor);
extern int32_t round32(int32_t val, int32_t rounder);
extern uint8_t uint8_to_ascii(uint8_t val);
extern uint8_t get_ascii_hex_byte(uint8_t *ptr);
extern uint32_t get_u32_from_string(uint8_t *ptr, uint8_t size_t);
extern uint8_t intel_hex_line_validate(uint8_t *ptr);

#endif

/* * * END OF FILE * * */
