#include "modules/library.h"
#include "modules/cpu.h"
#include "modules/memory.h"

#include "modules/debug.h"

//uint8_t memory[sixtyfourK];

char convert(char);

int main(){

	Memory *mem = new Memory();

	CPU cpu(mem);
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

	mem->load_kernal_and_basic(KERNAL_BASIC_ROM);
	mem->load_charset(CHARSET_ROM);

	/*loadKernalAndBasic(memory,KERNAL_BASIC_ROM);
	loadCharset(memory,CHARSET_ROM);
*/

	bool loop = true;
	uint8_t opcode;

	char c;

	while(loop){

		opcode = cpu.fetch();
		loop = cpu.decode(opcode);
		//cpu.dump_reg();

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