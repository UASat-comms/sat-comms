#include <cstdio>
#include "rf.hpp"
#include "settings.h"
#include "easylogging++.h"
#include "serial.hpp"
#include "picosha2.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

std::string getSHA256Hash(const char *str) {
  std::stringstream s;
  s << str;
  return picosha2::hash256_hex_string(s.str());
}

std::string getSHA256Hash(std::string str) {
  return picosha2::hash256_hex_string(str);
}

void setup() {    
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
    
    // Make sure we enter the name of file we want to send.
    if(argc < 2) {
        LOG(FATAL) << "Must enter name of file to send.";
    }
           

    return 0;
}
