#include "serial.hpp"

/* ======================================================
 * Desc: Returns the number of BYTES in a file.
 *
 * Args:
 * - fp: The file pointer to the file of interest.
 *
 * Notes: The file pointer will be set at the beginning
 *        of the file after finding the size of the file.
 *	  Also, it includes the EOF char in the count.
 * ======================================================
 */
static int fileSize(FILE *fp) {
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
static char *IntToString(int input) {
	char *buffer = (char *) malloc(sizeof(char) * IDENTIFIER_LENGTH);
	sprintf(buffer, "%d", input);
	return buffer;
}

static int getMessageSize(int fd) {
     char *stringFileSize = (char *) malloc(sizeof(char) * IDENTIFIER_LENGTH);
     int c = 0;
     for(int i = 0; i < IDENTIFIER_LENGTH; i++) {
          c = serialGetchar(fd);
	  if(c == -1) { printf("ERROR 2\n"); return 2; }
	  stringFileSize[i] = (char) c;
     }
     return atoi(stringFileSize);
}

void transmitFile(char *fileName) {
     FILE *fp = fopen(fileName, "r");
     int fsize = fileSize(fp);

     char *stringFileSize = IntToString(fsize);

     LOG(INFO) << "File Size: <" << fsize << ">";
     //printf("FILE SIZE: <%d>\n", fsize);
     int fd = serialOpen(INTERFACE, BAUD_RATE);
     if(fd < 0) {
          printf("ERROR 1\n");
	  exit(1);
     }

     char *fileData = (char *) malloc(sizeof(char) * fsize);
     for(int i = 0; i < fsize; i++) fileData[i] = fgetc(fp);

     for(int i = 0; i < IDENTIFIER_LENGTH; i++) {
          serialPutchar(fd, stringFileSize[i]);
     }


     for(int i = 0; i < fsize; i++) serialPutchar(fd, fileData[i]);

     free(fileData);
     fclose(fp);
     serialClose(fd);
}

void receiveFile() {
     int fd = serialOpen(INTERFACE, BAUD_RATE);
     if(fd < 0) {
          printf("ERROR 1\n");
          exit(1);
     }

     int fileSize = getMessageSize(fd);
     printf("Size of file to receive IN BYTES: <%d>\n", fileSize);

     char *filedata = (char *) malloc(sizeof(char) * (fileSize));
     //filedata[fileSize - 1] = '\0';
     time_t start, end;
     start = time(0);
     for(int i = 0; i < fileSize; i++) filedata[i] = serialGetchar(fd);
     end = time(0);
     double avg = ((double) end - (double) start);

     printf("Time in seconds taken to transmit IN SECONDS: <%lf>\n", avg);
     printf("Resulting BYTES/s rate: <%lf>\n", fileSize / avg);
     printf("Resulting bits/s rate: <%lf>\n", (fileSize * 8) / avg);

     FILE *fp = fopen("RECD_data", "w");
     for(int i = 0; i < fileSize; i++) fputc(filedata[i], fp);

     free(filedata);
     fclose(fp);
     serialClose(fd);     
}
