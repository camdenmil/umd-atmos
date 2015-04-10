/**
 * \file TGS2600.c
 *
 * \brief Figaro TGS2600 air quality sensor implementation
 *
 * Created: 4/10/2015 11:03:19
 *  Author: Camden
 */ 

#include "common.h"
#include "devices/TGS2600.h"
#include "drivers/ADC.h"
#include "drivers/PWR.h"

void TGS2600_Init(void){
	DDRB |= 0b00100000;
	TGS2600_TurnOn();
}

void TGS2600_TurnOn(void){
	PORTB |= 0b00100000;
}

void TGS2600_TurnOff(void){
	PORTB &= 0b11011111;
}

float TGS2600_GetResistance(void){
	float r;
	ADC_Reference(REFERENCE_AREF);
	r=(float)ADC_Convert(ADC1);
	r*=(1.8/1024);
	return ((10000*1.8)-(10000*r))/r;
}