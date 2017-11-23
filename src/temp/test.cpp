#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <RHReliableDatagram.h>
#include <RH_RF95.h>

#define CLIENT 1
#define SERVER 2

RH_RF95 rf95(RPI_V2_GPIO_P1_24, RPI_V2_GPIO_P1_22);
RHReliableDatagram manager(rf95, SERVER); 

int main(int argc, const char **argv[]) {
    if(!bcm2835_init()) {
        printf("nope\n");
        return 1;
    }
    if(!manager.init()) {
        printf("nope again\n");
        return 2;
    }
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    while(true) {
        uint8_t len = sizeof(buf);
        uint8_t from, to, id, flags;

        if(manager.available()) {
            if(manager.recvfromAck(buf, &len, &from)) {
                printf((char *) buf);
            }
        bcm2835_close();
        }
    }
}
