
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define GPIO_FILEPATH "/sys/class/gpio"
#define MAX_LEN	64

int8_t gpio_export(uint8_t gpio)
{
	int fd, len;
	char buf[MAX_LEN];
	
	if ( ( fd = open(GPIO_FILEPATH "/export", O_WRONLY) ) < 0 )
	{
		printf("Error : File Opeing\ngpio_export");
		return -1;
	}
	
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	
	write(fd, buf, len);
	
	close(fd);
	return 0;
	
}


int8_t gpio_unexport(uint8_t gpio )
{
	
	int fd, len;
	char buf[MAX_LEN];
	
	if( (fd = open(GPIO_FILEPATH "/unexport", O_WRONLY ) ) < 0 )
	{
		printf("Error in file opening : gpio_unexport\n");
		return -1;
	}
	
	
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	
	close(fd);
	
	return 1;
	
}


int8_t gpio_setDir(uint8_t gpio, uint8_t dir )
{
	int fd, len;
	char buf[MAX_LEN];
	
	snprintf(buf, sizeof(buf), GPIO_FILEPATH "/gpio%d/direction", gpio );
	
	if( ( fd = open(buf, O_WRONLY ) ) < 0 )
	{
		printf("Error in file opeinging");
		return -1;		
	}
	
	if( !dir )
	{
		write(fd, "in" , 3);
	}
	else
	{
		write(fd, "out", 4);
	}
	
	close(fd);
	
	return 1;
}


int8_t gpio_setValue(uint8_t gpio, uint8_t value )
{
	
	int fd, len;
	char buf[MAX_LEN];
	
	snprintf(buf, sizeof(buf), GPIO_FILEPATH "/gpio%d/value", gpio );
	
	if( ( fd = open(buf, O_WRONLY ) ) < 0 )
	{
		printf("Error in file opeinging");
		return -1;		
	}
	
	if( value )
	write(fd, "1", 2 );
	else
	write(fd, "0", 2);
	
	
	close(fd);
	return 1;
}



int8_t gpio_getValue(uint8_t gpio)
{
	
	int fd, len;
	char buf[MAX_LEN];
	int8_t value;
	
	snprintf(buf, sizeof(buf), GPIO_FILEPATH "/gpio%d/value", gpio );
	
	if( ( fd = open(buf, O_RDONLY ) ) < 0 )
	{
		printf("Error in file opeinging");
		return -1;		
	}
	
	
	read(fd, &value, 1 );
	
	close(fd);
	return (value-48);
}


int main(int argc, char *argv[] )
{
	printf("\n***********************************************\n");
	
	int8_t value[2];
	
	while(1)
	{
		
			gpio_setValue(60, 1);			
			printf("value : %d\n", gpio_getValue(60) );
			sleep(1);
			
			gpio_setValue(60, 0);			
			printf("value : %d\n", gpio_getValue(60));
			sleep(1);


	};
	
	

return 0;
}