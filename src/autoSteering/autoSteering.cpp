#include "autoSteering.hpp"

// The function the Tx cube uses to find the Rx cube.
int findRxCube(unsigned boxSize) {
     sendRF("LOCKON_START");

     int RxConnected = 0;
     int vertMotorDirection = 0;

     for(unsigned horiIndex = 0; horiIndex < boxSize; horiIndex++) {
          // Transmit over the vertical range, and then see if the Rx
          // side saw the transmission.
          for(unsigned vertIndex = 0; vertIndex < boxSize; vertIndex++) {
               // Send the connection byte.
               transmitData(connectionByte, 1);

               // We're currently on a downward sweep.
               if(!vertMotorDirection) {
                    motorDown(1);
               } else {
                    motorUp(1);
               }
          }

          // Set the next sweep to be the opposite direction.
          vertMotorDirection = !vertMotorDirection;

          // Ask if the Rx side saw the Tx side.
          sendRF("SEE_ME?");

          // Get the reply and see if they saw the transmission.
          char *reply = recvRF();
          // The Rx side saw the transmission on the vertical sweep.
          if(strcmp(reply, "SAW_YOU") == 0) {
               // Break from the 2D search loop to go to the 1D loop.
               RxConnected = 1;
               break;
          } else if(strcmp(reply, "DID_NOT_SEE_YOU") == 0) {
               // The Rx side didn't see anything, move over horizontally.
               motorRight(1);
               continue;
          } else {
               // The transmission back from the Rx cube wasn't one of the
               // two expected strings.
               LOG(FATAL) << "Received unexpected reply from Rx Side!";
          }
     }

     // If the Rx side didn't see the connection at all, we have fatal error.
     if(!RxConnected) {
          LOG(FATAL) << "Rx Cube did not see connenction transmission!";
     }

     // If they saw the signal sometime, we need to slowly loop back through
     // the verical sweep to connect.
     for(unsigned vertIndex = 0; vertIndex < boxSize; vertIndex++) {
          transmitData(connectionByte, 1);

          // Ask if the Rx side saw the Tx side.
          sendRF("SEE_ME?");

          // Get the reply and see if they saw the transmission.
          char *reply = recvRF();
          // The Rx side saw the transmission on the vertical sweep.
          if(strcmp(reply, "SAW_YOU") == 0) {
               // we're connected!
               return 1;
          } else if(strcmp(reply, "DID_NOT_SEE_YOU") == 0) {
               // Rx side didn't see, move again.
               if(!vertMotorDirection) {
                    motorDown(1);
               } else {
                    motorUp(1);
               }
               continue;
          } else {
               // The transmission back from the Rx cube wasn't one of the
               // two expected strings.
               LOG(FATAL) << "Received unexpected reply from Rx Side!";
          }
     }

     // Let the Rx side know the system failed.
     sendRF("SYSTEM_FAILED!");
     
     LOG(FATAL) << "Rx cube did not see Tx transmission on 1D sweep!";
     return 0;
}


void findTxCube() {
     char *reply = recvRF();

     if(strcmp(reply, "LOCKON_START") != 0) {
          LOG(FATAL) << "Rx cube received unexpected start string from Tx cube!";
     }

     // Keep checking for a successful transmission, this is the 2D sweep.
     while(1) {
          char *byte = receiveData(1);

          char *reply = recvRF();
          if(strcmp(reply, "SEE_ME?") != 0) {
               LOG(FATAL) << "Unexpected RF question from Tx side!";
          }

          if(byte[0] == connectionByte) {
               // We have connection.
               sendRF("SAW_YOU");
               break;
          } else {
               // No connection.
               sendRF("DID_NOT_SEE_YOU");
          }
     }

     // 1D sweep.
     while(1) {
          char *byte = receiveData(1);

          char *reply = recvRF();
          if(strcmp(reply, "SEE_ME?") != 0) {
               LOG(FATAL) << "Unexpected RF question from Tx side!";
          }

          if(byte[0] == connectionByte) {
               // We have connection.
               sendRF("SAW_YOU");
               break;
          } else {
               // No connection.
               sendRF("DID_NOT_SEE_YOU");
          }
     }
}
