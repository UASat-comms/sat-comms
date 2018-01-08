#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>

#include "rf.hpp"
#include "settings.h"
#include "easylogging++.h"
#include "serial.hpp"
#include "picosha2.h"

using namespace std;

// Required macro to initialize the easylogging module.
INITIALIZE_EASYLOGGINGPP

void setup() {
     printf("-----> %s running | setting up..\n", __BASEFILE__);
     // Configure logging module.
     el::Configurations conf(LOGCONFIG);
     el::Loggers::reconfigureAllLoggers(conf);
     // Configure wiringpi for serial comms.
     wiringPiSetup();
     // Configure bcm2835 for RF comms.
     setupRF();
}

int main(int argc, const char **argv) {
     setup();

     // Use RF HW to get the checksum and file size;
     LOG(DEBUG) << "Waiting for RF checksum & file size transmission..";
     char *metadata = recvRF();
     int metadataSize = strlen(metadata);
     char *checksum = (char *) malloc(sizeof(char) * (64 + 1));
     checksum[64] = '\0';
     char *stringFileSize = (char *) malloc(sizeof(char) * (IDENTIFIER_LENGTH + 1));
     stringFileSize[IDENTIFIER_LENGTH] = '\0';
     for(int i = 0; i < metadataSize; i++) {
          if(i < 64)
               checksum[i] = metadata[i];
          else
               stringFileSize[i - 64] = metadata[i];
     }
     LOG(DEBUG) << "Checksum is: " << checksum;
     int fileSize = atoi(stringFileSize);
     LOG(DEBUG) << "File size is: " << fileSize;

     // Let the Tx side know that the Rx side is okay to receive the file.
     sendRF((char *) "GOAHEAD");

     /* Receive data and make sure it matches checksums.  If not, try again
     * until the limit is reached.
     */
     int tryCount = 0;
     int success = 0;
     char *fileData;
     while(tryCount < TRY_LIMIT) {
          // Get the file data over serial.
          fileData = receiveData(fileSize);
          stringstream temp;
          temp << fileData;

          // Calculate the checksum.
          string recdChecksum = picosha2::hash256_hex_string(temp.str());
          LOG(DEBUG) << "Received checksum is: " << recdChecksum;

          // Compare checksums.
          if(strcmp(checksum, recdChecksum.c_str()) == 0) {
               bcm2835_delay(2000);
               LOG(DEBUG) << "Checksums match!";
               sendRF((char *) "GOOD");
               success = 1;
               break;
          } else {
               bcm2835_delay(2000);
               LOG(DEBUG) << "Checksums do not match!";
               ++tryCount;
               sendRF((char *) "BAD");
          }
     }

     if(success) {
          LOG(INFO) << "attempting to write file data to 'RECD_data'...";
          FILE *fp = fopen("RECD_data", "w");
          if(fp == NULL) {
               LOG(FATAL) << "Unable to open file.";
          }
          for(int i = 0; i < fileSize; i++) fputc(fileData[i], fp);
          LOG(INFO) << "File data written.";
          fclose(fp);
     } else {
     LOG(FATAL) << "All attempts failed!";
     }
     return 0;
}
