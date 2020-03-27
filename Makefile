FLAGS = -Wall -Wextra -pedantic -g

all: main.o library.o

	g++ main.o library.o -o main $(FLAGS)

main.o: main.cpp
	g++ -c main.cpp

library.o: library.cpp
	g++ -c library.cpp $(FLAGS)

clean:
	rm *.o
	rm main
