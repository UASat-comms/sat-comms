
#ifndef __RF_HPP_
#define __RF_HPP_

#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include <RH_RF95.h>

#include "settings.h"
#include "easylogging++.h"

typedef struct RF_MESSAGE {
     uint8_t *data;
     uint8_t len;
} rfMessage;

RHReliableDatagram setupRF(int CS_PIN, int IRQ_PIN, int RF_RST_PIN, int MY_ADDRESS, int RF_FREQUENCY);
void send(RHReliableDatagram rf95Manager, RF_MESSAGE *);
RF_MESSAGE recv(RHReliableDatagram rf95Manager);

#endif
