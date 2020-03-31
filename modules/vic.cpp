#include "vic.h"

VIC::VIC(){
    
    control_reg_one(0x9B);
    control_reg_two(0x08);

    registers[BASE_ADDR_REG - REG_START] = 0x14;
	registers[IRQ_REQ_REG - REG_START] = 0x0F;
	registers[IRQ_MASK_REG - REG_START] = 0x0;

	rasterline = 0;

}

VIC::~VIC(){

}

uint8_t VIC::read_register(uint16_t addr){
    
    DEBUG_PRINT("read from VIC memory"<<endl);
    DEBUG_PRINT(hex<<unsigned(registers[addr-IO_START])<<endl);

	if(addr == RASTER_CNT)
		return 0;


    //exit(1);

    return registers[addr-IO_START];

}

void VIC::write_register(uint16_t addr, uint8_t data){

    DEBUG_PRINT("write to VIC memory"<<endl);
    DEBUG_PRINT(hex<<unsigned(registers[addr-IO_START])<<endl);

    switch(addr){
        case CTRL_REG_1:
            control_reg_one(data);
            return;
    }

    addr -= IO_START;
    registers[addr] = data;


}

void VIC::control_reg_one(uint8_t data){

	//Bitmap Mode
	if(GET_I_BIT(data,5)){
		graphic_mode = BITMAP_MODE;
	}

	if(GET_I_BIT(data,3)){
		visible_rows = 24;
	} else {
		visible_rows = 25;
	}

	registers[CTRL_REG_1 - IO_START] = data;


}

void VIC::control_reg_two(uint8_t data){

	if(GET_I_BIT(data,3)){
		visible_cols = 40;
	} else {
		visible_cols = 38;
	}

}