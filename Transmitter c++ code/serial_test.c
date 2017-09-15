#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

int main(int argc, char *argv[]) {
	if (wiringPiSetup() < 0){
		fprintf(stderr, "setup failed\n");
		return 1;
	}

	FILE *fp;
	size_t size;
	unsigned char c;
	int i;
	int fd = serialOpen("/dev/serial0", 9600);
	char size_bytes[4];

	if (fd == -1){
		printf("Serial error: <%d>\n",fd);
		return 0;
	}

	printf("NO SERIAL ERROR!!\n");

	fp = fopen(argv[1], "rb");
	// fseek(fp, 0, SEEK_END) places the file pointer at the end of the file.
	fseek(fp, 0, SEEK_END);
	// ftell returns the # of bytes that are in the file.
	size = ftell(fp);
	// Puts file pointer back @ beginning -> fseek is better b/c we can
	// check if it's successful. Fix later.
	rewind(fp);

	// I think the serial might have big-endian or little-endian issue
	// or something here.  It looks they they flip the size vairable at the
	// bit level to get its reverse.

	// Okay, so what they're doing is reversing the size @ the bit level before
	// sending the size first through the serial port. Then, they read in the bits
	// from least significant to most significant on the receiving end. This seems
	// really unnecessary if you know on the receiving end that the bits will come
	// most significant to least significant.

	// One thing they did correctly is breaking the 32-bit number into 4 bytes
	// b/c you have to send the serial data one byte at a time.
	size_bytes[0] = (size >> 24) & 0xFF;
	size_bytes[1] = (size >> 16) & 0xFF;
	size_bytes[2] = (size >> 8) & 0xFF;
	size_bytes[3] = size & 0xFF;

	printf("%d\t%d\t%d\t%d\n",size_bytes[0],size_bytes[1],size_bytes[2],size_bytes[3]);

	int newsize = size_bytes[3] | (uint32_t) size_bytes[2] << 8 | (uint32_t) size_bytes[1] << 16 | (uint32_t) size_bytes[0] << 24;

	printf("%d\n",size);
	printf("%d\n",newsize);

	// Size bytes go through first so that the receiving program knows
	// how many bytes to anticipate for.
	for(i = 0; i < 4; i++)
		serialPutchar(fd,size_bytes[i]);

	// Send the real data through.
	for(i = 0; i < size; i++){
		c = fgetc(fp);
		serialPutchar(fd, c);
	}

	fclose(fp);
	serialClose(fd);
}
