/*
 * SPI.c
 *
 * Created: 4/28/2015 16:40:08
 *  Author: Camden
 */ 

#include <drivers/SPI.h>
#include <avr/io.h>

unsigned char buffer[16];

void SPI_SlaveInit(void)
{
	DDRB |= (1<<PORTB3); //Set MISO output
	SPCR = (1<<SPE); //Enable SPI
}
char SPI_SlaveReceive(void)
{
	while(!(SPSR & (1<<SPIF))); //Wait for reception complete
	
	return SPDR;
}
void SPI_SlaveTransmit(char d)
{
	char dummy;
	SPDR=d;
	while(!(SPSR & (1<<SPIF))); //Wait for reception complete
	
	dummy=SPDR;
	SPDR=dummy;
}