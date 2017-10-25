#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
    el::Configurations conf("logconfig.txt");
    el::Loggers::reconfigureAllLoggers(conf);
    LOG(DEBUG) << "This is a test.";
    LOG(INFO) << "This is another test.";
    LOG(FATAL) << "Sending a message on FATAL level.";
    return 0;
}
