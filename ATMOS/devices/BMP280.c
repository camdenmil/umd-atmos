/*
 * BMP280.c
 *
 * Created: 2/10/2015 20:24:55
 *  Author: Camden Miller
 */ 

//////////////////////////////////
//BMP280 Pressure Sensor Library//
//////////////////////////////////

//Note: This library is intended only to work on the REV1 base station, check BMP280.h for address defines and the like

#include "devices/BMP280.h"
#include "drivers/TWI.h"
#include <math.h>

static int dig_T2 , dig_T3 , dig_T4 , dig_P2 , dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
static unsigned int dig_P1,dig_T1 ;

static short oversampling, oversampling_t;
static long signed int t_fine;
static char error, status;

/*
BMP280_Init
-----------
Function: Initializes the BMP280 and reads the calibration data from the device
Args:
	void
Returns:
	int - status (0 on success, nonzero otherwise)
*/
char BMP280_Init(void){
	// Initialize the TWI library at 200kHz
	TWI_Init(200000);

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
		return (1);
	}
	else 
		return (0);
}

/*
static BMP280_ReadInt
-----------
Function: Reads an unsigned int from the BMP280
Args:
	char - The register address of the first byte of the uint
	unsigned int * - A pointer to a uint to store the received data to
Returns:
	char - status (0 on success, nonzero otherwise)
*/
static char BMP280_ReadInt(char address, int *val){
	unsigned char data[2];	//char is 4 bits, 1 byte

	data[0] = address;
	if (BMP280_ReadBytes(&data[0],2)){
		*val = (((int)data[1]<<8)|(int)data[0]);
		return(1);
	}
	*val = 0;
	return(0);
}

/*
static BMP280_ReadUInt
-----------
Function: Reads an unsigned int from the BMP280
Args:
	char - The register address of the first byte of the uint
	unsigned int * - A pointer to a uint to store the received data to
Returns:
	char - status (zero on failure, nonzero otherwise)
*/
static char BMP280_ReadUInt(char address, unsigned int *val){
	unsigned char data[2];	//4 bits
	data[0] = address;
	if (BMP280_ReadBytes(&data[0],2)){
		*val = (((unsigned int)data[1]<<8)|(unsigned int)data[0]);
		return(1);
	}
	*val = 0;
	return(0);
}


/*
static BMP280_ReadBytes
-----------
Function: Reads some bytes from the BMP280
Args:
	unsigned char * - pointer to an array to store the bytes, put the starting register in values[0]
	unsigned char - how many bytes to read
Returns:
	char - status (zero on failure, nonzero otherwise)
Note:
	Has no buffer overrun protection
*/
static char BMP280_ReadBytes(unsigned char *values, char length){
	TWI_BeginWrite(BMP280_ADDR);
	TWI_WriteByte(values[0]); //Write the register address
	status = TWI_BeginRead(BMP280_ADDR); //Send a repeated start
	if (status == TWI_SLAR_ACK){
		if((TWI_Read(&values[0],length,true) == TWI_REC_ACK) && (TWI_Stop() != 0)) return(1); //Receive bytes, send a STOP bit, and check for success
	}
	return(0);
}

/*
static BMP280_WriteBytes
-----------
Function: Writes some bytes to the BMP280
Args:
	unsigned char * - pointer to an array to send, put the starting register in values[0]
	unsigned char - how many bytes to write (including the starting register ie. sizeof(values))
Returns:
	char - status (zero on failure, nonzero otherwise)
Note:
	Has no buffer overrun protection
*/
static char BMP280_WriteBytes(unsigned char *values, char length){
	TWI_BeginWrite(BMP280_ADDR);
	if((TWI_Write(&values[0],length) == TWI_SENT_ACK) && (TWI_Stop() != 0)) return 1; //Send bytes, a STOP bit, and check for success
	return(0);
}

short BMP280_GetOversampling(void){
	return oversampling;
}

char BMP280_SetOversampling(short oss){
	oversampling = oss;
	return (1);
}

/*
char BMP280_StartMeasurement
-----------
Function: Starts a measurement
Args:
	void
Returns:
	char - time to wait for result (in ms)
*/
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
	result = BMP280_WriteBytes(data, 2);
	if (result) // good write?
	return(delay); // return the delay in ms (rounded up) to wait before retrieving data
	else
	return(0); // or return 0 if there was a problem communicating with the BMP
}

