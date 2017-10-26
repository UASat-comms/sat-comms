# all: library.cpp main.cpp
# $@ evaluates to all
# $< evaluates to library.cpp
# $^ evaluates to library.cpp main.cpp

# foo.xyz:
#	make $* clean
# evalues to 'make foo clean'

CC            		= g++
CFLAGS        		= -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY -D__BASEFILE__=\"$*\" -O3 -std=c++11 -DLOGCONFIG=\"./../logconfig.txt\"
LIBS          		= -lbcm2835 -lwiringPi
PROJECT_HEAD		= ./../../
SRC_BASE 	     	= $(PROJECT_HEAD)/src/
EASYLOGGING_BASE 	= $(PROJECT_HEAD)/easyloggingpp
EASYSRC_BASE		= $(EASYLOGGING_BASE)/src
RADIOHEAD_BASE		= $(PROJECT_HEAD)/RadioHead
RASPIBOARD_BASE 	= $(RADIOHEAD_BASE)/examples/raspi
INCLUDE       		= -I$(SRC_BASE) -I$(EASYSRC_BASE) -I$(RADIOHEAD_BASE) -I$(RASPIBOARD_BASE)

rf.o: rf.cpp rf.hpp $(SRC_BASE)/settings.h
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

clean:
	rm -f $(OBJS) rf.o
