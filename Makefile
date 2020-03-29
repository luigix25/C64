FLAGS = -Wall -Wextra -pedantic -g

all: main.o library.o cpu.o memory.o

	g++ main.o library.o cpu.o memory.o -o main $(FLAGS)

main.o: main.cpp
	g++ -c main.cpp

library.o: modules/library.cpp modules/library.h
	g++ -c modules/library.cpp $(FLAGS)

cpu.o: modules/cpu.cpp modules/cpu.h
	g++ -c modules/cpu.cpp $(FLAGS)

memory.o: modules/memory.cpp modules/memory.h
	g++ -c modules/memory.cpp $(FLAGS)

clean:
	rm -f *.o
	rm -f main
