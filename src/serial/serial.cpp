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
int getFileSize(FILE *fp) {
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
char *intToString(int input) {
	char *buffer = (char *) malloc(sizeof(char) * (IDENTIFIER_LENGTH + 1));
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

void transmitData(char *data, int datalen) {
     LOG(DEBUG) << "Attempting to open serial interface.";
     int fd = serialOpen(INTERFACE, BAUD_RATE);
     if(fd < 0) {
          LOG(FATAL) << "Unable to open serial interface.";
     }
     LOG(DEBUG) << "Serial interface opened successfully.";

     LOG(DEBUG) << "Baud Rate: <" << BAUD_RATE << ">";

     LOG(INFO) << "Transmitting data via serial..";
     for(int i = 0; i < datalen; i++) serialPutchar(fd, data[i]);
     LOG(INFO) << "Data transmitted.";

     LOG(DEBUG) << "Freeing resources...";
     serialClose(fd);
     LOG(DEBUG) << "Resources freed.";
}

char *receiveData(int datalen) {
     LOG(DEBUG) << "Attempting to open serial interface...";
     int fd = serialOpen(INTERFACE, BAUD_RATE);
     if(fd < 0) {
          LOG(FATAL) << "Unable to open serial interface.";
     }

     LOG(DEBUG) << "Baud Rate: <" << BAUD_RATE << ">";

     LOG(DEBUG) << "Serial interface opened successfully.";

     LOG(DEBUG) << "Attempting to reserve memory for file data...";
     char *data = (char *) malloc(sizeof(char) * (datalen + 1));
     if(data == NULL) {
          LOG(FATAL) << "Unable to reserve memory for file data.";
     }
     LOG(DEBUG) << "Memory successfully reserved for file data.";
     data[datalen] = '\0';

     time_t start, end;
     LOG(INFO) << "Receiving file data...";
     start = time(0);
     for(int i = 0; i < datalen; i++) {
          data[i] = serialGetchar(fd);
          LOG(INFO) << "Received character" << (int) data[i];
     }
     end = time(0);
     LOG(INFO) << "File data received.";

     double avg = ((double) end - (double) start);
     LOG(DEBUG) << "Time in seconds taken to receive: <" << avg << ">";
     LOG(DEBUG) << "Resulting BYTE/s: <" << (datalen / avg) << ">";
     LOG(INFO) << "Resulting bit/s: <" << (datalen * 8 / avg) << ">";

     LOG(DEBUG) << "Freeing resources...";
     serialClose(fd);
     LOG(DEBUG) << "Resources freed.";

     return data;
}

void noInputHandler(int sig) {
    LOG(FATAL) << "Lost connection from serial Tx side!";
}

char *receiveData(int datalen, int timeout) {
     LOG(DEBUG) << "Setting up serial timeout handler.";
     signal(SIGALRM, noInputHandler);

     LOG(DEBUG) << "Attempting to open serial interface...";
     int fd = serialOpen(INTERFACE, BAUD_RATE);
     if(fd < 0) {
          LOG(FATAL) << "Unable to open serial interface.";
     }

     LOG(DEBUG) << "Baud Rate: <" << BAUD_RATE << ">";

     LOG(DEBUG) << "Serial interface opened successfully.";

     LOG(DEBUG) << "Attempting to reserve memory for file data...";
     char *data = (char *) malloc(sizeof(char) * (datalen + 1));
     if(data == NULL) {
          LOG(FATAL) << "Unable to reserve memory for file data.";
     }
     LOG(DEBUG) << "Memory successfully reserved for file data.";
     data[datalen] = '\0';

     time_t start, end;
     LOG(INFO) << "Receiving file data...";
     start = time(0);
     for(int i = 0; i < datalen; i++) {
          alarm(timeout);
          data[i] = serialGetchar(fd);
     }
     end = time(0);
     LOG(INFO) << "File data received.";

     double avg = ((double) end - (double) start);
     LOG(DEBUG) << "Time in seconds taken to receive: <" << avg << ">";
     LOG(DEBUG) << "Resulting BYTE/s: <" << (datalen / avg) << ">";
     LOG(INFO) << "Resulting bit/s: <" << (datalen * 8 / avg) << ">";

     LOG(DEBUG) << "Freeing resources...";
     serialClose(fd);
     LOG(DEBUG) << "Resources freed.";

     return data;
}
