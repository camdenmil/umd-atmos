/*
 * BMP280.h
 *
 * Created: 2/10/2015 20:25:27
 *  Author: Camden Miller
 */ 


#ifndef BMP280_H_
#define BMP280_H_

#include <stdbool.h>

#define BMP280_ADDR 0x76 // 7-bit address

#define	BMP280_REG_CONTROL 0xF4
#define	BMP280_REG_RESULT_PRESSURE 0xF7			// 0xF7(msb) , 0xF8(lsb) , 0xF9(xlsb) : stores the pressure data.
#define BMP280_REG_RESULT_TEMPRERATURE 0xFA		// 0xFA(msb) , 0xFB(lsb) , 0xFC(xlsb) : stores the temperature data.

#define	BMP280_COMMAND_TEMPERATURE 0x2E
#define	BMP280_COMMAND_PRESSURE0 0x25
#define	BMP280_COMMAND_PRESSURE1 0x29
#define	BMP280_COMMAND_PRESSURE2 0x2D
#define	BMP280_COMMAND_PRESSURE3 0x31
#define	BMP280_COMMAND_PRESSURE4 0x5D

char BMP280_Init(void);
static char BMP280_ReadInt(char, int *);
static char BMP280_ReadUInt(char, unsigned int *);
static char BMP280_ReadBytes(unsigned char *, char);
static char BMP280_WriteBytes(unsigned char *, char);
char BMP280_GetUnPT(double *, double *);
short BMP280_GetOversampling(void);
char BMP280_SetOversampling(short oss);
char BMP280_StartMeasurment(void);
char BMP280_GetUnPT(double *, double *);
char BMP280_GetTemperatureAndPressure(double *,double *);
char BMP280_CalcTemperature(double *, double *);
char BMP280_CalcPressure(double *,double);
double BMP280_Sealevel(double , double);
double BMP280_Altitude(double , double);
char BMP280_GetError(void);

#endif /* BMP280_H_ */