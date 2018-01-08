
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
     wiringPiSetup();

     if(argc < 2) {
          LOG(FATAL) << "Must enter file size as command-line argument!";
     }

     LOG(INFO) << "Detected baud rate: <" << BAUD_RATE << ">";

     int datalen = atoi(argv[1]);
     LOG(INFO) << "File size received as command-line argument: <" << datalen << ">";
     char *data = receiveData(datalen);

     LOG(INFO) << "attempting to write file data to 'RECD_data'...";
     FILE *fp = fopen("RECD_data", "w");
     if(fp == NULL) {
          LOG(FATAL) << "Unable to open file.";
     }
     for(int i = 0; i < datalen; i++) fputc(data[i], fp);
     LOG(INFO) << "File data written.";

     fclose(fp);

     return 0;
}
