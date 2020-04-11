#pragma once

class VIC;

#include "library.h"
#include "debug.h"

#include "memory.h"
#include "SDLManager.h"

#define REG_START 0xD000
#define REG_END 0xD02E

#define CTRL_REG_1 0xD011
#define RASTER_CNT 0xD012
#define CTRL_REG_2 0xD016
#define BASE_ADDR_REG 0xD018
#define IRQ_REQ_REG 0xD019
#define IRQ_MASK_REG 0xD01A

#define CTRL_REG_1_OFF CTRL_REG_1 - REG_START
#define CTRL_REG_2_OFF CTRL_REG_2 - REG_START

enum MODES {CHAR_MODE,MCM_TEXT_MODE,EXT_BACK_MODE,BITMAP_MODE,MCB_BITMAP_MODE};

class VIC {
	private:
		uint8_t registers[0x400];
		MODES graphic_mode;

		uint8_t visible_rows;
		uint8_t visible_cols;

		uint8_t rasterline;

		uint16_t screen_memory_base_addr;
		uint16_t char_memory_base_addr;
		//uint16_t bitmap_memory_base_addr;

		void control_reg_one(uint8_t);
		void control_reg_two(uint8_t);

		void set_graphic_mode();

		Memory *memory;
		SDLManager *sdl;

		uint8_t *host_video_memory = nullptr;
		uint8_t *guest_video_memory = nullptr;

	public:
		VIC();
		~VIC();
		
		void setMemory(Memory*);
		void setSDL(SDLManager*);

		uint8_t read_register(uint16_t);
		void write_register(uint16_t,uint8_t);

};
