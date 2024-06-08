//memory.cpp
#include "memory.h"

Memory::Memory(){

	//Default values
	LORAM_mode = RAM;
	HIRAM_mode = RAM;
	CHAR_mode = RAM;

	basic = new uint8_t[eightK];
	kernal = new uint8_t[eightK];
	charset = new uint8_t[fourK];
	memory = new uint8_t[sixtyfourK];

	color_ram = new uint8_t[1000];

	//clearing video mem and color ram
	memset(memory+0x400,0,1000);
	memset(color_ram,0,1000);

	bankSwitch(LORAM_MASK | HIRAM_MASK | CHAREN_MASK);

}

Memory::~Memory(){

	delete[] basic;
	delete[] kernal;
	delete[] charset;
	delete[] memory;
	delete[] color_ram;
}

void Memory::dump_memory(uint16_t addr,uint16_t bytes){

	cout<<endl<<"---------------------"<<endl;

	int conta = 0;
	for(uint16_t i=0;i<bytes;i++){
		cout<<hex<<unsigned(read_byte(addr+i))<<" ";

		conta++;
		if(conta == 16){
			cout<<endl;
			conta = 0;
		}



	}

	cout<<endl<<"---------------------"<<endl;

}


void Memory::dump_color_memory(){

	cout<<endl<<"---------------------"<<endl;

	hexDump(color_ram,1000);

	/*for(uint16_t i=0;i<bytes;i++){
		cout<<hex<<unsigned(color_ram[addr+i])<<" ";
	}*/

	cout<<endl<<"---------------------"<<endl;

}

uint8_t Memory::read_byte(uint16_t addr){

	//uint16_t page = addr & 0xff00;

	//TODO: implementare cartridge!
	if(addr >= BASIC_START and addr <= BASIC_END){

		if(LORAM_mode == RAM)
			return memory[addr];
		else
			return basic[addr-BASIC_START];

	//I/O CHAR or RAM
	} else if(addr >= IO_START and addr <= IO_END){

		if(CHAR_mode == RAM)
			return memory[addr];

		else if(CHAR_mode == ROM)								//Charset
			return charset[addr-IO_START];

		else if(CHAR_mode == IO){								//Multiplexing on IO peripherals

			if(addr >= VIC_START && addr <= VIC_END){			//VIC

				return vic->read_register(addr);

			} else if(addr >= CIA1_START and addr <= CIA1_END){		//CIA1

				return cia1->read_register(addr);

			} else if(addr >= CIA2_START and addr <= CIA2_END){		//CIA2

				return cia2->read_register(addr);

			} else if(addr >= COLOR_RAM_START && addr <= COLOR_RAM_END){

				return color_ram[addr - COLOR_RAM_START];

			} else {
				//Unimplemented
				return 0xFF;
			}

		}

	}  else if(addr >= KERNAL_START /*and addr <= KERNAL_END  inutile */){

		if(HIRAM_mode == RAM)
			return memory[addr];
		else
			return kernal[addr-KERNAL_START];

	}

	return memory[addr];
}

uint16_t Memory::read_word(uint16_t addr){

	uint16_t data = read_byte(addr);
	uint16_t tmp = read_byte(addr+1);

	tmp = tmp <<8;
	data |= tmp;

	return data;

}

void Memory::write_byte(uint16_t addr, uint8_t data){

	uint16_t page = (addr & 0xff00) >> 8;

	//Zero Page
	if(page == 0){

		if(addr == MEMORY_LAYOUT_ADDR){
			bankSwitch(data);
		} else if(addr == 0xCC){
			//cout<<"scrivo blanking "<<hex<<unsigned(data)<<endl;
			//raise(SIGPIPE);
		}

	} else if(addr >= VIC_START and addr <= VIC_END){
		if(CHAR_mode == IO){		//VIC
			vic->write_register(addr,data);
			return;
		}
	} else if(addr >= CIA1_START and addr <= CIA1_END){
		cia1->write_register(addr,data);
		return;

	} else if(addr >= CIA2_START and addr <= CIA2_END){
		cia2->write_register(addr,data);
		return;

	} else if(addr >= COLOR_RAM_START and addr <= COLOR_RAM_END){
		if(CHAR_mode == IO){
			color_ram[addr - COLOR_RAM_START] = data;
			return;
		}
	}

	memory[addr] = data;

}

uint8_t Memory::VIC_read_byte(uint16_t addr){


	uint8_t VICBank = cia2->getVICBank();

	addr += 0x4000 * VICBank;

	//cout<<"VIC READ "<<hex<<unsigned(addr)<<endl;

	if(VICBank == 0 and addr >= 0x1000 and addr <= 0x1FFF){
		//Charset mirroring
		return charset[addr-0x1000];
	} else if(VICBank == 2 and addr >= 0x9000 and addr <= 0x9FFF){
		return charset[addr-0x9000];
	} else {
		return memory[addr];
	}

}

void Memory::bankSwitch(uint8_t value){

	bool loram_en  = ((value & LORAM_MASK) != 0);
	bool hiram_en = ((value & HIRAM_MASK) != 0);
	bool char_en = ((value & CHAREN_MASK) != 0);

	//Set everything to RAM as default
	HIRAM_mode = RAM;
	LORAM_mode = RAM;
	CHAR_mode = RAM;

	//Kernal
	if(hiram_en){
		HIRAM_mode = ROM;
	}

	//Basic
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

void Memory::load_kernal_and_basic(const string& filename){

	streampos size;
	uint8_t* rom = readBinFile(filename,size);

	if(!rom){
		cerr<<"Errore ROM"<<endl;
		return;
	}

	//FIRST 8K are basic
	memcpy(basic, rom, eightK);
	memcpy(kernal, rom+eightK, eightK);

	delete[] rom;

}

void Memory::load_charset(const string& filename){

	streampos size;

	uint8_t* rom = readBinFile(filename,size);
	if(!rom){
		cout<<"Errore ROM"<<endl;
		return;
	}
	memcpy(charset, rom, fourK);

	delete[] rom;
}

void Memory::load_custom_memory(const string& filename, uint16_t offset) {

	streampos size;
	uint8_t* buffer = readBinFile(filename,size);
	memcpy(memory+offset, buffer, size);
	delete[] buffer;

}

void Memory::loadPrg(const string& filename) {

	streampos size;
	uint8_t* buffer = readBinFile(filename,size);

	uint16_t addr = buffer[1] << 8 | buffer[0];

	size -= 2;

	memcpy(memory+addr, buffer+2, size);

	delete[] buffer;

}

void Memory::setVIC(VIC* vic){
	this->vic = vic;
}

void Memory::setCIA1(CIA1* cia1){
	this->cia1 = cia1;
}

void Memory::setCIA2(CIA2* cia2){
	this->cia2 = cia2;
}

uint8_t* Memory::getMemPointer(){
	return memory;
}

uint8_t* Memory::getKerPointer(){
	return kernal;
}

uint8_t* Memory::getColorMemoryPtr(){
	return color_ram;
}
