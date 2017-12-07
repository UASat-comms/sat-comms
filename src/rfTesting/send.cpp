#include <stdio.h>
#include "rf.hpp"
#include "settings.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

el::Configurations conf(LOGCONFIG);

int main() {
     el::Loggers::reconfigureAllLoggers(conf);
     setupRF();
     char data[] = "testtesttest";
     rfMessage *m = createRFMessage((char *) data);
     sendRF(m);
     return 0;
}
