#include "rf.hpp"

// Create driver instance.
RH_RF95 rf95(RF_CS_PIN, RF_IRQ_PIN);

rfMessage *createRFMessage(char *str) {
     if(strlen(str) > 255)
          LOG(FATAL) << "RF message must be between 0 and 255 chars. (8-bit unsigned)";
     rfMessage *mess = (rfMessage *) malloc(sizeof(rfMessage));
     mess->data = (uint8_t *) str;
     mess->len = (uint8_t) (strlen((const char *) str) + 1); // Needs to include null char.
     LOG(DEBUG) << "RF Message created.";
     return mess;
}

void setupRF() {
     if(!bcm2835_init()) {
          LOG(FATAL) << "bcm2835 failed to initialize.";
     }

     pinMode(RF_RST_PIN, OUTPUT);
     digitalWrite(RF_RST_PIN, HIGH);
     // Manual reset of RF95 module (HW reset).
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

     rf95.setTxPower(RF_TX_POWER, false);
     LOG(DEBUG) << "rf95 transmission power set with power level: <" << RF_TX_POWER << ">";
}

void closeRF() {
     bcm2835_close();
}

void sendRF(rfMessage *mess) {
     LOG(DEBUG) << "Starting RF packet transmission.";
     rf95.send(mess->data, mess->len);
     LOG(DEBUG) << "Waiting until transmission finishes..";
     rf95.waitPacketSent();
     LOG(INFO) << "Packet sent.";
}

rfMessage *recvRF() {
     LOG(DEBUG) << "Starting RF packet receive.";
     while(1) {
          // See if the RF HW module has received a message we can grab.
          if(rf95.available()) {
               uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
               uint8_t len = sizeof(buf);
               if(rf95.recv(buf, &len)) {
                    LOG(INFO) << "Succeeded in grabbing RF message from RF HW.";
                    rfMessage *mess = createRFMessage((char *) buf);
                    LOG(DEBUG) << "Saved received RF message.";
                    return mess;
               } else {
                    LOG(FATAL) << "Failed to grab RF message from RF HW!";
               }
          } else {
               LOG(DEBUG) << "No message available from RF HW yet..";
               bcm2835_delay(500);
          }
     }
}
