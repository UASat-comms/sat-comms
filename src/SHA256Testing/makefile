CC            		= g++
CFLAGS        		= -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY -D__BASEFILE__=\"$*\" -O3 -std=c++11 -DLOGCONFIG=\"./../logconfig.txt\"
PROJECT_HEAD		= ./../../
PICOSHA_BASE		= $(PROJECT_HEAD)/PicoSHA2/

app: main.o
	$(CC) $(CFLAGS) $^ -o app

main.o: main.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

clean:
	rm -f app main.o
