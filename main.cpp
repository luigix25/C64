#include "modules/library.h"
#include "modules/cpu.h"
#include "modules/debug.h"

uint8_t memory[sixtyfourK];

char convert(char);

int main(){

	CPU cpu(memory);
	cpu.dump_reg();


	/*regs.PC = 0;
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
	*/

	loadKernalAndBasic(memory,KERNAL_BASIC_ROM);

	//hexDump(memory+0xFFFC,10);

	//DEBUG_PRINT("ciao"<<" "<<"we"<<endl);

	bool loop = true;
	uint8_t opcode;


	while(loop){

		opcode = cpu.fetch();
		loop = cpu.decode(opcode);

	}

	cpu.dump_reg();

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