#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <RH_RF69.h>
#include <RH_RF95.h>

#define BOARD_LORASPI

#include "../RasPiBoards.h"

#define RF_FREQUENCY 	915.00
#define RF_NODE_ID 	1
#define RF_GATEWAY_ID	10

RH_RF95 rf95(RF_CS_PIN, RF_IRQ_PIN);

volatile sig_atomic_t force_exit = false;

void sig_handler(int sig)
{
   printf("\n%s Break received, exiting!\n", __BASEFILE__);
   force_exit=true;
}

int main(int argc, const char* argv[])
{

   signal(SIGINT, sig_handler);
   printf( "%s\n", __BASEFILE__);

   if (!bcm2835_init()) {
      fprintf( stderr, "%s bcm2835_init() Failed\n\n", __BASEFILE__);
      return 1;
   }

   printf( "RF95 CS=GPIO%d", RF_CS_PIN);

#ifdef RF_IRQ_PIN
   printf( ", IRQ=GPIO%d", RF_IRQ_PIN );
   pinMode(RF_IRQ_PIN, INPUT);
   bcm2835_gpio_set_pud(RF_IRQ_PIN, BCM2835_GPIO_PUD_DOWN);
   bcm2835_gpio_ren(RF_IRQ_PIN);
#endif

#ifdef RF_RST_PIN
   printf( ", RST=GPIO%d", RF_RST_PIN );
   pinMode(RF_RST_PIN, OUTPUT);
   digitalWrite(RF_RST_PIN, LOW);
   bcm2835_delay(150);
   digitalWrite(RF_RST_PIN, HIGH);
   bcm2835_delay(100);
#endif

   if (!rf95.init()) {
	fprintf( stderr, "\nRF95 module init failed, Please verify wiring/module\n");
   } else {

	rf95.setTxPower(14, false);
	rf95.setFrequency( RF_FREQUENCY );
	rf95.setThisAddress(RF_NODE_ID);
	rf95.setHeaderFrom(RF_NODE_ID);
	rf95.setHeaderTo(RF_GATEWAY_ID);
	rf95.setPromiscuous(true);
	rf95.setModeRx();

	printf("RF95 node #%d init OK @ %3.2fMHz\n", RF_NODE_ID, RF_FREQUENCY);
	printf("Listening packet...\n");

	while(!force_exit){
#ifdef RF_IRQ_PIN
	   if(bcm2835_gpio_eds(RF_IRQ_PIN)) {
		bcm2835_gpio_set_eds(RF_IRQ_PIN);
#endif
		if (rf95.available()){
			uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
			uint8_t len = sizeof(buf);
			uint8_t ack[] = "ACK";
			uint8_t lenack = sizeof(ack);
			uint8_t from = rf95.headerFrom();
			uint8_t to = rf95.headerTo();
			uint8_t id = rf95.headerId();
			uint8_t flags = rf95.headerFlags();
			int8_t rssi = rf95.lastRssi();

			if(rf95.recv(buf,&len)){
			  printf("Packet[%02d] #%d => #%d %ddB: ",len,from,to,rssi);
			  printbuffer(buf, len);
			  uint8_t ack[] = "ACK";
			  uint8_t lenack = sizeof(ack);
			  printf("Sending %02d bytes to node #%d => ",lenack,RF_GATEWAY_ID);
			  printbuffer(ack,lenack);
			  printf("\n");
			  rf95.send(ack,lenack);
			  rf95.waitPacketSent();
			} else{
			  printf("receive failed");
			}
			printf("\n");
		   }
#ifdef RF_IRQ_PIN
		}
#endif
		bcm2835_delay(5);
   	}
   }
   printf("\n%s Ending \n", __BASEFILE__ );
   bcm2835_close();
   return 0;
}
