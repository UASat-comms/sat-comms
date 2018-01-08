

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cinttypes>
#include <ctime>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "settings.h"
#include "easylogging++.h"

int getFileSize(FILE *fp);
void transmitData(char *data, int datalen);
char *receiveData(int datalen);

#endif
