#pragma once
#include "library.h"

class VIC {
	private:
		/* data */
	public:
		VIC();
		~VIC();
		
		uint8_t read_register(uint16_t);
};
