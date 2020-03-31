#pragma once
#include "library.h"
#include "debug.h"

#define REG_START 0xD000
#define REG_END 0xD02E

#define CTRL_REG_1 0xD011
#define RASTER_CNT 0xD012
#define CTRL_REG_2 0xD016
#define BASE_ADDR_REG 0xD018
#define IRQ_REQ_REG 0xD019
#define IRQ_MASK_REG 0xD01A

enum MODES {CHAR_MODE,MCM_TEXT_MODE,EXT_BACK_MODE,BITMAP_MODE,MCB_BITMAP_MODE};

class VIC {
	private:
		uint8_t registers[0x400];
		MODES graphic_mode;

		uint8_t visible_rows;
		uint8_t visible_cols;

		uint8_t rasterline;

		void control_reg_one(uint8_t);
		void control_reg_two(uint8_t);


	public:
		VIC();
		~VIC();
		
		uint8_t read_register(uint16_t);
		void write_register(uint16_t,uint8_t);

};
