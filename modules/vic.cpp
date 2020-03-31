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

void VIC::setMemory(Memory *mem){
	this->memory = mem;
}

uint8_t VIC::read_register(uint16_t addr){
    
    DEBUG_PRINT("read from VIC memory"<<endl);
    DEBUG_PRINT(hex<<unsigned(registers[addr-IO_START])<<endl);

	if(addr == RASTER_CNT)
		return 0;

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

	set_graphic_mode();

	if(GET_I_BIT(data,3)){
		visible_rows = 24;
	} else {
		visible_rows = 25;
	}

	registers[CTRL_REG_1_OFF] = data;


}

void VIC::control_reg_two(uint8_t data){

	set_graphic_mode();

	if(GET_I_BIT(data,3)){
		visible_cols = 40;
	} else {
		visible_cols = 38;
	}

}

void VIC::set_graphic_mode(){

	bool ecm = GET_I_BIT(registers[CTRL_REG_1_OFF],6);
	bool bmm = GET_I_BIT(registers[CTRL_REG_1_OFF],5); 
	bool mcm = GET_I_BIT(registers[CTRL_REG_2_OFF],4); 

	if(!ecm && !bmm && !mcm){ 
		graphic_mode = CHAR_MODE;
		cout<<"CHAR MODE"<<endl;
	}
	else if(!ecm && !bmm && mcm)
		graphic_mode = MCM_TEXT_MODE;
	else if(!ecm && bmm && !mcm)
		graphic_mode = BITMAP_MODE;
	else if(!ecm && bmm && mcm)
		graphic_mode = MCB_BITMAP_MODE;
	else
		cout<<"UNIMPL MODE"<<endl;

	//Unimplemented
	/*else if(ecm && !bmm && !mcm)
		graphic_mode_ = kExtBgMode;
	else 
		graphic_mode_ = kIllegalMode;
	*/
}