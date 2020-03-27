#include "library.h"


uint8_t memory[sixtyfourK];
registers regs;

char convert(char);
bool decode(uint8_t);
bool fetch();
uint8_t immediate();
void OR(register_name, uint8_t);
uint16_t absolute();
uint16_t absolute_X();

uint8_t zero_page();
uint8_t zero_page_addr_X();

uint8_t indirect_Y();

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
		dump_reg();

	}



	//hexDump(memory+BASIC_START,2);

	//decode(memory[BASIC_START]);

}

bool fetch(){

	uint8_t opcode = memory[regs.PC++];
	return decode(opcode);

}

bool decode(uint8_t opcode){

  uint8_t high = opcode & 0xFF00;
  uint8_t low = opcode & 0x00FF;

  uint16_t addr;

  switch(opcode){
    case 0x00:        //BRK
      return false;
    case 0x01:        //ORA (ind,X)

      //read from the zero page
      addr = zero_page_addr_X();

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

    case 0x8D:						//STA abs
    	cout<<"STA ABS"<<endl;

    	addr = absolute();
    	memory[addr] = regs.reg[regA];
    	break;

    case 0xA9:						//LDA imm
    	cout<<"LOAD IMM"<<endl;
    	addr = immediate();
    	regs.reg[regA] = (uint8_t)addr;
    	break;

  }

  return true;

}

uint8_t zero_page_addr_X(){
  //Immediate operand
  
  uint8_t addr;
  addr = memory[regs.PC];

  //Sum X register
  addr += regs.reg[regX];
	regs.PC++;

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

uint16_t absolute_X(){

	uint16_t addr = absolute();
	addr+= regs.reg[regX];

	return addr;

}


uint8_t indirect_Y(){

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

}


void OR(register_name index, uint8_t operand){

	regs.reg[index] |= operand;

}

void LD(register_name index, uint8_t operand){


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