#include "cia2.h"

CIA2::CIA2(){

    VICBank = 0;

}


void CIA2::setCPU(CPU* cpu){

	this->cpu = cpu;
}

void CIA2::setSDL(SDLManager* sdl){

	this->sdl = sdl;
}

uint8_t CIA2::read_register(uint16_t address){

	//Masking first byte
	address &= 0x00FF;
	//For mirroring
	address = address % 16;

    return registers[address];

}


void CIA2::write_register(uint16_t address, uint8_t data){

	//Masking first byte
	address &= 0x00FF;
	//For mirroring
	address = address % 16;

    //DD00
    if(address == 0){
        VICBank = (~data) & 0x03;
    }


    registers[address] = data;
}

uint8_t CIA2::getVICBank(){
    return VICBank;
}

