# all: library.cpp main.cpp
# $@ evaluates to all
# $< evaluates to library.cpp
# $^ evaluates to library.cpp main.cpp

# foo.xyz:
#	make $* clean
# evalues to 'make foo clean'

CC            		= g++
CFLAGS        		= -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY -D__BASEFILE__=\"$*\" -O3 -std=c++11 -DLOGCONFIG=\"./../logconfig.txt\"
#LIBS          		= 
PROJECT_HEAD		= ./../../
EASYLOGGING_BASE 	= $(PROJECT_HEAD)/easyloggingpp
EASYSRC_BASE		= $(EASYLOGGING_BASE)/src
SRC_BASE 	      	= $(PROJECT_HEAD)/src
SCHIFRA_BASE		= $(PROJECT_HEAD)/schifra
INCLUDE       		= -I$(SRC_BASE) -I$(EASYSRC_BASE) -I$(SCHIFRA_BASE)

OBJS = easylogging++.o

rstest: rstest.o $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $^$(LIBS) -o $@

rstest.o: rstest.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

easylogging++.o: $(EASYSRC_BASE)/easylogging++.cc $(EASYSRC_BASE)/easylogging++.h
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

clean:
	rm -rf *.o rstest
