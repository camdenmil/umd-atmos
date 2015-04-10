/**
 * \file TWI.h
 *
 * \brief TWI hardware interface in master mode
 *
 * Created: 2/7/2015 21:12:13
 *  Author: Camden
 */ 


#ifndef TWI_H_
#define TWI_H_

#include <stdbool.h>

//Status code defines


/** @name TWI Status Codes
 *  All the codes that could be given via TWSR
 */
///@{
#define TWI_BUS_ERROR		0x00 /**<@brief Bus error due to illegal START or STOP condition. */
#define TWI_START			0x08 /**<@brief A START condition has been transmitted. */
#define TWI_REPEAT_START	0x10 /**<@brief A repeated START condition has been transmitted. */
#define TWI_SLAW_ACK		0x18 /**<@brief SLA+W has been transmitted; ACK has been received. */
#define TWI_SLAW_NACK		0x20 /**<@brief SLA+W has been transmitted; NOT ACK has been received. */
#define TWI_SENT_ACK		0x28 /**<@brief Data byte has been transmitted; ACK has been received. */
#define TWI_SENT_NACK		0x30 /**<@brief Data byte has been transmitted; NOT ACK has been received. */
#define TWI_SLAW_LOST		0x38 /**<Arbitration lost in SLA+W or data bytes
(Transmitter); Arbitration lost in SLA+R or
NOT ACK bit (Receiver).
@brief Arbitration Lost */
#define TWI_SLAR_ACK		0x40 /**<@brief SLA+R has been transmitted; ACK has been received. */
#define TWI_SLAR_NACK		0x48 /**<@brief SLA+R has been transmitted; NOT ACK has been received. */
#define TWI_REC_ACK			0x50 /**<@brief Data byte has been received; ACK has been returned */
#define TWI_REC_NACK		0x58 /**<@brief Data byte has been received; NOT ACK has been returned. */
#define TWI_OWN_SLAW_ACK	0x60 /**<@brief Own SLA+W has been received; ACK has been returned. */
#define TWI_SLAR_LOST		0x68 /**<Arbitration lost in SLA+R/W as Master; own
SLA+W has been received; ACK has been
returned.
@brief Arbitration Lost */
#define TWI_GEN_CALL_ACK	0x70 /**<@brief General call address has been received; ACK has been returned. */
#define TWI_GEN_LOST_ACK	0x78 /**<Arbitration lost in SLA+R/W as Master;
general call address has been received;
ACK has been returned.
@brief Arbitration Lost */
#define TWI_SLA_REC_ACK		0x80 /**<@brief Previously addressed with own SLA+W; data has been received; ACK has been returned. */
#define TWI_SLA_REC_NACK	0x88 /**<@brief Previously addressed with own SLA+W; data has been received; NOT ACK has been returned. */
#define TWI_GEN_REC_ACK		0x90 /**<@brief Previously addressed with general call; data has been received; ACK has been returned. */
#define TWI_GEN_REC_NACK	0x98 /**<@brief Previously addressed with general call; data has been received; NOT ACK has been returned. */
#define TWI_SLA_STOP		0xA0 /**<@brief A STOP condition or repeated START condition has been received while still addressed as Slave. */
#define TWI_OWN_SLAR_ACK	0xA8 /**<@brief Own SLA+R has been received; ACK has been returned. */
#define TWI_O_SLAR_LOST_ACK	0xB0 /**<Arbitration lost in SLA+R/W as Master; own
SLA+R has been received; ACK has been
returned.
@brief Arbitration Lost */
#define TWI_TWDR_SENT_ACK	0xB8 /**<@brief Data byte in TWDR has been transmitted; ACK has been received. */
#define TWI_TWDR_SENT_NACK	0xC0 /**<@brief Data byte in TWDR has been transmitted; NO ACK has been received. */
#define TWI_LAST_TWDR_SENT	0xC8 /**<@brief Last data byte in TWDR has been transmitted (TWEA = 0); ACK has been received. */
#define TWI_NO_INFO			0xF8 /**<@brief No relevant state information available; TWINT = 0. */
///@}

/** @name TWI Masks
 * Mask off TWSR with 0xF8 (TWSR_MASK) to get status bits
 */
///@{
#define TWSR_MASK			0xF8 /**<@brief Mask off with TWSR to get status codes */
#define SLAW_MASK			0xFE /**<@brief AND i2c address with this to reset R/W bit */
#define SLAR_MASK			0x01 /**<@brief OR i2c address with this to set R/W bit */

///@}

//Prototypes//
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