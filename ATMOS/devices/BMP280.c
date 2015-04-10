/**
 * \file BMP280.c
 *
 * \brief BMP280 Pressure Sensor Library implementation
 * 
 * This library is intended only to work on the REV1 base station, check BMP280.h for address defines and the like
 *
 * Created: 2/10/2015 20:24:55
 *  Author: Camden Miller
 */ 

//Includes//
#include "devices/BMP280.h"
#include "drivers/TWI.h"
#include "drivers/usart0.h"
#include "math.h"

static int dig_T2 , dig_T3 , dig_P2 , dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9; //!<Calibration values from the BMP280
static unsigned int dig_P1,dig_T1 ; //!<Calibration values from the BMP280

static short oversampling, oversampling_t; //!<Oversampling sertings
static long signed int t_fine; 
static char error, status; //1<Error and status codes

//static function prototypes//
static char BMP280_ReadInt(char, int *);
static char BMP280_ReadUInt(char, unsigned int *);
static char BMP280_ReadBytes(unsigned char *, char);
static char BMP280_WriteBytes(unsigned char *, char);

/*************************************************************************//**
  @brief Initializes the BMP280 and reads the calibration data from the device
  @return status (zero on failure, nonzero otherwise)
*****************************************************************************/
char BMP280_Init(void){
	// Initialize the TWI library at 200kHz
	//TWI_Init(200000);

	// The BMP280 includes factory calibration data stored on the device.
	// Each device has different numbers, these must be retrieved and
	// used in the calculations when taking measurements.

	// Retrieve calibration data from device:
	
	if (    
		BMP280_ReadUInt(0x88, &dig_T1) &&
		BMP280_ReadInt(0x8A, &dig_T2)  &&
		BMP280_ReadInt(0x8C, &dig_T3)  &&
		BMP280_ReadUInt(0x8E, &dig_P1) &&
		BMP280_ReadInt(0x90, &dig_P2)  &&
		BMP280_ReadInt(0x92, &dig_P3)  &&
		BMP280_ReadInt(0x94, &dig_P4)  &&
		BMP280_ReadInt(0x96, &dig_P5)  &&
		BMP280_ReadInt(0x98, &dig_P6)  &&
		BMP280_ReadInt(0x9A, &dig_P7)  &&
		BMP280_ReadInt(0x9C, &dig_P8)  &&
		BMP280_ReadInt(0x9E, &dig_P9)){
			printf("\nT: %i ,%i ,%i P: %i ,%i ,%i ,%i ,%i ,%i ,%i ,%1 ,%i \n",dig_T1,dig_T2,dig_T3,dig_P1,dig_P2,dig_P3,dig_P4,dig_P5,dig_P6,dig_P7,dig_P8,dig_P9);
		return (1);
	}
	else 
		return (0);
}

/*************************************************************************//**
  @brief Reads an int from the BMP280
  @param[in] address The register address of the first byte of the uint
  @param[out] val* A pointer to a uint to store the received data to
  @return status (zero on failure, nonzero otherwise)
*****************************************************************************/
static char BMP280_ReadInt(char address, int *val){
	//printf("\nBMP280_ReadInt");
	unsigned char data[2];	//char is 4 bits, 1 byte

	data[0] = address;
	if (BMP280_ReadBytes(&data[0],2)){
		*val = (((int)data[1]<<8)|(int)data[0]);
		return(1);
	}
	*val = 0;
	return(0);
}

/*************************************************************************//**
  @brief Reads an unsigned int from the BMP280
  @param[in] address The register address of the first byte of the uint
  @param[out] val* A pointer to a uint to store the received data to
  @return status (zero on failure, nonzero otherwise)
*****************************************************************************/
static char BMP280_ReadUInt(char address, unsigned int *val){
	//printf("\nBMP280_ReadUInt");
	unsigned char data[2];	//4 bits
	
	data[0] = address;
	if (BMP280_ReadBytes(&data[0],2)){
		*val = (((unsigned int)data[1]<<8)|(unsigned int)data[0]);
		return(1);
	}
	*val = 0;
	return(0);
}

