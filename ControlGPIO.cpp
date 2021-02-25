/*******************************************************
* Example code for digital output on a GPIO pin
* on the new beaglebone black running the
* Linux Angstrom distribution.
********************************************************
*
* Instructions:
* -Compile and run from the root directory
* -Windows users can use WinSCP to transfer
* code files to the beaglebone. This will
* allow you to edit code in your favorite
* editor on Windows
* -It is recommended that you do not try to
* source more than 4 - 6 mA. Use a transistor
* e.g. the PN2222A to source higher currents,
* such as for driving an LED
*
* Code adapted from:
* - Derek Molloy, "Beaglebone: C/C++ Programming
* Introduction for ARM Embedded Linux Development
* using Eclipse CDT" video tutorial,
* (link: <a href="http://www.youtube.com/watch?v=SaIpz00lE84" title="http://www.youtube.com/watch?v=SaIpz00lE84">http://www.youtube.com/watch?v=SaIpz00lE84</a>)
* -  Mark A. Yoder, EBC Exercise 10 Flashing an LED
* (link: http://www.elinux.org/EBC_Exercise_10_Flashing_an_LED)
*
*******************************************************/
 
#include 
#include 
#include 
 
using namespace std;
 
int main(int argc, char** argv) {
 
    int GPIOPin=60, /* GPIO1_28 or pin 12 on the P9 header */ times=10;
 
    printf("\nStarting GPIO output program\n");
    FILE *myOutputHandle = NULL;
    char setValue[4], GPIOString[4], GPIOValue[64], GPIODirection[64];
    sprintf(GPIOString, "%d", GPIOPin);
    sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", GPIOPin);
    sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", GPIOPin);
 
    // Export the pin
    if ((myOutputHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
        printf("Unable to export GPIO pin\n");
        return 1;
    }
    strcpy(setValue, GPIOString);
    fwrite(&amp;setValue, sizeof(char), 2, myOutputHandle);
    fclose(myOutputHandle);
 
    // Set direction of the pin to an output
    if ((myOutputHandle = fopen(GPIODirection, "rb+")) == NULL){
        printf("Unable to open direction handle\n");
        return 1;
    }
    strcpy(setValue,"out");
    fwrite(&amp;setValue, sizeof(char), 3, myOutputHandle);
    fclose(myOutputHandle);
 
    for(int i=0; i&lt;times; i++){
        // Set output to high
        if ((myOutputHandle = fopen(GPIOValue, &quot;rb+&quot;)) == NULL){
            printf(&quot;Unable to open value handle\n&quot;);
            return 1;
        }
        strcpy(setValue, &quot;1&quot;); // Set value high
        fwrite(&amp;setValue, sizeof(char), 1, myOutputHandle);
        fclose(myOutputHandle);
        sleep(1); // wait for 1 sec
 
        // Set output to low
        if ((myOutputHandle = fopen(GPIOValue, &quot;rb+&quot;)) == NULL){
            printf(&quot;Unable to open value handle\n&quot;);
            return 1;
        }
        strcpy(setValue, &quot;0&quot;); // Set value low
        fwrite(&amp;setValue, sizeof(char), 1, myOutputHandle);
        fclose(myOutputHandle);
        sleep(1); // wait for 1 sec
 
    }
 
    // Unexport the pin
    if ((myOutputHandle = fopen(&quot;/sys/class/gpio/unexport&quot;, &quot;ab&quot;)) == NULL) {
        printf(&quot;Unable to unexport GPIO pin\n&quot;);
        return 1;
    }
    strcpy(setValue, GPIOString);
    fwrite(&amp;setValue, sizeof(char), 2, myOutputHandle);
    fclose(myOutputHandle);
    printf(&quot;\nCompleted GPIO output program\n&quot;);
 
    return 0;
}