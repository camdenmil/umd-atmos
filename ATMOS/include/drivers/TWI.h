/*
 * TWI.h
 *
 * Created: 2/7/2015 21:12:13
 *  Author: Camden
 */ 


#ifndef TWI_H_
#define TWI_H_

#include <stdbool.h>

//Status code defines
//Mask off TWSR with 0xF8 (TWSR_MASK) to get status bits

#define TWI_BUS_ERROR		0x00
#define TWI_START			0x08
#define TWI_REPEAT_START	0x10
#define TWI_SLAW_ACK		0x18
#define TWI_SLAW_NACK		0x20
#define TWI_SENT_ACK		0x28
#define TWI_SENT_NACK		0x30
#define TWI_SLAW_LOST		0x38
#define TWI_SLAR_ACK		0x40
#define TWI_SLAR_NACK		0x48
#define TWI_REC_ACK			0x50
#define TWI_REC_NACK		0x58
#define TWI_OWN_SLAW_ACK	0x60
#define TWI_SLAR_LOST		0x68
#define TWI_GEN_CALL_ACK	0x70
#define TWI_GEN_LOST_ACK	0x78
#define TWI_SLA_REC_ACK		0x80
#define TWI_SLA_REC_NACK	0x88
#define TWI_GEN_REC_ACK		0x90
#define TWI_GEN_REC_NACK	0x98
#define TWI_SLA_STOP		0xA0
#define TWI_OWN_SLAR_ACK	0xA8
#define TWI_O_SLAR_LOST_ACK	0xB0
#define TWI_TWDR_SENT_ACK	0xB8
#define TWI_TWDR_SENT_NACK	0xC0
#define TWI_LAST_TWDR_SENT	0xC8
#define TWI_NO_INFO			0xF8

//Useful Masks

#define TWSR_MASK			0xF8
#define SLAW_MASK			0x01
#define SLAR_MASK			0xFE

char TWI_Write(unsigned char *data, int amount);
char TWI_Read(unsigned char *data, int amount, bool ack);
char TWI_Stop(void);
char TWI_ReadNack(unsigned char *data);
char TWI_ReadAck(unsigned char *data);
char TWI_WriteByte(unsigned char data);
char TWI_BeginRead(unsigned char address);
char TWI_BeginWrite(unsigned char address);
char TWI_Init(unsigned long freq);

#endif /* TWI_H_ */