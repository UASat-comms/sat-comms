#include "settings.h"
#include "easylogging++.h"
#include "rf.hpp"
#include "serial.hpp"

INITIALIZE_EASYLOGGINGPP

void setup() {
     el::Configurations conf(LOGCONFIG);
     el::Loggers::reconfigureAllLoggers(conf);
     wiringPiSetup();
     if(!bcm2835_init()) {
          LOG(FATAL) << "Could not initialize bcm2835 library.";
     }
}

int main(void) {
     
     closeRF();
     return 0;
}
