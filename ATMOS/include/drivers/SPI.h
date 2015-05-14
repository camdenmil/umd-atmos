/*
 * SPI.h
 *
 * Created: 4/28/2015 16:40:21
 *  Author: Camden
 */ 


#ifndef SPI_H_
#define SPI_H_


void SPI_SlaveInit(void);
char SPI_SlaveReceive(void);
void SPI_SlaveTransmit(char d);


#endif /* SPI_H_ */