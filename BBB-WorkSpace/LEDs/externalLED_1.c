
/*
	This application will be used to ON/OFF external leds.
	Status : Not completed Yet.
*/


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

/****************************************************************
 * Constants
 ****************************************************************/

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64


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
	printf("snprintf = len -->> %d\n", len);
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
   char filePath[MAX_BUF];
   
   len = snprintf(filePath, sizeof(filePath), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);
   
   fd = open(filePath, O_WRONLY);
   if( fd < 0)
   {
	   printf("Error in file opening\n\n");
	   return -1;
   }
   
   if(out_flag)
	write(fd, "out", 4); 
   else
	write(fd, "in", 3);
   
   close(fd);
   return 0;
}

/****************************************************************
 * gpio_get_dir
 ****************************************************************/
char *gpio_get_dir(unsigned int gpio)
{
   int fd, len;
   char filePath[MAX_BUF];
   char buf[10];
   
   len = snprintf(filePath, sizeof(filePath), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);
   
   fd = open(filePath, O_RDONLY);
   if( fd < 0)
   {
	   printf("Error in file opening\n\n");
	   return "ERROR\n";
   }
   
 
	read(fd, buf, sizeof(buf)); 
   printf("--->>> %s", buf);
   close(fd);
   return buf;
}

int main(int argc, char *argv[] )
{
	 unsigned int gpio = 60;
	printf("We are using external Pin to toggle LEDs.");
	
	gpio_get_dir(gpio);
	
	while(1);

return 0;
}