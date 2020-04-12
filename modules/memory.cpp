//memory.cpp
#include <signal.h>
#include <stdlib.h>

#include "memory.h"

Memory::Memory(){

	//Default values
	LORAM_mode = RAM;
	HIRAM_mode = RAM;
	CHAR_mode = RAM;

	vic = nullptr;

	basic = new uint8_t[eightK];
	kernal = new uint8_t[eightK];
	charset = new uint8_t[fourK];
	memory = new uint8_t[sixtyfourK];

	setup_memory_mode(LORAM_MASK | HIRAM_MASK | CHAR_MASK);

}

Memory::~Memory(){

	delete[] basic;
	delete[] kernal;
	delete[] charset;
}

void Memory::dump_memory(uint16_t addr,uint16_t bytes){

	cout<<endl<<"---------------------"<<endl;

	for(uint16_t i=0;i<bytes;i++){
		cout<<hex<<unsigned(read_byte(addr+i))<<" ";
	}

	cout<<endl<<"---------------------"<<endl;

}

uint8_t Memory::read_byte(uint16_t addr){

	//uint16_t page = addr & 0xff00;

	/*if(addr >= 0xE473 && addr <= 0xE498){
		cout<<"scritta comm"<<endl;
		//raise(SIGPIPE);
	}*/

	//TODO: implementare cartridge!
	if(addr >= BASIC_START and addr <= BASIC_END){
		
		if(LORAM_mode == RAM)
			return memory[addr];
		else
			return basic[addr-BASIC_START];
	
	//VIC CHAR or RAM
	} else if(addr >= IO_START and addr <= IO_END){

		if(CHAR_mode == RAM)
			return memory[addr];
		else if(CHAR_mode == ROM)					//Charset
			return charset[addr-IO_START];
		else if(CHAR_mode == IO){					//VIC
			if(vic != nullptr)
				return vic->read_register(addr);
			else
				return 0xFF;
		}
		
	//TODO: implementare cartridge!
	}else if(addr >= KERNAL_START /*and addr <= KERNAL_END  inutile */){
		
		if(HIRAM_mode == RAM)
			return memory[addr];
		else
			return kernal[addr-KERNAL_START];
	
	} 
	
	return memory[addr];
}

void Memory::write_byte(uint16_t addr, uint8_t data){

  	uint16_t page = (addr & 0xff00) >> 8;

  	//Zero Page
  	if(page == 0){

  		if(addr == MEMORY_LAYOUT_ADDR){
  			setup_memory_mode(data);
  		} else if(addr == 0xCC){
  			cout<<"scrivo blanking "<<hex<<unsigned(data)<<endl;
  			raise(SIGPIPE);
  		} 

  	} else if(addr >= VIC_START and addr <= VIC_END){
		if(CHAR_mode == IO){		//VIC
			vic->write_register(addr,data);
			return;
		} else {
			memory[addr] = data;
		}

	}

	memory[addr] = data;


}

void Memory::setup_memory_mode(uint8_t value){
  
	bool loram_en  = ((value & HIRAM_MASK) != 0);
	bool hiram_en = ((value & LORAM_MASK) != 0);
	bool char_en = ((value & CHAR_MASK) != 0);

	//Set everything to RAM as default
	HIRAM_mode = RAM;
	LORAM_mode = RAM;
	CHAR_mode = RAM;

	//Basic
	if(hiram_en)
		HIRAM_mode = ROM;

	//Kernal
	if(loram_en && hiram_en)
		LORAM_mode = ROM;

	//Char I/O
	if(char_en && (loram_en || hiram_en))
		CHAR_mode = IO;
	else if(char_en && !loram_en && !hiram_en)
		CHAR_mode = RAM;
	else 
		CHAR_mode = ROM;
	
	memory[MEMORY_LAYOUT_ADDR] = value;

}

void Memory::load_kernal_and_basic(const char* filename){

	streampos size;
	uint8_t* rom = read_bin_file(filename,size);

	//FIRST 8K are basic
	memcpy(basic, rom, eightK);
	memcpy(kernal, rom+eightK, eightK);

	delete[] rom;

}

void Memory::load_charset(const char* filename){

	streampos size;

	uint8_t* rom = read_bin_file(filename,size);

	memcpy(charset, rom, fourK);
	
	delete[] rom;
}

uint8_t* Memory::read_bin_file(const char* filename, streampos &size){

    ifstream file(filename,ios::in | ios::binary | ios::ate);
    //streampos size;

    if (file.is_open())
    {

        size = file.tellg();

        uint8_t *rom = new uint8_t[size];

        file.seekg (0, ios::beg);
        file.read ((char*)rom, size);
        file.close();
        return rom;
    }

    return null;

}

void Memory::load_custom_memory(const char* filename, uint16_t offset) {

	streampos size;
	uint8_t* buffer = read_bin_file(filename,size);
	memcpy(memory+offset, buffer, size);

}

void Memory::setVIC(VIC* vic){
	this->vic = vic;
}

uint8_t* Memory::getMemPointer(){
	return memory;
}

uint8_t* Memory::getKerPointer(){
	return kernal;
}

uint8_t* Memory::getVideoMemoryPtr(){
	return memory+0x400;
}

uint8_t* Memory::getColorMemoryPtr(){
	return memory+0xD800;
}

uint8_t* Memory::getCharROMPtr(){
	return charset;
}

