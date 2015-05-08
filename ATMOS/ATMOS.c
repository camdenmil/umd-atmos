
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
#include "devices/TGS2600.h"
#include "devices/K30.h"
#include "common.h"

unsigned char data[3];
char dummy;
float humidity, temp2;
float temp3, resistance;
int co2;
double T,P;

void my_delay_ms(int n) {
	while(n--) {
		_delay_ms(1);
	}
}

static void APP_Init(void){
	PWR_Init();
	PWR_TurnOn5V();
	USART0_Init(9600);
	DDRB |= 0b00010000;
	TWI_Init(10000);
	ADC_Init();
	TGS2600_Init();
	Si7020_init();
	printf("BMP280 Status %i\n", BMP280_Init());
	BMP280_SetOversampling(4);
}

static void APP_TaskHandler(void)
{
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
