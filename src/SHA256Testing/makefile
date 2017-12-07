FLAGS = -O3 -Wall -Wextra -std=c++14
OBJS = main.o

app: $(OBJS)
	g++ $(FLAGS) $(OBJS) -o app
main.o: main.cpp picosha2.h
	g++ -c $(FLAGS) main.cpp
clean:
	rm -f $(OBJS) ./app
