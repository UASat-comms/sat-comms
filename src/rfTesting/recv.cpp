#include <stdio.h>
#include "rf.hpp"
#include "settings.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

el::Configurations conf(LOGCONFIG);

int main() {
    el::Loggers::reconfigureAllLoggers(conf);
    setupRF();
    char *mess = recvRF();
    LOG(INFO) << "Message received: <" << mess << ">";
    bcm2835_delay(500);
    char *reply = (char *) "Got your message!";
    sendRF(reply);
    closeRF();
    return 0;
}
