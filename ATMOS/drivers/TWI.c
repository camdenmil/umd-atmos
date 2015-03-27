/*
 * TWI.c
 *
 * Created: 2/7/2015 21:12:00
 *  Author: Camden Miller
 */ 

////////////////////////////////////////////
//Library code for TWI operating as master//
////////////////////////////////////////////

#include "drivers/TWI.h"
#include <avr/io.h>
#include <stdbool.h>

/*
TWI_Init
-----------
Function: Initializes the 2-wire Serial Interface as a master
Args:
	unsigned long - SCL frequency (Do not exceed 400kHz)
Returns:
	char - status (0 on failure, nonzero otherwise)
*/
char TWI_Init(unsigned long freq){
	TWBR=(unsigned char)(((F_CPU/freq)-16UL)/8UL); //Compute settings value and set frequency
	TWCR=(1 << TWEN);
	
	TWAR=0; //Disable slave mode
	
	return 1; //May want to actually check for success here
}

/*
TWI_BeginWrite
-----------
Function: Starts a transmission over the TWI Bus
Args:
	unsigned char - Address of device to send to
Returns:
	char - status code, TWI_SLAW_ACK on success, other code otherwise
*/
char TWI_BeginWrite(unsigned char address){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //Send a start bit
	while ((TWCR & (1<<TWINT)) == 0);
	
	char stat = (TWSR & TWSR_MASK);
	if(stat!=TWI_START && stat!=TWI_REPEAT_START)return TWSR & TWSR_MASK; //Check for success
	
	TWDR = (address<<1) | SLAW_MASK; //set bit 0 to send a write bit
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	
	return TWSR & TWSR_MASK;//Return the status code
}

/*
TWI_BeginRead
-----------
Function: Starts a read over the TWI Bus
Args:
	unsigned char - Address of device to read from
Returns:
	char - status code, TWI_SLAR_ACK on success, other code otherwise
*/
char TWI_BeginRead(unsigned char address){
	TWCR = (0<<TWINT)|(1<<TWSTA)|(1<<TWEN); //Send a start bit
	while ((TWCR & (1<<TWINT)) == 0);
	
	char stat = (TWSR & TWSR_MASK);
	if(stat!=TWI_START && stat!=TWI_REPEAT_START)return TWSR & TWSR_MASK; //Check for success
	
	TWDR = (address<<1) & SLAR_MASK; //reset bit 0 to send a read bit
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	
	return TWSR & TWSR_MASK;//Return the status code
}

/*
TWI_WriteByte
-----------
Function: Sends a byte over TWI
Args:
	unsigned char - byte to send
Returns:
	char - status code, TWI_SENT_ACK or TWI_SENT_NACK on success, be sure to check for ACK or NACK
*/
char TWI_WriteByte(unsigned char data){
	TWDR = data; //Stage the data to send
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);//Send the data
	
	return TWSR&TWSR_MASK;//Return the status code
}

/*
TWI_ReadAck
-----------
Function: Receives a byte over TWI and returns an ACK
Args:
	unsigned char *- pointer to a byte to store the received byte in
Returns:
	char - status code, TWI_REC_ACK on success, other code otherwise
*/
char TWI_ReadAck(unsigned char *data){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); //Start receiving, ending with an ACK
	while ((TWCR & (1<<TWINT)) == 0);
	*data=TWDR; //Store the data
	
	return TWSR&TWSR_MASK;//Return the status code
}

/*
TWI_ReadNack
-----------
Function: Receives a byte over TWI and returns a NACK
Args:
	unsigned char *- pointer to a byte to store the received byte in
Returns:
	char - status code, TWI_REC_NACK on success, other code otherwise
*/
char TWI_ReadNack(unsigned char *data){
	TWCR = (1<<TWINT)|(1<<TWEN); //Start receiving, ending with a NACK
	while ((TWCR & (1<<TWINT)) == 0);
	*data=TWDR; //Store the data
	
	return TWSR&TWSR_MASK;//Return the status code
}

/*
TWI_Stop
-----------
Function: Sends a Stop bit, be sure to send a Start bit before future interactions on the bus
Args:
	void
Returns:
	char - should always be nonzero
*/
char TWI_Stop(void){
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	return 1;
}

/*
TWI_Read
-----------
Function: Reads multiple bytes from the TWI bus, be sure to send a Start first
Args:
	unsigned char *- Pointer to an array to store the bytes
	int - number of bytes to receive
	bool - whether or not to send an ACK on the last byte, sends NACK if false
Returns:
	char - status code, TWI_REC_ACK or TWI_REC_NACK on success depending on function arguments
Note:
	Does not contain buffer overrun protection
*/
char TWI_Read(unsigned char *data, int amount, bool ack){
	char status=0;
	for(int i=0;i<amount;i++){
		if((!ack) && ((amount-1)==i)){
			status=TWI_ReadNack(&data[i]); //if a NACK as chosen, receive with a NACK
		}else{
			status=TWI_ReadAck(&data[i]); //If we're not at the last byte yet (or NACK not selected) receive with an ACK
		}
		if(status!=TWI_REC_ACK)return status; //If NACK was selected, returning here is fine since the loop is already ending
		//will return if something goes wrong partway through
	}
	return status;
}

/*
TWI_Write
-----------
Function: Writes multiple bytes to the TWI bus, be sure to send a Start first
Args:
	unsigned char *- Pointer to an array of bytes to send
	int - number of bytes to send
Returns:
	char - status code, TWI_REC_ACK or TWI_REC_NACK on success, if you don't expect a NACK, be sure to check for it anyway
Note:
	Does not contain buffer overrun protection, will return if a NACK is received partway through
*/
char TWI_Write(unsigned char *data, int amount){
	char status=0;
	for(int i=0;i<amount;i++){
		status=TWI_WriteByte(data[i]); //If we're not at the last byte yet (or NACK not selected) receive with an ACK
		if(status!=TWI_SENT_ACK)return status; //If NACK is received on the last byte, returning here is still success
		//will return if something goes wrong partway through
	}
	return status;
}