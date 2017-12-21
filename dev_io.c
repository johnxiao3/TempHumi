#include "Fx2.h"
#include "fx2regs.h"
#include "LED_Key.h"

#include "iic_comm.h"
#include "ssd1306.h"

#include "ESP8266.h"	
#include "string.h"

#include "DHT11.H"

BYTE ScanKey();
 
extern unsigned char temp_dataT[70];
extern unsigned char count;
extern unsigned char dht11_dat[5];

/*code char Publish[93]=
{0x30,0x5d,0x00,0x5a,0x76,0x31,0x2f,0x37,0x37,0x32,0x36,0x31,0x62,0x62,0x30,0x2d
,0x34,0x64,0x32,0x65,0x2d,0x31,0x31,0x65,0x37,0x2d,0x61,0x31,0x63,0x38,0x2d,0x30
,0x33,0x61,0x33,0x31,0x30,0x62,0x30,0x37,0x62,0x62,0x32,0x2f,0x74,0x68,0x69,0x6e
,0x67,0x73,0x2f,0x64,0x63,0x37,0x37,0x63,0x32,0x32,0x30,0x2d,0x34,0x64,0x38,0x33
,0x2d,0x31,0x31,0x65,0x37,0x2d,0x39,0x30,0x64,0x63,0x2d,0x63,0x64,0x36,0x33,0x39
,0x63,0x61,0x39,0x30,0x39,0x61,0x38,0x2f,0x64,0x61,0x74,0x61,0x2f};
void MQTTPublish0(char channel, char value)
{
	unsigned char len=0;
	for(len=0;len<93;len++)
	{
		Serial0_SendChar(Publish[len]);
	}
	Serial0_SendChar(channel+0x30);
	Serial0_SendChar(value+0x30);
}*/


code char clientID[]="dc77c220-4d83-11e7-90dc-cd639ca909a8";
code char username[]="77261bb0-4d2e-11e7-a1c8-03a310b07bb2";
code char password[]="6ecdc74199d4b43d7c01c4c640162b71ab50a77c";
void MQTTConnect()
{
	int len=0;
	Serial0_SendChar(0x10);//header
	len=12;
	len+=(strlen(clientID)+2);
	len+=(strlen(username)+2);
	len+=(strlen(password)+2);
	do{
		unsigned char SendByte=len%128;
		len=len/128;
		if(len>0)
		{
			SendByte|=0x80;
		}
		Serial0_SendChar(SendByte);
	}while (len>0);  //length

	Serial0_SendInt(6);//length of MQIsqd
	Serial0_SendString("MQIsdp");
	Serial0_SendChar(3);//version of MQTT
	Serial0_SendChar(0xc2);//flags
	Serial0_SendInt(60);//time interval
	
	Serial0_SendInt(strlen(clientID));
	Serial0_SendString(clientID);
	Serial0_SendInt(strlen(username));
	Serial0_SendString(username);
	Serial0_SendInt(strlen(password));
	Serial0_SendString(password);
	Delay(5000);
	ABC1Show();	
	Delay(5000);
}


void MQTTPublish(char channel, int value)
{
	int len=0;
	Serial0_SendChar(0x32);//header
	len=24;
	len+=(strlen(clientID));
	len+=(strlen(username));
	do{
		unsigned char SendByte=len%128;
		len=len/128;
		if(len>0)
		{
			SendByte|=0x80;
		}
		Serial0_SendChar(SendByte);
	}while (len>0);  //length

	Serial0_SendChar(0);
	Serial0_SendChar(0X5A);
	Serial0_SendString("v1/");
	Serial0_SendString(username);
	Serial0_SendString("/things/");
	Serial0_SendString(clientID);
	Serial0_SendString("/data/");
	Serial0_SendChar(channel+0x30);
	Serial0_SendChar(0x00);
	Serial0_SendChar(0x00);		
	Serial0_SendChar((unsigned char)(value/10)+0x30);
	Serial0_SendChar((value%10)+0x30);	
}

void exitSentMode()
{
	count=0;
	while(count==0)
	{
		Serial0_SendString("+++");
		Delay(3000);
	}
	ABCShow();
	Delay(3000);
}
void connectServer()
{
	Serial0_SendString("AT+CIPSTART=\"TCP\",\"mqtt.mydevices.com\",1883");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(10000);
	ABCShow();
	Delay(5000);
	ABCShow();
}
void Reset8266()
{
	Serial0_SendString("AT+RST");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(10000);
	ABCShow();
	Delay(15000);
	ABCShow();
}
void connectWifi()
{
	Serial0_SendString("AT+CWJAP=\"zhiyong_yang\",\"26996014\"");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(10000);
	ABCShow();
	Delay(10000);
	ABCShow();
}
void sentMode()
{
	Serial0_SendString("AT+CIPSEND");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(10000); 
	ABCShow();
}
void cipMode()
{
	Serial0_SendString("AT+CIPMODE=1");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(10000);
	ABCShow();
}

void main(void)
{  

	int len=0;
	int light=0;
	int i=0;

	OED=0xff;
	OEB=0xff;
	OEE=0x0f;
	Delay(1000);
	PD5=1;	//PD5 is the display
	//PD7=0;   //PD7 is channel 1; 0 is off; 1 is on;
	//LEDFlash();
	I2CInit();
	ssd1306_initalize();
	ssd1306_clear();
	ssd1306_printf("Temperatur2");
	
	Serial0_Init();

Restart:
	ssd1306_clear();
	ssd1306_printf("restart");

	//exitSentMode(); 
	//Reset8266();
	//connectWifi();
    //connectServer();
	//cipMode();
	//sentMode();
	MQTTConnect();
	ssd1306_clear();
	while(1){
		read_dht11_dat();	  
		MQTTPublish(1,dht11_dat[0]);
		MQTTPublish(2,dht11_dat[2]);
		if (light==0){
			MQTTPublish(3,0);
			light=1;
		}
		else
		{
			MQTTPublish(3,1);
			light=0;
		}
		Delay(3000);
		//ABC1Show();
		count=0;
	} 
}
				 