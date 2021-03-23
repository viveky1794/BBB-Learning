#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include "beaglebone_gpio.h"

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
#include <sys/time.h>
#include <time.h>


struct timespec Ti, rTi; 
#define GPIO_DATAIN	0x138 

int main(int argc, char *argv[]) {
    volatile void *gpio_addr = NULL;
    volatile unsigned int *gpio_oe_addr = NULL;
    volatile unsigned int *gpio_setdataout_addr = NULL;
    volatile unsigned int *gpio_cleardataout_addr = NULL;
	volatile unsigned int *gpio_dataIN_addr = NULL;
    unsigned int reg;
  
  int fd = open("/dev/mem", O_RDWR);

    printf("Mapping %X - %X (size: %X)\n", GPIO1_START_ADDR, 
GPIO1_END_ADDR, GPIO1_SIZE);

    gpio_addr = mmap(0, GPIO1_SIZE, PROT_READ | PROT_WRITE, 
MAP_SHARED, fd, GPIO1_START_ADDR);

    gpio_oe_addr = gpio_addr + GPIO_OE;
    gpio_setdataout_addr = gpio_addr + GPIO_SETDATAOUT;
    gpio_cleardataout_addr = gpio_addr + GPIO_CLEARDATAOUT;
	gpio_dataIN_addr = gpio_addr + GPIO_DATAIN;
	
	
    if(gpio_addr == MAP_FAILED) {
        printf("Unable to map GPIO\n");
        exit(1);
    }
    printf("GPIO mapped to %p\n", gpio_addr);
    printf("GPIO OE mapped to %p\n", gpio_oe_addr);
    printf("GPIO SETDATAOUTADDR mapped to %p\n", 
gpio_setdataout_addr);
    printf("GPIO CLEARDATAOUT mapped to %p\n", 
gpio_cleardataout_addr);

    reg = *gpio_oe_addr;
    printf("GPIO1 configuration: %X\n", reg);
    reg = reg & (0xFFFFFFFF - PIN);
    *gpio_oe_addr = reg;
    printf("GPIO1 configuration: %X\n", reg);


	// set gpio1_18=Pin_48 as Input
	// by defualt every pin is Input in gpio1 bank

    printf("Start toggling PIN \n");
	int8_t readyFlag = 1, value=0,cnt=0;
	unsigned long int data=0,sum=0;

// you can adjust the time duration value using loops.	
#define SCK_Write_HIGH	*gpio_setdataout_addr= PIN; for(int i=0; i < 100; i++ );// SCK HIGH for 1.5us 
#define SCK_Write_LOW	*gpio_cleardataout_addr= PIN; for(int i=0; i < 120; i++ );// SCK LOW for 1.5us 

// SCK pin  = P9_12 = GPIO1_28 = gpio_60
// Dout pin  = P9_15 = GPIO1_16 = gpio_48

	    while(1) {
      //  SCK_Write_HIGH
		SCK_Write_LOW
	
		readyFlag = ( (*gpio_dataIN_addr) >> 16 ) & 1;
		
		// READ pin 
		if( !( readyFlag) )
		{
			data=0;
			value=0;
			for(int8_t i=0; i < 24; i++ ) 
			{
					
				SCK_Write_HIGH
				data = data<<1;
				
				value = ( (*gpio_dataIN_addr) >> 16 ) & 1; // read input data bit
				if( value ) 
				{
					data++;
					value=0;
				}
					
				SCK_Write_LOW// SCK-->> Low		
					
			}
			SCK_Write_HIGH //25th pulse SCK-->> HI
			value = ( (*gpio_dataIN_addr) >> 16 ) & 1; // read input data bit
			SCK_Write_LOW// SCK-->> Low	
		//	printf("25th pluse--->>> %d\n", value);
			data = data^0x800000;
			
			if( cnt++ < 2 )
				sum += data;
			else
			{
				printf("value : %x\n", ( data ) );
				printf("avg > %u\n", ( sum/2 ) );
				sum=0;
				cnt=0;
				//sleep(1);
			}
			
		}	

			
      }

    close(fd);
    return 0;
}
