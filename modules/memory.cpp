//memory.cpp

#include "memory.h"

Memory::Memory(){

	LORAM_enabled = false;
	HIRAM_enabled = false;
	CHAR_enabled = false;
}

uint8_t Memory::read_byte(uint16_t addr){

	//uint16_t page = addr & 0xff00;

	if(addr >= BASIC_START and addr <= BASIC_END){
		
		if(LORAM_enabled)
			return memory[addr];
		else
			return basic[addr-BASIC_START];
	
	} else if(addr >= KERNAL_START /*and addr <= KERNAL_END  inutile */){
		
		if(HIRAM_enabled)
			return memory[addr];
		else{
			return kernal[addr-KERNAL_START];
		}
	
	} else if(addr >= CHAR_START and addr <= CHAR_END){

		if(CHAR_enabled)
			return memory[addr];
		else
			return charset[addr-CHAR_START];

	}
	
	return memory[addr];
}

void Memory::write_byte(uint16_t addr, uint8_t data){

  	uint16_t page = addr & 0xff00;

  	//Zero Page
  	if(page == 0){

  		if(addr == MEMORY_LAYOUT_ADDR){
  			setup_memory_mode(data);
  			return;
  		}

  	}

	memory[addr] = data;


}

void Memory::setup_memory_mode(uint8_t value){
  
  	cout<<"CHANGE IN MEMORY MODE!!"<<endl;
	LORAM_enabled  = ((value & HIRAM_MASK) == 0);
	HIRAM_enabled  = ((value & LORAM_MASK) == 0);
	CHAR_enabled = ((value & CHAR_MASK) == 0);

}

void Memory::load_kernal_and_basic(const char* filename){

	uint8_t* rom = read_bin_file(filename);

	//FIRST 8K are basic
	memcpy(basic, rom, eightK);
	memcpy(kernal, rom+eightK, eightK);

	/*memcpy(memory+BASIC_START, rom, eightK);
	memcpy(memory+KERNAL_START, rom+eightK, eightK);

	uint16_t addr = 0xFCE2 - KERNAL_START;
*/
	delete[] rom;

}

void Memory::load_charset(const char* filename){

	uint8_t* rom = read_bin_file(filename);

	memcpy(charset, rom, fourK);
	
	delete[] rom;
}

uint8_t* Memory::read_bin_file(const char* filename){

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

    return null;

}
