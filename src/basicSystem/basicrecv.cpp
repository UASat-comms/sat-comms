#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>

#include "rf.hpp"
#include "settings.h"
#include "easylogging++.h"
#include "serial.hpp"
#include "checksum.hpp"

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

void setdown() {
     bcm2835_close();
}

int main(int argc, const char **argv) {
     setup();

     // Use RF HW to get the checksum and file size;
     LOG(DEBUG) << "Waiting for RF checksum & file size transmission..";
     string metadata(recvRF());
     string fchecksum = metadata.substr(0, 8);
     string stringfsize = metadata.substr(8, metadata.length() - 8);

     LOG(DEBUG) << "Checksum is: " << fchecksum;
     size_t fsize = (size_t) atoi(stringfsize.c_str());
     LOG(DEBUG) << "File size is: " << fsize;

     // Let the Tx side know that the Rx side is okay to receive the file.
     bcm2835_delay(1500);
     sendRF((char *) "GOAHEAD");

     /* Receive data and make sure it matches checksums.  If not, try again
     * until the limit is reached.
     */
     int tryCount = 0;
     int success = 0;
     char *fdata;
     while(tryCount < TRY_LIMIT) {
          // Get the file data over serial.
          fdata = receiveData(fsize);

          FILE *fp = fopen("TEMP.file", "w");
          for(int i = 0; i < fsize; i++) {
               fputc(fdata[i], fp);
          }
          fclose(fp);

          // Calculate the checksum.
          string recdchecksum = checksum(fdata, fsize);

          LOG(DEBUG) << "Received checksum is: " << recdchecksum;

          // Compare checksums.
          LOG(DEBUG) << "fchecksum: <" << fchecksum << "> | recdchecksum: <" << recdchecksum << ">";
          if(fchecksum.compare(recdchecksum) == 0) {
               bcm2835_delay(3000);
               LOG(DEBUG) << "Checksums match!";
               sendRF((char *) "GOOD");
               success = 1;
               break;
          } else {
               bcm2835_delay(3000);
               LOG(DEBUG) << "Checksums do not match!";
               ++tryCount;
               sendRF((char *) "BAD");
               recdchecksum.clear();
               free(fdata);
          }
     }

     if(success) {
          LOG(INFO) << "attempting to write file data to 'RECD_data'...";
          FILE *fp = fopen("RECD_DATA", "w");
          for(int i = 0; i < fsize; i++) {
               fputc(fdata[i], fp);
          }
          fclose(fp);
     } else {
          LOG(FATAL) << "All attempts failed!";
     }

     // Free resources
     free(fdata);

     // Close down resources
     setdown();

     return 0;
}
