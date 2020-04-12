#include "vic.h"

VIC::VIC(){

	registers = new uint8_t[0x400];    

	registers[CTRL_REG_1_OFF] = 0x9B;
	registers[CTRL_REG_2_OFF] = 0x08;

    control_reg_one(0x9B);
    control_reg_two(0x08);

	screen_memory_base_addr = 0x400;
	char_memory_base_addr = 0xd000;
	//bitmap_memory_base_addr = 0x0;

    registers[BASE_ADDR_REG - REG_START] = 0x14;
	registers[IRQ_REQ_REG - REG_START] = 0x0F;
	registers[IRQ_MASK_REG - REG_START] = 0x0;

	rasterline = 0;

	video_loop_thread = new thread(&VIC::video_loop,this);

}

VIC::~VIC(){

	delete[] registers;
	delete video_loop_thread;

}


void VIC::show_char(uint8_t *character, int X, int Y){

	uint8_t bg_color = registers[0xD021-0xD000];
	uint8_t fg_color = *(guest_color_memory + 40 * X/8 + Y/8);

	for(int i = 0; i < CHAR_WIDTH; i++){
		uint8_t *ptr = host_video_memory + SCREEN_WIDTH * (i+X) + Y;

		for(int j=0; j < CHAR_WIDTH; j++){
			ptr[j] = *(character + (i*8) +j) ? fg_color : bg_color;
		}

		//memcpy(ptr, character + (i*8) ,8);
	}

}

void VIC::video_loop(){

	while(true){
		if(sdl == nullptr)
			continue;

		uint32_t cursorX = 0;
		uint32_t cursorY = 0;

		for(int i=0;i<1000;i++){

			show_char(host_charset + 64 * guest_video_memory[i] ,cursorX,cursorY);

			cursorY +=8;

			if(cursorY >= SCREEN_WIDTH-7){
				cursorY = 0;
				cursorX += 8;
			}
		}

		sdl->render_frame();
		//cpu->irq_line = 0;
		usleep(1000);
	}

}

void VIC::init_host_charset(){

	host_charset = new uint8_t[4098 *8];

	uint8_t byte = 0;

	//ROM SIZE
	for(int i=0;i<4096;i++){

		byte = guest_charset[i];		

		for(int m=7;m>=0;m--){
			host_charset[i*8+7-m] = (GET_I_BIT(byte,m)) ? 0xFF : 0;
		}

	}

}


void VIC::setMemory(Memory *mem){
	this->memory = mem;
	this->guest_video_memory = mem->getVideoMemoryPtr();
	this->guest_color_memory = mem->getColorMemoryPtr();
	this->guest_charset = mem->getCharROMPtr();

	init_host_charset();
}

void VIC::setSDL(SDLManager *sdl){
	this->sdl = sdl;
	this->host_video_memory = sdl->getVideoMemoryPtr();

	memset(host_video_memory,0xE0,64000);

}

uint8_t VIC::read_register(uint16_t addr){
    
    //DEBUG_PRINT("read from VIC memory"<<endl);
    //DEBUG_PRINT(hex<<unsigned(registers[addr-IO_START])<<endl);

	if(addr == RASTER_CNT){
		cout<<"PC"<<hex<<unsigned(cpu->regs.PC)<<endl;
		cout<<"reading from RASTER_CNT"<<endl;
		//cout<<hex<<unsigned(data)<<endl;

		return 0;
	}

    return registers[addr-IO_START];

}

void VIC::write_register(uint16_t addr, uint8_t data){

	//not mapped
	//cout<<"PC"<<hex<<unsigned(cpu->regs.PC)<<endl;

    //DEBUG_PRINT("write to VIC memory"<<endl);
    //DEBUG_PRINT(hex<<unsigned(registers[addr-IO_START])<<endl);

    switch(addr){
        case CTRL_REG_1:
            control_reg_one(data);
            return;
		case CTRL_REG_2:
			control_reg_one(data);
			return;
		case BASE_ADDR_REG:
			//cout<<"BASE ADDR"<<endl;
			char_memory_base_addr   = (data & 0xE) << 10;
    		screen_memory_base_addr = (data & 0xF0) << 6;
    		break;
    		//bitmap_memory_base_addr = (data & 0x8) << 10;

		case 0xD019:							//interrupt register
			if(GET_I_BIT(data,0) == 0x0){			//answer to interrupt
				cpu->irq_line = true;
			}
			break;

		case RASTER_CNT:
			cout<<"writing to raster cnt"<<endl;
			cout<<hex<<unsigned(data)<<endl;



    }

    addr -= IO_START;
    registers[addr] = data;


}

void VIC::control_reg_one(uint8_t data){

	//Bitmap Mode


	if(GET_I_BIT(data,3)){
		visible_rows = 24;
	} else {
		visible_rows = 25;
	}

	registers[CTRL_REG_1_OFF] = data;

	set_graphic_mode();

}

void VIC::control_reg_two(uint8_t data){

	if(GET_I_BIT(data,3)){
		visible_cols = 40;
	} else {
		visible_cols = 38;
	}

	registers[CTRL_REG_2_OFF] = data;

	set_graphic_mode();

}

void VIC::set_graphic_mode(){

	bool ecm = GET_I_BIT(registers[CTRL_REG_1_OFF],6);
	bool bmm = GET_I_BIT(registers[CTRL_REG_1_OFF],5); 
	bool mcm = GET_I_BIT(registers[CTRL_REG_2_OFF],4); 

	if(!ecm && !bmm && !mcm){ 
		graphic_mode = CHAR_MODE;
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

void VIC::setCPU(CPU* cpu){

	this->cpu = cpu;

}
