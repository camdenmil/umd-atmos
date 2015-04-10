/**
 * \file BMP280.h
 *
 * \brief BMP280 Interface Library
 *
 * Created: 2/10/2015 20:25:27
 *  Author: Camden Miller
 */ 


#ifndef BMP280_H_
#define BMP280_H_

#include <stdbool.h>

#define BMP280_ADDR 0x76//!<Device address

/** @name TWI Status Codes
 *  All the codes that could be given via TWSR
 */
///@{
#define	BMP280_REG_CONTROL 0xF4
#define	BMP280_REG_RESULT_PRESSURE 0xF7/**<@brief 0xF7(msb) , 0xF8(lsb) , 0xF9(xlsb) : stores the pressure data.*/
#define BMP280_REG_RESULT_TEMPRERATURE 0xFA	/**<@brief 0xFA(msb) , 0xFB(lsb) , 0xFC(xlsb) : stores the temperature data. */
///@}

/** @name Command Macros
 *  Measurement commands to issue to BMP280_REG_CONTROL
 */
///@{
#define	BMP280_COMMAND_TEMPERATURE 0x2E
#define	BMP280_COMMAND_PRESSURE0 0x25
#define	BMP280_COMMAND_PRESSURE1 0x29
#define	BMP280_COMMAND_PRESSURE2 0x2D
#define	BMP280_COMMAND_PRESSURE3 0x31
#define	BMP280_COMMAND_PRESSURE4 0x5D
///@}

//Prototypes//
char BMP280_Init(void);
char BMP280_GetUnPT(double *, double *);
short BMP280_GetOversampling(void);
char BMP280_SetOversampling(short oss);
char BMP280_StartMeasurment(void);
char BMP280_GetUnPT(double *, double *);
char BMP280_GetTemperatureAndPressure(double *,double *);
char BMP280_CalcTemperature(double *, double *);
char BMP280_CalcPressure(double *,double *);
double BMP280_Sealevel(double , double);
double BMP280_Altitude(double , double);
char BMP280_GetError(void);

//static functions are prototyped in BMP280.c as they're only intended for use there

#endif /* BMP280_H_ */