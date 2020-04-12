#include "cia1.h"

CIA1::CIA1(){

}

CIA1::~CIA1(){

}

uint8_t CIA1::read_register(uint16_t address){

	//cout<<"reading from cia1"<<endl;
	//cout<<hex<<unsigned(address)<<endl;

	//Masking first byte
	address &= 0x0F;
	//For mirroring
	address = address % 16;


	return registers[address];

}

void CIA1::write_register(uint16_t address, uint8_t data){

	//cout<<"writing to cia1"<<endl;
	//cout<<hex<<unsigned(address)<<" : "<<hex<<unsigned(data)<<endl;

	//Masking first byte
	address &= 0x0F;
	//For mirroring
	address = address % 16;



	registers[address] = data;

}