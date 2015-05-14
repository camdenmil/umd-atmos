
#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include "devices/Si7020.h"
#include "devices/BMP280.h"
#include "drivers/usart0.h"
#include "drivers/TWI.h"
#include "drivers/PWR.h"
#include "drivers/ADC.h"
#include "drivers/SPI.h"
#include "devices/TGS2600.h"
#include "devices/K30.h"
#include "common.h"

unsigned char data[3];
char dummy;
float humidity, temp2;
float temp3, resistance;
int co2;
double T,P;
char buff[3] = {0,0,0};

char ref1[5]={'T','P','H','C',0};
char refT[4]={'1','2','3',0};
char refP[2]={'1',0};
char refH[2]={'1',0};
char refC[3]={'2','4',0};
	
char sendbuff[100];

void my_delay_ms(int n) {
	while(n--) {
		_delay_ms(1);
	}
}

char checkChar(char *ref, char test){
	int j=0;
	while((ref[j]!=test) && (ref[j]!=0)) j++;
	if(ref[j]==0) return 100;
	return j;
}

static void APP_Init(void){
	PWR_Init();
	PWR_TurnOn5V();
	USART0_Init(76800);
	DDRB |= 0b00010000;
	DDRE |= 0b00001000;
	PORTE|= 0b00001000;
	TWI_Init(10000);
	ADC_Init();
	TGS2600_Init();
	Si7020_init();
	printf("BMP280 Status %i\n", BMP280_Init());
	BMP280_SetOversampling(4);
	SPI_SlaveInit();
}

static void APP_TaskHandler(void)
{

	printf("Top of loop\n");
	//SPI Test
	int i;
	for(i=0;i<=1;i++){
		if((i==1) && (checkChar(&ref1[0],buff[0])==100))i=0; 
		buff[i]=SPI_SlaveReceive();
	}
	PORTB |= 0b00010000; //LED on
	printf("Received %s\n",buff);
	char result;
	switch(buff[0]){
		case 'T':
			//switch(checkChar(&refT[0], buff[1])){
			switch(buff[1]){
				case '1':
					result = BMP280_StartMeasurment();
					if(result!=0){
						my_delay_ms(result);
						result = BMP280_GetTemperatureAndPressure(&T,&P);
					}
					sprintf(&sendbuff[0],"%.3F\r",T);
					break;
				case '2':
					dummy = Si7020_readTemperature(&data[0],3);
					sprintf(&sendbuff[0],"%.3F\r",Si7020_calTemperature(&data[0]));
					break;
				case '3':
					sprintf(&sendbuff[0],"%.3F\r",ADC_DieTemp());
					break;
				default:
					sendbuff[0]='\r';
					break;
			}
			break;
		case 'P':
			//switch(checkChar(&refP[0],buff[1])){
			switch(buff[1]){
				case '1':
					result = BMP280_StartMeasurment();
					if(result!=0){
						my_delay_ms(result);
						result = BMP280_GetTemperatureAndPressure(&T,&P);
					}
					sprintf(&sendbuff[0],"%.3F\r",P);
					break;
				default:
					sendbuff[0]='\r';
					break;
			}
			break;
		case 'H':
			//switch(checkChar(&refH[0],buff[1])){
			switch(buff[1]){
				case '1':
					dummy = Si7020_readHumidity(&data[0],3);
					sprintf(&sendbuff[0],"%.3F\r",Si7020_calHumidity(&data[0]));
					break;
				default:
					sendbuff[0]='\r';
					break;
			}
			break;
		case 'C':
			//switch(checkChar(&refC[0],buff[1])){
			switch(buff[1]){
				case '2':
					sprintf(&sendbuff[0],"%i\r",K30_readCO2());
					break;
				case '4':
					sprintf(&sendbuff[0],"%.3F\r",TGS2600_GetResistance());		
					break;			
				default:
					sendbuff[0]='\r';
					break;
			}
			break;
		default:
			break;
	}
	printf("Sending SPI: \n%s\n",sendbuff);
	i=0;
	PORTE&=0b11110111;
	while(sendbuff[i]!='\r'){
		SPI_SlaveTransmit(sendbuff[i]);
		i++;
	}
	SPI_SlaveTransmit('\r');
	PORTE|=0b00001000;
	PORTB &= 0b11101111; //LED off
	
	/* Busy loop all-sensor test //
	PORTB |= 0b00010000; //LED on
	_delay_ms(1000);
	PORTB &= 0b11101111; //LED off
  _delay_ms(1000);
  
  char result = BMP280_StartMeasurment();
  //printf("Measure result: %i",result);
  if(result!=0){
	  my_delay_ms(result);
	  result = BMP280_GetTemperatureAndPressure(&T,&P);
	  if(result!=0){ 
		  printf("Pres:%.3F : Temp1:%.3F",P,T);
	  }
  }
  dummy = Si7020_readHumidity(&data[0],3);
  humidity = Si7020_calHumidity(&data[0]);
  dummy = Si7020_readTemperature(&data[0],3);
  temp2 = Si7020_calTemperature(&data[0]);
  temp3 = ADC_DieTemp();
  resistance=TGS2600_GetResistance();
  co2=K30_readCO2();
  //printf("Co2: %i\n",co2);
  printf(" : Hum %.3F : Temp2 %.3F : Temp3 %.3F : Res %.3F : C02 %i\n",humidity,temp2,temp3,resistance,co2);
  */
}

int main(void)
{
  //SYS_Init(); //Commented out until wireless hardware is tuned
  APP_Init();
  printf("\n======================\n");
  while (1)
  {
    //SYS_TaskHandler(); //Commented out until wireless hardware is tuned
    APP_TaskHandler();
  }
}
