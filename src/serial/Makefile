CC	= g++
CFLAGS 	= -O3 -std=c++11 -Wall -Wextra
BASE 	= ./../
INCLUDE = -I$(BASE)

serial.o: serial.cpp serial.hpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<
clean:
	rm -rf serial.o