/*************************************************************************//**
Has no buffer overrun protection
  @brief Reads some bytes from the BMP280
  @param[in] *values pointer to an array to store the bytes, put the starting register in values[0]
  @param[in] length how many bytes to read
  @return status (zero on failure, nonzero otherwise)
*****************************************************************************/
static char BMP280_ReadBytes(unsigned char *values, char length){
	//printf("BMP280_ReadBytes");
	//printf("0x%1x",(unsigned)TWI_BeginWrite(BMP280_ADDR));
	TWI_BeginWrite(BMP280_ADDR);
	//printf("0x%1x",(unsigned)TWI_WriteByte(values[0]));
	TWI_WriteByte(values[0]);//Write the register address
	status = TWI_BeginRead(BMP280_ADDR); //Send a repeated start
	//printf("0x%1x",status);
	if (status == TWI_SLAR_ACK){
		if(((TWI_Read(&values[0],length,false)&TWSR_MASK) == TWI_REC_NACK) && (TWI_Stop() != 0)) return(1); //Receive bytes, send a STOP bit, and check for success
	}
	return(0);
}

/*************************************************************************//**
Has no buffer overrun protection
  @brief Reads some bytes from the BMP280
  @param[in] *values pointer to an array to send, put the starting register in values[0]
  @param[in] length how many bytes to write (including the starting register ie. sizeof(values))
  @return status (zero on failure, nonzero otherwise)
*****************************************************************************/
static char BMP280_WriteBytes(unsigned char *values, char length){
	TWI_BeginWrite(BMP280_ADDR);
	if((TWI_Write(&values[0],length) == TWI_SENT_ACK) && (TWI_Stop() != 0)) return 1; //Send bytes, a STOP bit, and check for success
	return(0);
}

/*************************************************************************//**
  @brief Gets the oversampling setting for the library
  @return oversampling
*****************************************************************************/
short BMP280_GetOversampling(void){
	return oversampling;
}

/*************************************************************************//**
  @brief Sets the oversampling setting for the library
  @param[in] oss Oversampling setting
  @return 1
*****************************************************************************/
char BMP280_SetOversampling(short oss){
	oversampling = oss;
	return (1);
}

/*************************************************************************//**
  @brief Starts a measurement
  @return time to wait for result (in ms)
*****************************************************************************/
char BMP280_StartMeasurment(void){
	unsigned char data[2], result, delay;
	
	data[0] = BMP280_REG_CONTROL;

	switch (oversampling){
		case 0:
		data[1] = BMP280_COMMAND_PRESSURE0;
		oversampling_t = 1;
		delay = 8;
		break;
		case 1:
		data[1] = BMP280_COMMAND_PRESSURE1;
		oversampling_t = 1;
		delay = 10;
		break;
		case 2:
		data[1] = BMP280_COMMAND_PRESSURE2;
		oversampling_t = 1;
		delay = 15;
		break;
		case 3:
		data[1] = BMP280_COMMAND_PRESSURE3;
		oversampling_t = 1;
		delay = 24;
		break;
		case 4:
		data[1] = BMP280_COMMAND_PRESSURE4;
		oversampling_t = 1;
		delay = 45;
		break;
		default:
		data[1] = BMP280_COMMAND_PRESSURE0;
		delay = 9;
		break;
	}
	result = BMP280_WriteBytes(&data[0], 2);
	if (result) // good write?
	return(delay); // return the delay in ms (rounded up) to wait before retrieving data
	else
	return(0); // or return 0 if there was a problem communicating with the BMP
}

/*************************************************************************//**
  @brief Gets the uncalibrated temperature and pressure data
  @param[out] pointer to a place to store the pressure data
  @param[out] pointer to a place to store the temperature data
  @return status
*****************************************************************************/
char BMP280_GetUnPT(double *uP, double *uT){
	unsigned char data[6];
	char result;
	
	data[0] = BMP280_REG_RESULT_PRESSURE; //0xF7

	result = BMP280_ReadBytes(&data[0], 6); // 0xF7; xF8, 0xF9, 0xFA, 0xFB, 0xFC
	if (result){ // good read
		double factor = pow(2, 4);
		*uP = (( (data[0] *256.0) + data[1] + (data[2]/256.0))) * factor ;	//20bit UP
		*uT = (( (data[3] *256.0) + data[4] + (data[5]/256.0))) * factor ;	//20bit UT
		
	}
	return(result);
}

/*************************************************************************//**
  @brief Gets temperature and pressure
  @param[out] pointer to a place to store the pressure data
  @param[out] pointer to a place to store the temperature data
  @return status
*****************************************************************************/
char BMP280_GetTemperatureAndPressure(double *T,double *P){
	double uP,uT ;
	char result = BMP280_GetUnPT(&uP,&uT);
	if(result!=0){
		// calculate the temperature
		result = BMP280_CalcTemperature(T,&uT);
		if(result){
			// calculate the pressure
			result = BMP280_CalcPressure(P,&uP);
			if(result)return (1);
			else error = 3 ;	// pressure error ;
			return (0);
		}else
		error = 2;	// temperature error ;
	}
	else
	error = 1;
	
	return (0);
}

