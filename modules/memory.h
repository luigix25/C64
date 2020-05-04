#pragma once

class Memory;

#include "library.h"
#include "vic.h"
#include "cia1.h"
#include "cia2.h"

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

		uint8_t VIC_read_byte(uint16_t);

		void load_kernal_and_basic(const string&);
		void load_charset(const string&);
		void load_custom_memory(const string&,uint16_t);
		void load_prg(const string&);

		void setVIC(VIC*);
		void setCIA1(CIA1*);
		void setCIA2(CIA2*);

		void setup_memory_mode(uint8_t value);

		uint8_t* getColorMemoryPtr();

		//Debug
		uint8_t* getMemPointer();
		uint8_t* getKerPointer();
		void dump_memory(uint16_t,uint16_t);
		void dump_color_memory();

	private:

		VIC		*vic = nullptr;
		CIA1 	*cia1 = nullptr;
		CIA2 	*cia2 = nullptr;

		uint8_t *memory;
		uint8_t *color_ram;

		uint8_t *basic;
		uint8_t *kernal;
		uint8_t *charset;


		uint8_t* read_bin_file(const string&,streampos&);

		bank_mode LORAM_mode;
		bank_mode HIRAM_mode;
		bank_mode CHAR_mode;

};