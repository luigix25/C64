#pragma once

class Memory;

#include "library.h"
#include "vic.h"
#include "cia1.h"

#define MEMORY_LAYOUT_ADDR 0x1
#define LORAM_MASK 0x1
#define HIRAM_MASK 0x2
#define CHAR_MASK 0x4

#define VIDEO_MEM_START 

enum bank_mode {RAM,ROM,IO,CARTRIDGE};

class Memory{

	public:

		Memory();
		~Memory();

		uint8_t read_byte(uint16_t);
		void write_byte(uint16_t,uint8_t);

		void load_kernal_and_basic(const char*);
		void load_charset(const char*);
		void load_custom_memory(const char*,uint16_t);

		void setVIC(VIC*);
		void setCIA1(CIA1*);

		void setup_memory_mode(uint8_t value);

		uint8_t* getVideoMemoryPtr();
		uint8_t* getColorMemoryPtr();
		uint8_t* getCharROMPtr();

		//Debug
		uint8_t* getMemPointer();
		uint8_t* getKerPointer();
		void dump_memory(uint16_t,uint16_t);


	private:

		VIC		*vic;
		CIA1 	*cia1;

		uint8_t *memory;

		uint8_t *basic;
		uint8_t *kernal;
		uint8_t *charset;

		uint8_t* read_bin_file(const char*,streampos&);

		bank_mode LORAM_mode;
		bank_mode HIRAM_mode;
		bank_mode CHAR_mode;

};