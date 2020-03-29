//memory.cpp

#include "memory.h"

Memory::Memory(){


}

uint8_t Memory::read_byte(uint16_t addr){

	return 1;
}

void Memory::write_byte(uint16_t adrr, uint8_t data){


}

void Memory::load_kernal_and_basic(const char* filename){

	uint8_t* rom = read_bin_file(filename);

	//FIRST 8K are basic
	memcpy(&basic, rom, eightK);
	memcpy(&kernal, rom+eightK, eightK);

	delete[] rom;

}

void Memory::load_charset(const char* filename){

	uint8_t* rom = read_bin_file(filename);

	memcpy(&charset, rom, fourK);
	
	delete[] rom;
}

uint8_t* read_bin_file(const char* filename){

    ifstream file(filename,ios::in | ios::binary | ios::ate);
    streampos size;

    if (file.is_open())
    {

        size = file.tellg();

        uint8_t *rom = new uint8_t[size];

        file.seekg (0, ios::beg);
        file.read ((char*)rom, size);
        file.close();
        return rom;
    }

    return nullptr;

}
