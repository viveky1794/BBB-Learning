

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


/*********** Define MACROs	*******************/
#define i2c_device_path		"/dev/i2c-2"
#define HX711_SLAVE_ADDR	


/********** Declare Global varibales **************/
int fd;


int main()
{
	
	// 1. open i2c device file  in read and write mode.
	if ( (fd = open(i2c_device_path, O_RDWR)) < 0 )
	{
		perror("File to open I2C file\n");
		return -1;
	}
	
	// 2. Set the I2C slave address using ioctl command.
	if( (ioctl(fd, I2C_SLAVE, HX711_SLAVE_ADDR )) < 0 )
	{
		perror("File to open I2C file\n");
		close(fd);
		return -1;
	}
	
	// 3. 
	
	
	
	
	return 0;
}