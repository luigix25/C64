#pragma once

class VIC;

#include "library.h"
#include "debug.h"

#include "memory.h"
#include "SDLManager.h"
#include "cpu.h"
#include "cia1.h"

#define REG_START 0xD000
#define REG_END 0xD02E

#define CTRL_REG_1 0xD011
#define RASTER_CNT 0xD012
#define CTRL_REG_2 0xD016
#define BASE_ADDR_REG 0xD018
#define IRQ_REQ_REG 0xD019
#define IRQ_EN_REG 0xD01A

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

#define BACKGROUND_COLOR_ADDR = 0xD021

#define CTRL_REG_1_OFF CTRL_REG_1 - REG_START
#define CTRL_REG_2_OFF CTRL_REG_2 - REG_START

#define CLOCK_NUMBER 20000					//50Hz and clock is 1 MHz

enum MODES {CHAR_MODE,MCM_TEXT_MODE,EXT_BACK_MODE,BITMAP_MODE,MCB_BITMAP_MODE};

class VIC {
	private:
		uint8_t *registers;
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

		void init_host_charset();

		void show_char(uint8_t *, int, int);

		bool interrupt_enabled;


		Memory *memory = nullptr;
		SDLManager *sdl = nullptr;
		CPU *cpu = nullptr;
		CIA1 *cia1 = nullptr;

		//SDL

		uint8_t *host_video_memory = nullptr;
		uint8_t *guest_video_memory = nullptr;

		uint8_t *guest_charset = nullptr;
		uint8_t *host_charset = nullptr;

		uint8_t *guest_color_memory = nullptr;


		uint32_t clocks_to_new_render;
		//chrono::time_point last_time_rendered;

	public:
		VIC();
		~VIC();
		
		void clock();

		void setMemory(Memory*);
		void setSDL(SDLManager*);
		void setCPU(CPU*);
		void setCIA1(CIA1*);

		uint8_t read_register(uint16_t);
		void write_register(uint16_t,uint8_t);
		thread *video_loop_thread;

};
