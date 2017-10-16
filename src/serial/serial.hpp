

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <cstdlib>
#include <cstdio>
#include <cinttypes>
#include <ctime>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "settings.h"

void transmitFile(char *);
void receiveFile();

#endif
