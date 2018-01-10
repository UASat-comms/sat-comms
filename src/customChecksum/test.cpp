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

     file.seekg(0, file.end);
     LOG(DEBUG) << "C++ file length: <" << file.tellg() << ">";
     file.seekg(0, file.beg);

     LOG(INFO) << "Checksum is " << checksum(file);

     file.close();

     FILE *fp = fopen(argv[1], "rb");
     
     size_t fsize = 0; 
     fseek(fp, 0, SEEK_END);
     fsize = ftell(fp);
     LOG(DEBUG) << "C file length: <" << fsize << ">";
     fseek(fp, 0, SEEK_SET);
     
     char *fdata = (char *) malloc(sizeof(char) * (fsize + 1));
     fdata[fsize] = '\0';

     for(int i = 0; i < fsize; i++) {
          fdata[i] = fgetc(fp);
     }

     fclose(fp);

     LOG(INFO) << "Checksum is " << checksum(fdata, fsize);
     
     free(fdata);

     return 0;
}
