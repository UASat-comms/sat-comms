FLAGS = -O3 -Wall -Wextra -lm -lwiringPi
# The -O3 tells the compiler to use maximum optimization -> better exe time.
# The -lwiringPi is necessary for serial_test (any perhaps the others, not sure yet).

OBJS = serial_test.o

serial_test: serial_test.o
	g++ $(FLAGS) serial_test.o -o serial_test
serial_test.o: serial_test.c
	g++ $(FLAGS) -c serial_test.c


clean:
	rm -rf $(OBJS) serial_test
