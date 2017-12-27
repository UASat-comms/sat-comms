
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
     char *fileData = receiveFile();
     int fileSize = strlen(fileData); 
     LOG(INFO) << "attempting to write file data to 'RECD_data'...";
     FILE *fp = fopen("RECD_data", "w");
     if(fp == NULL) {
          LOG(FATAL) << "Unable to open file.";
     }
     for(int i = 0; i < fileSize; i++) fputc(fileData[i], fp);
     LOG(INFO) << "File data written.";
     fclose(fp);
}
