#include "loader.h"

Loader::Loader(CPU* cpu, Memory* memory, const string& filename){

	this->filename = filename;
	this->cpu = cpu;
	this->mem = memory;

	if(filename != "")
		shouldLoad = true;
}

void Loader::clock(){

	if(!shouldLoad)
		return;

	if(!loaded && cpu->PC == BASIC_READY){
		mem->loadPrg(filename);
		loaded = true;
		cout<<"Loaded!"<<endl;
	}

}
