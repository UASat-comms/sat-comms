#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include "settings.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

el::Configurations conf(LOGCONFIG);
RH_RF95 rf95(RF_CS_PIN, RF_IRQ_PIN);
RHReliableDatagram manager(rf95, 2); //client is 2

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
/*
     if(!rf95.init()) {
          LOG(FATAL) << "rf95 driver failed to initialize.";
     }
*/

     if(!rf95.setFrequency(RF_FREQUENCY)) {
          LOG(FATAL) << "rf95 frequency failed to be set.";
     }
     
     rf95.setTxPower(23, false);
    
     if(!manager.init()) {
        LOG(FATAL) << "RHRaliableDatagram manager failed to initialze.";
    }
     
     uint8_t data[] = "test";
     uint8_t len = sizeof(data);
   
    manager.setRetries(25);

     LOG(DEBUG) << "Starting packet transmission.";
     if(manager.sendtoWait(data, len, 9)) {
        LOG(DEBUG) << "Packet sent.";
     } else {
        LOG(DEBUG) << "Server failed to acknowledge!";
     }
    bcm2835_close();
     return 0;
}
