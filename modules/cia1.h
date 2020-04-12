#pragma once

class CIA1;

#include "library.h"

class CIA1
{
	public:
		CIA1();
		~CIA1();
		

	private:
		uint8_t registers[16];

};