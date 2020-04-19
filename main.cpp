#include "modules/library.h"
#include "modules/memory.h"
#include "modules/cpu.h"
#include "modules/SDLManager.h"
#include "modules/cia1.h"

#include "modules/debug.h"

#include <stdlib.h>
#include <SDL2/SDL.h>

void test_cpu(CPU*);

char convert(char);
Memory *mem;
CPU *cpu;
SDLManager *sdl;

void dump_mem_handler(int s){
	cout<<endl<<"Dump Video Mem.."<<endl;
	mem->dump_memory(0x400,1000);					//1000 byte not 1024!
	mem->dump_color_memory(0xD800,1000);					//1000 byte not 1024!

	//cpu->changeIRQ();


}

void dump_cpu_handler(int s){
	cout<<endl<<"Dump CPU"<<endl;

	sdl->checkFPS();
}

void chiudi(int s){

	SDL_Quit();
	exit(-1);
}

int main(){
	//CTRL-Z
	signal(SIGTSTP,dump_mem_handler);
	//signal(SIGTSTP,dump_cpu_handler);
	signal(SIGINT,chiudi);

	VIC *vic = new VIC();
	CIA1 *cia1 = new CIA1();

	mem = new Memory();
	mem->load_kernal_and_basic(KERNAL_BASIC_ROM);
	mem->load_charset(CHARSET_ROM);

	cpu = new CPU(mem);

	cia1->setCPU(cpu);

	sdl = new SDLManager();
	sdl->setCIA1(cia1);

	mem->setVIC(vic);
	mem->setCIA1(cia1);

	vic->setMemory(mem);
	vic->setSDL(sdl);
	vic->setCPU(cpu);
	vic->setCIA1(cia1);

	while(true){
		cpu->clock();
		cia1->clock();
		vic->clock();
	}

}

void test_cpu(CPU *cpu)
{
	uint16_t pc=0;
	uint8_t opcode;
	bool loop = true;

	while(loop)
	{
		
		if(pc == cpu->regs.PC)
		{
			cout<<"infinite loop at "<<hex<<unsigned(pc)<<endl;
			break;

		} else if(cpu->regs.PC == 0x3463)
		{
			cout<<"test passed!"<<endl;
			break;
		}
		
		pc = cpu->regs.PC;
		
		opcode = cpu->fetch();
		cout<<"OPCODE: "<<hex<<unsigned(opcode)<<endl;

		loop = cpu->decode(opcode);
		if(loop == false){
			cout<<"Bloccato "<<endl;
		}

		cpu->dump_reg();

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