

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
#define DS1307_SLAVE_ADDR	0x68


#define secondREG	0x00
#define minuteREG	0x01
#define hourREG		0x02

/********** Declare Global varibales **************/
int fd;


/*********** Write Data into I2C  device *********/

int8_t setClock(int8_t hh, int8_t mm, int8_t ss)
{
	
	int ret;
	char buf[2];
	buf[0] = secondREG;
	buf[1] = ss;
	
		
	if( ( ret = write(fd, buf, 2) ) <= 0 )
	{
		printf("secondREG->> Write Failed\n");
		return -1;
	}
	usleep(500);
	buf[0] = minuteREG;
	buf[1] = mm;
		
	if( ( ret = write(fd, buf, 2) ) <= 0 )
	{
		printf("minuteREG->> Write Failed\n");
		return -1;
	}
	usleep(500);
	buf[0] = hourREG;
	buf[1] = hh;	
	
	if( ( ret = write(fd, buf, 2) ) <= 0 )
	{
		printf("hourREG->> Write Failed\n");
		return -1;
	}
	usleep(500);
	return 0;
	
}


/*********** Read I2C Data from DS1307 **********/


int8_t readClock()
{
	int ret;
	char buf[2];
	char receivebuf[2];
	
	buf[0] = secondREG;
	
	if( ( ret = write(fd, buf, 1) ) <= 0 )
	{
		printf("secondREG->> R-Write Failed\n");
		return -1;
	}
	usleep(500);
	if( ( ret = read(fd, receivebuf, 1) ) <= 0 )
	{
		printf("secondREG->> read Failed\n");
		return -1;
	}
	usleep(500);
	printf("SS : %d\n", &receivebuf[0]);
	return 0;
}


int main()
{
	
// 1. open i2c device file  in read and write mode.
	if ( (fd = open(i2c_device_path, O_RDWR)) < 0 )
	{
		printf("File to open I2C file\n");
		return -1;
	}
	
	// 2. Set the I2C slave address using ioctl command.
	if( (ioctl(fd, I2C_SLAVE, DS1307_SLAVE_ADDR )) < 0 )
	{
		printf("File to open I2C file\n");
		close(fd);
		return -1;
	}
	
	// 3. Write Time into DS1307 registors.
		setClock(12, 45, 45 );
	
	while(1)
	{
		
		if( readClock() < 0 )
		{
			perror("Wrror in reading\n\n");
			
		}
		
	}
	
	
	return 0;
	
}