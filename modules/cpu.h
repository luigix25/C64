#include "library.h"
#include "memory.h"

#define RESET_routine 0xFCE2

#define SET_ZF(val)     (regs.zero_flag = 	!(uint8_t)(val))
#define SET_NF(val)     (regs.sign_flag =  	((uint8_t)(val) & 0x80 ))

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
		registers regs;

	private:

		Memory *memory;

		//registers

		//utils
		uint8_t read_byte(uint16_t);
		uint16_t read_word(uint16_t);

		//IRQs
		void handle_irq(bool);
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

		void flags(uint8_t);

		//Istructions

		void LD(register_name, uint8_t);
		void ST(register_name, uint16_t);

		void TSX();

		void PUSH(register_name);
		void PUSH(uint8_t);
		uint8_t POP();


		void PLA();
		
		void JMP(uint16_t);
		void JSR(uint16_t);
		void CMP_addr(uint16_t);
		void CMP_data(uint8_t);
		
		void CPX(uint8_t);

		void BNE(uint8_t);
		void BEQ(uint8_t);
		void BCS(uint8_t);
		void BCC(uint8_t);
		void BPL(uint8_t);
		void BMI(uint8_t);
		void BVS(uint8_t);
		void BVC(uint8_t);

		void DE(register_name);
		void DEC(uint16_t);

		void INC(register_name index);
		void INC(uint16_t);
		void ADC(uint8_t);
		void SBC(uint8_t);

		void CP(register_name, uint8_t);

		void OR(register_name, uint8_t);
		void AND(uint8_t);
		void ROL(register_name);
		void LSR(register_name);
		void ROR();
		void ASL(register_name);

		void left_shift_mem(uint16_t);
		void right_shift_mem(uint16_t);
		void rotate_right_mem(uint16_t);



		//void ASL(uint16_t);
		void EOR(uint8_t);
		void BIT(uint16_t);

		void TAX();
		void TAY();
		void TXA();


};