#pragma once

class CIA1;

#include "library.h"

#include "cpu.h"

#define TA_LOW 0x04
#define TA_HI 0x05
#define TA_CTRL 0x0E

#define TB_LOW 0x06
#define TB_HI 0x07
#define TB_CTRL 0x0F

#define IRQ_REG 0x0D

#define KEYBOARD_ROW 0x01
#define KEYBOARD_COL 0x00

class CIA1
{
	public:
		CIA1();
		~CIA1();

		uint8_t read_register(uint16_t);
		void write_register(uint16_t,uint8_t);

		void setCPU(CPU*);
		void clock();

		void setKeyPressed(KeyboardMatrix);

	private:
		uint8_t registers[16];

		uint16_t timerA_latch;
		uint16_t timerB_latch;

		uint16_t timerA;
		uint16_t timerB;

		bool timerA_enabled;
		bool timerB_enabled;

		bool timerA_irq_enabled;
		bool timerB_irq_enabled;

		bool timerA_irq_raised;
		bool timerB_irq_raised;

		bool timerA_reload;
		bool timerB_reload;

		CPU *cpu;

		KeyboardMatrix last_pressed;

};