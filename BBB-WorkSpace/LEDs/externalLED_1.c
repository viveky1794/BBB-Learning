
/*
	This application will be used to ON/OFF external leds.

*/


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>


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