
#ifndef __RF_HPP_
#define __RF_HPP_

#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <RH_RF69.h>
#include <RH_RF95.h>

#include "settings.h"
#include "RasPiBoards.h"
#include "easylogging++.h"

void setupRF();
char *receiveRF();
void transmitRF(char *, int);

#endif
