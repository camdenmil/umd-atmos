/**
 * \file Si7020.h
 *
 * \brief Si7020 Interface Library
 *
 * Created: 3/27/2015 11:14:55
 *  Author: Hui Shi
 */ 
 
//#include <stdbool.h>
#include <stdint.h>
 
#define SI7020_ADDR 0x40 // 7-bit address

/** @nam Status Codes
 */
#define SI7020_REL_HUMIDITY_HOLD	0xE5
#define SI7020_REL_HUMIDITY_NONHOLD	0xF5
#define SI7020_TEMPERATURE_HOLD		0xE3
#define SI7020_TEMPERATURE_NOHOLD	0xF3
#define SI7020_RD_TEMP_PRE_RH		0xE0
#define SI7020_RESET				0xFE
#define SI7020_WR_RH_T_REG1			0xE6
#define SI7020_RD_RH_T_REG1			0xE7
#define	SI7020_WR_HEAT_CTR_REG		0x51
#define SI7020_RD_HEAT_CTR_REG		0x11
#define SI7020_ELCID_1ST_BYTE		0xFA //0x0F
#define SI7020_ELCID_2ND_BYTE		0xFC //0xC9	
#define SI7020_FIRMWARE_REV			0x84 //

#define SHIFTED_DIVISOR	0x988000

char Si7020_init();
char Si7020_readHumidity(unsigned char *, char);
char Si7020_readTemperature(unsigned char *, char);
float Si7020_calHumidity(unsigned char *);
float Si7020_calTemperature(unsigned char *);

//byte Si7020_check_crc(message, check_value)

//void setResolution(byte resBits);
//byte read_user_register(void);
//byte check_crc(uint16_t message_from_sensor, uint8_t check_value_from_sensor);
