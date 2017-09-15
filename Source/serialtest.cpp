/* ======================================================
 * Author: Matt
 *
 * Description: Tests the serial communication on RPi.
 *
 *
 * ======================================================
 */


#include <cstdlib>
#include <cstdio>
#include <cinttypes>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "serialconstants.h"
#include "picosha2.h"

/* ======================================================
 * Desc: Returns the number of BYTES in a file.
 *
 * Args:
 * - fp: The file pointer to the file of interest.
 *
 * Notes: The file pointer will be set at the beginning
 *        of the file after finding the size of the file.
 * ======================================================
 */
int fileSize(FILE *fp) {
     int size;
     fseek(fp, 0, SEEK_END);
     size = ftell(fp);
     fseek(fp, 0, SEEK_SET);
     return size;
}

/* ======================================================
 * Desc: Returns an integer converted to a char pointer.
 *
 * Args:
 * - input: The integer to convert to a string.
 * ======================================================
 */
char *IntToString(int input) {
	char *buffer = (char *) malloc(sizeof(char) * IDENTIFIER_LENGTH);
	sprintf(buffer, "%d", input);
	return buffer;
}

int main(int argc, const char **argv) {
     wiringPiSetup();

     FILE *fp = fopen(argv[1], "r");
     int fsize = fileSize(fp);
     char *stringFileSize = IntToString(fsize);
     
     int fd = serialOpen(INTERFACE, BAUD_RATE);
     if(fd < 0) {
          printf("ERROR 0!\n");
	  return 1;
     }
     	// check to see if this returns an error int.
	serialPuts(fd, stringFileSize);

     fclose(fp);
     serialClose(fd);

     return 0;
}

