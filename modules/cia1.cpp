#include "cia1.h"

CIA1::CIA1(){

	timerA_latch = timerB_latch = 0;
	timerA_enabled = timerB_enabled = false;
	timerA_reload = timerB_reload = false;
	timerA_irq_enabled = timerB_irq_enabled = false;
	timerA_irq_raised = timerB_irq_raised = false;
	
	timerA = timerB = 0;

	key_pressed = false;


	//timer_thread = new thread(&CIA1::timer_loop,this);
	//timer_thread->detach();

}

CIA1::~CIA1(){

	delete timer_thread;

}


void CIA1::setCPU(CPU* cpu){

	this->cpu = cpu;
}

void CIA1::clock(){

	if(timerA_enabled){
		timerA--;
		//cout<<unsigned(timerA)<<endl;
	}

	if(timerB_enabled)
		timerB--;

	if(timerA_irq_enabled and timerA_enabled and timerA == 0){
		timerA_irq_raised = true;
		/* Timer A reset */

		if(timerA_reload)
			timerA = timerA_latch;
		else
			timerA_enabled = false;

	}

	if(timerB_irq_enabled and timerB_enabled and timerB == 0){
		timerB_irq_raised = true;
		/* Timer B reset */

		if(timerB_reload)
			timerB = timerB_latch;
		else
			timerB_enabled = false;
	}

	if(timerA_irq_raised or timerB_irq_raised){
		cpu->setIRQline();
	}

}

uint8_t CIA1::read_register(uint16_t address){

	//cout<<"reading from cia1"<<endl;
	//cout<<hex<<unsigned(address)<<endl;

	//Masking first byte
	address &= 0x00FF;
	//For mirroring
	address = address % 16;

	uint8_t return_value = 0;

	switch(address){

		case IRQ_REG:

			if(timerA_irq_raised || timerB_irq_raised){
				return_value |= 0x80;
				return_value |= timerA_irq_raised << 0;
				return_value |= timerB_irq_raised << 1;
				cpu->resetIRQline();
				timerA_irq_raised = timerB_irq_raised = false;
			}
			return return_value;

		//Keyboard column
		case KEYBOARD_ROW:
					
			if(key_pressed && registers[KEYBOARD_COL] == 0){
				//cout<<"return col 0 "<<hex<<unsigned(last_pressed.row)<<endl;
				return (last_pressed.row);
			}
			else if(key_pressed && registers[KEYBOARD_COL] == last_pressed.col){
				key_pressed = true;
				//cout<<"return col giusta "<<hex<<unsigned(last_pressed.row)<<endl;

				return last_pressed.row;
			} else {
				return 0xFF;
			}
	}

	return registers[address];

}

void CIA1::write_register(uint16_t address, uint8_t data){

	//cout<<"writing to cia1"<<endl;
	//cout<<hex<<unsigned(address)<<" : "<<hex<<unsigned(data)<<endl;

	//Masking first byte
	address &= 0x00FF;
	//For mirroring
	address = address % 16;

	uint16_t data_expanded = data;

	switch(address){
		case IRQ_REG:
			timerA_irq_enabled = GET_I_BIT(data,0);
			timerB_irq_enabled = GET_I_BIT(data,1);
			break;

		case TA_LOW:
			timerA_latch &= 0xFF00;
			timerA_latch |= data_expanded;
			break;

		case TA_HI:
			timerA_latch &= 0x00FF;
			timerA_latch |= (data_expanded<<8);
			break;

		case TA_CTRL:
			timerA_enabled = GET_I_BIT(data,0);
			timerA_reload = !(GET_I_BIT(data,3));
			
			if(GET_I_BIT(data,4))
				timerA = timerA_latch;

			break;

		case TB_LOW:
			timerB_latch &= 0xFF00;
			timerB_latch |= data_expanded;
			break;

		case TB_HI:
			timerB_latch &= 0x00FF;
			timerB_latch |= (data_expanded<<8);
			break;

		case TB_CTRL:
			timerB_enabled = GET_I_BIT(data,0);
			timerB_reload = !(GET_I_BIT(data,3));

			if(GET_I_BIT(data,4))
				timerB = timerB_latch;
			break;
		case KEYBOARD_COL:				//DRA
			//cout<<"Writing to DRA "<<hex<<unsigned(data)<<endl;
			break;

	}

	registers[address] = data;

}


void CIA1::setKeyPressed(KeyboardMatrix matrix){

	cout<<"setting Key Pressed"<<endl;
	key_pressed = true;
	last_pressed = matrix;

}
