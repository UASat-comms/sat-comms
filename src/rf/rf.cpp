#include "rf.hpp"

// Create driver instance.
RH_RF95 driver(RF_CS_PIN, RF_IRQ_PIN);

void setupRF() {
     LOG(DEBUG) << "Setting up RF interface...";

     LOG(DEBUG) << "Starting with pin assignments: ";
     if(!bcm2835_init())
          LOG(FATAL) << "Could not initialize bcm2835 library.";
     LOG(DEBUG) << "CS Pin (GPIO): <" << RF_CS_PIN << ">";
     LOG(DEBUG) << "IRQ Pin (GPIO): <" << RF_IRQ_PIN << ">";
     pinMode(RF_IRQ_PIN, INPUT);
     bcm2835_gpio_set_pud(RF_IRQ_PIN, BCM2835_GPIO_PUD_DOWN);
     bcm2835_gpio_ren(RF_IRQ_PIN);
     LOG(DEBUG) << "RST Pin (GPIO): <" << RF_RST_PIN << ">";
     pinMode(RF_RST_PIN, OUTPUT);
     digitalWrite(RF_RST_PIN, LOW);
     bcm2835_delay(150);
     digitalWrite(RF_RST_PIN, HIGH);
     bcm2835_delay(100);
     LOG(DEBUG) << "Pins assigned.";

     LOG(DEBUG) << "Setting RF settings: ";
     LOG(DEBUG) << "RF Tx Power: <" << TX_POWER << ">";
     driver.setTxPower(TX_POWER, false);
     LOG(DEBUG) << "RF Frequency: <" << RF_FREQUENCY << ">";
     driver.setFrequency(RF_FREQUENCY);
     LOG(DEBUG) << "RF Node ID: <" << RF_NODE_ID << ">";
     driver.setThisAddress(RF_NODE_ID);
     driver.setHeaderFrom(RF_NODE_ID);
     driver.setPromiscuous(true);
     driver.setModeRx();
     LOG(DEBUG) << "RF settings set.";

     LOG(DEBUG) << "Done setting up RF interface.";
}

char *receiveRF() {
     uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
     uint8_t len    = sizeof(buf);
     uint8_t from   = driver.headerFrom();
     uint8_t to     = driver.headerTo();
     uint8_t id     = driver.headerId();
     uint8_t flags  = driver.headerFlags();;
     int8_t rssi    = driver.lastRssi();

     time_t start = time(NULL);
     // wait 10s for a message.
     while(time(NULL) - start < 10) {
          if(bcm2835_gpio_eds(RF_IRQ_PIN)) {
               bcm2835_gpio_set_eds(RF_IRQ_PIN);
               if(driver.available()) {
                    if(driver.recv(buf, &len)) {
                         LOG(DEBUG) << "RECEIVED: " << buf;
                    } else {
                         LOG(FATAL) << "RF receive failed.";
                    }
               }
          }
          // Let OS do some stuff real quick.
          bcm2835_delay(5);
     }
}

void transmitRF(char *s, int len) {

}
