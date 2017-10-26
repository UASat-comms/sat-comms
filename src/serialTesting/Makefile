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
SERIAL_BASE 		= $(SRC_BASE)/serial/
EASYLOGGING_BASE 	= $(PROJECT_HEAD)/easyloggingpp
EASYSRC_BASE		= $(EASYLOGGING_BASE)/src
INCLUDE       		= -I$(SRC_BASE) -I$(SERIAL_BASE) -I$(EASYSRC_BASE)

OBJS = serial.o easylogging++.o

all: serialreceivetest serialsendtest

serialreceivetest: serialreceivetest.o $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $^ $(LIBS) -o $@
serialsendtest: serialsendtest.o $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $^ $(LIBS) -o $@

serialreceivetest.o: serialreceivetest.cpp $(SRC_BASE)/settings.h
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
serialsendtest.o: serialsendtest.cpp $(SRC_BASE)/settings.h
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

serial.o: $(SERIAL_BASE)/serial.cpp $(SERIAL_BASE)/serial.hpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

easylogging++.o: $(EASYSRC_BASE)/easylogging++.cc $(EASYSRC_BASE)/easylogging++.h
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

clean:
	rm -f $(OBJS) serialsendtest.o serialreceivetest.o serialsendtest serialreceivetest
