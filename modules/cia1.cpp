#include "cia1.h"

CIA1::CIA1(){

}

CIA1::~CIA1(){

}

uint8_t CIA1::read_register(uint16_t address){

	//Masking first byte
	address &= 0x0F;
	//For mirroring
	address = address % 16;
		cout<<"reading from cia1"<<endl;

	return registers[address];

}

void CIA1::write_register(uint16_t address, uint8_t data){

	//Masking first byte
	address &= 0x0F;
	//For mirroring
	address = address % 16;

	cout<<"writing to cia1"<<endl;

	registers[address] = data;

}