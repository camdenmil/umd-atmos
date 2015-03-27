
#define DISPC 7

//#define REFERENCE_1.8 //whatever mask goes here//

#include <avr/io.h>
#include <stdbool.h>

char ADC_init() {
	ADCSRA |= (1<<ADEN); //enables the ADC module

	ADMUX |= (1<<REFS1); //internal Vref 1.6 V
	ADMUX |= (1<<REFS0);

	ADCSRA |= (1<<ADPS2); //prescales the system clock by 16
	ADCSRA |= (0<<ADPS1);
	ADCSRA |= (0<<ADPS0);
	
	ADMUX = (1<<ADLAR); //ADLAR becomes 1 so it is a left-adjusted result

	ADMUX = (1<<MUX5)&(1<<MUX4)&(1<<MUX3)&(1<<MUX2)&(1<<MUX1)&(1<<MUX0); //selects ADC Channel 0 with single-ended output

	return 0;
}

char ADC_reference(unsigned char ref){
	ADMUX &= 0b00111111;
	ADMUX |= ref;

	return 0;
}

char ADC_convert(unsigned char *adch, unsigned char *adcl) {
	ADC_init();

	ADCSRA |= (1<<ADSC); //starts a single conversion

	while (!(ADCSRA<<ADIF)); //new conversion completed when ADIF becomes 1

	*adch = ADCH;
	*adcl = ADCL;

	return 0;
}

uint16_t ADC_die_temp (void) {
	ADCSRC = 1<<ADSUT0; // set start-up time
	ADCSRB = 1<<MUX5; // set MUX5 first
	ADMUX = (3<<REFS0) + (9<<MUX0); // store new ADMUX, 1.6V AREF
	// switch ADC on, set prescaler, start conversion
	ADCSRA = (1<<ADEN) + (1<<ADSC) + (4<<ADPS0);

	do {
		
	} while( (ADCSRA & (1<<ADSC))); // wait for conversion
	ADCSRA = 0; // disable the ADC
	return (ADC);
}


uint16_t ADC_sram_v_drtbbp (void) {
	DRTRAM2 = (1<<DISPC) & (1<<ENDRT); //SRAM DRT on
	ADCSRC = 1 <<ADSUT0; // set start-up time
	ADCSRB = 1<<MUX5; // set MUX5 first
	ADMUX = (3<<REFS0) + (20<<MUX0); // store new ADMUX, 1.6V AREF
	// switch ADC on, set prescaler, start conversion
	ADCSRA = (1<<ADEN) + (1<<ADSC) + (4<<ADPS0);

	do {
		
	} while( (ADCSRA & (1<<ADSC))); // wait for conversion
	ADCSRA = 0; // disable the ADC
	return (ADC);
}


uint16_t ADC_sram_v_drtbbn (void) {
	DRTRAM2 = (1<<DISPC) & (1<<ENDRT); //SRAM DRT on
	ADCSRC = 1 <<ADSUT0; // set start-up time
	ADCSRB = 1<<MUX5; // set MUX5 first
	ADMUX = (3<<REFS0) + (29<<MUX0); // store new ADMUX, 1.6V AREF
	// switch ADC on, set prescaler, start conversion
	ADCSRA = (1<<ADEN) + (1<<ADSC) + (4<<ADPS0);

	do {
		
	} while( (ADCSRA & (1<<ADSC))); // wait for conversion
	ADCSRA = 0; // disable the ADC
	return (ADC);
}