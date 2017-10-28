#include "Fx2.h"
#include "fx2regs.h"
#include "string.h"

#include "LED_Key.h"

#include "ssd1306.h"

#include "DHT11.h"

int dht11_dat[5] = { 0, 0, 0, 0, 0 };

void SSDShow(unsigned char i)
{
	unsigned char h,l;
	h= (i>>4)&0x0f;
	l=	i&0x0f;
	if (h<0x0a)
		ssd1306_printchar(h+0x30);
	else
		ssd1306_printchar(h+0x57);
	if (l<0x0a)
		ssd1306_printchar(l+0x30);
	else
		ssd1306_printchar(l+0x57);	
}
 
void read_dht11_dat()
{
	 unsigned char i,j;
	 unsigned char a,b,c,d,e,f;
	 a=b=c=d=e=f=0;
	 OED=0xff;
	 DHT=0;
	 Delay(20);
	 DHT=1;
	 OED=0x00;
	 while(DHT==1)
	 	;
	 while(DHT==0)
	 	;
	 while(DHT==1)
	 	;
	for (i=0;i<5;i++)
	{	
		for(j=0;j<8;j++)
		{
			a=0;
			while(DHT==0);
			while(DHT==1)
				a++;
			if (a>0x0f)
			   dht11_dat[i]|=(0x80>>j);
		}
	 } 
	 
	 SSDShow(dht11_dat[0]);
	 ssd1306_printchar('-');
	 SSDShow(dht11_dat[1]);
	 ssd1306_printchar('-');
	 SSDShow(dht11_dat[2]);
	 ssd1306_printchar('-');
	 SSDShow(dht11_dat[3]);
	 ssd1306_printchar('-');
	 ssd1306_printchar('-');
	 ssd1306_printchar('-');
	 ssd1306_printchar('-');
	 ssd1306_printchar('-');

	 ssd1306_printf1("----------------");	 
	 ssd1306_printf1("Humidity:");
	 ssd1306_printchar(dht11_dat[0]/10+0x30);
	 ssd1306_printchar((dht11_dat[0]%10)+0x30);
	 ssd1306_printchar('%');
	 ssd1306_printf1("    ");
	 ssd1306_printf1("Temperat:");
	 ssd1306_printchar(dht11_dat[2]/10+0x30);
	 ssd1306_printchar((dht11_dat[2]%10)+0x30);
	 ssd1306_printchar('C');
	 //SSDShow(dht11_dat[0]);
}
				