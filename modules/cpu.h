#include "library.h"

#define RESET_routine 0xFCE2

class CPU
{

public:

	CPU(uint8_t*);
	//~CPU();

	uint8_t fetch();
	bool decode(uint8_t);

	//DEBUG
	void dump_reg();


private:

	uint8_t *memory;

	//registers
	registers regs;

	//Utils
	uint8_t read_byte(uint16_t);
	uint16_t read_word(uint16_t);


	//Memory Addressing Modes
	uint8_t immediate();
	
	uint16_t absolute();
	uint16_t absolute(register_name);

	uint8_t zero_page();
	uint8_t zero_page(register_name);

	uint16_t indirect_Y();
	uint16_t indirect_X();

	//Istructions

	void LD(register_name, uint8_t);
	void OR(register_name, uint8_t);
	void ST(register_name, uint16_t);
	void JMP(uint16_t);
	
};