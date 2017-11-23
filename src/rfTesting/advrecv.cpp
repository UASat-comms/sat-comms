#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <RH_RF95.h>
#include "settings.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

el::Configurations conf(LOGCONFIG);
RH_RF95 rf95(RF_CS_PIN, RF_IRQ_PIN);

int main() {
    el::Loggers::reconfigureAllLoggers(conf);
     LOG(INFO) << "Starting program";

     if(!bcm2835_init()) {
          LOG(FATAL) << "bcm2835 failed to initialize.";
     }

     pinMode(RF_RST_PIN, OUTPUT);
     digitalWrite(RF_RST_PIN, HIGH);
     // manual reset
     digitalWrite(RF_RST_PIN, LOW);
     delay(10);
     digitalWrite(RF_RST_PIN, HIGH);
     delay(10);

     if(!rf95.init()) {
          LOG(FATAL) << "rf95 driver failed to initialize.";
     }

     if(!rf95.setFrequency(RF_FREQUENCY)) {
          LOG(FATAL) << "rf95 frequency failed to be set.";
     }
     
     rf95.setTxPower(23, false);

    while(1) {
        if(rf95.available()) {
            uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
            uint8_t len = sizeof(buf);
            if(rf95.recv(buf, &len)) {
                LOG(DEBUG) << "MESS_LEN: <" << (int) len << ">";
                LOG(DEBUG) << "MESS: " << buf;
                break;
            } else {
                LOG(FATAL) << "recv failed!";
            }
        }
        LOG(INFO) << "No message yet..";
        bcm2835_delay(500);
    }
    bcm2835_close();
     return 0;
}
