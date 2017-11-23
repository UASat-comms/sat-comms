#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <RHReliableDatagram.h>
#include <RH_RF95.h>

#include "settings.h"

#define CLIENT 1
#define SERVER 2

int main(int argc, const char **argv) {
    printf("CS: %d RST: %d IRQ: %d\n", RF_CS_PIN, RF_RST_PIN, RF_IRQ_PIN);
    
    if(!bcm2835_init()) {
        printf("nope\n");
        return 1;
    }
    
    RH_RF95 rf95(RF_CS_PIN, RF_IRQ_PIN);
     
    pinMode(RF_IRQ_PIN, INPUT);
    bcm2835_gpio_set_pud(RF_IRQ_PIN, BCM2835_GPIO_PUD_DOWN);

    pinMode(RF_RST_PIN, OUTPUT);
    digitalWrite(RF_RST_PIN, HIGH);
    bcm2835_delay(150);
    digitalWrite(RF_RST_PIN, LOW);
    bcm2835_delay(150);
    digitalWrite(RF_RST_PIN, HIGH);
    bcm2835_delay(150);
    
    if(!rf95.init()) {
        printf("nope first time\n");
        return -1;
    }
    
    RHReliableDatagram manager(rf95, CLIENT);
    
    if(!manager.init()) {
        printf("nope again\n");
        return 2;
    }
 
    rf95.available();
    bcm2835_gpio_ren(RF_IRQ_PIN);

    if(!rf95.setFrequency(RF_FREQUENCY)) {
        printf("bad freq!\n");
        return 3;
    }

    uint8_t data[] = "test";
    uint8_t len = sizeof(data);
    
    while(true) {
        if(manager.available()) {
            if(manager.sendtoWait(data, len, SERVER)) {
                printf("Sent!");
                bcm2835_close();
                return 0;
            }
            printf("%d\n", manager.retries());
        }
    }
}
