
#ifndef _STEERING__H_
#define _STEERING__H_

#include <wiringPi.h>
#include <bcm2835.h>

#include "settings.h"

void motorUp(unsigned numSteps);
void motorDown(unsigned numSteps);
void motorLeft(unsigned numSteps);
void motorRight(unsigned numSteps);

#endif
