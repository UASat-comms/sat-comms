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
     
     uint8_t data[] = "test";
     uint8_t len = sizeof(data);

     LOG(DEBUG) << "Starting packet transmission.";
     rf95.send(data, len);
     LOG(DEBUG) << "Waiting until transmission finishes..";
     rf95.waitPacketSent();
     LOG(DEBUG) << "Packet sent.";

     return 0;
}
