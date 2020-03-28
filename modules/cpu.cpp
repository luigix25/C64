#include "cpu.h"


CPU::CPU(uint8_t* memory){

	this->memory = memory;
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

uint8_t CPU::fetch(){

	uint8_t opcode = read_byte(regs.PC);

	regs.PC++;
	return opcode;

}

//DEBUG
void CPU::dump_reg(){

	cout<<endl<<endl;
	cout<<"---------REG STATUS-----------"<<endl;
	cout<<"RegA: 0x"<<hex<<unsigned(regs.reg[regA])<<endl;
	cout<<"RegX: 0x"<<hex<<unsigned(regs.reg[regX])<<endl;
	cout<<"RegY: 0x"<<hex<<unsigned(regs.reg[regY])<<endl;
	cout<<"PC  : 0x"<<hex<<unsigned(regs.PC)<<endl;
	//hexDump(&regs.PC,1);
	cout<<"------------------------------"<<endl;

	cout<<endl<<endl;

}

bool CPU::decode(uint8_t opcode){

	//OR LD E ST fatte

	uint16_t addr,tmp;

	switch(opcode){
		case 0x00:        //BRK
		cout<<"BRK"<<endl;
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
	    
		case 0x08:			//PHP
			break;

		case 0x09:			//ORA imm
			addr = immediate();
			OR(regA,addr);
			break;

		case 0x0D:
			cout<<"ORA ABS"<<endl;
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
			cout<<"SEI"<<endl;
			regs.interrupt_flag = true;
			break;

	    case 0x81:						//STA (ind,X)
	    	cout<<"STA indirect X"<<endl;

	    	addr = indirect_X();
	    	ST(regA,addr);

	    	//memory[addr] = regs.reg[regA];
	    	break;

	    case 0x84:						//STY zpg
	    	cout<<"STY zpg"<<endl;

	    	addr = zero_page();
	    	ST(regY,addr);

	    	//memory[addr] = regs.reg[regA];
	    	break;

	    case 0x85:						//STA zpg
	    	cout<<"STA zpg"<<endl;

	    	addr = zero_page();
	    	ST(regA,addr);

	    	//memory[addr] = regs.reg[regA];
	    	break;

	    case 0x86:						//STX zpg
	    	cout<<"STX zpg"<<endl;

	    	addr = zero_page();
	    	ST(regX,addr);

	    	//memory[addr] = regs.reg[regA];
	    	break;

	    case 0x8C:						//STY abs
	    	cout<<"STY abs"<<endl;

	    	addr = absolute();
	    	ST(regY,addr);

	    	//memory[addr] = regs.reg[regA];
	    	break;

	    case 0x8D:						//STA abs
	    	cout<<"STA ABS"<<endl;

	    	addr = absolute();
	    	ST(regA,addr);
	    	break;

	    case 0x8E:						//STX abs
	    	cout<<"STX ABS"<<endl;

	    	addr = absolute();
	    	ST(regX,addr);
	    	break;

		case 0x91:						//STA (ind,x)
	    	cout<<"STA ind y"<<endl;

	    	addr = indirect_Y();
	    	ST(regA,addr);
	    	break;

		case 0x94:						//STA zpg,X
	    	cout<<"STY zero page x"<<endl;

	    	addr = zero_page(regX);
	    	ST(regY,addr);
	    	break;

		case 0x95:						//STA zpg,X
	    	cout<<"STA zero page x"<<endl;

	    	addr = zero_page(regX);
	    	ST(regA,addr);
	    	break;

		case 0x96:						//STA zpg,Y
	    	cout<<"STX zero page x"<<endl;

	    	addr = zero_page(regY);
	    	ST(regX,addr);
	    	break;
		
		case 0x99:						//STA abs,Y
	    	cout<<"STA abs y"<<endl;

	    	addr = absolute(regY);
	    	ST(regA,addr);
	    	break;
		
		case 0x9A:						//TXS
	    	cout<<"TXS"<<endl;
	    	regs.SP = regs.reg[regX];
	    	break;

		case 0x9D:						//STA abs,X
	    	cout<<"STA abs X"<<endl;

	    	addr = absolute(regX);
	    	ST(regA,addr);
	    	break;

		case 0xA0:						//LDY imm
			cout<<"LDY IMM"<<endl;
			addr = immediate();
			LD(regY,addr);
			break;

		case 0xA1:						//LDA (ind,X)
			cout<<"LDY IMM"<<endl;
			addr = indirect_X();
			LD(regA,memory[addr]);
			break;

		case 0xA2:						//LDX imm
			cout<<"LDX IMM"<<endl;
			addr = immediate();
			LD(regX,addr);
			break;

		case 0xA4:						//LDY zpg
			cout<<"LDY zero"<<endl;
			addr = zero_page();
			LD(regY,memory[addr]);
			break;

		case 0xA5:						//LDA zpg
			cout<<"LDA zero"<<endl;
			addr = zero_page();
			LD(regA,memory[addr]);
			break;

		case 0xA6:						//LDX zpg
			cout<<"LDX zero"<<endl;
			addr = zero_page();
			LD(regX,memory[addr]);
			break;

		case 0xA9:						//LDA imm
			cout<<"LOAD IMM"<<endl;
			addr = immediate();
			LD(regA,addr);
			break;

		case 0xAC:						//LDY abs
			cout<<"LOAD abs"<<endl;
			addr = absolute();
			LD(regY,memory[addr]);
			break;

		case 0xAD:						//LDA abs
			cout<<"LOAD abs"<<endl;
			addr = absolute();
			LD(regA,memory[addr]);
			break;

		case 0xAE:						//LDX abs
			cout<<"LOAD abs"<<endl;
			addr = absolute();
			LD(regX,memory[addr]);
			break;

		case 0xB1:						//LDA ind y
			cout<<"LOAD ind y"<<endl;
			addr = indirect_Y();
			LD(regA,memory[addr]);
			break;

		case 0xB4:						//LDY zpg,X
			cout<<"LOAD ind y"<<endl;
			addr = indirect_X();
			LD(regY,memory[addr]);
			break;

		case 0xB5:						//LDA zpg,X
			cout<<"LOAD ind y"<<endl;
			addr = indirect_X();
			LD(regA,memory[addr]);
			break;
				
		case 0xB6:						//LDX zpg,Y
			cout<<"LOAD ind y"<<endl;
			addr = indirect_Y();
			LD(regX,memory[addr]);
			break;

		case 0xB9:						//LDA abs,Y
			cout<<"LOAD abs y"<<endl;
			addr = absolute(regY);
			LD(regA,memory[addr]);
			break;

		case 0xBC:						//LDY abs,X
			cout<<"LOAD abs X"<<endl;
			addr = absolute(regX);
			LD(regY,memory[addr]);
			break;

		case 0xBD:						//LDA abs,X
			cout<<"LOAD abs X"<<endl;
			addr = absolute(regX);
			LD(regA,memory[addr]);
			break;

		case 0xBE:						//LDX abs,X
			cout<<"LOAD abs Y"<<endl;
			addr = absolute(regY);
			LD(regX,memory[addr]);
			break;

		case 0xEA:						//NOP
			cout<<"NOP"<<endl;
			break;
		
		case 0xD8:						//CLD
			cout<<"CLD"<<endl;
			regs.decimal_mode_flag = false;
			break;
		
		default:
			cout<<"unimplemented: "<<hex<<unsigned(opcode)<<endl;
 	}

  return true;

}