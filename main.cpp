#include "modules/library.h"
#include "modules/memory.h"

#include "modules/cpu.h"

#include "modules/debug.h"

char convert(char);

int main(){

	Memory *mem = new Memory();
	VIC *vic = new VIC();

	mem->setVIC(vic);

	vic->setMemory(mem);

	CPU cpu(mem);
	cpu.dump_reg();

    /* Load original Kernal + Basic ROM */
	// mem->load_kernal_and_basic(KERNAL_BASIC_ROM);

    /* Load custom Kernal rom */
    mem->load_custom_kernal_rom(CUSTOM_KERNAL_0);

	mem->load_charset(CHARSET_ROM);

	bool loop = true;
	uint8_t opcode;

	char c;

	while(loop){

		opcode = cpu.fetch();
		loop = cpu.decode(opcode);
		//cpu.dump_reg();

		if(cpu.regs.PC == 0xE39D){
			cout<<"WOOOOOOOOO"<<endl;
			cpu.dump_reg();
			mem->dump_memory(0x314,32);

			return 0;
		}

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
