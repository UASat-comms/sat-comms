#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <RH_RF69.h>
#include <RH_RF95.h>

#define BOARD_LORASPI

#include "../RasPiBoards.h"

#define RF_FREQUENCY 	915.00
#define RF_GATEWAY_ID 	1
#define RF_NODE_ID 	10

RH_RF95 rf95(RF_CS_PIN, RF_IRQ_PIN);

volatile sig_atomic_t force_exit = false;

void sig_handler(int sig)
{
   printf("\n%s Break received, exiting!\n", __BASEFILE__);
   force_exit=true;
}

int main(int argc, const char* argv[])
{
   FILE *fp, *wp;
   size_t size;
   static unsigned long last_millis;

   fp = fopen(argv[1],"rb");
   wp = fopen("output", "wb");
   fseek(fp,0,SEEK_END);
   size = ftell(fp);
   fseek(fp,0,SEEK_SET);
   signal(SIGINT, sig_handler);
   printf( "%s\n", __BASEFILE__);
   printf("SIZE: %d\n",size);

   if (!bcm2835_init()) {
      fprintf( stderr, "%s bcm2835_init() Failed\n\n", __BASEFILE__);
      return 1;
   }

   printf( "RF95 CS=GPIO%d", RF_CS_PIN);

#ifdef RF_IRQ_PIN
   printf( ", IRQ=GPIO%d", RF_IRQ_PIN );
   pinMode(RF_IRQ_PIN, INPUT);
   bcm2835_gpio_set_pud(RF_IRQ_PIN, BCM2835_GPIO_PUD_DOWN);
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
	printf( "\nRF95 module seen OK!\r\n");

#ifdef RF_IRQ_PIN
	rf95.available();
	bcm2835_gpio_ren(RF_IRQ_PIN);
#endif
	rf95.setTxPower(14, false);
	rf95.setFrequency( RF_FREQUENCY );
	rf95.setThisAddress(RF_NODE_ID);
	rf95.setHeaderFrom(RF_NODE_ID);
	rf95.setHeaderTo(RF_GATEWAY_ID);

	printf("RF95 node #%d init OK @ %3.2fMHz\n", RF_NODE_ID, RF_FREQUENCY);

	last_millis = millis();
	int i = 0;
	int stop = 0;

	while(!force_exit && !stop){
	   if ( millis() - last_millis > 400) {
		int j;
		uint8_t lendata;
		if (size - (rf95.txGood()*RH_RF95_MAX_MESSAGE_LEN) > RH_RF95_MAX_MESSAGE_LEN){
			lendata = RH_RF95_MAX_MESSAGE_LEN;
		}
		else{
			lendata = (size - (rf95.txGood()*RH_RF95_MAX_MESSAGE_LEN));
			stop = 1;
		}
		uint8_t data[lendata];
		for (j=0;j<lendata;j++){
			char c = fgetc(fp);
			data[j]=c;
			fputc(c,wp);
			i++;
		}
		printf("Sending: %s\nLENGTH: %d\n",data,lendata);
		rf95.send(data,lendata);
		rf95.waitPacketSent();
		printf("Packets sent: %d\n", rf95.txGood());
		last_millis = millis();
	   }
	}
	fclose(fp);
	fclose(wp);
   }
}