/*************************************************************************//**
  @brief Calculates temperature
  @param[out] pointer to a place to store the temperature data
  @param[in] pointer to the uncalibrated temperature data
  @return status
*****************************************************************************/
char BMP280_CalcTemperature(double *T, double *uT){
	double adc_T = *uT ;
	//printf("adc_T = %d",adc_T);
	
	double var1 = (((double)adc_T)/16384.0-((double)dig_T1)/1024.0)*((double)dig_T2);
	double var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0)*(((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0))*((double)dig_T3);
	t_fine = (long signed int)(var1+var2);
	
	*T = (var1+var2)/5120.0;
	
	if(*T>100.0 || *T <(-100.0))return 0;
	
	return (1);
}

/*************************************************************************//**
  @brief Calculates pressure
  @param[out] pointer to a place to store the pressure data
  @param[in] pointer to the uncalibrated pressure data
  @return status
*****************************************************************************/
char BMP280_CalcPressure(double *P,double *uP){
	//char result;
	double var1 , var2 ;
	
	var1 = ((double)t_fine/2.0) - 64000.0;
	//Serial.print("var1 = ");Serial.println(var1,2);
	var2 = var1 * (var1 * ((double)dig_P6)/32768.0);	//not overflow
	//Serial.print("var2 = ");Serial.println(var2,2);
	var2 = var2 + (var1 * ((double)dig_P5)*2.0);	//overflow
	//Serial.print("var2 = ");Serial.println(var2,2);
	
	var2 = (var2/4.0)+(((double)dig_P4)*65536.0);
	//Serial.print("var2 = ");Serial.println(var2,2);
	
	var1 = (((double)dig_P3) * var1 * var1/524288.0 + ((double)dig_P2) * var1) / 524288.0;
	//Serial.print("var1 = ");Serial.println(var1,2);
	
	
	//Serial.print("(32768.0 + var1) = ");Serial.println((32768.0 + var1),5);
	
	//double t_var = (32768.0 + var1)/32768.0;
	//Serial.print("((32768.0 + var1)/32768.0) = "); Serial.println(t_var,5);
	//Serial.print("dig_P1 = ");Serial.println(dig_P1);
	//Serial.print("dig_P1 = ");Serial.println((double)dig_P1,5);
	//double tt_var = t_var * (double)dig_P1;
	
	//Serial.print("mulipication = "); Serial.println(tt_var,5);
	
	var1 = ((32768.0 + var1)/32768.0)*((double)dig_P1);
	//Serial.print("var1 = ");Serial.println(var1,2);
	
	double p = 1048576.0- (double)*uP;
	//Serial.print("p = ");Serial.println(p,2);
	
	p = (p-(var2/4096.0))*6250.0/var1 ;	//overflow
	//Serial.print("p = ");Serial.println(p,2);
	
	var1 = ((double)dig_P9)*p*p/2147483648.0;	//overflow
	
	var2 = p*((double)dig_P8)/32768.0;
	//Serial.print("var1 = ");Serial.println(var1,2);
	p = p + (var1+var2+((double)dig_P7))/16.0;
	//Serial.print("p = ");Serial.println(p,2);
	
	*P = p/100.0 ;
	
	if((*P>1200.0) || (*P < 800.0))return (0);
	return (1);
}

/*************************************************************************//**
  @brief Calculates pressure at sea level given an altitude
  @param[in] pressure reading
  @param[in] altitude
  @return the corrected reading
*****************************************************************************/
double BMP280_Sealevel(double P, double A){
// Given a pressure P (mb) taken at a specific altitude (meters),
// return the equivalent pressure (mb) at sea level.
// This produces pressure readings that can be used for weather measurements.
	return(P/pow(1-(A/44330.0),5.255));
}

/*************************************************************************//**
  @brief Calculates altitude
  @param[in] pressure reading
  @param[in] sea level pressure
  @return the corrected reading
*****************************************************************************/
double BMP280_Altitude(double P, double P0){
// Given a pressure measurement P (mb) and the pressure at a baseline P0 (mb),
// return altitude (meters) above baseline.
	return(44330.0*(1-pow(P/P0,1/5.255)));
}

/*************************************************************************//**
  @brief Returns the internal library error value
  @param[in] pressure reading
  @param[in] sea level pressure
  @return error value
*****************************************************************************/
char BMP280_GetError(void){
// If any library command fails, you can retrieve an extended
// error code using this command. Errors are from the wire library:
// 0 = Success
// 1 = Data too long to fit in transmit buffer
// 2 = Received NACK on transmit of address
// 3 = Received NACK on transmit of data
// 4 = Other error
	return(error);
}