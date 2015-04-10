/**
 * \file Si7020.c
 *
 * Created: 3/27/2015 11:14:55
 *  Author: Hui Shi
 */ 

//Includes// 
#include "include/devices/Si7020.h"
#include "include/drivers/TWI.h"
#include <math.h>

/*************************************************************************//**
  @brief Initializes the Si7020
  @return status zero
*****************************************************************************/
char Si7020_init() {
	// Initialize the TWI library at 200kHz
	//TWI_Init(200000);
	return 0;
}

/*************************************************************************//**
  @brief Read humidity from Si7020
  @return status one if successfully read and crc check, otherwise return zero
*****************************************************************************/
char Si7020_readHumidity(unsigned char *data, char length){
	char status;
	
	TWI_BeginWrite(SI7020_ADDR); 	
	status = TWI_WriteByte(SI7020_REL_HUMIDITY_HOLD);	
	if(status != TWI_SENT_ACK) return 0; //Error out
	
	status = TWI_BeginRead(SI7020_ADDR);
	if(status != TWI_SLAR_ACK)	return 0;
	
	status = TWI_Read(data,3,false);
	if(status !=TWI_REC_ACK)	return 0;
	
	unsigned char msb = data[0];
	unsigned char lsb = data[1];
	unsigned char checksum = data[2];
	unsigned int rawHumidity = ((unsigned int) msb << 8) | (unsigned int) lsb;
	if(check_crc(rawHumidity, checksum) != 0) return 0;
	
	TWI_Stop();
	return 1;
}

/*************************************************************************//**
  @brief Calculate humidity from Si7020
  @return humidity value
*****************************************************************************/
float Si7020_calHumidity(unsigned char *data) {
	unsigned char msb = data[0];
	unsigned char lsb = data[1];

	unsigned int rawHumidity = ((unsigned int) msb << 8) | (unsigned int) lsb;
	
	//Given the raw humidity data, calculate the actual relative humidity
	float tempRH = rawHumidity / (float)65536;
	float rh = 125 * tempRH -6;
	
	return rh;
}

/*************************************************************************//**
  @brief Read temperature from Si7020
  @return status one if successfully read and crc check, otherwise return zero
*****************************************************************************/
char Si7020_readTemperature(unsigned char *data, char length){
	char status;
	
	TWI_BeginWrite(SI7020_ADDR); 	
	status = TWI_WriteByte(SI7020_TEMPERATURE_HOLD);	
	if(status != TWI_SENT_ACK) return 0; //Error out
	
	status = TWI_BeginRead(SI7020_ADDR);
	if(status != TWI_SLAR_ACK)	return 0;
	
	status = TWI_Read(data,3,false);
	if(status !=TWI_REC_ACK)	return 0;
	
	unsigned char msb = data[0];
	unsigned char lsb = data[1];
	unsigned char checksum = data[2];
	unsigned int rawTemperature = ((unsigned int) msb << 8) | (unsigned int) lsb;
	if(check_crc(rawTemperature, checksum) != 0) return 0;
	
	TWI_Stop();
	return 1;
}

/*************************************************************************//**
  @brief Calculate temperature from Si7020
  @return temperature value
*****************************************************************************/
float Si7020_calTemperature(unsigned char *data) {
	unsigned char msb = data[0];
	unsigned char lsb = data[1];
	unsigned int rawTemperature = ((unsigned int) msb << 8) | (unsigned int) lsb;
	
	//Given the raw temperature data, calculate the actual temperature
	float tempTemperature = rawTemperature / (float)65536; 
	float realTemperature = 175.72 * tempTemperature - 46.85;
	
	return realTemperature;
}