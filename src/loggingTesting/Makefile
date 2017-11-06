CC					= g++
CFLAGS				= -std=c++11 -O3 -Wall -Wextra -DLOGCONFIG=\"./../logconfig.txt\"
PROJECT_HEAD			= ./../../
SRC_BASE				= $(PROJECT_HEAD)/src
EASYLOGGING_BASE		= $(PROJECT_HEAD)/easyloggingpp
EASYSRC_BASE			= $(EASYLOGGING_BASE)/src
INCLUDE				= -I $(SRC_BASE) -I $(EASYLOGGING_BASE) -I $(EASYSRC_BASE)

OBJS = main.o easylogging++.o

loggingTest: $(OBJS)
	$(CC) $(CFLAGS) $(NCLUDE) $^ -o $@

main.o: main.cpp $(EASYSRC_BASE)/easylogging++.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

easylogging++.o: $(EASYSRC_BASE)/easylogging++.cc $(EASYSRC_BASE)/easylogging++.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

test: app
	./app

clean:
	rm -rf $(OBJS) app
