#include "rf.hpp"

// Create driver instance.
RH_RF95 driver(RF_CS_PIN, RF_IRQ_PIN);

rfMessage *createRFMessage(char *str) {
     if(strlen(str) > 255)
          LOG(FATAL) << "RF message must be between 0 and 255 chars. (8-bit unsigned)";
     rfMessage *mess = (rfMessage *) malloc(sizeof(rfMessage));
     mess->data = (uint8_t *) str;
     mess->len = (uint8_t) strlen((const char *) str);
     LOG(DEBUG) << "RF Message created.";
     return mess;
}

void transmitModeRF() {
     LOG(DEBUG) << "Setting up RF for transmit...";
#ifdef RF_IRQ_PIN
     // IRQ input pull-down setup.
     pinMode(RF_IRQ_PIN, INPUT);
     bcm2835_gpio_set_pud(RF_IRQ_PIN, BCM2835_GPIO_PUD_DOWN);
     LOG(DEBUG) << "RF_IRQ_PIN setup complete. Pin: <" << RF_IRQ_PIN << ">";
#endif

#ifdef RF_RST_PIN
     // Do a reset pulse.
     pinMode(RF_RST_PIN, OUTPUT);
     digitalWrite(RF_RST_PIN, LOW);
     bcm2835_delay(150);
     digitalWrite(RF_RST_PIN, HIGH);
     bcm2835_delay(100);
     LOG(DEBUG) << "RF_RST_PIN setup complete. Pin: <" << RF_RST_PIN << ">";
#endif

     if(!driver.init()) {
          LOG(FATAL) << "RF95 module init failed.";
     } else {
          LOG(DEBUG) << "RF95 module init succeeded.";
     }

#ifdef RF_IRQ_PIN
     driver.available();
     bcm2835_gpio_ren(RF_IRQ_PIN);
#endif

     driver.setTxPower(TX_POWER, false);
     LOG(DEBUG) << "TX_POWER set: <" << TX_POWER << ">";
     driver.setFrequency(RF_FREQUENCY);
     LOG(DEBIG) << "RF_FREQUENCY set: <" << RF_FREQUENCY << ">";
     driver.setThisAddress(RF_NODE_ID);
     driver.setHeaderFrom(RF_NODE_ID);
     LOG(DEBUG) << "Address and HeaderFrom set: <" << RF_NODE_ID << ">";
     driver.setHeaderTo(RF_GATEWAY_ID);
     LOG(DEBUG) << "HeaderTo set: <" << RF_GATEWAY_ID << ">";

     LOG(INFO) << "RF configured for transmit.";
}

void receiveModeRF() {
     LOG(DEBUG) << "Setting up RF for receive...";
#ifdef RF_IRQ_PIN
     pinMode(RF_IRQ_PIN, INPUT);
     bcm2835_gpio_set_pud(RF_IRQ_PIN, BCM2835_GPIO_PUD_DOWN);
     bcm2835_gpio_ren(RF_IRQ_PIN);
     LOG(DEBUG) << "RF_IRQ_PIN setup complete. Pin: <" << RF_IRQ_PIN << ">";
#endif

#ifdef RF_RST_PIN
     pinMode(RF_RST_PIN, OUTPUT);
     digitalWrite(RF_RST_PIN, LOW );
     bcm2835_delay(150);
     digitalWrite(RF_RST_PIN, HIGH );
     bcm2835_delay(100);
     LOG(DEBUG) << "RF_RST_PIN setup complete. Pin: <" << RF_RST_PIN << ">";
#endif

     if(!driver.init()) {
          LOG(FATAL) << "RF95 module init failed.";
     } else {
          LOG(DEBUG) << "RF95 module init succeeded.";
     }

     driver.setTxPower(TX_POWER, false);
     LOG(DEBUG) << "TX_POWER set: <" << TX_POWER << ">";
     driver.setFrequency(RF_FREQUENCY);
     LOG(DEBUG) << "RF_FREQUENCY set: <" << RF_FREQUENCY << ">";
     driver.setThisAddress(RF_NODE_ID);
     driver.setHeaderFrom(RF_NODE_ID);
     LOG(DEBUG) << "Address and HeaderFrom set: <" << RF_NODE_ID << ">";
     driver.setPromiscuous(true);
     driver.setModeRx();
     LOG(DEBUG) << "Promiscuous mode enabled and mode set to Rx.";

     LOG(INFO) << "RF configured for receive.";
}

void transmitRF(rfMessage m) {
     driver.send(m->data, m->len);
     driver.waitPacketSent();
     LOG(INFO) << "Message transmitted.";
}

rfMessage *receiveRF() {
     uint8_t *buf   = malloc(sizeof(uint8_t) * RH_RF95_MAX_MESSAGE_LEN);
     uint8_t len    = RH_RF95_MAX_MESSAGE_LEN;
     uint8_t from   = driver.headerFrom();
     uint8_t to     = driver.headerTo();
     uint8_t id     = driver.headerId();
     uint8_t flags  = driver.headerFlags();;
     int8_t rssi    = driver.lastRssi();

     if(driver.recv(buf, &len)) {
          LOG(DEBUG) << "RF message received.";
     } else {
          LOG(FATAL) << "RF messaged failed to receive.";
     }

     rfMessage *mess = malloc(sizeof(rfMessage));
     mess->data = buf;
     mess->len = len;

     LOG(DEBUG) << "Message received";
     return mess;
}

void closeRF() {
     bcm2835_close();
}
