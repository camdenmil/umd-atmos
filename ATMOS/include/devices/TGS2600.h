/**
 * \file TGS2600.h
 *
 * \brief Figaro TGS2600 air quality sensor libaray
 *
 * Created: 4/10/2015 11:03:35
 *  Author: Camden
 */ 


#ifndef TGS2600_H_
#define TGS2600_H_

//Prototypes//
void TGS2600_Init(void);
void TGS2600_TurnOn(void);
void TGS2600_TurnOff(void);
float TGS2600_GetResistance(void);


#endif /* TGS2600_H_ */