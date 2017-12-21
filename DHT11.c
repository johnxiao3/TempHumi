#include "Fx2.h"
#include "fx2regs.h"
#include "string.h"

#include "LED_Key.h"

#include "ssd1306.h"

#include "DHT11.h"

unsigned char dht11_dat[5] = { 0, 0, 0, 0, 0 };

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
	 unsigned char a;
	 dht11_dat[0]=0;
	 dht11_dat[1]=0;
	 dht11_dat[2]=0;
	 dht11_dat[3]=0;
	 dht11_dat[4]=0;
	 a=0;
Resignal:
	 OED=0xff;	  //DHT PD4
	 DHT=0;
	 Delay(20);
	 DHT=1;
	 OED=0xef;

	ES0 = 0;//stop the serial break

	 while((DHT==1)&&(a<254))
	 	a++;
	 if (a>=254) goto Resignal; 
	 a=0;
	 while((DHT==0)&&(a<254))
	 	a++;
	if (a>=254) goto Resignal;
	 while((DHT==1)&&(a<254))
	 	a++;
	if (a>=254) goto Resignal;
	a=0;
	for (i=0;i<5;i++)
	{	
		for(j=0;j<8;j++)
		{
			a=0;
			while(DHT==0);
			while(DHT==1)
				a++;
			if (a>0x1f)
			   dht11_dat[i]|=(0x80>>j);
		}
	 } 
	 ES0 = 1;//stop the serial break
	 /*ssd1306_clear();
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
	 ssd1306_printf1("Humidity:");	  */
	 ssd1306_clear();
	 ssd1306_printchar(dht11_dat[0]/10+0x30);
	 ssd1306_printchar((dht11_dat[0]%10)+0x30);
	 ssd1306_printchar('%');
	 ssd1306_printf1(" ");
	 //ssd1306_printf1("Temperat:");
	 ssd1306_printchar(dht11_dat[2]/10+0x30);
	 ssd1306_printchar((dht11_dat[2]%10)+0x30);
	 ssd1306_printchar('C'); 
	 //SSDShow(dht11_dat[0]);
}
				