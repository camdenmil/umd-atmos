/**
 * \file common.c
 *
 * \brief Common functions
 * 
 * Created: 4/10/2015 11:49:15
 *  Author: Camden
 */ 

#include "common.h"

/*************************************************************************//**
  @brief CRC check
  @return remainder
*****************************************************************************/
unsigned int check_crc(uint16_t message_from_sensor, uint8_t check_value_from_sensor) {
	uint32_t remainder = (uint32_t)message_from_sensor << 8; //Pad with 8 bits because we have to add in the check value
	remainder |= check_value_from_sensor; //Add on the check value

	uint32_t divsor = (uint32_t)SHIFTED_DIVISOR;

	for (int i = 0 ; i < 16 ; i++) //Operate on only 16 positions of max 24. The remaining 8 are our remainder and should be zero when we're done.
	{
		if( remainder & (uint32_t)1<<(23 - i) ) //Check if there is a one in the left position
		remainder ^= divsor;

		divsor >>= 1; //Rotate the divsor max 16 times so that we have 8 bits left of a remainder
	}

	return (unsigned int)remainder;
}