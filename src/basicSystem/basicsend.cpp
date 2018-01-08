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
     myfile.close();
	return picosha2::hash256_hex_string(mystr.str());
}

int main(int argc, char **argv) {
	setup();

	// Make sure we enter the name of file we want to send.
	if(argc < 2) {
		LOG(FATAL) << "Must enter name of file to send.";
	}

	/* Default fname to given command-line argument. It will be changed if
	compression is enabled and/or FEC is enabled. */
	char *fname = argv[1];

	// Compress the file we want to send if compression enabled.
	if(COMPRESSION_ENABLED) {
		LOG(DEBUG) << "Compressing file..";
		fname = (char *) COMP_FILE_NAME; // Make sure to use transmit the compressed file.
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
	string checksum = getFileChecksum(fname);
	LOG(DEBUG) << "File checksum for " << fname << " is: " << checksum;

     // Get the file size.
     FILE *fp = fopen(fname, "r");
     int fileSize = getFileSize(fp);
     fclose(fp);
     char *stringFileSize = intToString(fileSize);

     /* Put the file checksum and filesize into a single string. The RF
     * hardware can send up to 255 bytes in a single packet. The checksum is
     * always 64 bytes and the file size will only take around 15 bytes. */
     stringstream fileData;
     fileData << checksum << stringFileSize;

     // transmit the file checksum and file size the Rx side.
	sendRF((char *) fileData.str().c_str());
	LOG(INFO) << "File checksum and size transmitted.";

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
	transmitFile((char *) fname);

	/* See if the checksum matches. Try again until correct or
	 * until try limit is reached.
	*/
	char *reply = recvRF();
	int tryCount = 1;
	int success = 1;
	while((strcmp(reply, "GOOD") != 0) && tryCount < TRY_LIMIT) {
		bcm2835_delay(1000);
		transmitFile((char *) fname);
		++tryCount;
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
