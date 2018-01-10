#include <iostream>
#include <fstream>
#include "checksum.hpp"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP
el::Configurations conf(LOGCONFIG);

int main(int argc, const char **argv) {
     el::Loggers::reconfigureAllLoggers(conf);

     if(argc != 2) {
          LOG(FATAL) << "Must enter filename to generate checksum on!";
     }

     std::ifstream file(argv[1]);
     if(!file.is_open()) {
          LOG(FATAL) << "Error opening file!";
     }

     LOG(INFO) << "Checksum is " << checksum(file);

     file.close();

     return 0;
}
