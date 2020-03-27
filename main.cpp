#include "library.h"


uint8_t memory[sixtyfourK];
registers regs;

char convert(char);
bool decode(uint8_t);
bool fetch();
uint8_t immediate();
void ORA(uint8_t);
uint16_t absolute();
uint8_t zero_page();
uint8_t zero_page_addr_X();


void dump_reg(){

	cout<<"RegA: "<<endl;
	hexDump(&regs.regA,1);

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

      ORA(memory[addr]);
      break;

    case 0x05:      //ORA zpg
    	addr = zero_page();
    	ORA(memory[addr]);
    	break;

    case 0x06:			//ASL
    	break;
    
    case 0x08:			//PHP
    	break;

    case 0x09:			//ORA imm
    	addr = immediate();
    	ORA(addr);
    	break;

    case 0x0D:
      cout<<"ORA ABS"<<endl;

    	addr = absolute();		//ORA ABS
    	ORA(memory[addr]);
    	break;

    case 0x8D:						//STA abs
    	cout<<"STA ABS"<<endl;

    	addr = absolute();
    	memory[addr] = regs.regA;
    	break;

    case 0xA9:						//LDA imm
    	cout<<"LOAD IMM"<<endl;
    	addr = immediate();
    	regs.regA = (uint8_t)addr;
    	//cout<<"rega: "<<regs.regA<<endl;

    	break;

  }

  return true;

}

uint8_t zero_page_addr_X(){
  //Immediate operand
  
  uint8_t addr;
  addr = memory[regs.PC];

  //Sum X register
  addr += regs.regX;
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

void ORA(uint8_t operand){

	regs.regA |= operand;

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