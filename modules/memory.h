#pragma once
#include "library.h"

#define MEMORY_LAYOUT_ADDR 0x0001
#define LORAM_MASK 0x1
#define HIRAM_MASK 0x2
#define CHAR_MASK 0x4

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

		/*
		uint8_t basic[eightK];
		uint8_t kernal[eightK];
		uint8_t charset[fourK];
		*/

		uint8_t* read_bin_file(const char*);

		void setup_memory_mode(uint8_t value);

		bool LORAM_enabled;
		bool HIRAM_enabled;
		bool CHAR_enabled;

};