#include "library.h"


uint8_t memory[sixtyfourK];
registers regs;

char convert(char);
bool decode(uint8_t);
bool fetch();
uint8_t immediate();

uint16_t read_two_bytes(uint16_t);

void LD(register_name, uint8_t);
void OR(register_name, uint8_t);
void ST(register_name, uint16_t);
void JMP(uint16_t);

uint16_t absolute();
uint16_t absolute(register_name);

uint8_t zero_page();
uint8_t zero_page(register_name);

uint16_t indirect_Y();
uint16_t indirect_X();

void dump_reg(){

	cout<<"RegA: "<<endl;
	hexDump(&regs.reg[regA],1);

	cout<<"PC: "<<endl;
	hexDump(&regs.PC,1);

	cout<<endl<<endl;


}

int main(){

	dump_reg();

	regs.PC = 0;
	memory[0] = 0xa9;
	memory[1] = 0x0a;
	memory[2] = 0x8d;
	memory[3] = 0x34;
	memory[4] = 0x12;
	memory[5] = 0xa9;
	memory[6] = 0x00;
	memory[7] = 0x0d;
	memory[8] = 0x34;
	memory[9] = 0x12;
	memory[10] = 0x00;



	regs.PC = 0;

	bool loop = true;

	while(loop){

		loop = fetch();
	}

	dump_reg();

	//hexDump(memory+BASIC_START,2);

	//decode(memory[BASIC_START]);

}

bool fetch(){

	uint8_t opcode = memory[regs.PC++];
	return decode(opcode);

}

bool decode(uint8_t opcode){


	//OR LD E ST fatte

  uint8_t high = opcode & 0xFF00;
  uint8_t low = opcode & 0x00FF;

  uint16_t addr,tmp;

  switch(opcode){
    case 0x00:        //BRK
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

		addr = read_two_bytes(tmp);

		JMP(tmp);
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
		break;
  }

  return true;

}

uint8_t zero_page(register_name index){
  //Immediate operand
  
	uint8_t addr;

	addr = immediate();

 	//Sum X register
	addr += regs.reg[index];

	return addr;

}

uint8_t zero_page(){
  
	uint8_t addr;
	addr = memory[regs.PC];

	regs.PC++;

	return addr;

}

uint8_t immediate(){

	uint8_t addr;
	addr = memory[regs.PC];

	regs.PC++;

	return addr;
}

uint16_t read_two_bytes(uint16_t addr){

	uint16_t data = memory[addr];

	uint16_t tmp = memory[addr+1];

	tmp = tmp <<8;

	data |= tmp;


	return data;

}

uint16_t absolute(){

	//Prima parte bassa e poi parte alta
	uint16_t addr;
	addr = memory[regs.PC];

	regs.PC++;

	uint16_t tmp;
	tmp = memory[regs.PC];
	regs.PC++;

	tmp = tmp <<8;
	addr |= tmp;


	return addr;

}

uint16_t absolute(register_name index){

	uint16_t addr = absolute();
	addr+= regs.reg[index];

	return addr;

}

uint16_t indirect_X(){

	uint16_t addr;

	//zero page addr!!
	uint8_t zero_page_addr = immediate();
	uint16_t tmp;

	zero_page_addr += regs.reg[regX];

	//least significant byte
	addr = memory[zero_page_addr];

	tmp = memory[zero_page_addr+1];
	tmp = tmp << 8;

	addr |= tmp;

	return addr; 


}

uint16_t indirect_Y(){

	uint16_t addr;

	//zero page addr!!
	uint8_t zero_page_addr = immediate();
	uint16_t tmp;

	//address is stored inside the zero-page
	//Least significant byte
	addr = memory[zero_page_addr];

	//Most sig. byte
	tmp = memory[zero_page_addr+1];
	tmp = tmp << 8;
	addr |= tmp;


	addr += regs.reg[regY];

	return addr;

}


void OR(register_name index, uint8_t operand){

	regs.reg[index] |= operand;

}

void LD(register_name index, uint8_t operand){

	regs.reg[index] = operand;

}

void ST(register_name index, uint16_t addr){

	memory[addr] = regs.reg[index];

}

void JMP(uint16_t addr){

	regs.PC = memory[addr];

}



/*char convert(char c){
	cout<<"ciao"<<endl;

	if(c >= 65 && c <= 90){					//lower case letters
		cout<<"lower case";
		c+= (97-65);
	} else if(c>=193 && c<= 218){
		cout<<"upper case";
		c-= (193-65);
	}

	return c;


}*/