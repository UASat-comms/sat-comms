# all: library.cpp main.cpp
# $@ evaluates to all
# $< evaluates to library.cpp
# $^ evaluates to library.cpp main.cpp

# foo.xyz:
#	make $* clean
# evalues to 'make foo clean'

CC            = g++
CFLAGS        = -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY -D__BASEFILE__=\"$*\" -O3 -std=c++11
LIBS          = -lbcm2835 -lwiringPi
RADIOHEADBASE = ./../../githubLibraries/RadioHead/
SETTINGSBASE = ./../
SERIALBASE = ./../serial/
INCLUDE       = -I$(RADIOHEADBASE) -I$(SETTINGSBASE) -I$(SERIALBASE)

OBJS = RH_RF95.o RasPi.o RHHardwareSPI.o RHGenericSPI.o RHGenericDriver.o RHSPIDriver.o

all: serialreceivetest serialsendtest

serialreceivetest: serialreceivetest.o #$(OBJS)
	$(CC) $(INCLUDE) $^ $(LIBS) -o $@
serialsendtest: serialsendtest.o serial.o#$(OBJS)
	$(CC) $(INCLUDE) $^ $(LIBS) -o $@


serialreceivetest.o: serialreceivetest.cpp $(SETTINGBASE)/settings.h
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
serialsendtest.o: serialsendtest.cpp $(SETTINGSBASE)/settings.h
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

serial.o: serial.cpp serial.hpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

clean:
	rm -f *.o serialsendtest serialreceivetest


# =======================================================================
# -------------- Dependency files for RF communications -----------------
#
RasPi.o: $(RADIOHEADBASE)/RHutil/RasPi.cpp
	$(CC) $(CFLAGS) -c $(RADIOHEADBASE)/RHutil/RasPi.cpp $(INCLUDE)
rf95_client.o: rf95_client.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
rf95_server.o: rf95_server.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
RH_RF95.o: $(RADIOHEADBASE)/RH_RF95.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
RHDatagram.o: $(RADIOHEADBASE)/RHDatagram.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
RHHardwareSPI.o: $(RADIOHEADBASE)/RHHardwareSPI.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
RHSPIDriver.o: $(RADIOHEADBASE)/RHSPIDriver.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
RHGenericDriver.o: $(RADIOHEADBASE)/RHGenericDriver.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
RHGenericSPI.o: $(RADIOHEADBASE)/RHGenericSPI.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
