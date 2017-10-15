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

void transmitFile(char *fileName) {
     FILE *fp = fopen(fileName);
     int fsize = fileSize(fp);

     char *stringFileSize = IntToString(fsize);

     printf("FILE SIZE: <%d>\n", fsize);
     int fd = serialOpen(INTERFACE, BAUD_RATE);
     if(fd < 0) {
          printf("ERROR 1\n");
	  return 1;
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
