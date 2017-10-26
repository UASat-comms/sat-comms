# all: library.cpp main.cpp
# $@ evaluates to all
# $< evaluates to library.cpp
# $^ evaluates to library.cpp main.cpp

# foo.xyz:
#	make $* clean
# evalues to 'make foo clean'

CC            		= g++
CFLAGS        		= -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY -D__BASEFILE__=\"$*\" -O3 -std=c++11
LIBS          		= -lbcm2835 -lwiringPi
PROJECT_HEAD		= ./../../
RADIOHEAD_BASE		= $(PROJECT_HEAD)/RadioHead
RASPIBOARD_BASE 	= $(RADIOHEAD_BASE)/examples/raspi
SRC_BASE 	      	= $(PROJECT_HEAD)/src
INCLUDE       		= -I$(RADIOHEAD_BASE) -I$(SRC_BASE) -I$(RASPIBOARD_BASE)

OBJS = RH_RF95.o RasPi.o RHHardwareSPI.o RHGenericSPI.o RHGenericDriver.o RHSPIDriver.o

all: rfreceivetest rfsendtest

rfreceivetest: rfreceivetest.o $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $^ $(LIBS) -o $@
rfsendtest: rfsendtest.o $(OBJS)
	$(CC) $(CLFAGS) $(INCLUDE) $^ $(LIBS) -o $@

rfreceivetest.o: rfreceivetest.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
rfsendtest.o: rfsendtest.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

clean:
	rm -f $(OBJS) rfreceivetest.o rfsendtest.o rfreceivetest rfsendtest


# =======================================================================
# -------------- Dependency files for RF communications -----------------
RasPi.o: $(RADIOHEAD_BASE)/RHutil/RasPi.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
rf95_client.o: rf95_client.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
rf95_server.o: rf95_server.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
RH_RF95.o: $(RADIOHEAD_BASE)/RH_RF95.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
RHDatagram.o: $(RADIOHEAD_BASE)/RHDatagram.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
RHHardwareSPI.o: $(RADIOHEAD_BASE)/RHHardwareSPI.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
RHSPIDriver.o: $(RADIOHEAD_BASE)/RHSPIDriver.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
RHGenericDriver.o: $(RADIOHEAD_BASE)/RHGenericDriver.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
RHGenericSPI.o: $(RADIOHEAD_BASE)/RHGenericSPI.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
