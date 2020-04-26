#include "vic.h"

VIC::VIC(){

	registers = new uint8_t[0x400];    
	host_charset = new uint8_t[4098 *8];

	registers[CTRL_REG_1_OFF] = 0x9B;
	registers[CTRL_REG_2_OFF] = 0x08;

    control_reg_one(0x9B);
    control_reg_two(0x08);

	screen_memory_base_addr = 0x400;
	char_memory_base_addr = 0xd000;
	//bitmap_memory_base_addr = 0x0;

    registers[BASE_ADDR_REG - REG_START] = 0x14;
	registers[IRQ_REQ_REG - REG_START] = 0x0F;
	registers[IRQ_EN_REG - REG_START] = 0x0;

	rasterline = 0;

	interrupt_enabled = false;
	clocks_to_new_render = 1;
	last_time_rendered = chrono::steady_clock::now();

	memset(&color_palette[0],0,0xF*sizeof(uint8_t));

}

VIC::~VIC(){

	delete[] registers;
	delete[] host_charset;

}

void VIC::init_color_palette(){

    SDL_PixelFormat *format = sdl->getPixelFormat();

	color_palette[0] 	= SDL_MapRGB(format, 0x00, 0x00, 0x00);		//black
	color_palette[1] 	= SDL_MapRGB(format, 0xFF, 0xFF, 0xFF);		//white
	color_palette[2] 	= SDL_MapRGB(format, 0xab, 0x31, 0x26);
	color_palette[3] 	= SDL_MapRGB(format, 0x66, 0xda, 0xff);
	color_palette[4] 	= SDL_MapRGB(format, 0xbb, 0x3f, 0xb8);
	color_palette[5]	= SDL_MapRGB(format, 0x55, 0xce, 0x58);
	color_palette[6]	= SDL_MapRGB(format, 0x00, 0x00, 0xAA);
	color_palette[7]	= SDL_MapRGB(format, 0xea, 0xf5, 0x7c);
	color_palette[8]	= SDL_MapRGB(format, 0xb9, 0x74, 0x18);
	color_palette[9]	= SDL_MapRGB(format, 0x78, 0x53, 0x00);
	color_palette[10]	= SDL_MapRGB(format, 0xdd, 0x93, 0x87);
	color_palette[11]	= SDL_MapRGB(format, 0x5b, 0x5b, 0x5b);
	color_palette[12]	= SDL_MapRGB(format, 0x8b, 0x8b, 0x8b);
	color_palette[13]	= SDL_MapRGB(format, 0xb0, 0xf4, 0xac);
	color_palette[14]	= SDL_MapRGB(format, 0x00, 0x88, 0xff);
	color_palette[15]	= SDL_MapRGB(format, 0xb8, 0xb8, 0xb8);

}


void VIC::show_char(uint8_t *character, int X, int Y){

	uint8_t bg_color_idx = registers[0xD021-0xD000];
	uint8_t bg_color = color_palette[bg_color_idx];

	uint8_t fg_color_idx = *(guest_color_memory + 40 * X/8 + Y/8);
	uint8_t fg_color = color_palette[fg_color_idx];

	for(int i = 0; i < CHAR_WIDTH; i++){
		uint8_t *ptr = host_video_memory + SCREEN_WIDTH * (i+X) + Y;

		for(int j=0; j < CHAR_WIDTH; j++){
			ptr[j] = *(character + (i*8) +j) ? fg_color : bg_color;
		}

		//memcpy(ptr, character + (i*8) ,8);
	}

}


void VIC::clock(){

	clocks_to_new_render--;

	if(clocks_to_new_render > 0){
		return;
	}


	clocks_to_new_render = 20000;

	//TODO: fare con altre raster line
	if(interrupt_enabled){
		cpu->setIRQline();
	}

	uint32_t cursorX = 0;
	uint32_t cursorY = 0;

	update_host_charset();

	for(int i=0;i<1000;i++){

		uint8_t value = memory->VIC_read_byte(screen_memory_base_addr+i);
		show_char(host_charset + 64 * value ,cursorX,cursorY);

		cursorY +=8;

		if(cursorY >= SCREEN_WIDTH-7){
			cursorY = 0;
			cursorX += 8;
		}
	}

	sdl->render_frame();

	auto current_time = chrono::steady_clock::now();

	//auto c = chrono::duration_cast<chrono::nanoseconds>(current_time - last_time_rendered);


	auto c = current_time - last_time_rendered;

	//auto temp = chrono::duration_cast<chrono::milliseconds>(c);

	//cout<<"Current - Last "<<dec<<temp.count()<<" ms"<<endl;

	c = chrono::milliseconds(20) - c;

	this_thread::sleep_for(c);


	last_time_rendered = chrono::steady_clock::now();


}

void VIC::update_host_charset(){

	uint8_t byte = 0;
	
	//ROM SIZE
	for(int i=0;i<4096;i++){

		byte = memory->VIC_read_byte(char_memory_base_addr+i);

		for(int m=7;m>=0;m--){
			host_charset[i*8+7-m] = (GET_I_BIT(byte,m)) ? 0xFF : 0;
		}

	}

}


void VIC::setMemory(Memory *mem){
	this->memory = mem;
	this->guest_color_memory = mem->getColorMemoryPtr();

}

void VIC::setSDL(SDLManager *sdl){
	this->sdl = sdl;
	this->host_video_memory = sdl->getVideoMemoryPtr();

	memset(host_video_memory,0xE0,64000);

	init_color_palette();

}

void VIC::setCIA1(CIA1 *cia1){

	this->cia1 = cia1;

}

void VIC::setCIA2(CIA2 *cia2){

	this->cia2 = cia2;

}

//TODO: 
uint8_t VIC::read_register(uint16_t addr){
    
    //DEBUG_PRINT("read from VIC memory"<<endl);
    //DEBUG_PRINT(hex<<unsigned(registers[addr-IO_START])<<endl);

	if(addr == RASTER_LINE){
		//cout<<"PC"<<hex<<unsigned(cpu->regs.PC)<<endl;
		//cout<<"reading from RASTER_CNT"<<endl;

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
			control_reg_two(data);
			return;
		case BASE_ADDR_REG:
			char_memory_base_addr   = (data & 0xE) << 10;
    		screen_memory_base_addr = (data & 0xF0) << 6;
    		break;

    	case IRQ_EN_REG:
    		interrupt_enabled = data;
    		break;

		case 0xD019:								//interrupt register
			if(GET_I_BIT(data,0) == 0x0){			//answer to interrupt
				cpu->resetIRQline();
			}

			break;

		case RASTER_LINE:
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

	if(!ecm && !bmm && !mcm)
		graphic_mode = CHAR_MODE;
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
