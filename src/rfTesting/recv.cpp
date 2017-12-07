#include <stdio.h>
#include "rf.hpp"
#include "settings.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

el::Configurations conf(LOGCONFIG);

int main() {
    el::Loggers::reconfigureAllLoggers(conf);
    setupRF();
    rfMessage *m;
    m = recvRF();
    LOG(INFO) << "Message received: <" << m->data << ">";
    return 0;
}
