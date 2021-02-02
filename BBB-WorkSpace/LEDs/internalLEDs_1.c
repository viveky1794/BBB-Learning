
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define ledpath "/sys/class/leds"

// argv[0] = LED number
// argv[1] = LED option 1. trigger 2. brightness

int writeTriggerValue(char *value, int Led)
{
	int fd, ret;
	char buf[50];

	snprintf(buf, sizeof(buf), ledpath"/beaglebone:green:usr%c/trigger", Led);


	printf("\n%s\n", buf);

	fd = open(buf, O_WRONLY);
	if(fd <= 0)
	{
		perror("open trigger error\n");
				return -1;
	}

	ret = write(fd, (void *)value, strlen(value));

	if(ret <= 0)
	{
			perror(" write trigger error\n");
			return -1;
	}

return 0;
}
//sys/class/leds/beaglebone:green:usr3/trigger

void operation_trigger(char *value, int Led)
{

	if( ! (strcmp(value, "timer") && strcmp(value, "heartbeat") && \
				strcmp(value, "none") && strcmp(value, "oneshot") && \
				strcmp(value, "default-on") ) )
	{
		writeTriggerValue(value, Led);
	}


}


void operation_brightness( int led)
{



}


int main(int argc, char *argv[] )
{

printf("we are ready to Rock. This app will do operations on LED : %s\n", argv[1]);

if( argc < 3 )
{
printf("Please check your arguments. It may be less or more then requirement \n");
}
else
{
	if( strcmp(argv[2], "trigger" ) == 0 )
		{
			printf("You want to trigger this LED.\n");
			operation_trigger(argv[3], *argv[1] );
		}

	else if( strcmp(argv[2], "brightness" ) == 0 )
		{
			printf("You want to control brightness of this LED.\n");
//			operation_brightness( atoi(argv[3] );
		}

	else
		printf("Sorry ! This option is not available in this application\n");

}

return 0;
}

