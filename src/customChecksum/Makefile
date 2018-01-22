# all: library.cpp main.cpp
# $@ evaluates to all
# $< evaluates to library.cpp
# $^ evaluates to library.cpp main.cpp

# foo.xyz:
#	make $* clean
# evalues to 'make foo clean'

CC            		= g++
CFLAGS        		= -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY -D__BASEFILE__=\"$*\" -O3 -std=c++11 -DLOGCONFIG=\"./../logconfig.txt\"
# Don't need these: LIBS          		= -lbcm2835 -lwiringPi
PROJECT_HEAD		= ./../../
SRC_BASE 	     	= $(PROJECT_HEAD)/src/
EASYLOGGING_BASE 	= $(PROJECT_HEAD)/easyloggingpp
EASYSRC_BASE		= $(EASYLOGGING_BASE)/src
INCLUDE       		= -I$(SRC_BASE) -I$(EASYSRC_BASE)

OBJS = checksum.o easylogging++.o test.o

app: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $^ $(LIBS) -o $@

test.o: test.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

checksum.o: checksum.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

easylogging++.o: $(EASYSRC_BASE)/easylogging++.cc $(EASYSRC_BASE)/easylogging++.h
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

clean:
	rm -f $(OBJS) app
