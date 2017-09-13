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
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	size_bytes[0] = (size >> 24) & 0xFF;
	size_bytes[1] = (size >> 16) & 0xFF;
	size_bytes[2] = (size >> 8) & 0xFF;
	size_bytes[3] = size & 0xFF;

	printf("%d\t%d\t%d\t%d\n",size_bytes[0],size_bytes[1],size_bytes[2],size_bytes[3]);

	int newsize = size_bytes[3] | (uint32_t) size_bytes[2] << 8 | (uint32_t) size_bytes[1] << 16 | (uint32_t) size_bytes[0] << 24;

	printf("%d\n",size);
	printf("%d\n",newsize);

	for(i = 0; i < 4; i++)
		serialPutchar(fd,size_bytes[i]);

	for(i = 0; i < size; i++){
		c = fgetc(fp);
		serialPutchar(fd, c);
	}

	fclose(fp);
	serialClose(fd);
}
