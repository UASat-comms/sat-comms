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
     uint8_t data[] = "HELLO THERE!";
     uint8_t len = sizeof(data);
     transmitRF(data, len);
     closeRF();
     return 0;
}
