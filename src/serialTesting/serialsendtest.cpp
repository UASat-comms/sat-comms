/* ======================================================
 * Author: Matt
 *
 * Description: Tests the serial communication on RPi.
 *
 * ======================================================
 */

#include <unistd.h>
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
     el::Configurations conf(LOGCONFIG);
     el::Loggers::reconfigureAllLoggers(conf);
     wiringPiSetup();

     LOG(INFO) << "Detected baud rate: <" << BAUD_RATE << ">";

     FILE *fp = fopen(argv[1], "r");
     int datalen = getFileSize(fp);
     char *data = (char *) malloc(sizeof(char) * (datalen + 1));
     data[datalen] = '\0';

     char c;
     LOG(INFO) << "File size is: <" << datalen << ">";
     for(int i = 0; i < datalen; i++) data[i] = fgetc(fp);
     fclose(fp);

     printf("PRESS ENTER TO CONTINUE TRANSMISSION ");
     scanf("%c", &c);
     transmitData(data, datalen);

     return 0;
}
