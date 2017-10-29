#include "Fx2.h"
#include "fx2regs.h"
#include "LED_Key.h"

#include "iic_comm.h"
#include "ssd1306.h"

#include "esp8266.h"
#include "MQTTPACKET.H"	
#include "string.h"

#include "DHT11.H"

BYTE ScanKey();
 
extern unsigned char temp_dataT[70];
extern unsigned char count;
unsigned char Lig=1;
unsigned char CountineCheck=0;

void main(void)
{  
	/*****************************************
 	* Write to slave device with
 	* slave address e.g. say 0x20
 	*****************************************/
	//unsigned char ack;
	//MQTTClient_connectOptions pdata a;

	int len=0;
	int i=0;
	int Light1=0;

	OED=0xff;
	OEB=0xff;
	OEE=0x0f;
	Delay(1000);
	PD5=1;	//PD5 is the display
	//PD7=0;   //PD7 is channel 1; 0 is off; 1 is on;
	LEDFlash();
	
	I2CInit();
	ssd1306_initalize();
Restart0:	
	ssd1306_clear();
	ssd1306_printf0();
	ssd1306_printf1("  Temperature & ");
	ssd1306_printf1("    Humidity    ");
	//ssd1306_printf1("----------------");
	ssd1306_printf1("     2017-10    ");
	ssd1306_printf1("     by John    ");
	//Delay(5000);
	read_dht11_dat();
	Delay(5000);
	goto Restart0;
		   

	//LEDKeyTest();
}
