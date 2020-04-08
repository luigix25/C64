#include "Bus.h"

bool Bus::addDevice(BusDevice* dev) {
    this->devices.push_back(dev);
    return true;
}

uint8_t Bus::read(uint16_t address) {

    /* TODO based on some special address that is accessed,
    enable or disable bus devices =)
     */

    for (BusDevice* device : this->devices) {
        if (! device->isEnabled()) continue;

        if (address >= device->getBase() && address < device->getBase() + device->getSize()) {
            return device->read(address - device->getBase());
        }
    }

    return 0x55;

    /*
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
	}else if(addr >= KERNAL_START){

		if(HIRAM_mode == RAM)
			return memory[addr];
		else
			return kernal[addr-KERNAL_START];

	}*/
}

void Bus::write(uint16_t address, uint8_t data) {
    ; // TODO
}