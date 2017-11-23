#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <RHReliableDatagram.h>
#include <RH_RF95.h>

#define CLIENT 1
#define SERVER 2

RH_RF95 rf95(RPI_V2_GPIO_P1_24, RPI_V2_GPIO_P1_22);
RHReliableDatagram manager(rf95, CLIENT);

int main(int argc, const char **argv) {
    if(!bcm2835_init()) {
        printf("nope\n");
        return 1;
    }
    if(!manager.init()) {
        printf("nope again\n");
        return 2;
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
