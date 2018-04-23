
#ifndef _AUTO_STEER_H__
#define _AUTO_STEER_H__

#include <cstring>

#include "rf.hpp"
#include "settings.h"
#include "easylogging++.h"
#include "serial.hpp"

// Loop vertically, then horizontally
int findRxCube(unsigned boxSize);
int findTxCube();

#endif
