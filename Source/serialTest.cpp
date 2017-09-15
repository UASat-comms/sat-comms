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
	char *buffer = (char *) malloc(sizeof(char) * 50);
	sprintf(buffer, "%d", input);
	return buffer;
}

int main(int argc, const char **argv) {
     wiringPiSetup();

     FILE *fp = fopen(argv[1], "r");
     int fsize = fileSize(fp);
     char *stringFileSize = IntToString(fsize);
     printf("%s\n", stringFileSize);
	printf("%d\n", atoi(stringFileSize));
     int fd = serialOpen("/dev/serial0", BAUD_RATE);

     serialPuts(fd, stringFileSize);

     fclose(fp);
     serialClose(fd);

     return 0;
}

