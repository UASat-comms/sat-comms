
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cinttypes>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "settings.h"

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
     printf("Size of file to receive: <%d>\n", fileSize);
     
     char *filedata = (char *) malloc(sizeof(char) * (fileSize));
     //filedata[fileSize - 1] = '\0';
     time_t start, end;
     start = time(0);
     for(int i = 0; i < fileSize; i++) filedata[i] = serialGetchar(fd);     
     end = time(0);
     double avg = ((double) end - (double) start);

     printf("Time in seconds taken to transmit: <%lf>\n", avg);
     printf("Resulting bit/s rate: <%lf>\n", fileSize / avg);

     FILE *fp = fopen("RECD_data", "w");
     for(int i = 0; i < fileSize; i++) fputc(filedata[i], fp);

     free(filedata);	
     fclose(fp);
     serialClose(fd);


}
