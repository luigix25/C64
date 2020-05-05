FLAGS = -Wall -Wextra -pedantic -g3 -std=c++11 -O3
DEPENDENCIES = library.o cpu.o memory.o vic.o SDLManager.o cia1.o cia2.o loader.o
HEADERS = library.h memory.h vic.h cpu.h

all: main.o $(DEPENDENCIES)
	g++ main.o $(DEPENDENCIES) -o main $(FLAGS) -lpthread -lSDL2

main.o: main.cpp
	g++ -c main.cpp $(FLAGS)

library.o: modules/library.cpp modules/library.h
	g++ -c modules/library.cpp $(FLAGS)

cpu.o: modules/cpu.cpp modules/cpu.h
	g++ -c modules/cpu.cpp $(FLAGS)

memory.o: modules/memory.cpp modules/memory.h
	g++ -c modules/memory.cpp $(FLAGS)

vic.o: modules/vic.cpp modules/vic.h
	g++ -c modules/vic.cpp $(FLAGS)

SDLManager.o: modules/SDLManager.cpp modules/SDLManager.h
	g++ -c modules/SDLManager.cpp $(FLAGS)

cia1.o: modules/cia1.cpp modules/cia1.h
	g++ -c modules/cia1.cpp $(FLAGS)

cia2.o: modules/cia2.cpp modules/cia2.h
	g++ -c modules/cia2.cpp $(FLAGS)

loader.o: modules/loader.cpp modules/loader.h
	g++ -c modules/loader.cpp $(FLAGS)

clean:
	rm -f *.o
	rm -f main
