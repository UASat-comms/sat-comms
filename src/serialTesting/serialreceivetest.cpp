
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cinttypes>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "settings.h"
#include "serial.hpp"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, const char **argv) {
     el::Configurations conf(LOGCONFIG);
     el::Loggers::reconfigureAllLoggers(conf);     
     LOG(INFO) << "Detected baud rate: <" << BAUD_RATE << ">";
     wiringPiSetup();
     receiveFile();
}
