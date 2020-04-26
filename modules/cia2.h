#pragma once

class CIA2;

#include "library.h"
#include "cpu.h"
#include "SDLManager.h"

class CIA2{

	public:
		CIA2();

		uint8_t read_register(uint16_t);
		void write_register(uint16_t,uint8_t);

		void setCPU(CPU*);
		void clock();

		void setSDL(SDLManager*);

		uint8_t getVICBank();

	private:
		uint8_t registers[16];

		uint8_t VICBank;

		CPU *cpu;
		SDLManager *sdl;

};