

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

#include <gpio.h>

/*********** Define MACROs	*******************/
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64
#define true 1
#define false 0

#define gpioOUT 60
#define gpioIN  48

#define SCK_LOW gpio_set_value( gpioOUT, 0 );
#define SCK_HI gpio_set_value( gpioOUT, 1 );

#define isDout_ready	while( gpio_getValue( gpioIN ) );

/********** Declare Global varibales **************/
int8_t gpio_getValue(uint8_t gpio)
{
	
	int fd, len;
	char buf[MAX_BUF];
	int8_t value;
	
	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio );
	
	if( ( fd = open(buf, O_RDONLY ) ) < 0 )
	{
		printf("Error in file opeinging");
		return -1;		
	}
	
	
	read(fd, &value, 1 );
	
	close(fd);
	return (value-48);
}
int hx711_get_data()
{
	/* 1. reset condition
		SCK  -->> LOW
		Dout -->> HI		
		*/
	SCK_LOW
	isDout_ready
	usleep(2);	
	int ADC=0;
	for(uint8_t i=0; i < 25; i++)
	{
		
		SCK_HI
		usleep(2);
		ADC =  ( ADC  | ( gpio_getValue( gpioIN ) << i ) ); // MSB first
		SCK_LOW
		usleep(2);
		isDout_ready
			
	}
	
	printf("ADC -- > %x", ADC);
}


int main()
{
	
	printf(" This program is made for communication with ADC-HX711 \n");	
	gpio_export(gpioOUT);
	gpio_set_dir( gpioOUT, true);
	gpio_export(gpioIN);
	gpio_set_dir( gpioIN, false);
	
	while(1);
	
	while(1)
	{
		 hx711_get_data();
		 usleep(20);		
	}
	
	
	
	return 0;
}