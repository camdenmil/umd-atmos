/**
 * \file K30.c
 *
 * Created: 04/02/2015 18:01:55
 *  Author: Hui Shi
 */ 

//Includes// 
#include "include/devices/K30.h"
#include "include/drivers/TWI.h"
#include "avr/delay.h"
#include "drivers/usart0.h"
#include <math.h>

unsigned char readcmd[4]={0x22,0x00,0x08,0x2A};

/*************************************************************************//**
  @brief Initializes the K30
  @return status zero
*****************************************************************************/
/*char K30_init() {
	// Initialize the TWI library at 200kHz
	TWI_Init(200000);
	return 0;
}
*/

int K30_readCO2(){
	int val=0;
	int sum=0;
	
	char status;
	//printf("K30_readCO2");
	unsigned char buffer[4]={0,0,0,0};
	status=TWI_BeginWrite(0x68);
	if(status!=TWI_SLAW_ACK) return 0;
	status=TWI_Write(&readcmd[0],4);
	if(status!=TWI_SENT_ACK) return 0;
	TWI_Stop();
	_delay_ms(10);
	status=TWI_BeginRead(0x68);
	if(status!=TWI_SLAR_ACK) return 0;
	status=TWI_Read(&buffer[0],4,true);
	if(status!=TWI_REC_ACK) return 0;
	TWI_Stop();
	
	val = 0;
	val |= buffer[1] & 0xFF;
	val = val << 8;
	val |= buffer[2] & 0xFF;
	sum = buffer[0] + buffer[1] + buffer[2];
	if(sum==buffer[3]){return val;}
	else{return 0;}
	
}

/*************************************************************************//**
  @brief Read CO2 from K30
  @return status one if successfully read and crc check, otherwise return zero
*****************************************************************************/
/*
static char K30_readCO2(unsigned char *data, char length){
	char status;
	
	TWI_BeginWrite(K30_ADDR); 	
	status = TWI_WriteByte(K30_WRITE);	
	if(status != TWI_SENT_ACK)	return 0; //Error out
	
	status = TWI_BeginRead(K30_ADDR);
	if(status != TWI_SLAR_ACK)	return 0;
	
	status = TWI_Read(K30_RAM_HI_BYTE,3,false);
	if(status !=TWI_REC_ACK)	return 0;
	
	unsigned char msb = data[0];
	unsigned char lsb = data[1];
	unsigned char checksum = data[2];
	unsigned int rawCO2 = ((unsigned int) msb << 8) | (unsigned int) lsb;

	if(check_crc(rawCO2, checksum) != 0) return 0;
	
	TWI_Stop();
	return 1;
}
*/