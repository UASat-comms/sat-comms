#ifndef __CHECKSUM_H_
#define __CHECKSUM_H_

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/* [NOTE]:
 * This checksum function was found on:
 * https://stackoverflow.com/questions/3463976/c-file-checksum
 */
unsigned char checksum(FILE *fp);

void printChecksum(unsigned char csum);

#endif
