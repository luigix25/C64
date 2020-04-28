#include "modules/library.h"

#include "modules/memory.h"
#include "modules/cpu.h"
#include "modules/SDLManager.h"
#include "modules/cia1.h"
#include "modules/cia2.h"


void test_cpu(CPU*);

Memory *mem;
CPU *cpu;
SDLManager *sdl;

void dump_mem_handler(int s){
	cout<<endl<<"Dump Video Mem.."<<endl;
	//mem->dump_memory(0x400,1000);							//1000 byte not 1024!
	mem->dump_color_memory(0,1000);							//1000 byte not 1024!

}

void dump_cpu_handler(int s){
	cout<<endl<<"Dump CPU"<<endl;

	sdl->checkFPS();
}

void load_roba(int s){
	mem->load_prg("roms/highentropy.prg");
	//mem->load_prg("roms/delysid_multicolor_testscreen.prg");
	//mem->load_prg("roms/snake.prg");

}

void chiudi(int s){

	SDL_Quit();
	exit(s);
}

int main(){
	//CTRL-Z
//	signal(SIGTSTP,dump_mem_handler);
	signal(SIGTSTP,load_roba);

	//signal(SIGTSTP,dump_cpu_handler);
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