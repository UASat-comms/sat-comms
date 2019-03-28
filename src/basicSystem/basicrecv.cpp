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

#include "schifra_galois_field.hpp"
#include "schifra_sequential_root_generator_polynomial_creator.hpp"
#include "schifra_reed_solomon_decoder.hpp"
#include "schifra_reed_solomon_file_decoder.hpp"

using namespace std;

// Required macro to initialize the easylogging module.
INITIALIZE_EASYLOGGINGPP

/* Finite Field Parameters */
const std::size_t field_descriptor                = FIELD_DESCRIPTOR;
const std::size_t generator_polynomial_index      = GENERATOR_POLYNOMIAL_INDEX;
const std::size_t generator_polynomial_root_count = GENERATOR_POLYNOMIAL_ROOT_COUNT;

/* Reed Solomon Code Parameters */
const std::size_t code_length = CODE_LENGTH;
const std::size_t fec_length  = FEC_LENGTH;
const std::size_t data_length = DATA_LENGTH;

/* Instantiate Finite Field and Generator Polynomials */
const schifra::galois::field field(field_descriptor,
                                  schifra::galois::primitive_polynomial_size06,
                                  schifra::galois::primitive_polynomial06);

schifra::galois::field_polynomial generator_polynomial(field);

/* Instantiate Decoder (Codec) */
typedef schifra::reed_solomon::decoder<code_length,fec_length> decoder_t;
typedef schifra::reed_solomon::file_decoder<code_length,fec_length> file_decoder_t;

void setup() {
     printf("-----> %s running | setting up..\n", __BASEFILE__);
     // Configure logging module.
     el::Configurations conf(LOGCONFIG);
     el::Loggers::reconfigureAllLoggers(conf);
     // Configure wiringpi for serial comms.
     wiringPiSetup();
     // Configure bcm2835 for RF comms.
     setupRF();
     LOG(DEBUG) << "LASER/UV pin is: " << LASER_UV_SELECT;
     pinMode(LASER_UV_SELECT, OUTPUT);
     digitalWrite(LASER_UV_SELECT, LOW);
     LOG(INFO) << "Setup Complete!";
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

          /* Need to store the checkum, but we have to see if FEC work is
             required first.
          */
          string recdchecksum = checksum(fdata, fsize);

          /* If FEC is enabled, we need to decode the file FIRST, then
             compare the checksums from the decoded file and the received
             checksum over RF comms. To make things simple, we use the whole
             file decoder code from schifra, so we need to write that data
             to a file.
          */
          if(FEC_ENABLED) {
               LOG(DEBUG) << "Writing data to file so FEC can be ran..";
               FILE *fp = fopen(FEC_FILE_NAME, "w");
               for(int i = 0; i < fsize; i++) {
                    fputc(fdata[i], fp);
               }
               fclose(fp);
               LOG(DEBUG) << "Data written to <" << FEC_FILE_NAME << ">";

               LOG(DEBUG) << "Decoding file..";
               const decoder_t rs_decoder(field, generator_polynomial_index);
               file_decoder_t(rs_decoder, FEC_FILE_NAME, FEC_FILE_DECODED_NAME);
               LOG(DEBUG)
                    << "File decoded.  Ouput file: <"
                    << FEC_FILE_DECODED_NAME << ">";

               LOG(DEBUG) << "Reading decoded file for checksum..";
               ifstream tempreader(FEC_FILE_DECODED_NAME);
               recdchecksum = checksum(tempreader);  // Get checksum if FEC is enabled.
               LOG(DEBUG) << "Checksum calculated.";
               tempreader.close();
          } else {
               recdchecksum = checksum(fdata, fsize); // Get checksum if FEC not enabled.
          }

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
          // Just rename the decoded file if FEC was enabled.
          if(FEC_ENABLED) {
               stringstream movecommand;
               movecommand
                    << "mv "
                    << FEC_FILE_DECODED_NAME
                    << " " << "RECD_DATA";
               system(movecommand.str().c_str());
          // Need to write the data if FEC not enabled.
          } else {
               LOG(INFO) << "attempting to write file data to 'RECD_data'...";
               FILE *fp = fopen("RECD_DATA", "w");
               for(int i = 0; i < fsize; i++) {
                    fputc(fdata[i], fp);
               }
               fclose(fp);
          }
     } else {
          LOG(FATAL) << "All attempts failed!";
     }

     // Free resources
     free(fdata);

     // Close down resources
     setdown();

     return 0;
}
