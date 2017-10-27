#include "settings.h"
#include "easylogging++.h"
#include "rf.hpp"

INITIALIZE_EASYLOGGINGPP

void setup() {
     el::Configurations conf(LOGCONFIG);
     el::Loggers::reconfigureAllLoggers(conf);
     if(!bcm2835_init()) {
          LOG(FATAL) << "Could not initialize bcm2835 library.";
     }
}

int main(void) {
     setup();
     transmitModeRF();
     char data[] = "sup dude.";
     rfMessage *m = createRFMessage(data);
     closeRF();
     return 0;
}
