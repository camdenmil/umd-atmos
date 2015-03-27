/**
 * \file usart0.h
 *
 * \brief USART0 in UART mode
 *
 * Created: 2/7/2015 20:37:01
 *  Author: Camden Miller
 *			York
 */ 


#ifndef USART0_H_
#define USART0_H_

#include <stdio.h>

#ifndef F_CPU
#define F_CPU 16000000UL //F_CPU should be defined in compiler command line options
#endif /* F_CPU */

//Function Prototypes//
int USART0_putchar_printf(char var, FILE *stream);
void USART0_Init(unsigned long baud);
void USART0_Transmit(unsigned char data);
unsigned char USART_Receive(void);
void USART_Flush(void);

static FILE USART0_stdout = FDEV_SETUP_STREAM(USART0_putchar_printf, NULL, _FDEV_SETUP_WRITE); //FILE handle for stdout

#endif /* USART0_H_ */