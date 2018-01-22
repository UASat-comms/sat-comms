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

	// Get the file checksum.
     ifstream file(fname);
     string fchecksum = checksum(file);
     LOG(DEBUG) << "File checksum for <" << fname << "> is: " << fchecksum;

     // Get the file size.
     file.seekg(0, file.end);
     size_t fsize = file.tellg();
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
     FILE *fp = fopen(fname, "r");
     char *fdata = (char *) malloc(sizeof(char) * (fileSize + 1));
     fdata[fileSize] = '\0';
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
