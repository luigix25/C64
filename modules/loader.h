#pragma once

class Loader;

#include "library.h"

#include "cpu.h"
#include "memory.h"

#define BASIC_READY 0xa65c

class Loader{

	public:
		Loader(CPU*,Memory*,string&);

		void clock();
		bool loaded = false;
		
	private:
		CPU *cpu = nullptr;
		Memory *mem = nullptr;
		string filename;

		bool shouldLoad = false;

};