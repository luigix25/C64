#pragma once

class CIA1;

#include "library.h"

class CIA1
{
	public:
		CIA1();
		~CIA1();

		uint8_t read_register(uint16_t);
		void write_register(uint16_t,uint8_t);

	private:
		uint8_t registers[16];

};