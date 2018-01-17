#include <iostream>

#include "steering.h"
#include "easylogging++.h"
#include "settings.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

void setup() {
     printf("-----> %s running | setting up..\n", __BASEFILE__);
	 // Configure logging module.
	 el::Configurations conf(LOGCONFIG);
	 el::Loggers::reconfigureAllLoggers(conf);
	 // Configure wiringpi for serial comms.
	  wiringPiSetup();
     LOG(INFO) << "Setup complete.";
}

int main() {
     setup();
     LOG(DEBUG) << "Entering continuous loop.";
     
     unsigned direction;
     unsigned steps;

     while(true) {
          cout << "Enter direction (1=up,2=down,3=left,4=right,5=QUIT): ";
          cin >> direction;

          if(direction == 5) {
               break;
          }

          if(direction < 1 || direction > 5) {
               LOG(INFO) << "[Err]: Direction selection not valid!";
               continue;
          }

          cout << "Enter number of steps: ";
          cin >> steps;

          switch(direction) {
               case 1:
                   motorUp(steps);
                   break;
               case 2:
                   motorDown(steps);
                   break;
               case 3:
                   motorLeft(steps);
                   break;
               case 4:
                   motorRight(steps);
                   break;
          }

          LOG(DEBUG) << "Done with motor movement.";
     }
 
     LOG(DEBUG) << "Program ending.";
     return 0;
}
