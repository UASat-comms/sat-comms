#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <cstddef>

#include "schifra_galois_field.hpp"
#include "schifra_galois_field_polynomial.hpp"
#include "schifra_sequential_root_generator_polynomial_creator.hpp"
#include "schifra_reed_solomon_encoder.hpp"
#include "schifra_reed_solomon_decoder.hpp"
#include "schifra_reed_solomon_block.hpp"
#include "schifra_error_processes.hpp"
#include "schifra_reed_solomon_file_encoder.hpp"

#include "rf.hpp"
#include "settings.h"
#include "easylogging++.h"
#include "serial.hpp"
#include "checksum.hpp"

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

/* Instantiate Encoder and Decoder (Codec) */
typedef schifra::reed_solomon::encoder<code_length,fec_length,data_length> encoder_t;
typedef schifra::reed_solomon::file_encoder<code_length,fec_length> file_encoder_t;

void setup() {
     printf("-----> %s running | setting up..\n", __BASEFILE__);
	// Configure logging module.
	el::Configurations conf(LOGCONFIG);
	el::Loggers::reconfigureAllLoggers(conf);
	// Configure wiringpi for serial comms.
	wiringPiSetup();
	// Configure bcm2835 for RF comms.
	setupRF();
     LOG(INFO) << "Setup complete.";
}

int main(int argc, char **argv) {
	setup();

	// Make sure we enter the name of file we want to send.
	if(argc < 2) {
		LOG(FATAL) << "Must enter name of file to send.";
	}

	/* Default fname to given command-line argument. It will be changed if
	compression is enabled and/or FEC is enabled. */
	string fname = argv[1];

	// Compress the file we want to send if compression enabled.
	if(COMPRESSION_ENABLED) {
		LOG(DEBUG) << "Compressing file..";
		fname = COMP_FILE_NAME; // Make sure to use transmit the compressed file.
        	stringstream compcommand;
		compcommand << "zip ";
		if(USE_MAX_COMPRESSSION) {
			compcommand << "-9 ";
		}
		compcommand << COMP_FILE_NAME << " ";
		compcommand << argv[1];
		system(compcommand.str().c_str()); // Do a system call to compress file.
		LOG(DEBUG) << "Compression complete. Compressed file name is: " << COMP_FILE_NAME;
	}

     // Apply Forward-Error Correction if enabled.
     if(FEC_ENABLED) {
          if(
               !schifra::make_sequential_root_generator_polynomial(
                    field,
                    generator_polynomial_index,
                    generator_polynomial_root_count,
                    generator_polynomial)
          ) {
               LOG(FATAL) << "Failed to create sequential root generator!";
          } else {
               LOG(DEBUG) << "Starting RS file encoding..";
               const encoder_t rs_encoder(field, generator_polynomial);
               file_encoder_t(rs_encoder, fname, FEC_FILE_NAME);
               LOG(DEBUG) << "File RS encoded.";
               fname = FEC_FILE_NAME;
          }
     }

	// Get the file checksum.
     ifstream file(fname);
     string fchecksum = checksum(file);
     file.close();
     LOG(DEBUG) << "File checksum for <" << fname << "> is: " << fchecksum;


     // Get the file size.
     file.open(fname);
     file.seekg(0, file.end);
     int fsize = file.tellg();
     printf("--%d--\n", fsize);
     file.seekg(0, file.beg);
     LOG(DEBUG) << "File size for <" << fname << "> is: <" << fsize << ">";

     /* Put the file checksum and file size into a single string. The RF
      * hardware can send up to 255 bytes in a single packet. The checksum is
      * always 8 bytes and the file size will only take around 15 bytes max.
      */
     stringstream metadata;
     metadata << fchecksum << fsize;

     // transmit the file checksum and file size the Rx side.
	sendRF((char *) metadata.str().c_str());
	LOG(INFO) << "File checksum and size transmitted.";

     // Go ahead and read file data while letting Rx side process checksum
     // and file size.
     FILE *fp = fopen(fname.c_str(), "r");
     char *fdata = (char *) malloc(sizeof(char) * (fsize + 1));
     fdata[fsize] = '\0';
     for(int i = 0; i < fsize; i++)
          fdata[i] = fgetc(fp);
     fclose(fp);

	// Wait for the receiving side to give us the go-ahead to transmit.
	char *sendprompt = recvRF();
	if(strcmp(sendprompt, "GOAHEAD") != 0) {
		LOG(FATAL) << "Receiving HW had an issue and could not receive file.";
	} else {
		LOG(DEBUG) << "Rx side is good to receive file.";
	}

	// Transmit the file via serial.
	bcm2835_delay(500);
	LOG(DEBUG) << "Doing initial transmit attempt..";
	transmitData(fdata, fsize);

	/* See if the checksum matches. Try again until correct or
	 * until try limit is reached.
	*/
	char *reply = recvRF();
	int tryCount = 1;
	int success = 1;
	while((strcmp(reply, "GOOD") != 0) && tryCount < TRY_LIMIT) {
		bcm2835_delay(1000);
		transmitData(fdata, fsize);
		++tryCount;
          bcm2835_delay(1000);
		reply = recvRF();
		if(strcmp(reply, "GOOD") == 0) {
			success = 1;
			LOG(DEBUG) << "Rx side said transmission was good.";
		} else {
			LOG(DEBUG) << "Rx side said transmission was bad.";
			success = 0;
		}
	}

	if(success) {
		LOG(INFO)
			<< "File transmission was successful! No. of attempts: "
			<< tryCount;
	} else {
		LOG(INFO)
			<< "File transmission failed! Max attempt limit of "
			<< tryCount << " reached.";
	}
	return 0;
}
