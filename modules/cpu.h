#include "library.h"
#include "memory.h"

#define RESET_routine 0xFCE2

#define SET_ZF(val)     (regs.zero_flag = !(uint8_t)(val))
#define SET_NF(val)     (regs.sign_flag = ((uint8_t)(val)&0x80)!=0)

class CPU
{

public:

	CPU(Memory *);
	CPU(Memory *,uint16_t);

	//~CPU();

	uint8_t fetch();
	bool decode(uint8_t);

	//DEBUG
	void dump_reg();

	//active low
	bool irq_line;
	bool nmi_line;

private:

	Memory *memory;

	//registers
	registers regs;

	//utils
	uint8_t read_byte(uint16_t);
	uint16_t read_word(uint16_t);

	//IRQs
	void handle_irq();
	void handle_nmi();

	//Memory Addressing Modes
	uint8_t immediate();
	
	uint16_t absolute();
	uint16_t absolute(register_name);

	uint8_t zero_page();
	uint8_t zero_page(register_name);

	uint16_t indirect_Y();
	uint16_t indirect_X();

	uint8_t flags();
	void reset_flags();

	//Istructions

	void LD(register_name, uint8_t);
	void ST(register_name, uint16_t);

	void PUSH(register_name);
	void PUSH(uint8_t);
	uint8_t POP();
	
	void JMP(uint16_t);
	void JSR(uint16_t);
	void CMP(uint16_t);

	void BNE(uint8_t);
	void BEQ(uint8_t);
	void BCS(uint8_t);
	void BCC(uint8_t);
	void BPL(uint8_t);
	void BMI(uint8_t);

	void DE(register_name);
	void INC(register_name index);
	void INC(uint16_t);
	void ADC(uint8_t);

	void CP(register_name, uint8_t);

	void OR(register_name, uint8_t);
	void AND(uint8_t);
	void ROL(register_name);

	void TAX();
	void TAY();
	void TXA();


};