#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include "rf.hpp"
#include "settings.h"
#include "easylogging++.h"
#include "serial.hpp"
#include "picosha2.h"

using namespace std;

// Required macro to initialize the easylogging module.
INITIALIZE_EASYLOGGINGPP

void setup() {
    // Configure logging module.
    el::Configurations conf(LOGCONFIG);
    el::Loggers::reconfigureAllLoggers(conf);
    // Configure wiringpi for serial comms.
    wiringPiSetup();
    // Configure bcm2835 for RF comms.
    setupRF();
}

std::string getFileChecksum(const char *fname) {
     ifstream myfile(fname);
     stringstream mystr;
     string line;

     LOG(DEBUG) << "Reading file data to get SHA256 checksum...";
     if(myfile.is_open()) {
          while(getline(myfile, line)) {
               mystr << line << endl;
          }
     } else {
          LOG(FATAL) << "Error reading file data for checksum.";
     }
     return picosha2::hash256_hex_string(mystr.str());
}

int main(int argc, const char **argv) {
    setup();

    // Use RF HW to get the checksum.
    LOG(DEBUG) << "Waiting for RF checksum transmission..";
    char *checksum = recvRF();
    LOG(DEBUG) << "Checksum is: " << checksum;

    // Let the Tx side know that the Rx side is okay to receive the file.
    sendRF((char *) "GOAHEAD");

    /* Receive data and make sure it matches checksums.  If not, try again
     * until the limit is reached.
     */
    int tryCount = 0;
    stringstream temp;
    char *fileData;
    while(tryCount < TRY_LIMIT) {
         // Get the file data over serial.
         fileData = receiveFile();
         temp << fileData;
         char *recdChecksum = (char *) picosha2::hash256_hex_string(temp.str()).c_str();
         LOG(DEBUG) << "Received checksum is: " << recdChecksum;
         if(strcmp(checksum, recdChecksum) == 0) {
              LOG(DEBUG) << "Checksums match!";
              sendRF((char *) "GOOD");
              break;
         } else {
              LOG(DEBUG) << "Checksums do not match!";
              ++tryCount;
              sendRF((char *) "BAD");
         }
    }

    int fileSize = strlen(fileData);
    LOG(INFO) << "attempting to write file data to 'RECD_data'...";
    FILE *fp = fopen("RECD_data", "w");
    if(fp == NULL) {
         LOG(FATAL) << "Unable to open file.";
    }
    for(int i = 0; i < fileSize; i++) fputc(fileData[i], fp);
    LOG(INFO) << "File data written.";
    fclose(fp);

    return 0;
}
