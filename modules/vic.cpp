#include "vic.h"

VIC::VIC(){

}

VIC::~VIC(){

}

uint8_t VIC::read_register(uint16_t addr){
    
    DEBUG_PRINT("read from VIC memory"<<endl);
    DEBUG_PRINT(hex<<unsigned(registers[addr-IO_START])<<endl);
    exit(1);

    return registers[addr-IO_START];

}

void VIC::write_register(uint16_t addr, uint8_t data){

    DEBUG_PRINT("write to VIC memory"<<endl);
    DEBUG_PRINT(hex<<unsigned(registers[addr-IO_START])<<endl);


    switch(addr){
        
    }

    addr -= IO_START;
    registers[addr] = data;


}
