#include "modules/library.h"

#include "modules/memory.h"
#include "modules/cpu.h"
#include "modules/SDLManager.h"
#include "modules/cia1.h"
#include "modules/cia2.h"
#include "modules/loader.h"


void test_cpu(CPU*);

Memory *mem;
CPU *cpu;
SDLManager *sdl;

void dump_mem_handler(int s){
	cout<<endl<<"Dump Video Mem.."<<endl;
	//mem->dump_memory(0x400,1000);							//1000 byte not 1024!
	mem->dump_color_memory();							//1000 byte not 1024!

}

void dump_cpu_handler(int s){
	cout<<endl<<"Dump CPU"<<endl;

	cout<<hex<<"PC: "<<unsigned(cpu->PC)<<endl;

	//sdl->checkFPS();
}

void chiudi(int s){

	SDL_Quit();
	exit(s);
}

int main(int argc, const char **argv){
	//CTRL-Z
//	signal(SIGTSTP,dump_mem_handler);

	signal(SIGTSTP,dump_cpu_handler);
	signal(SIGINT,chiudi);

	VIC *vic = new VIC();
	CIA1 *cia1 = new CIA1();
	CIA2 *cia2 = new CIA2();

	mem = new Memory();
	mem->load_kernal_and_basic(KERNAL_BASIC_ROM);
	mem->load_charset(CHARSET_ROM);

	cpu = new CPU(mem);


	sdl = new SDLManager();

	cia1->setCPU(cpu);
	cia1->setSDL(sdl);

	cia2->setCPU(cpu);
	cia2->setSDL(sdl);

	mem->setVIC(vic);
	mem->setCIA1(cia1);
	mem->setCIA2(cia2);

	vic->setMemory(mem);
	vic->setSDL(sdl);
	vic->setCPU(cpu);
	vic->setCIA1(cia1);
	vic->setCIA2(cia2);

	Loader *loader;

	string s = "";

	if(argc > 1){
		s = argv[1];
		loader = new Loader(cpu,mem,s);
	} else {
		loader = new Loader(cpu,mem,s);
	}

	while(true){
		cpu->clock();
		cia1->clock();
		vic->clock();
		loader->clock();

	}

}

void test_cpu(CPU *cpu)
{
	uint16_t pc=0;
	uint8_t opcode;
	bool loop = true;

	while(loop)
	{
		
		if(pc == cpu->PC)
		{
			cout<<"infinite loop at "<<hex<<unsigned(pc)<<endl;
			break;

		} else if(cpu->PC == 0x3463)
		{
			cout<<"test passed!"<<endl;
			break;
		}
		
		pc = cpu->PC;
		
		opcode = cpu->fetch();
		cout<<"OPCODE: "<<hex<<unsigned(opcode)<<endl;

		loop = cpu->decode(opcode);
		if(loop == false){
			cout<<"Bloccato "<<endl;
		}

		cpu->dump_reg();

	}

}