
#include <cstdlib>
#include <cstdio>
#include <cinttypes>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "settings.h"
#include "picosha2.h"

int getMessageSize(int fd) {
     char *stringFileSize = (char *) malloc(sizeof(char) * IDENTIFIER_LENGTH);
     int c = 0;
     for(int i = 0; i < IDENTIFIER_LENGTH; i++) {
          c = serialGetchar(fd);
	  if(c == -1) { printf("ERROR 2\n"); return 2; }
	  stringFileSize[i] = (char) c;
     }
     return atoi(stringFileSize);
}

int main(int argc, const char **argv) {
     wiringPiSetup();

     int fd = serialOpen(INTERFACE, BAUD_RATE);
     if(fd < 0) {
          printf("ERROR 1\n");
          return 1;
     }

	int fileSize = getMessageSize(fd);
	printf("%d\n", fileSize);
	serialClose(fd);
}
