/**
 * \file ADC.c
 * 
 * \brief ADC library implementation
 *
 *  Author: Tony Zhang
 *			Camden Miller
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include "drivers/ADC.h"

/*************************************************************************//**
  @brief Initializes the ADC
*****************************************************************************/
void ADC_Init(void) {
	ADCSRA = (1<<ADEN)|(4<<ADPS0); //enables the ADC module, CPU clock prescale=16
	
	ADCSRC = 0x05|(1<<ADTHT0)|(1<<ADTHT1); //ADSUT=5
	while((ADCSRB&(1<<AVDDOK))==0);
}

void ADC_Reference(unsigned char ref){
	ADMUX &= 0b00111111;
	ADMUX |= ref;
}

int ADC_Convert(unsigned char channel) {
	int result =0;
	ADMUX &= 0b11000000;
	ADMUX |= channel;
	ADCSRA |= (1<<ADSC); //starts a single conversion

	while (ADCSRA & (1<<ADSC)); //new conversion completed when ADSC becomes 0

	return result | ((ADCL)|(ADCH<<8));
}

float ADC_DieTemp (void) {
	int t;
	/*ADC_Reference(REFERENCE_1_6V);
	t=ADC_Convert(ADC_TEMP);
	*/
	ADCSRC = 10<<ADSUT0; // set start-up time
	ADCSRB = 1<<MUX5; // set MUX5 first
	ADMUX = (3<<REFS0) + (9<<MUX0); // store new ADMUX, 1.6V AREF
	// switch ADC on, set prescaler, start conversion
	ADCSRA = (1<<ADEN) + (1<<ADSC) + (4<<ADPS0);
	return (1.13*(float)ADC)-272.8;
}

