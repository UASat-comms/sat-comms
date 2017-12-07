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
    closeRF();
    return 0;
}
