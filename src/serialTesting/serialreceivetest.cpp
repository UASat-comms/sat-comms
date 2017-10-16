
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cinttypes>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "settings.h"



int main(int argc, const char **argv) {
     wiringPiSetup();
     receiveFile();
}
