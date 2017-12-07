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
RADIOHEAD_BASE		= $(PROJECT_HEAD)/RadioHead
EASYLOGGING_BASE 	= $(PROJECT_HEAD)/easyloggingpp
EASYSRC_BASE		= $(EASYLOGGING_BASE)/src
SRC_BASE 	      	= $(PROJECT_HEAD)/src
RF_BASE			= $(SRC_BASE)/rf
INCLUDE       		= -I$(RADIOHEAD_BASE) -I$(SRC_BASE) -I$(EASYSRC_BASE)

OBJS = RH_RF95.o RasPi.o RHHardwareSPI.o RHGenericSPI.o RHGenericDriver.o RHSPIDriver.o RHReliableDatagram.o RHDatagram.o easylogging++.o rf.o

all: recv send

recv: recv.o $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $^ $(LIBS) -o $@

send: send.o $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $^ $(LIBS) -o $@

#advrecv: advrecv.o $(OBJS)
#	$(CC) $(CFLAGS) $(INCLUDE) $^ $(LIBS) -o $@

#advsend: advsend.o $(OBJS)
#	$(CC) $(CFLAGS) $(INCLUDE) $^ $(LIBS) -o $@

#advrecv.o: advrecv.cpp
#	$(CC) $(CFLAGS) -c $(INCLUDE) $<

#advsend.o: advsend.cpp
#	$(CC) $(CFLAGS) -c $(INCLUDE) $<

recv.o: recv.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

send.o: send.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

rf.o: rf.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

easylogging++.o: $(EASYSRC_BASE)/easylogging++.cc $(EASYSRC_BASE)/easylogging++.h
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

clean:
	rm -f $(OBJS) send.o send recv.o recv advsend.o advsend advrecv.o advrecv


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
RHReliableDatagram.o: $(RADIOHEAD_BASE)/RHReliableDatagram.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
