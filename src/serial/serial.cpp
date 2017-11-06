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
     LOG(DEBUG) << "Attempting to open file...";
     FILE *fp = fopen(fileName, "r");
     if(fp == NULL) {
          LOG(FATAL) << "Unable to open file to transmit.";
     }
     LOG(DEBUG) << "File opened successfully.";

     int fsize = fileSize(fp);
     char *stringFileSize = IntToString(fsize);
     LOG(INFO) << "File Size in BYTES: <" << fsize << ">";

     LOG(DEBUG) << "Attempting to open serial interface.";
     int fd = serialOpen(INTERFACE, BAUD_RATE);
     if(fd < 0) {
          LOG(FATAL) << "Unable to open serial interface.";
     }
     LOG(DEBUG) << "Serial interface opened successfully.";

     LOG(DEBUG) << "Attempting to reserve memory for file data.";
     char *fileData = (char *) malloc(sizeof(char) * fsize);
     if(fileData == NULL) {
          LOG(FATAL) << "Unable to reserve memory for file data.";
     }
     LOG(DEBUG) << "Memory successfully reserved for file data.";

     LOG(DEBUG) << "Storing file data in memory.";
     for(int i = 0; i < fsize; i++) fileData[i] = fgetc(fp);

     LOG(INFO) << "Transmitting file size...";
     for(int i = 0; i < IDENTIFIER_LENGTH; i++) {
          serialPutchar(fd, stringFileSize[i]);
     }
     LOG(INFO) << "File size transmit complete.";

     LOG(INFO) << "Sleeping shortly (2s) to allow setup on Rx side.";
     sleep(2);

     LOG(INFO) << "Transmitting file data...";
     for(int i = 0; i < fsize; i++) serialPutchar(fd, fileData[i]);
     LOG(INFO) << "File data transmitted.";

     LOG(DEBUG) << "Freeing resources...";
     free(fileData);
     fclose(fp);
     serialClose(fd);
     LOG(DEBUG) << "Resources freed.";
}

void receiveFile() {
     LOG(DEBUG) << "Attempting to open serial interface...";
     int fd = serialOpen(INTERFACE, BAUD_RATE);
     if(fd < 0) {
          LOG(FATAL) << "Unable to open serial interface.";
     }
     LOG(DEBUG) << "Serial interface opened successfully.";

     LOG(DEBUG) << "Waiting for file size transmission...";
     char *stringFileSize = (char *) malloc(sizeof(char) * IDENTIFIER_LENGTH);
     int c = 0;
     for(int i = 0; i < IDENTIFIER_LENGTH; i++) {
          c = serialGetchar(fd);
          if(c == -1) {
               LOG(FATAL) << "File size transmission timed out (10s).";
          }
          stringFileSize[i] = (char) c;
     }
     LOG(DEBUG) << "File size transmission received.";

     int fileSize = atoi(stringFileSize);
     free(stringFileSize);

     LOG(INFO) << "Size of file of file to receive in BYTES: <" << fileSize << ">";

     LOG(DEBUG) << "Attempting to reserve memory for file data...";
     char *fileData = (char *) malloc(sizeof(char) * (fileSize));
     if(fileData == NULL) {
          LOG(FATAL) << "Unable to reserve memory for file data.";
     }
     LOG(DEBUG) << "Memory successfully reserved for file data.";

     //filedata[fileSize - 1] = '\0';
     time_t start, end;

     LOG(INFO) << "Receiving file data...";
     start = time(0);
     for(int i = 0; i < fileSize; i++) fileData[i] = serialGetchar(fd);
     end = time(0);
     LOG(INFO) << "File data received.";


     double avg = ((double) end - (double) start);
     LOG(INFO) << "Time in seconds taken to receive: <" << avg << ">";
     LOG(INFO) << "Resulting BYTE/s: <" << (fileSize / avg) << ">";
     LOG(INFO) << "Resulting bit/s: <" << (fileSize * 8 / avg) << ">";

     LOG(INFO) << "attempting to write file data to 'RECD_data'...";
     FILE *fp = fopen("RECD_data", "w");
     if(fp == NULL) {
          LOG(FATAL) << "Unable to open file.";
     }
     for(int i = 0; i < fileSize; i++) fputc(fileData[i], fp);
     LOG(INFO) << "File data written.";

     LOG(DEBUG) << "Freeing resources...";
     free(fileData);
     fclose(fp);
     serialClose(fd);
     LOG(DEBUG) << "Resources freed.";
}
