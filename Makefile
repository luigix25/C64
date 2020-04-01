FLAGS = -Wall -Wextra -pedantic -g -O2 -std=c++11
DEPENDENCIES = library.o cpu.o memory.o vic.o
HEADERS = library.h memory.h vic.h cpu.h

all: main.o $(DEPENDENCIES)
	g++ main.o $(DEPENDENCIES) -o main $(FLAGS)

main.o: main.cpp
	g++ -c main.cpp

library.o: modules/library.cpp modules/library.h
	g++ -c modules/library.cpp $(FLAGS)

cpu.o: modules/cpu.cpp modules/cpu.h
	g++ -c modules/cpu.cpp $(FLAGS)

memory.o: modules/memory.cpp modules/memory.h
	g++ -c modules/memory.cpp $(FLAGS)

vic.o: modules/vic.cpp modules/vic.h
	g++ -c modules/vic.cpp $(FLAGS)

clean:
	rm -f *.o
	rm -f main
