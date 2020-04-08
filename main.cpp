#include "modules/library.h"
#include "modules/memory.h"
#include "modules/cpu.h"
#include "modules/debug.h"

#include "modules/Rom.h"
#include "modules/BusDevice.h"
#include "modules/Bus.h"

#include <signal.h>
#include <stdlib.h>

//uint8_t memory[sixtyfourK];
void test_cpu(CPU*);

char convert(char);
/*Memory *mem;
CPU *cpu;*/

/*
void dump_mem_handler(int s){
	cout<<endl<<"Dump Video Mem.."<<endl;
	mem->dump_memory(0x400,1024);
}

void dump_cpu_handler(int s){
	cout<<endl<<"Dump CPU"<<endl;
	cout<<hex<<unsigned(cpu->regs.PC)<<endl;
}
*/

int main(int argc, char** argv) {
	// CTRL-Z
	//signal(SIGTSTP,dump_mem_handler);
	//signal(SIGPIPE,dump_cpu_handler);

	//mem = new Memory();
	//VIC *vic = new VIC();

	//mem->setVIC(vic);

	//vic->setMemory(mem);

	Bus mainBus;

	Rom rom_kernal(0xe000, 1 << 13);
	rom_kernal.enable(true);
	rom_kernal.fromDisk("roms/custom_0.bin");
	mainBus.addDevice(&rom_kernal);

	CPU cpu(mainBus);

//	cpu = new CPU(mem);



	//mem->load_kernal_and_basic(KERNAL_BASIC_ROM);
	//mem->load_charset(CHARSET_ROM);

	/*mem->load_custom_memory("roms/6502_functional_test.bin",0x400);
	mem->setup_memory_mode(0);*/

/*
	cpu->regs.PC = 0x400;
	cpu->dump_reg();

	test_cpu(cpu);

	return -1;


*/
	/*loadKernalAndBasic(memory,KERNAL_BASIC_ROM);
	loadCharset(memory,CHARSET_ROM);
*/

	bool loop = true;
	uint8_t opcode;

	char c;

	bool debug = true;

	while (loop) {
		cout << "loop " << flush;

		opcode = cpu.fetch();
		loop = cpu.decode(opcode);
/*
		if(cpu->regs.PC == 0xF1CA){
			cout<<"Routine Stampa carattere: ";
			cout<<hex<<unsigned(cpu->regs.reg[regA])<<endl;
			debug = true;
			//cpu->dump_reg();

			//return 0;
		}
*/
		if(debug)
			cpu.dump_reg();


	}

	cpu.dump_reg();

}

void test_cpu(CPU *cpu)
{
	uint16_t pc=0;
	/* unmap C64 ROMs */
	uint8_t opcode;
	bool loop = true;

	while(loop)
	{

		if(pc == cpu->regs.PC)
		{
			cout<<"infinite loop at "<<hex<<unsigned(pc)<<endl;
			//mem->dump_memory(0x0, 0x16);
			break;

		} else if(cpu->regs.PC == 0x3463)
		{
			cout<<"test passed!"<<endl;
			break;
		}

		pc = cpu->regs.PC;

		opcode = cpu->fetch();
		cout<<"OPCODE: "<<hex<<unsigned(opcode) << " ";

		loop = cpu->decode(opcode);
		if(loop == false){
			cout<<"Bloccato "<<endl;
		}

		cpu->dump_reg();

		char c;
		//cin>>c;
	}

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