/**
 * \file K30.h
 *
 * \brief K30 Interface Library
 *
 * Created: 04/02/2015 18:04:55
 *  Author: Hui Shi
 */ 
 
//#include <stdbool.h>
#include <stdint.h>

#define K30_ADDR			0x68 // 7-bit address
#define K30_I2_RAM_ADDR		0x20
#define K30_I2_EEPROM_ADDR	0x00

#define SHIFTED_DIVISOR	0x988000
 
/** @nam Status Codes
 */
#define K30_WRITE			0xD0
#define K30_READ_RAM		0x22
#define K30_RAM_HI_BYTE		0x08
#define K30_RAM_LO_BYTE		0x09

//char K30_init();
//static char K30_readCO2(unsigned char *, char);
int K30_readCO2();
