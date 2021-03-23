





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

//#include "gpio.h"

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
int gpio_get_value_fd(int fd, unsigned int *value);

/****************************************************************
 * gpio_export
 ****************************************************************/
int gpio_export(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);

    return 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int gpio_unexport(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);
    return 0;
}


/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int gpio_set_dir(unsigned int gpio, unsigned int out_flag)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/direction", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/direction");
        return fd;
    }

    if (out_flag)
        write(fd, "out", 4);
    else
        write(fd, "in", 3);

    close(fd);
    return 0;
}



/****************************************************************
 * gpio_set_value
 ****************************************************************/
int gpio_set_value(unsigned int gpio, unsigned int value)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/set-value");
        return fd;
    }

    if (value)
        write(fd, "1", 2);
    else
        write(fd, "0", 2);

    close(fd);
    return 0;
}


/****************************************************************
 * gpio_get_value
 ****************************************************************/
int gpio_get_value(unsigned int gpio, unsigned int *value)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(buf, O_RDONLY);
    if (fd < 0) {
        perror("gpio/get-value");
        return fd;
    }

    gpio_get_value_fd(fd, value);

    close(fd);
    return 0;
}


int gpio_get_value_fd(int fd, unsigned int *value)
{
    char ch;

    read(fd, &ch, 1);

    if (ch != '0') {
        *value = 1;
    } else {
        *value = 0;
    }

    return 0;
}

#define dataisReady  { gpio_set_value(gpioSCK, 0 ); \
		int readyFlag[2]; \
		readyFlag[0] = 1; \
		while( readyFlag[0] ) \
		{ \
			gpio_get_value(gpioDin, readyFlag ); \			
		} \
	}
	

#define	SCK_Write_LOW	write(fsck, "0", 2);
#define SCK_Write_HIGH	write(fsck, "1", 2);
#define Dout_Read_value read(fdout, &value,1);
#define Dout_Read_ready read(fdout, &readyFlag, 1);

int main()
{
	
	printf(" This program is made for communication with ADC-HX711 \n");	
	int len;
	int fsck, fdout;
    char buf[MAX_BUF];
	char readBuf[2];
	unsigned int data=0;
	
	int gpioSCK = 60;
	int gpioDin = 48;

	unsigned int value[2] = {0}; 	
	unsigned int readyFlag[2];
	readyFlag[0] = 1;

	//1. unexport first
	gpio_unexport( gpioSCK );
	gpio_unexport( gpioDin );
	
	//2. export GPIO
	gpio_export( gpioSCK );
	gpio_export( gpioDin );
	
	// 3. set direction
	gpio_set_dir(gpioSCK, 1); //out
	gpio_set_dir(gpioDin, 0); //in
	
	
/******************************************************************/	
	// 5. open gpioSCK.
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/value", gpioSCK);
    fsck = open(buf, O_WRONLY);
    if (fsck < 0) {
        perror("gpio/get-value");
        return fsck;
    }

	// 6. open gpioDout
	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/value", gpioDin);
    fdout = open(buf, O_RDONLY);
    if (fdout < 0) {
        perror("gpio/get-value");
        return fdout;
    }
	
/**************************************************************************/

	SCK_Write_LOW
	printf("\n 1 before while ....!!! ");
//SCK_Write_LOW
// SCK_Write_HIGH
// SCK_Write_LOW

printf("\n 4 before while ....!!! ");
	while(1)
	{
				
		//dataisReady
		readyFlag[0] = 1;
		while( readyFlag[0] )
		{
			Dout_Read_ready
		}
		
		if( !( readyFlag[0]) )
		{
			data=0;
			for(int8_t i=0; i < 24; i++ ) 
			{
				value[0]=0;	
				SCK_Write_HIGH// SCK-->> HI
				data = data<<1;
				//usleep(1);
				Dout_Read_value // read input data bit
				if( ( value[0]) == 1 ) 
					data++;
				SCK_Write_LOW// SCK-->> Low		
				//usleep(1);	
			}
			SCK_Write_HIGH //25th pulse SCK-->> HI
			Dout_Read_value // read input data bit
			SCK_Write_LOW// SCK-->> Low	
			printf("25th pluse--->>> %d\n", value[0]);
			data = data^0x800000;
			//data = (~data)+1;
			printf("value : %x\n", ( data ) );
			printf("value > %u\n", ( data ) );
			
		}
	}
	
	usleep(100000);
	
	close(fsck);
	close(fdout);
	return 0;
}
