#include "steering.h"

static void setStepUD(int w1, int w2, int w3, int w4){
        digitalWrite(IN1_UD, w1);
        digitalWrite(IN2_UD, w2);
        digitalWrite(IN3_UD, w3);
        digitalWrite(IN4_UD, w4);
}

static void setStepLR(int w1, int w2, int w3, int w4){
        digitalWrite(IN1_LR, w1);
        digitalWrite(IN2_LR, w2);
        digitalWrite(IN3_LR, w3);
        digitalWrite(IN4_LR, w4);
}

void motorUp(unsigned numSteps) {
     for(unsigned counter = 0; counter < numSteps; counter++) {
          setStepUD(1,0,0,0);
          bcm2835_delay(1);
          setStepUD(1,1,0,0);
          bcm2835_delay(1);
          setStepUD(0,1,0,0);
          bcm2835_delay(1);
          setStepUD(0,1,1,0);
          bcm2835_delay(1);
          setStepUD(0,0,1,0);
          bcm2835_delay(1);
          setStepUD(0,0,1,1);
          bcm2835_delay(1);
          setStepUD(0,0,0,1);
          bcm2835_delay(1);
          setStepUD(1,0,0,1);
          bcm2835_delay(1);
     }
}

void motorDown(unsigned numSteps) {
     for(unsigned counter = 0; counter < numSteps; counter++) {
          setStepUD(0,0,0,1);
          bcm2835_delay(1);
          setStepUD(0,0,1,1);
          bcm2835_delay(1);
          setStepUD(0,0,1,0);
          bcm2835_delay(1);
          setStepUD(0,1,1,0);
          bcm2835_delay(1);
          setStepUD(0,1,0,0);
          bcm2835_delay(1);
          setStepUD(1,1,0,0);
          bcm2835_delay(1);
          setStepUD(1,0,0,0);
          bcm2835_delay(1);
          setStepUD(1,0,0,1);
          bcm2835_delay(1);
     }
}

void motorLeft(unsigned numSteps) {
     for(unsigned counter = 0; counter < numSteps; counter++) {
          setStepLR(1,0,0,0);
          bcm2835_delay(1);
          setStepLR(1,1,0,0);
          bcm2835_delay(1);
          setStepLR(0,1,0,0);
          bcm2835_delay(1);
          setStepLR(0,1,1,0);
          bcm2835_delay(1);
          setStepLR(0,0,1,0);
          bcm2835_delay(1);
          setStepLR(0,0,1,1);
          bcm2835_delay(1);
          setStepLR(0,0,0,1);
          bcm2835_delay(1);
          setStepLR(1,0,0,1);
          bcm2835_delay(1);
     }
}

void motorRight(unsigned numSteps) {
     for(unsigned counter = 0; counter < numSteps; counter++) {
          bcm2835_delay(1);
          setStepLR(0,0,1,1);
          bcm2835_delay(1);
          setStepLR(0,0,1,0);
          bcm2835_delay(1);
          setStepLR(0,1,1,0);
          bcm2835_delay(1);
          setStepLR(0,1,0,0);
          bcm2835_delay(1);
          setStepLR(1,1,0,0);
          bcm2835_delay(1);
          setStepLR(1,0,0,0);
          bcm2835_delay(1);
          setStepLR(1,0,0,1);
          bcm2835_delay(1);
     }
}
