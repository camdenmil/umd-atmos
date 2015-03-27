/**
 * \file usart0.c
 *
 * \brief USART0 in UART mode implementation
 *
 * Created: 2/7/2015 18:35:59
 *  Author: Camden Miller
 *			York
 */ 

#include "drivers/usart0.h"
#include <avr/io.h>

/*************************************************************************//**
Calls USART0_Transmit which contains busy loops, not production code
  @brief Called when stdout is being printed to USART0 (this is set up at runtime)
  @param[in] var byte to transmit
  @param[in/out] *stream Pointer to a stdio.h FILE io stream
  @return Status
*****************************************************************************/
int USART0_putchar_printf(char var, FILE *stream) {
	if (var == '\n') USART0_Transmit('\r');
	USART0_Transmit(var);
	return 0;
}

/*************************************************************************//**
  @brief Initializes USART0 in asynchronous UART mode, 8 bit data, 1 stop bit
  @param[in] desired baud rate (configuration values computed using F_CPU)
*****************************************************************************/
void USART0_Init(unsigned long baud){
	
	stdout = &USART0_stdout; //make stdout print to USART0
	
	//Compute UBRR for baudrate setting
	unsigned short ubrr=(unsigned short)((F_CPU/(baud*16UL))-1);
	
	//Set BAUD rate.
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
	
	/*Set frame format. 8 data bit(settings bits UCSZ00 and UCSZ01  UCSR0B), 1 stop bit(by default and need not to be worried about). 
	The UCSZ01:0 bits combined with the UCSZ02 bit in UCSR0B sets the number of data
	bits (Character Size) in the frame that the Receiver and Transmitter use. 
	If a change on frame format is needed, make sure to modify UCSR0B register above accordingly.
	See datasheet pg. 390.*/
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
	
	//Enable receiver and transmitter. Enable the USART Receive Complete interrupt(RXCIE).
	//UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0));
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);//Enable RX/TX, no interrupts
}

/*************************************************************************//**
Function contains busy loops for simplicity in debugging. Not production code.
  @brief Transmits a byte via USART0
  @param[in] data byte to be transmitted
*****************************************************************************/
void USART0_Transmit(unsigned char data){
	while(!(UCSR0A & (1 << UDRE0))); //Wait for the empty transmit buffer. (Waiting UDR0 to be empty.)
		UDR0 = data;			//store the data in the USART Data Register
}

/*************************************************************************//**
Function contains busy loops for simplicity in debugging. Not production code.
  @brief Receives a byte via USART0
  @return the received byte
*****************************************************************************/
unsigned char USART_Receive(void){
	while (!(UCSR0A & (1 << RXC0))); //Do nothing until data have been received and is ready to be read from USART Data Register
	return UDR0;	//when data is ready to be received, return the content stored in the USART Data Register(UDR0.)
}

/*************************************************************************//**
Function contains busy loops for simplicity in debugging. Not production code.
  @brief Flushes out the USART0 receive buffer
*****************************************************************************/
void USART_Flush(void){
	unsigned char dummy;
	while(UCSR0A & (1 << RXC0))
		dummy = UDR0;
}

void ISR(unsigned int USART0_RX_vect){
	//Write to a buffer?
	
	/*
	//Echo serial debug code
	char data;
	data = UDR0;	//read the data from the buffer (USART Data register: UDR0.)
	UDR0 = data;	//output the data to the buffer (USART Data register: UDR0.)
	*/
	return;
}