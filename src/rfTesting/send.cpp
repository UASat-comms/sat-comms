#include <stdio.h>
#include "rf.hpp"
#include "settings.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

el::Configurations conf(LOGCONFIG);

int main() {
     el::Loggers::reconfigureAllLoggers(conf);
     setupRF();
     char data[] = "mynameismatthewdalemorganwhatisyournamedude?";
     sendRF(data);
     char *mess = recvRF();
     LOG(DEBUG) << "Reply was: <" << mess << ">";
     closeRF();
     return 0;
}
