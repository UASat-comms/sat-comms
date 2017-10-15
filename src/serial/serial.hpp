

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <cstdlib>
#include <cstdio>
#include <cinttypes>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "settings.h"

static int fileSize(FILE *);
static char *IntToString(int);
void transmitFile(char *);

#endif
