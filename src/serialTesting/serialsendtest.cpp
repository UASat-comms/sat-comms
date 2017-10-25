/* ======================================================
 * Author: Matt
 *
 * Description: Tests the serial communication on RPi.
 *
 * ======================================================
 */


#include <cstdlib>
#include <cstdio>
#include <cinttypes>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "settings.h"
#include "serial.hpp"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, const char **argv) {
     wiringPiSetup();
     transmitFile((char *) argv[1]);
     return 0;
}
