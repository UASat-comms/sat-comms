#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main() {
    el::Configurations conf(LOGCONFIG);
    el::Loggers::reconfigureAllLoggers(conf);
    LOG(DEBUG) << "This is a test.";
    LOG(INFO) << "This is another test.";
    return 0;
}
