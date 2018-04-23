
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// ============================================================================
// -------------------- Serial Interface Settings -----------------------------
#define INTERFACE             "/dev/serial0"      // Should be tyAMA0
#define IDENTIFIER_LENGTH     32                  // Size identifier.
#define BAUD_RATE             1000000             // 1MBaud ~ 1MHz
#define TRY_LIMIT             5                   // No. of allowed attempts.

// ============================================================================
// -------------------- RF Interface Settings ---------------------------------
#define RF_FREQUENCY     915.00    // FCC allocated frequency band for hobbyists
#define RF_SERVER_ID     1
#define RF_CLIENT_ID     2
#define RF_GATEWAY_ID    1
#define RF_TX_POWER      7       // Lowest power required if using 3.3V DC Power pin!

// -------------------- RF Pin Allocations ------------------------------------
// bcm2835 pin constants defined @:
// http://www.airspayce.com/mikem/bcm2835/group__constants.html#ga63c029bd6500167152db4e57736d0939
// What each pin is for defined @:
// https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=3&ved=0ahUKEwigvqff8dLXAhVizIMKHXQ6CBwQFgg4MAI&url=https%3A%2F%2Fcdn-learn.adafruit.com%2Fdownloads%2Fpdf%2Fadafruit-rfm69hcw-and-rfm96-rfm95-rfm98-lora-packet-padio-breakouts.pdf&usg=AOvVaw0QSAlez_2xZeBRtOOmf2BB
// NOTE: Hallard defined pinouts in RasPiBoards.h, but we're going to define
//       them here for the sake of clarity & ease of change.
#define RF_CS_PIN  RPI_V2_GPIO_P1_24 // "Chip Select" pin.
#define RF_IRQ_PIN RPI_V2_GPIO_P1_22 // "Interrupt Request" pin.
#define RF_RST_PIN RPI_V2_GPIO_P1_15 // "Reset" pin.

// -------------------- Steering Pin Allocations & Stuff ------------------------------------
// For motor driver boards
#define IN1_UD 22
#define IN2_UD 23
#define IN3_UD 24
#define IN4_UD 25

#define IN1_LR 26
#define IN2_LR 27
#define IN3_LR 28
#define IN4_LR 29

const char connectionByte = 0b01010011;

// -------------------- Enable Pin Allocations ------------------------------------
// Laser and UV enable pins
// [TODO]: Choose pins
#define LASER_ENABLE 0
#define UV_ENABLE    2

// -------------------- Multiplexer Pin Allocations ---------------------------
#define LASER_UV_SELECT 25

// ============================================================================
// ---------------------------- Compression Settings --------------------------
#define COMPRESSION_ENABLED  0 // 1 Enables compression, 0 disables it.
#define USE_MAX_COMPRESSSION 1 // Generates smaller file but takes longer.
#define COMP_FILE_NAME       "TEMP.zip"
// ============================================================================


// ============================================================================
// -------------------------- Reed-Solomon Settings ---------------------------
#define FEC_ENABLED           0               // 0 = not enabled, 1 = enable
#define FEC_FILE_NAME         "TEMP.schifa"   // Temporary file name to house encoded data
#define FEC_FILE_DECODED_NAME "TEMP.decoded"  // Temporary file name to hosue decoded data (after FEC)

// Finite Field (Galois Field) Params.
#define FIELD_DESCRIPTOR                8
#define GENERATOR_POLYNOMIAL_INDEX      120
#define GENERATOR_POLYNOMIAL_ROOT_COUNT 64

// RS Codeword Params.
#define CODE_LENGTH 255
#define FEC_LENGTH  64
#define DATA_LENGTH (CODE_LENGTH-FEC_LENGTH)

// ============================================================================
#endif
