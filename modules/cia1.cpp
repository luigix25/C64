#include "cia1.h"

CIA1::CIA1(){

	timerA_latch = timerB_latch = 0;
	timerA_enabled = timerB_enabled = false;
	timerA_reload = timerB_reload = false;
	timerA = timerB = 0;

	timer_thread = new thread(&CIA1::timer_loop,this);
	timer_thread->detach();

}

CIA1::~CIA1(){

	delete timer_thread;

}


void CIA1::setCPU(CPU* cpu){

	this->cpu = cpu;
}

void CIA1::timer_loop(){

	while(true){

		if(timerA_enabled)
			timerA--;

		if(timerB_enabled)
			timerB--;

		if( (timerA_enabled && timerA <= 0) || (timerB_enabled && timerB <= 0)){
			//cpu->setIRQline();
		}

		//10 microseconds
		usleep(10);
	}


}

uint8_t CIA1::read_register(uint16_t address){

	//cout<<"reading from cia1"<<endl;
	//cout<<hex<<unsigned(address)<<endl;

	//Masking first byte
	address &= 0x0F;
	//For mirroring
	address = address % 16;


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
			//cout<<"CIA1 IRQ REG "<<hex<<unsigned(data)<<endl;
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

	}


	cout<<"VIC TIMERS: "<<endl;
	cout<<"TIMERA "<<hex<<unsigned(timerA_latch)<<endl;
	cout<<"TIMERB "<<hex<<unsigned(timerB_latch)<<endl;

	registers[address] = data;

}