/*
char BMP280_GetUnPT
-----------
Function: Gets the uncalibrated temperature and pressure data
Args:
	double * - pointer to a place to store the pressure data
	double * - pointer to a place to store the temperature data
Returns:
	char - status
*/
char BMP280_GetUnPT(double *uP, double *uT){
	unsigned char data[6];
	char result;
	
	data[0] = BMP280_REG_RESULT_PRESSURE; //0xF7

	result = BMP280_ReadBytes(data, 6); // 0xF7; xF8, 0xF9, 0xFA, 0xFB, 0xFC
	if (result){ // good read
		double factor = pow(2, 4);
		*uP = (( (data[0] *256.0) + data[1] + (data[2]/256.0))) * factor ;	//20bit UP
		*uT = (( (data[3] *256.0) + data[4] + (data[5]/256.0))) * factor ;	//20bit UT
		
	}
	return(result);
}

/*
char BMP280_GetTemperatureAndPressure
-----------
Function: Gets temperature and pressure
Args:
	double * - pointer to a place to store the pressure data
	double * - pointer to a place to store the temperature data
Returns:
	char - status
*/
char BMP280_GetTemperatureAndPressure(double *T,double *P){
	double uP,uT ;
	char result = BMP280_GetUnPT(&uP,&uT);
	if(result!=0){
		// calculate the temperature
		result = BMP280_CalcTemperature(T,&uT);
		if(result){
			// calculate the pressure
			result = BMP280_CalcPressure(&P,uP);
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

/*
char BMP280_CalcTemperature
-----------
Function: Calculates temperature
Args:
	double * - pointer to a place to store the temperature data
	double * - pointer to the uncalibrated temperature data
Returns:
	char - status
*/
char BMP280_CalcTemperature(double *T, double *uT){
	double adc_T = *uT ;
	//Serial.print("adc_T = "); Serial.println(adc_T,DEC);
	
	double var1 = (((double)adc_T)/16384.0-((double)dig_T1)/1024.0)*((double)dig_T2);
	double var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0)*(((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0))*((double)dig_T3);
	t_fine = (long signed int)(var1+var2);
	
	*T = (var1+var2)/5120.0;
	
	if(*T>100.0 || *T <(-100.0))return 0;
	
	return (1);
}

/*
char BMP280_CalcPressure
-----------
Function: Calculates temperature
Args:
	double * - pointer to a place to store the pressure data
	double * - pointer to the uncalibrated pressure data
Returns:
	char - status
*/
char BMP280_CalcPressure(double *P,double uP){
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
	
	double t_var = (32768.0 + var1)/32768.0;
	//Serial.print("((32768.0 + var1)/32768.0) = "); Serial.println(t_var,5);
	//Serial.print("dig_P1 = ");Serial.println(dig_P1);
	//Serial.print("dig_P1 = ");Serial.println((double)dig_P1,5);
	double tt_var = t_var * (double)dig_P1;
	
	//Serial.print("mulipication = "); Serial.println(tt_var,5);
	
	var1 = ((32768.0 + var1)/32768.0)*((double)dig_P1);
	//Serial.print("var1 = ");Serial.println(var1,2);
	
	double p = 1048576.0- (double)uP;
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


/*
char BMP280_SeaLevel
-----------
Function: Calculates pressure at sea level given an altitude
Args:
	double - pressure reading
	double - altitude
Returns:
	double - the corrected reading
*/
double BMP280_Sealevel(double P, double A){
// Given a pressure P (mb) taken at a specific altitude (meters),
// return the equivalent pressure (mb) at sea level.
// This produces pressure readings that can be used for weather measurements.
	return(P/pow(1-(A/44330.0),5.255));
}

/*
char BMP280_Altitude
-----------
Function: Calculates altitude
Args:
	double - pressure reading
	double - sea level pressure
Returns:
	double - the corrected reading
*/
double BMP280_Altitude(double P, double P0){
// Given a pressure measurement P (mb) and the pressure at a baseline P0 (mb),
// return altitude (meters) above baseline.
	return(44330.0*(1-pow(P/P0,1/5.255)));
}

/*
char BMP280_GetError
-----------
Function: Returns the internal library error value
Args:
	void
Returns:
	char - error value
*/
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