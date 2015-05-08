/*
 * SPI.c
 *
 * Created: 4/28/2015 16:40:08
 *  Author: Camden
 */ 

#include <drivers/SPI.h>

unsigned char buffer[16];

void SPI_Init(){
	SPCR=(1<<SPE)|(1<<SPIE);
}

void SPI_Send(char data){
	SPDR=data;
}

char SPI_Receive(){
	return SPDR;
}

ISR SP