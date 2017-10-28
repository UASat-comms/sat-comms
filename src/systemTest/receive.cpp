#include "settings.h"
#include "easylogging++.h"
#include "rf.hpp"
#include "serial.hpp"

INITIALIZE_EASYLOGGINGPP

int main(void) {
     el::Configurations conf(LOGCONFIG);
     el::Loggers::reconfigureAllLoggers(conf);
     wiringPiSetup();
     setupRF();
     receiveRF();
     closeRF();
     return 0;
}
