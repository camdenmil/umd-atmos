/**
 * \file ADC.h
 *
 * \brief ADC Library
 *
 * Created: 4/10/2015 00:19:53
 *  Author: Camden Miller
 */ 

#ifndef ADC_H_
#define ADC_H_

/** @name ADC Reference Selections
 *  All the refereneces that can be selected in ADMUX
 */
///@{
#define REFERENCE_AREF	0b00000000/**<@brief Reference the AREF pin*/
#define REFERENCE_AVDD	0b01000000/**<@brief Internal reference AVDD (1.8V)*/
#define REFERENCE_1_5V	0b10000000/**<@brief Internal reference 1.5V*/
#define REFERENCE_1_6V	0b11000000/**<@brief Internal reference 1.6V*/
///@}

/** @name ADC Input Channel Selections
 *  Single ended channel options
 */
///@{
#define ADC0		0b00000000/**<@brief ADC0 Pin*/
#define ADC1		0b00000001/**<@brief ADC1 Pin*/
#define ADC2		0b00000010/**<@brief ADC2 Pin*/
#define ADC3		0b00000011/**<@brief ADC3 Pin*/
#define ADC4		0b00000100/**<@brief ADC4 Pin*/
#define ADC5		0b00000101/**<@brief ADC5 Pin*/
#define ADC6		0b00000110/**<@brief ADC6 Pin*/
#define ADC7		0b00000111/**<@brief ADC7 Pin*/

#define ADC_VBG		0b00011110/**<@brief 1.2V internal bus(Vbg)*/
#define ADC_GND		0b00011111/**<@brief Analog ground (AVSS)*/
#define ADC_EVDD	0b00100110/**<@brief Internal digital power (EVDD)*/
#define ADC_TEMP	0b00101001/**<@brief Internal temperature sensor*/
#define ADC_VDRTBBP	0b00110100/**<@brief Positive SRAM back-bias*/
#define ADC_VDRTBBN	0b00111101/**<@brief Negative SRAM back-bias*/
///@}

//protoypes//

void ADC_Init(void);
void ADC_Reference(unsigned char ref);
int ADC_Convert(unsigned char channel);
float ADC_DieTemp (void);

#endif