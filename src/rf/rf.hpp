
#ifndef __RF_HPP_
#define __RF_HPP_

#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include <RH_RF69.h>
#include <RH_RF95.h>

#include "settings.h"
#include "RasPiBoards.h"
#include "easylogging++.h"

typedef struct RF_MESSAGE {
     uint8_t *data;
     uint8_t len;
} rfMessage;

rfMessage createRFMessage(char *data);
void transmitModeRF();
void receiveModeRF();
void closeRF();
void receiveRF();
void transmitRF(uint8_t *, uint8_t);

#endif
