#include "cpu.h"

CPU::CPU(uint8_t* memory, uint16_t PC){

	this->memory = memory;
	regs.PC = PC;
	regs.SP = 0;

}

CPU::CPU(uint8_t* memory){

	this->memory = memory;

	regs.SP = 0;
	regs.PC = RESET_routine;

}

uint8_t CPU::zero_page(register_name index){
  //Immediate operand
  
	uint8_t addr;

	addr = immediate();

 	//Sum X register
	addr += regs.reg[index];

	return addr;

}

uint8_t CPU::zero_page(){
  
	uint8_t addr;
	addr = memory[regs.PC];

	regs.PC++;

	return addr;

}

uint8_t CPU::immediate(){

	uint8_t addr;
	addr = read_byte(regs.PC);

	regs.PC++;

	return addr;
}

uint8_t CPU::read_byte(uint16_t addr){

	uint8_t data = memory[addr];

	return data;

}

uint16_t CPU::read_word(uint16_t addr){

	uint16_t data = memory[addr];
	uint16_t tmp = memory[addr+1];

	tmp = tmp <<8;
	data |= tmp;

	return data;

}

uint16_t CPU::absolute(){

	/*Prima parte bassa e poi parte alta
	uint16_t addr;
	addr = read_byte(regs.PC);

	regs.PC++;

	uint16_t tmp;
	tmp = memory[regs.PC];
	regs.PC++;

	tmp = tmp <<8;
	addr |= tmp;
*/

	uint16_t addr = read_word(regs.PC);

	regs.PC += 2;

	return addr;

}

uint16_t CPU::absolute(register_name index){

	uint16_t addr = absolute();
	addr+= regs.reg[index];

	return addr;

}

uint16_t CPU::indirect_X(){

	uint16_t addr;

	//zero page addr!!
	uint8_t zero_page_addr = immediate();
	//uint16_t tmp;

	zero_page_addr += regs.reg[regX];

	//least significant byte
	addr = read_word(zero_page_addr);

	/*addr = memory[zero_page_addr];

	tmp = memory[zero_page_addr+1];
	tmp = tmp << 8;

	addr |= tmp;*/

	return addr; 


}

uint16_t CPU::indirect_Y(){

	uint16_t addr;

	//zero page addr!!
	uint8_t zero_page_addr = immediate();
	
	//uint16_t tmp;

	addr = read_word(zero_page_addr);


	//address is stored inside the zero-page
	//Least significant byte
	/*addr = memory[zero_page_addr];

	//Most sig. byte
	tmp = memory[zero_page_addr+1];
	tmp = tmp << 8;
	addr |= tmp;
*/

	addr += regs.reg[regY];

	return addr;

}

void CPU::OR(register_name index, uint8_t operand){

	regs.reg[index] |= operand;

}

void CPU::LD(register_name index, uint8_t operand){

	regs.reg[index] = operand;

}

void CPU::ST(register_name index, uint16_t addr){

	memory[addr] = regs.reg[index];

}

void CPU::JMP(uint16_t addr){
	regs.PC = memory[addr];
}

void CPU::PUSH(register_name index){

	uint16_t addr = STACK_START + regs.SP;
	memory[addr] = regs.reg[index];
	regs.SP--;
}

void CPU::PUSH(uint8_t value){

	uint16_t addr = STACK_START + regs.SP;
	memory[addr] = value;
	regs.SP--;
}

uint8_t CPU::fetch(){

	uint8_t opcode = read_byte(regs.PC);

	regs.PC++;
	return opcode;

}

uint8_t CPU::flags()
{
	uint8_t v = 0;

	v |= regs.carry_flag  << 0;
	v |= regs.zero_flag  << 1;
	v |= regs.interrupt_flag << 2;
	v |= regs.decimal_mode_flag << 3;

	v |= 1 << 4;
	/* unused, always set */
	v |= 1     << 5;
	v |= regs.overflow_flag  << 6;
	v |= regs.sign_flag  << 7;

	return v;
}

//DEBUG
void CPU::dump_reg(){

	DEBUG_PRINT(endl<<endl);
	DEBUG_PRINT("---------REG STATUS-----------"<<endl);
	DEBUG_PRINT("RegA: 0x"<<hex<<unsigned(regs.reg[regA])<<endl);
	DEBUG_PRINT("RegX: 0x"<<hex<<unsigned(regs.reg[regX])<<endl);
	DEBUG_PRINT("RegY: 0x"<<hex<<unsigned(regs.reg[regY])<<endl);
	DEBUG_PRINT("PC  : 0x"<<hex<<unsigned(regs.PC)<<endl);
	//hexDump(&regs.PC,1);
	DEBUG_PRINT("------------------------------"<<endl);

	DEBUG_PRINT(endl<<endl);

}

