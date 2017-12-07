
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// ============================================================================
// -------------------- Serial Interface Settings -----------------------------
#define INTERFACE             "/dev/serial0"      // Should be tyAMA0
#define IDENTIFIER_LENGTH     32                  // Size identifier.
#define BAUD_RATE             1000000             // 1MBaud ~ 1MHz

// ============================================================================
// -------------------- RF Interface Settings ---------------------------------
#define RF_FREQUENCY     915.00    // FCC allocated frequency band for hobbyists
#define RF_SERVER_ID     1
#define RF_CLIENT_ID     2
#define RF_GATEWAY_ID    1
#define RF_TX_POWER      14        // Range: 13-23

// -------------------- RF Pin Allocations ------------------------------------
// bcm2835 pin constants defined @:
// http://www.airspayce.com/mikem/bcm2835/group__constants.html#ga63c029bd6500167152db4e57736d0939
// What each pin is for defined @:
// https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=3&ved=0ahUKEwigvqff8dLXAhVizIMKHXQ6CBwQFgg4MAI&url=https%3A%2F%2Fcdn-learn.adafruit.com%2Fdownloads%2Fpdf%2Fadafruit-rfm69hcw-and-rfm96-rfm95-rfm98-lora-packet-padio-breakouts.pdf&usg=AOvVaw0QSAlez_2xZeBRtOOmf2BB
// NOTE: Hallard defined pinouts in RasPiBoards.h, but we're going to define
//       them here for the sake of clarity & ease of change.
#define RF_CS_PIN  RPI_V2_GPIO_P1_24
#define RF_IRQ_PIN RPI_V2_GPIO_P1_22
#define RF_RST_PIN RPI_V2_GPIO_P1_15

// -------------------- RF System States --------------------------------------
enum SystemStates {
     hello,
     helloBack,
     send,
     received,
     badTx,
     retryTx
};

// ============================================================================
#endif
