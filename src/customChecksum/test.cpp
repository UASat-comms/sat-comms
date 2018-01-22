#include <stdio.h>
#include "checksum.hpp"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP
el::Configurations conf(LOGCONFIG);

int main(int argc, const char **argv) {
     el::Loggers::reconfigureAllLoggers(conf);

     if(argc != 2) {
          LOG(FATAL) << "Must enter filename to generate checksum on!";
     }

     FILE *fp = fopen(argv[1], "rb");

     LOG(INFO) << "Checksum is " << (int) checksum(fp);

     fclose(fp);
     return 0;
}
