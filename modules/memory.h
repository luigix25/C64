#pragma once
#include "library.h"

class Memory{

	public:

		Memory();
		uint8_t read_byte(uint16_t);
		void write_byte(uint16_t,uint8_t);

		void load_kernal_and_basic(const char*);
		void load_charset(const char*);

	private:

		uint8_t memory[sixtyfourK];

		uint8_t basic[eightK];
		uint8_t kernal[eightK];
		uint8_t charset[fourK];

		uint8_t* read_bin_file(const char*);

};