bool CPU::decode(uint8_t opcode){

	//OR LD ST PH fatte

	uint16_t addr,tmp;

	switch(opcode){
		case 0x00:        //BRK
		DEBUG_PRINT("BRK"<<endl);
		return false;

		case 0x01:        //ORA (ind,X)		

			addr = indirect_X();			

			OR(regA,memory[addr]);
			break;

		case 0x05:      //ORA zpg
			addr = zero_page();
			OR(regA, memory[addr]);
			break;

		case 0x06:			//ASL
			break;
	    
	    //TODO
		case 0x08:			//PHP
			PUSH(flags());
			break;

		case 0x09:			//ORA imm
			addr = immediate();
			OR(regA,addr);
			break;

		case 0x0D:
			DEBUG_PRINT("ORA ABS"<<endl);
			addr = absolute();		//ORA ABS
			OR(regA,memory[addr]);
			break;
			
		case 0x0E:						//ASL ABS
			break;

		case 0x10:						//BPL(?)
			break;
		
		case 0x11:						//ORA (ind),Y

			addr = indirect_Y();
			OR(regA,memory[addr]);
			break;

		case 0x15:						//ORA zpg,X

			addr = zero_page(regX);
			OR(regA,memory[addr]);
			break;

		case 0x1D:						//ORA abs,X

			addr = absolute(regX);
			OR(regA,memory[addr]);
			break;
		
		case 0x48:						//PHA

			PUSH(regA);
			break;
		
		case 0x4C:						//JMP abs

			addr = absolute();
			JMP(addr);
			break;
		
		case 0x6C:						//JMP (ind)

			tmp = absolute();

			addr = read_word(tmp);

			JMP(tmp);
			break;

		case 0x78:						//SEI
			DEBUG_PRINT("SEI"<<endl);
			regs.interrupt_flag = true;
			break;

	    case 0x81:						//STA (ind,X)
	    	DEBUG_PRINT("STA indirect X"<<endl);

	    	addr = indirect_X();
	    	ST(regA,addr);

	    	//memory[addr] = regs.reg[regA];
	    	break;

	    case 0x84:						//STY zpg
	    	DEBUG_PRINT("STY zpg"<<endl);

	    	addr = zero_page();
	    	ST(regY,addr);

	    	//memory[addr] = regs.reg[regA];
	    	break;

	    case 0x85:						//STA zpg
	    	DEBUG_PRINT("STA zpg"<<endl);

	    	addr = zero_page();
	    	ST(regA,addr);

	    	//memory[addr] = regs.reg[regA];
	    	break;

	    case 0x86:						//STX zpg
	    	DEBUG_PRINT("STX zpg"<<endl);

	    	addr = zero_page();
	    	ST(regX,addr);

	    	//memory[addr] = regs.reg[regA];
	    	break;

	    case 0x8C:						//STY abs
	    	DEBUG_PRINT("STY abs"<<endl);

	    	addr = absolute();
	    	ST(regY,addr);

	    	//memory[addr] = regs.reg[regA];
	    	break;

	    case 0x8D:						//STA abs
	    	DEBUG_PRINT("STA ABS"<<endl);

	    	addr = absolute();
	    	ST(regA,addr);
	    	break;

	    case 0x8E:						//STX abs
	    	DEBUG_PRINT("STX ABS"<<endl);

	    	addr = absolute();
	    	ST(regX,addr);
	    	break;

		case 0x91:						//STA (ind,x)
	    	DEBUG_PRINT("STA ind y"<<endl);

	    	addr = indirect_Y();
	    	ST(regA,addr);
	    	break;

		case 0x94:						//STA zpg,X
	    	DEBUG_PRINT("STY zero page x"<<endl);

	    	addr = zero_page(regX);
	    	ST(regY,addr);
	    	break;

		case 0x95:						//STA zpg,X
	    	DEBUG_PRINT("STA zero page x"<<endl);

	    	addr = zero_page(regX);
	    	ST(regA,addr);
	    	break;

		case 0x96:						//STA zpg,Y
	    	DEBUG_PRINT("STX zero page x"<<endl);

	    	addr = zero_page(regY);
	    	ST(regX,addr);
	    	break;
		
		case 0x99:						//STA abs,Y
	    	DEBUG_PRINT("STA abs y"<<endl);

	    	addr = absolute(regY);
	    	ST(regA,addr);
	    	break;
		
		case 0x9A:						//TXS
	    	DEBUG_PRINT("TXS"<<endl);
	    	regs.SP = regs.reg[regX];
	    	break;

		case 0x9D:						//STA abs,X
	    	DEBUG_PRINT("STA abs X"<<endl);

	    	addr = absolute(regX);
	    	ST(regA,addr);
	    	break;

		case 0xA0:						//LDY imm
			DEBUG_PRINT("LDY IMM"<<endl);
			addr = immediate();
			LD(regY,addr);
			break;

		case 0xA1:						//LDA (ind,X)
			DEBUG_PRINT("LDY IMM"<<endl);
			addr = indirect_X();
			LD(regA,memory[addr]);
			break;

		case 0xA2:						//LDX imm
			DEBUG_PRINT("LDX IMM"<<endl);
			addr = immediate();
			LD(regX,addr);
			break;

		case 0xA4:						//LDY zpg
			DEBUG_PRINT("LDY zero"<<endl);
			addr = zero_page();
			LD(regY,memory[addr]);
			break;

		case 0xA5:						//LDA zpg
			DEBUG_PRINT("LDA zero"<<endl);
			addr = zero_page();
			LD(regA,memory[addr]);
			break;

		case 0xA6:						//LDX zpg
			DEBUG_PRINT("LDX zero"<<endl);
			addr = zero_page();
			LD(regX,memory[addr]);
			break;

		case 0xA9:						//LDA imm
			DEBUG_PRINT("LOAD IMM"<<endl);
			addr = immediate();
			LD(regA,addr);
			break;

		case 0xAC:						//LDY abs
			DEBUG_PRINT("LOAD abs"<<endl);
			addr = absolute();
			LD(regY,memory[addr]);
			break;

		case 0xAD:						//LDA abs
			DEBUG_PRINT("LOAD abs"<<endl);
			addr = absolute();
			LD(regA,memory[addr]);
			break;

		case 0xAE:						//LDX abs
			DEBUG_PRINT("LOAD abs"<<endl);
			addr = absolute();
			LD(regX,memory[addr]);
			break;

		case 0xB1:						//LDA ind y
			DEBUG_PRINT("LOAD ind y"<<endl);
			addr = indirect_Y();
			LD(regA,memory[addr]);
			break;

		case 0xB4:						//LDY zpg,X
			DEBUG_PRINT("LOAD ind y"<<endl);
			addr = indirect_X();
			LD(regY,memory[addr]);
			break;

		case 0xB5:						//LDA zpg,X
			DEBUG_PRINT("LOAD ind y"<<endl);
			addr = indirect_X();
			LD(regA,memory[addr]);
			break;
				
		case 0xB6:						//LDX zpg,Y
			DEBUG_PRINT("LOAD ind y"<<endl);
			addr = indirect_Y();
			LD(regX,memory[addr]);
			break;

		case 0xB9:						//LDA abs,Y
			DEBUG_PRINT("LOAD abs y"<<endl);
			addr = absolute(regY);
			LD(regA,memory[addr]);
			break;

		case 0xBC:						//LDY abs,X
			DEBUG_PRINT("LOAD abs X"<<endl);
			addr = absolute(regX);
			LD(regY,memory[addr]);
			break;

		case 0xBD:						//LDA abs,X
			DEBUG_PRINT("LOAD abs X"<<endl);
			addr = absolute(regX);
			LD(regA,memory[addr]);
			break;

		case 0xBE:						//LDX abs,X
			DEBUG_PRINT("LOAD abs Y"<<endl);
			addr = absolute(regY);
			LD(regX,memory[addr]);
			break;

		case 0xEA:						//NOP
			DEBUG_PRINT("NOP"<<endl);
			break;
		
		case 0xD8:						//CLD
			DEBUG_PRINT("CLD"<<endl);
			regs.decimal_mode_flag = false;
			break;
		
		default:
			DEBUG_PRINT("unimplemented: "<<hex<<unsigned(opcode)<<endl);
 	}

  return true;

}