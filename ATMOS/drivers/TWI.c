/**
 * \file TWI.c
 * 
 * \brief TWI hardware interface library implementation in master mode
 *
 * Created: 2/7/2015 21:12:00
 *  Author: Camden Miller
 */ 

#include "drivers/TWI.h"
#include "drivers/usart0.h"
#include <avr/io.h>
#include <stdbool.h>
#include <avr/delay.h>

/*************************************************************************//**
  @brief Initializes the 2-wire Serial Interface as a master
  @param[in] freq SCL frequency (Do not exceed 400kHz)
  @return Status (@c 0 on failure, nonzero otherwise)
*****************************************************************************/
char TWI_Init(unsigned long freq){
	//printf("TWI_Init");
	TWSR=0x00;
	TWBR=(unsigned char)(((F_CPU/freq)-16UL)/8UL); //Compute settings value and set frequency
	TWCR=(1 << TWEN);
	
	//TWAR=0; //Disable slave mode
	
	return 1; //May want to actually check for success here
}

/*************************************************************************//**
  @brief Starts a transmission over the TWI Bus
  @param[in] address Address of device to send to
  @return Status code, TWI_SLAW_ACK on success, other code otherwise
*****************************************************************************/
char TWI_BeginWrite(unsigned char address){
	//printf("TWI_BeginWrite");
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWEA); //Send a start bit
	int i=0;
	while ((TWCR & (1<<TWINT)) == 0);
	_delay_ms(1);
	char stat = (TWSR & TWSR_MASK);
	if(stat!=TWI_START && stat!=TWI_REPEAT_START)return TWSR & TWSR_MASK; //Check for success
	
	stat = TWI_WriteByte((address<<1) & SLAW_MASK); //reset bit 0 to send a write bit, send
	//printf("0x%1x",(unsigned)stat);
	return stat;//Return the status code
}

/*************************************************************************//**
  @brief Starts a transmission over the TWI Bus
  @param[in] address Address of device to send to
  @return Status code, TWI_SLAR_ACK on success, other code otherwise
*****************************************************************************/
char TWI_BeginRead(unsigned char address){
	//printf("TWI_BeginRead");
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWEA); //Send a start bit
	while ((TWCR & (1<<TWINT)) == 0);
	
	char stat = (TWSR & TWSR_MASK);
	//printf("0x%1x",(unsigned)stat);
	if(stat!=TWI_START && stat!=TWI_REPEAT_START)return TWSR & TWSR_MASK; //Check for success
	
	stat = TWI_WriteByte((address<<1) | SLAR_MASK); //set bit 0 to send a read bit and send it
	//printf("0x%1x",(unsigned)stat);
	
	return stat;//Return the status code
}

/*************************************************************************//**
When checking the status code, bear in mind that it could be an ACK or a NACK
depending on what the slave is expecting/doing.
  @brief Sends a byte over TWI
  @param[in] data byte to send
  @return Status code, TWI_SENT_ACK or TWI_SENT_NACK on success
*****************************************************************************/
char TWI_WriteByte(unsigned char data){
	//printf("TWI_WriteByte");
	TWDR = data; //Stage the data to send
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);//Send the data
	
	return TWSR&TWSR_MASK;//Return the status code
}

/*************************************************************************//**
  @brief Receives a byte over TWI and returns an ACK
  @param[out] *data pointer to a byte to store the received byte in
  @return Status code, TWI_REC_ACK on success, other code otherwise
*****************************************************************************/
char TWI_ReadAck(unsigned char *data){
	//printf("TWI_ReadAck");
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); //Start receiving, ending with an ACK
	while ((TWCR & (1<<TWINT)) == 0);
	*data=TWDR; //Store the data
	
	return TWSR&TWSR_MASK;//Return the status code
}

/*************************************************************************//**
  @brief Receives a byte over TWI and returns a NACK
  @param[out] *data pointer to a byte to store the received byte in
  @return Status code, TWI_REC_NACK on success, other code otherwise
*****************************************************************************/
char TWI_ReadNack(unsigned char *data){
	//printf("TWI_ReadNack");
	TWCR = (1<<TWINT)|(1<<TWEN); //Start receiving, ending with a NACK
	while ((TWCR & (1<<TWINT)) == 0);
	*data=TWDR; //Store the data
	
	return TWSR&TWSR_MASK;//Return the status code
}

/*************************************************************************//**
  @brief Sends a Stop bit, be sure to send a Start bit before future interactions on the bus
  @return Status code, should always be nonzero
*****************************************************************************/
char TWI_Stop(void){
	//printf("TWI_Stop");
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	return 1;
}

/*************************************************************************//**
When using this function, be sure to send a START first; this only automates calls to TWI_ReadAck and/or TWI_ReadNack.
<p>Does not contain buffer overrun protection.
  @brief Reads multiple bytes from the TWI bus
  @param[out] *data Pointer to an array to store the bytes
  @param[in] amount Number of bytes to recieve
  @param[in] whether or not to send an ACK on the last byte, sends NACK if false
  @return Status code, TWI_REC_ACK or TWI_REC_NACK on success depending on function arguments
  @see TWI_BeginRead
*****************************************************************************/
char TWI_Read(unsigned char *data, int amount, bool ack){
	//printf("TWI_Read");
	char status=0;
	for(int i=0;i<amount;i++){
		if((ack==false) && ((amount-1)==i)){
			status=TWI_ReadNack(&data[i]); //if a NACK as chosen, receive with a NACK
			//printf("i=%i,0x%1x",i,(unsigned)status);
		}else{
			status=TWI_ReadAck(&data[i]); //If we're not at the last byte yet (or NACK not selected) receive with an ACK
			//printf("i=%i,0x%1x",i,(unsigned)status);
		}
		//printf("A");
		if(status!=TWI_REC_ACK)return status; //If NACK was selected, returning here is fine since the loop is already ending
		//will return if something goes wrong partway through
		//printf("B");
	}
	//printf("0x%1x",(unsigned)status);
	return status;
}

/*************************************************************************//**
When using this function, be sure to send a START first; this only automates calls to TWI_Write.
<p>Does not contain buffer overrun protection. Function will return if a NACK is received partway through. 
If you don't expect a NACK, be sure to check for it anyway.
  @brief Writes multiple bytes to the TWI bus
  @param[in] *data Pointer to an array of bytes to send
  @param[in] amount Number of bytes to send
  @return Status code, TWI_REC_ACK or TWI_REC_NACK on success
  @see TWI_BeginWrite
*****************************************************************************/
char TWI_Write(unsigned char *data, int amount){
	//printf("TWI_Write");
	char status=0;
	for(int i=0;i<amount;i++){
		status=TWI_WriteByte(data[i]); //If we're not at the last byte yet (or NACK not selected) receive with an ACK
		//printf("0x%1x",(unsigned)status);
		if(status!=TWI_SENT_ACK)return status; //If NACK is received on the last byte, returning here is still success
		//will return if something goes wrong partway through
	}
	return status;
}