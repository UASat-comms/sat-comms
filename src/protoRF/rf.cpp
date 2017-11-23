#include "rf.hpp"

RHReliableDatagram setupRF(int CS_PIN, int IRQ_PIN, int RF_RST_PIN, int MY_ADDRESS, int RF_FREQUENCY) {
     // Define and assign our driver and manager.
     RH_RF95 rf95(RF_CS, RF_IRQ);
     RHReliableDatagram rf95Manager(rf95, MY_ADDRESS);

     // Reset the RF module.
     pinMode(RF_RST_PIN, OUTPUT);
     digitalWrite(RF_RST_PIN, HIGH);
     bcm2835_delay(150);
     digitalWrite(RF_RST_PIN, LOW);
     bcm2835_delay(150);
     digitalWrite(RF_RST_PIN, HIGH);
     bcm2835_delay(150);

     // Initialize the realiable driver.
     if(!rf95Manager.init()) {
          LOG(FATAL) << "rf95Manager failed!";
     }
     // Set the frequency (with driver).
     if(!rf95.setFrequency(RF_FREQUENCY)) {
          LOG(FATAL) << "Frequency setup failed!";
     }
     // Set the Tx power (with driver).
     if(!rf95.setTxPower(15, false)) {
          LOG(FATAL) << "Power setup failed!";
     }
     return rf95Manager;
}
