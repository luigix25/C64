#include "cpu.h"

#include <signal.h>
#include <stdlib.h>

CPU::CPU(Memory* memory, uint16_t PC){

	this->memory = memory;
	regs.PC = PC;
	regs.SP = 0;
	
	reset_flags();

	//ative low
	nmi_line = true;
	irq_line = true;
	irq_counter = 0;

}

CPU::CPU(Memory* memory){

	this->memory = memory;

	regs.SP = 0;
	regs.PC = RESET_routine;

	reset_flags();

	//ative low
	nmi_line = true;
	irq_line = true;
	irq_counter = 0;

}

void CPU::reset_flags(){

	regs.sign_flag = 0;
	regs.overflow_flag = 0;
	regs.zero_flag = 0;
	regs.carry_flag = 0;
	regs.interrupt_flag = 1;
	regs.decimal_mode_flag = 0;
	regs.break_flag = 1;

}

void CPU::setIRQline(){

	cout<<"SETTING IRQ to false"<<endl;
	irq_counter++;
	irq_line = false;

}

void CPU::resetIRQline(){

	irq_counter--;
	if(irq_counter == 0){
		cout<<"SETTING IRQ to true"<<endl;
		irq_line = true;
	}

}

void CPU::changeIRQ(){

	cout<<"FORCING IRQ TO ";

	irq_line = !irq_line;
	if(irq_line)
		cout<<"true"<<endl;
	else
		cout<<"false"<<endl;

}


void CPU::handle_irq(bool ignore){

	//interrupt is masked
	if(ignore)
		return;

	//cout<<"arrivata IRQ"<<endl;

	uint8_t temp = ((regs.PC >> 8) & 0xFF);
	PUSH(temp);

	temp = (regs.PC & 0xFF);
	PUSH(temp);

	//BCD flag is cleared
	PUSH(flags() & 0xEF);

	regs.interrupt_flag = true;

	uint16_t addr = read_word(IRQ_vector);

	regs.PC = addr;

}

void CPU::handle_nmi(){

	uint8_t temp = ((regs.PC >> 8) & 0xFF);
	PUSH(temp);

	temp = (regs.PC & 0xFF);
	PUSH(temp);

	//BCD flag is cleared
	PUSH(flags() & 0xEF);

	uint16_t addr = read_word(NMI_vector);

	regs.PC = addr;

}

uint16_t CPU::zero_page(register_name index){
  //Immediate operand
  
	uint8_t addr;

	addr = immediate();

 	//Sum X register
	addr += regs.reg[index];

	return addr;

}

uint16_t CPU::zero_page(){
  
	uint8_t addr;
	addr = memory->read_byte(regs.PC);

	regs.PC++;

	return addr;

}

uint8_t CPU::immediate(){

	uint8_t addr;
	addr = read_byte(regs.PC);

	regs.PC++;

	return addr;
}

uint8_t CPU::read_byte(uint16_t addr){

	uint8_t data = memory->read_byte(addr);

	return data;

}

uint16_t CPU::read_word(uint16_t addr){

	uint16_t data = memory->read_byte(addr);
	uint16_t tmp = memory->read_byte(addr+1);

	tmp = tmp <<8;
	data |= tmp;

	return data;

}

uint16_t CPU::absolute(){

	uint16_t addr = read_word(regs.PC);

	regs.PC += 2;

	return addr;

}

uint16_t CPU::absolute(register_name index){

	uint16_t addr = absolute();
	addr+= regs.reg[index];

	return addr;

}

uint16_t CPU::indirect_X(){

	uint16_t addr;

	//zero page addr!!
	uint8_t zero_page_addr = immediate();

	zero_page_addr += regs.reg[regX];
	addr = read_word(zero_page_addr);

	return addr; 


}

uint16_t CPU::indirect_Y(){

	uint16_t addr;

	//zero page addr!!
	uint8_t zero_page_addr = immediate();
	
	//uint16_t tmp;

	addr = read_word(zero_page_addr);
	addr += regs.reg[regY];

	return addr;

}

void CPU::OR(register_name index, uint8_t operand){

	regs.reg[index] |= operand;
	SET_ZF(regs.reg[index]);
	SET_NF(regs.reg[index]);

}

void CPU::AND(uint8_t operand){

	regs.reg[regA] &= operand;
  	SET_ZF(regs.reg[regA]);
  	SET_NF(regs.reg[regA]);

}

void CPU::ROL(register_name index){

	uint16_t t = (regs.reg[index] << 1) | (uint8_t)regs.carry_flag;
	
	regs.carry_flag = ((t&0x100)!=0 );

	regs.reg[index] = t;

	SET_ZF(t);
	SET_NF(t);
}

void CPU::EOR(uint8_t data){

	regs.reg[regA]  = regs.reg[regA] ^ data;

	SET_ZF(regs.reg[regA]);
	SET_NF(regs.reg[regA]);

}


void CPU::BIT(uint16_t addr){

	uint8_t data = memory->read_byte(addr);
	
	regs.overflow_flag = (( data & 0x40 ) != 0);
	regs.zero_flag = ((data & regs.reg[regA]) == 0);

	SET_NF(data);

}

void CPU::LSR(register_name index){

	regs.carry_flag = regs.reg[index] & 0x1;
	regs.reg[index] = regs.reg[index] >> 1;


	SET_ZF(regs.reg[index]);
	SET_NF(regs.reg[index]);

}

void CPU::left_shift_mem(uint16_t addr){

	uint8_t data = memory->read_byte(addr);

	//BUG IN THE 6502!
	memory->write_byte(addr,data);

	regs.carry_flag = data & 0x80;
	data = data << 1;

	memory->write_byte(addr,data);

	SET_ZF(data);
	SET_NF(data);
	

}

void CPU::ASL(register_name index){

	uint8_t data = regs.reg[index];

	regs.carry_flag = data & 0x80;
	data = data << 1;

	regs.reg[index] = data;

	SET_ZF(data);
	SET_NF(data);

}

void CPU::right_shift_mem(uint16_t addr){

	uint8_t data = memory->read_byte(addr);
	//BUG IN THE 6502!
	memory->write_byte(addr,data);

	regs.carry_flag = data & 0x1;
	data = data >> 1;

	memory->write_byte(addr,data);

	SET_ZF(data);
	SET_NF(data);
}

void CPU::rotate_right_mem(uint16_t addr){

	uint8_t data = memory->read_byte(addr);

	//BUG IN THE 6502!
	memory->write_byte(addr,data);

	uint8_t carry = (regs.carry_flag) ? 1:0;

	regs.carry_flag = ((data & 0x1 ) !=0);

	uint16_t t = (data >> 1) | (carry << 7);
	
	SET_ZF(t);
	SET_NF(t);

	memory->write_byte(addr,t);

}

void CPU::rotate_left_mem(uint16_t addr){

	uint8_t data = memory->read_byte(addr);
	//BUG IN THE 6502!
	memory->write_byte(addr,data);

	uint8_t carry = (regs.carry_flag) ? 1:0;

	uint16_t t = (data << 1) | (carry);

	regs.carry_flag = ((t & 0x100 ) !=0);
	
	SET_ZF(t);
	SET_NF(t);
	
	memory->write_byte(addr,t);

}

void CPU::ROR(){

	uint8_t carry = (regs.carry_flag) ? 1:0;
	uint16_t t = (regs.reg[regA] >> 1) | (carry << 7);

	regs.carry_flag = ((regs.reg[regA] & 0x1 ) !=0);
	
	SET_ZF(t);
	SET_NF(t);

	regs.reg[regA] = t;

}


void CPU::LD(register_name index, uint8_t operand){

	regs.reg[index] = operand;
	SET_ZF(regs.reg[index]);
	SET_NF(regs.reg[index]);

}

void CPU::ST(register_name index, uint16_t addr){

	memory->write_byte(addr,regs.reg[index]);

}

void CPU::JMP(uint16_t addr){
	regs.PC = memory->read_byte(addr);
}

void CPU::PUSH(register_name index){

	uint16_t addr = STACK_START + regs.SP;
	memory->write_byte(addr,regs.reg[index]);
	regs.SP--;
}

void CPU::PUSH(uint8_t value){

	uint16_t addr = STACK_START + regs.SP;
	memory->write_byte(addr, value);
	regs.SP--;
}

uint8_t CPU::POP(){

  	uint16_t addr = ++regs.SP + STACK_START;
  	return memory->read_byte(addr);

}

void CPU::PLA(){
	regs.reg[regA] = POP();
	SET_ZF(regs.reg[regA]);
	SET_NF(regs.reg[regA]);
}

void CPU::JSR(uint16_t addr){

	PUSH((regs.PC -1 ) >>8);
	PUSH((regs.PC -1 ));
	regs.PC = addr;

}


uint8_t CPU::fetch(){

	// active low
	if(nmi_line == false){
		handle_nmi();
	} else if(irq_line == false){
		handle_irq(regs.interrupt_flag);
	}

	uint8_t opcode = read_byte(regs.PC);
	//DEBUG_PRINT(hex<<unsigned(opcode)<<endl);

	regs.PC++;
	return opcode;

}

uint8_t CPU::flags()
{
	uint8_t v = 0;

	v |= (regs.carry_flag != 0)  << 0;
	v |= (regs.zero_flag != 0) << 1;
	v |= (regs.interrupt_flag != 0) << 2;
	v |= (regs.decimal_mode_flag != 0)<< 3;
	
	//v |= (regs.break_flag != 0) << 4;
	//Always 1
	v |= 1 << 4;

	/* unused, always set */
	v |= 1     << 5;
	v |= (regs.overflow_flag != 0)  << 6;
	v |= (regs.sign_flag != 0) << 7;

	return v;
}


void CPU::flags(uint8_t v)
{
	regs.carry_flag = (GET_I_BIT(v,0));
	regs.zero_flag = (GET_I_BIT(v,1));
	regs.interrupt_flag = (GET_I_BIT(v,2));
	regs.decimal_mode_flag = (GET_I_BIT(v,3));
	
	//regs.break_flag = (GET_I_BIT(v,4));
	
	regs.overflow_flag = (GET_I_BIT(v,6));
	regs.sign_flag = (GET_I_BIT(v,7));
}

void CPU::CMP_addr(uint16_t addr){

	uint8_t data = memory->read_byte(addr);

	uint16_t t;
	t = regs.reg[regA] - data;
	regs.carry_flag = (t<0x100);
	
	t = t & 0xff;
	
	SET_ZF(t);
	SET_NF(t);

}

void CPU::CMP_data(uint8_t data){

	uint16_t t;
	t = regs.reg[regA] - data;
	regs.carry_flag = (t<0x100);
	
	t = t & 0xff;
	
	SET_ZF(t);
	SET_NF(t);

}

void CPU::CPX(uint8_t data){

	uint16_t t;
	t = regs.reg[regX] - data;
	regs.carry_flag = (t<0x100);
	
	t = t & 0xff;
	
	SET_ZF(t);
	SET_NF(t);

}

void CPU::BNE(uint8_t addr){

	uint16_t new_addr = (int8_t) addr + regs.PC;
	//cout<<"addr: "<<hex<<unsigned(new_addr)<<endl;

	if(regs.zero_flag == 0){
		DEBUG_PRINT("BNE to "<<hex<<unsigned(new_addr)<<endl);
		regs.PC = new_addr;
	}

}

void CPU::BCC(uint8_t addr){

	uint16_t new_addr = (int8_t) addr + regs.PC;
	//cout<<"addr: "<<hex<<unsigned(new_addr)<<endl;

	if(regs.carry_flag == 0){
		DEBUG_PRINT("BCC to "<<hex<<unsigned(new_addr)<<endl);
		regs.PC = new_addr;
	}

}

void CPU::BVS(uint8_t addr){

	uint16_t new_addr = (int8_t) addr + regs.PC;
	//cout<<"addr: "<<hex<<unsigned(new_addr)<<endl;

	if(regs.overflow_flag){
		DEBUG_PRINT("BVS to "<<hex<<unsigned(new_addr)<<endl);
		regs.PC = new_addr;
	}

}

void CPU::BVC(uint8_t addr){

	uint16_t new_addr = (int8_t) addr + regs.PC;
	//cout<<"addr: "<<hex<<unsigned(new_addr)<<endl;

	if(regs.overflow_flag == 0){
		DEBUG_PRINT("BVC to "<<hex<<unsigned(new_addr)<<endl);
		regs.PC = new_addr;
	}

}

void CPU::BMI(uint8_t addr){

	uint16_t new_addr = (int8_t) addr + regs.PC;
	//cout<<"addr: "<<hex<<unsigned(new_addr)<<endl;

	if(regs.sign_flag){
		DEBUG_PRINT("BMI to "<<hex<<unsigned(new_addr)<<endl);
		regs.PC = new_addr;
	}

}

void CPU::BCS(uint8_t addr){

	uint16_t new_addr = (int8_t) addr + regs.PC;
	//cout<<"addr: "<<hex<<unsigned(new_addr)<<endl;

	if(regs.carry_flag){
		DEBUG_PRINT("BCS to "<<hex<<unsigned(new_addr)<<endl);
		regs.PC = new_addr;
	}

}

void CPU::BPL(uint8_t addr){

	uint16_t new_addr = (int8_t) addr + regs.PC;
	
	if(regs.sign_flag == 0){
		DEBUG_PRINT("BPL to "<<hex<<unsigned(new_addr)<<endl);
		regs.PC = new_addr;
	}

}

void CPU::BEQ(uint8_t addr){

	uint16_t new_addr = (int8_t) addr + regs.PC;
	//cout<<"BEQ: "<<hex<<unsigned(new_addr)<<endl;

	if(regs.zero_flag) 
		regs.PC = new_addr;

}

void CPU::INC(register_name index)
{
	regs.reg[index] +=1;
	SET_ZF(regs.reg[index]);
	SET_NF(regs.reg[index]);
}

void CPU::TSX(){

	regs.reg[regX] = regs.SP;

	SET_NF(regs.reg[regX]);
	SET_ZF(regs.reg[regX]);

}

void CPU::INC(uint16_t addr)
{

	uint8_t value = memory->read_byte(addr);
	value++;
	memory->write_byte(addr,value);
	SET_ZF(value);
	SET_NF(value);

}

void CPU::ADC(uint8_t value){

	uint16_t t;
	if(regs.decimal_mode_flag)
	{
		t = (regs.reg[regA] & 0xf) + (value & 0xf) + (regs.carry_flag ? 1 : 0);
		
		if (t > 0x09) 
			t += 0x6;
		
		t += (regs.reg[regA] & 0xf0) + (value & 0xf0);
		
		if((t & 0x1f0) > 0x90) 
			t += 0x60;

	} else {
		t = regs.reg[regA] + value + (regs.carry_flag ? 1 : 0);
	}

	regs.carry_flag = t > 0xff;
	t = t & 0xff;


	regs.overflow_flag = !(( regs.reg[regA] ^ value ) &0x80 ) && (( regs.reg[regA] ^ t) & 0x80);

	SET_ZF(t);
	SET_NF(t);

	regs.reg[regA] = (uint8_t)t;

}

void CPU::SBC(uint8_t value){
	
	uint16_t t;
	if(regs.decimal_mode_flag){
		t = (regs.reg[regA] & 0xf) - (value & 0xf) - (regs.carry_flag ? 0 : 1);
		
		if((t & 0x10) != 0)
			t = ((t-0x6)&0xf) | ((regs.reg[regA] & 0xf0) - (value & 0xf0) - 0x10);
		else
			t = (t&0xf) | ((regs.reg[regA] & 0xf0) - (value & 0xf0));
		
		if((t & 0x100) !=0)
			t -= 0x60;

	} else {
		t = regs.reg[regA] - value - (regs.carry_flag ? 0 : 1);
	}

	regs.carry_flag = t < 0x100;

	t = t & 0xFF;

	regs.overflow_flag = (( regs.reg[regA] ^ t ) & 0x80 ) && (( regs.reg[regA] ^ value) & 0x80);

	SET_ZF(t);
	SET_NF(t);

	regs.reg[regA] = (uint8_t)t;

}

/*
  uint16_t t;
  if(dmf())
  {
    t = ( a() & 0xf ) - ( v &0xf ) - ( cf() ? 0 : 1);
    if((t & 0x10) != 0)
      t = ((t-0x6)&0xf) | ((a()&0xf0) - (v&0xf0) - 0x10);
    else
      t = (t&0xf) | ((a()&0xf0) - (v&0xf0));
    if((t&0x100)!=0)
      t -= 0x60;
  }
  else
  {
    t = a() - v - (cf() ? 0 : 1);
  }


  cf ( t<0x100 );
  t = t & 0xff;


  of = (( a() ^ t ) & 0x80) && ( (a() ^ v ) & 0x80) 


  a((uint8_t)t);*/



//Compare
void CPU::CP(register_name index, uint8_t v){
	uint16_t t;
	t = regs.reg[index] - v;
	
	/*if(v > regs.reg[index]){
		cout<<"NEGATIVO"<<endl;
	}*/

	regs.carry_flag = (t < 0x100);
	//regs.carry_flag = ((t & 0x8000) != 0);

	t = t & 0xff;

	SET_ZF(t);
	SET_NF(t);
}

void CPU::DE(register_name index)
{
	regs.reg[index] -=1;
	SET_ZF(regs.reg[index]);
	SET_NF(regs.reg[index]);
}

void CPU::DEC(uint16_t addr)
{
	uint8_t data = memory->read_byte(addr);
	
	//BUG IN 6502!
	memory->write_byte(addr,data);
	data--;
	memory->write_byte(addr,data);

	SET_ZF(data);
	SET_NF(data);
}


void CPU::TAX(){

	regs.reg[regX] = regs.reg[regA];
	SET_ZF(regs.reg[regX]);
	SET_NF(regs.reg[regX]);
}

void CPU::TAY(){

	regs.reg[regY] = regs.reg[regA];
	SET_ZF(regs.reg[regY]);
	SET_NF(regs.reg[regY]);
}

void CPU::TXA(){

	regs.reg[regA] = regs.reg[regX];
	SET_ZF(regs.reg[regA]);
	SET_NF(regs.reg[regA]);
}

void CPU::BRK(){

	regs.break_flag = true;
	//interrupt is masked

	cout<<"BRK RETURN ADDR: "<<hex<<unsigned(regs.PC)<<endl;

	uint8_t temp = (((regs.PC+1) >> 8) & 0xFF);
	PUSH(temp);

	temp = ((regs.PC+1) & 0xFF);
	PUSH(temp);

	PUSH(flags());

	regs.interrupt_flag = true;

	uint16_t addr = read_word(IRQ_vector);

	regs.PC = addr;

}

void CPU::RTI(){

	flags(POP());
	regs.PC = (POP() + (POP() << 8));

}

//DEBUG
void CPU::dump_reg(){

	DEBUG_PRINT(endl<<endl);
	DEBUG_PRINT("---------REG STATUS-----------"<<endl);
	DEBUG_PRINT("RegA: 0x"<<hex<<unsigned(regs.reg[regA])<<endl);
	DEBUG_PRINT("RegX: 0x"<<hex<<unsigned(regs.reg[regX])<<endl);
	DEBUG_PRINT("RegY: 0x"<<hex<<unsigned(regs.reg[regY])<<endl);
	DEBUG_PRINT("PC  : 0x"<<hex<<unsigned(regs.PC)<<endl);
	DEBUG_PRINT("SP  : 0x"<<hex<<unsigned(regs.SP)<<endl);

	DEBUG_PRINT("CF :"<<hex<<unsigned(regs.carry_flag)<<endl);
	DEBUG_PRINT("NF :"<<hex<<unsigned(regs.sign_flag)<<endl);
	DEBUG_PRINT("OF :"<<hex<<unsigned(regs.overflow_flag)<<endl);
	DEBUG_PRINT("ZF :"<<hex<<unsigned(regs.zero_flag)<<endl);
	DEBUG_PRINT("IF :"<<hex<<unsigned(regs.interrupt_flag)<<endl);
	DEBUG_PRINT("BF :"<<hex<<unsigned(regs.break_flag)<<endl);
	DEBUG_PRINT("DMF:"<<hex<<unsigned(regs.decimal_mode_flag)<<endl);


	DEBUG_PRINT("FLAGS: "<<hex<<unsigned(flags())<<endl);


	DEBUG_PRINT("------------------------------"<<endl);

	DEBUG_PRINT(endl<<endl);

}

bool CPU::decode(uint8_t opcode){

	//OR LD ST PH fatte

	uint16_t addr,tmp;

	switch(opcode){
		case 0x00:        //BRK
			DEBUG_PRINT("BRK!"<<endl);
			BRK();
			break;

		case 0x01:        //ORA (ind,X)		

			addr = indirect_X();			

			OR(regA,memory->read_byte(addr));
			break;

		case 0x05:      //ORA zpg
			addr = zero_page();
			OR(regA, memory->read_byte(addr));
			break;

		case 0x06:			//ASL
			addr = zero_page();	
			left_shift_mem(addr);
			break;
	    
		case 0x08:			//PHP
			DEBUG_PRINT("PHP"<<endl);
			PUSH(flags());
			break;

		case 0x09:			//ORA imm
			addr = immediate();
			DEBUG_PRINT("ORA "<<hex<<unsigned(addr)<<endl);
			OR(regA,addr);
			break;
		
		case 0xA:			//ASL
			DEBUG_PRINT("ASL"<<endl);
			ASL(regA);
			break;

		case 0x0D:
			DEBUG_PRINT("ORA ABS"<<endl);
			addr = absolute();		//ORA ABS
			OR(regA,memory->read_byte(addr));
			break;
			
		case 0x0E:						//ASL ABS
			DEBUG_PRINT("ASL"<<endl);
			addr = absolute();
			left_shift_mem(addr);
			break;

		case 0x10:						//BPL
			DEBUG_PRINT("BPL"<<endl);
			addr = immediate();
			BPL(addr);
			break;
		
		case 0x11:						//ORA (ind),Y
			DEBUG_PRINT("ORA ind Y"<<endl);
			addr = indirect_Y();
			OR(regA,memory->read_byte(addr));
			break;

		case 0x15:						//ORA zpg,X
			DEBUG_PRINT("ORA ZPG"<<endl);
			addr = zero_page(regX);
			OR(regA,memory->read_byte(addr));
			break;
		
		case 0x16:						//ASL zpg,X
			DEBUG_PRINT("ASL"<<endl);
			addr = zero_page(regX);
			left_shift_mem(addr);
			break;

		case 0x18:						//CLC
			DEBUG_PRINT("CLC"<<endl);
			regs.carry_flag = false;
			break;

		case 0x19:						//ORA ABS Y
			DEBUG_PRINT("ORA"<<endl);
			addr = absolute(regY);
			OR(regA,memory->read_byte(addr));
			break;

		case 0x1D:						//ORA abs,X
			addr = absolute(regX);
			OR(regA,memory->read_byte(addr));
			break;
		
		case 0x1E:
			DEBUG_PRINT("ASL"<<endl);
			addr = absolute(regX);
			left_shift_mem(addr);
			break;

		case 0x20:						//JSR 
			addr = absolute();
			DEBUG_PRINT("JSR "<<hex<<unsigned(addr)<<endl);
			JSR(addr);
			break;
		
		case 0x21:						//AND (ind X)
			DEBUG_PRINT("AND"<<endl);
			addr = indirect_X();
			AND(memory->read_byte(addr));
			break;

		case 0x24:						//JSR 
			addr = zero_page();
			DEBUG_PRINT("BIT "<<hex<<unsigned(addr)<<endl);
			BIT(addr);
			break;
		
		case 0x25:						//AND zpg
			DEBUG_PRINT("AND"<<endl);
			addr = zero_page();
			AND(memory->read_byte(addr));
			break;
		
		case 0x26:						//ROL zpg
			addr = zero_page();
			DEBUG_PRINT("ROL "<<endl);
			rotate_left_mem(addr);
			break;

		case 0x28:						//PLP 
			DEBUG_PRINT("PLP"<<endl);
			flags(POP());
			break;

		case 0x29:						//AND imm 
			addr = immediate();
			DEBUG_PRINT("AND "<<hex<<unsigned(addr)<<endl);
			AND(addr);
			break;

		case 0x2A:						//ROL A 
			DEBUG_PRINT("ROL A"<<endl);
			ROL(regA);
			break;

		case 0x2C:						//BIT ABS
			DEBUG_PRINT("BIT"<<endl);
			addr = absolute();
			BIT(addr);
			break;

		case 0x2D:						//AND abs
			DEBUG_PRINT("AND"<<endl);
			addr = absolute();
			AND(memory->read_byte(addr));
			break;	

		case 0x2E:						//ROL ABS
			DEBUG_PRINT("ROL ABS"<<endl);
			addr = absolute();
			rotate_left_mem(addr);
			break;

		case 0x30:						//BMI 
			addr = immediate();
			DEBUG_PRINT("BMI "<<hex<<unsigned(addr)<<endl);
			BMI(addr);
			break;

		case 0x31:						//AND (ind),Y
			DEBUG_PRINT("AND"<<endl);
			addr = indirect_Y();
			AND(memory->read_byte(addr));
			break;

		case 0x35:						//AND zpg,X
			DEBUG_PRINT("AND"<<endl);
			addr = zero_page(regX);
			AND(memory->read_byte(addr));
			break;

		case 0x36:						//ROL 
			DEBUG_PRINT("ROL"<<endl);
			addr = zero_page(regX);
			rotate_left_mem(addr);
			break;

		case 0x38:
			DEBUG_PRINT("SEC"<<endl);
			regs.carry_flag = true;
			break;

		case 0x39:						//AND abs Y
			DEBUG_PRINT("AND"<<endl);
			addr = absolute(regY);
			AND(memory->read_byte(addr));
			break;

		case 0x3D:						//AND abs X
			DEBUG_PRINT("AND"<<endl);
			addr = absolute(regX);
			AND(memory->read_byte(addr));
			break;

		case 0x3E:						//ROL
			DEBUG_PRINT("ROL"<<endl);
			addr = absolute(regX);
			rotate_left_mem(addr);
			break;

		case 0x41:						//EOR (ind,X)
			DEBUG_PRINT("EOR"<<endl;)
			addr = indirect_X();
			EOR(memory->read_byte(addr));
			break;

		case 0x45:						//EOR zpg
			DEBUG_PRINT("EOR"<<endl;)
			addr = zero_page();
			EOR(memory->read_byte(addr));
			break;
		
		case 0x40:
			DEBUG_PRINT("RTI"<<endl);
			RTI();
			break;

		case 0x46:						//LSR zpg
			DEBUG_PRINT("LSR"<<endl;)
			addr = zero_page();
			right_shift_mem(addr);
			break;

		case 0x48:						//PHA
			DEBUG_PRINT("PHA"<<endl);
			PUSH(regA);
			break;
		
		case 0x49:						//EOR
			DEBUG_PRINT("EOR"<<endl;)
			addr = immediate();
			EOR(addr);
			break;

		case 0x4A:						//LSR
			DEBUG_PRINT("LSR"<<endl);
			LSR(regA);
			break;

		case 0x4C:						//JMP abs
			addr = absolute();
			DEBUG_PRINT("JMP to "<<hex<<unsigned(addr)<<endl);

			regs.PC = addr;
			break;
		
		case 0x4D:						//EOR abs
			DEBUG_PRINT("EOR"<<endl;)
			addr = absolute();
			EOR(memory->read_byte(addr));
			break;
		
		case 0x4E:
			DEBUG_PRINT("LSR ABS"<<endl);
			addr = absolute();
			right_shift_mem(addr);
			break;

		case 0x50:						//JMP abs
			addr = immediate();
			DEBUG_PRINT("BVC"<<endl);
			BVC(addr);
			break;

		case 0x51:						//EOR (ind),Y
			DEBUG_PRINT("EOR"<<endl;)
			addr = indirect_Y();
			EOR(memory->read_byte(addr));
			break;

		case 0x55:						//EOR zpg X
			DEBUG_PRINT("EOR"<<endl;)
			addr = zero_page(regX);
			EOR(memory->read_byte(addr));
			break;

		case 0x56:						//LSR zpg_X
			DEBUG_PRINT("LSR zpg,X"<<endl);
			addr = zero_page(regX);
			right_shift_mem(addr);
			break;

		case 0x58:						//CLI
			DEBUG_PRINT("CLI"<<endl);
			regs.interrupt_flag = false;
			break;

		case 0x59:						//EOR abs Y
			DEBUG_PRINT("EOR"<<endl;)
			addr = absolute(regY);
			EOR(memory->read_byte(addr));
			break;

		case 0x5D:						//EOR abs X
			DEBUG_PRINT("EOR"<<endl;)
			addr = absolute(regX);
			EOR(memory->read_byte(addr));
			break;

		case 0x5E:
			DEBUG_PRINT("LSR"<<endl);
			addr = absolute(regX);
			right_shift_mem(addr);
			break;

		case 0x60:						//RTS
			DEBUG_PRINT("RTS"<<endl);
			addr = (POP() + (POP() << 8)) + 1;
  			regs.PC = addr;
			break;

		case 0x61:								//ADC (ind X)
			DEBUG_PRINT("ADC"<<endl);
			addr = indirect_X();
			ADC(memory->read_byte(addr));
			break;

		case 0x65:						//ADC imm
			addr = zero_page();
			DEBUG_PRINT("ADC "<<hex<<unsigned(addr)<<endl);
			ADC(memory->read_byte(addr));
			break;
		
		case 0x66:						//ROR zpg
			addr = zero_page();
			DEBUG_PRINT("ROR "<<endl);
			rotate_right_mem(addr);
			break;

		case 0x68:						//PLA
			DEBUG_PRINT("PLA"<<endl);
			PLA();
			break;
		
		case 0x69:						//ADC imm
			DEBUG_PRINT("ADC"<<endl);
			addr = immediate();
			ADC(addr);
			break;
		
		case 0x6A:						//ROR 
			DEBUG_PRINT("ROR"<<endl);
			ROR();
			break;
		
		case 0x6C:						//JMP (ind)

			tmp = absolute();
			DEBUG_PRINT("JMP "<<hex<<unsigned(tmp)<<endl);

			addr = read_word(tmp);
			DEBUG_PRINT("JUMPING TO: "<<hex<<unsigned(addr)<<endl);
			
			regs.PC = addr;

			//JMP(tmp);
			break;
		
		case 0x6D:
			DEBUG_PRINT("ADC"<<endl);	//ADC ABS
			addr = absolute();

			ADC(memory->read_byte(addr));
			break;

		case 0x6E:

			DEBUG_PRINT("ROR"<<endl);

			addr = absolute();
			rotate_right_mem(addr);
			break;

		case 0x70:
			DEBUG_PRINT("BVS"<<endl);
			addr = immediate();
			BVS(addr);
			break;

		case 0x71:								//ADC (ind) Y
			DEBUG_PRINT("ADC"<<endl);
			addr = indirect_Y();
			ADC(memory->read_byte(addr));
			break;

		case 0x75:								//ADC zpg X
			DEBUG_PRINT("ADC"<<endl);
			addr = zero_page(regX);
			ADC(memory->read_byte(addr));
			break;

		case 0x76:									//ROR
			DEBUG_PRINT("ROR ZPG,X"<<endl);
			addr = zero_page(regX);
			rotate_right_mem(addr);

			break;

		case 0x78:						//SEI
			DEBUG_PRINT("SEI"<<endl);
			regs.interrupt_flag = true;
			break;

		case 0x79:								//ADC abs Y
			DEBUG_PRINT("ADC"<<endl);
			addr = absolute(regY);
			ADC(memory->read_byte(addr));
			break;

		case 0x7D:								//ADC abs X
			DEBUG_PRINT("ADC"<<endl);
			addr = absolute(regX);
			ADC(memory->read_byte(addr));
			break;

		case 0x7E:						//ROR
			DEBUG_PRINT("ROR"<<endl);
			addr = absolute(regX);
			rotate_right_mem(addr);
			break;

	    case 0x81:						//STA (ind,X)
	    	DEBUG_PRINT("STA indirect X"<<endl);

	    	addr = indirect_X();
	    	ST(regA,addr);

	    	break;

	    case 0x84:						//STY zpg
	    	addr = zero_page();
			DEBUG_PRINT("STY "<<hex<<hex<<unsigned(addr)<<endl);

	    	ST(regY,addr);

	    	break;

	    case 0x85:						//STA zpg

	    	addr = zero_page();
	    	DEBUG_PRINT("STA "<<hex<<unsigned(addr)<<endl);

	    	ST(regA,addr);

	    	break;

	    case 0x86:						//STX zpg
	    	addr = zero_page();
			DEBUG_PRINT("STX "<<hex<<unsigned(addr)<<endl);
	    	ST(regX,addr);

	    	break;

		case 0x88:						//DEY
			DEBUG_PRINT("DEY"<<endl);
			DE(regY);
			break;

		case 0x8A:						//TXA
			DEBUG_PRINT("TXA"<<endl);
			TXA();
			break;

	    case 0x8C:						//STY abs
	    	DEBUG_PRINT("STY abs"<<endl);

	    	addr = absolute();
	    	ST(regY,addr);

	    	break;

	    case 0x8D:						//STA abs

	    	addr = absolute();
	    	DEBUG_PRINT("STA "<<hex<<unsigned(addr)<<endl);

	    	ST(regA,addr);
	    	break;

	    case 0x8E:						//STX abs
	    	addr = absolute();
			DEBUG_PRINT("STX "<<hex<<unsigned(addr)<<endl);

	    	ST(regX,addr);
	    	break;

		case 0x90:						//BCC
	    	DEBUG_PRINT("BCC"<<endl);

	    	addr = immediate();
	    	BCC(addr);
	    	break;

		case 0x91:						//STA (ind),Y
			addr = indirect_Y();
			DEBUG_PRINT("STA "<<hex<<unsigned(addr)<<endl);
			ST(regA,addr);
			break;

		case 0x94:						//STY zpg,X
	    	DEBUG_PRINT("STY zero page x"<<endl);

	    	addr = zero_page(regX);
	    	ST(regY,addr);
	    	break;

		case 0x95:						//STA zpg,X
	    	DEBUG_PRINT("STA zero page x"<<endl);

	    	addr = zero_page(regX);
	    	ST(regA,addr);
	    	break;

		case 0x96:						//STA zpg,Y
	    	DEBUG_PRINT("STX zero page x"<<endl);

	    	addr = zero_page(regY);
	    	ST(regX,addr);
	    	break;
		
		case 0x98:						//TYA
	    	DEBUG_PRINT("TYA"<<endl);
	    	regs.reg[regA] = regs.reg[regY];
			SET_ZF(regs.reg[regA]);
			SET_NF(regs.reg[regA]);
	    	break;

		case 0x99:						//STA abs,Y
			addr = absolute(regY);
			DEBUG_PRINT("STA 0x"<<hex<<unsigned(addr)<<endl);
			ST(regA,addr);
			break;
		
		case 0x9A:						//TXS
	    	DEBUG_PRINT("TXS"<<endl);
	    	regs.SP = regs.reg[regX];
	    	break;

		case 0x9D:						//STA abs,X
	    	DEBUG_PRINT("STA abs X"<<endl);

	    	addr = absolute(regX);
	    	ST(regA,addr);
	    	break;

		case 0xA0:						//LDY imm
			DEBUG_PRINT("LDY IMM"<<endl);
			addr = immediate();
			LD(regY,addr);
			break;

		case 0xA1:						//LDA (ind,X)
			DEBUG_PRINT("LDY IMM"<<endl);
			addr = indirect_X();
			LD(regA,memory->read_byte(addr));
			break;

		case 0xA2:						//LDX imm
			addr = immediate();
			DEBUG_PRINT("LDX "<<hex<<unsigned(addr)<<endl);
			LD(regX,addr);
			break;

		case 0xA4:						//LDY zpg
			addr = zero_page();
			DEBUG_PRINT("LDY "<<hex<<unsigned(addr)<<endl);
			LD(regY,memory->read_byte(addr));
			break;

		case 0xA5:						//LDA zpg
			DEBUG_PRINT("LDA zero"<<endl);
			addr = zero_page();
			LD(regA,memory->read_byte(addr));
			break;

		case 0xA6:						//LDX zpg
			DEBUG_PRINT("LDX zero"<<endl);
			addr = zero_page();
			LD(regX,memory->read_byte(addr));
			break;
		
		case 0xA8:						//TAY
			DEBUG_PRINT("TAY"<<endl);
			TAY();
			break;

		case 0xA9:						//LDA imm
			addr = immediate();
			DEBUG_PRINT("LDA "<<hex<<unsigned(addr)<<endl);
			LD(regA,addr);
			break;
		
		case 0xAA:						//TAX
			DEBUG_PRINT("TAX"<<endl);
			TAX();
			break;

		case 0xAC:						//LDY abs
			addr = absolute();
			DEBUG_PRINT("LOAD "<<hex<<unsigned(addr)<<endl);
			LD(regY,memory->read_byte(addr));
			break;

		case 0xAD:						//LDA abs
			DEBUG_PRINT("LOAD "<<hex<<unsigned(addr)<<endl);
			addr = absolute();
			LD(regA,memory->read_byte(addr));
			break;

		case 0xAE:						//LDX abs
			addr = absolute();
			LD(regX,memory->read_byte(addr));
			DEBUG_PRINT("LOAD "<<hex<<unsigned(addr)<<endl);
			break;
		
		case 0xB0:						//BCS
			DEBUG_PRINT("BCS"<<endl);
			addr = immediate();
			BCS(addr);
			break;

		case 0xB1:						//LDA ind y
			addr = indirect_Y();
			DEBUG_PRINT("LDA "<<hex<<unsigned(addr)<<endl);

			LD(regA,memory->read_byte(addr));
			break;

		case 0xB4:						//LDY zpg,X
			DEBUG_PRINT("LDY zpg X"<<endl);
			addr = zero_page(regX);
			LD(regY,memory->read_byte(addr));
			break;

		case 0xB5:						//LDA zpg,X
			DEBUG_PRINT("LDA ind y"<<endl);
			addr = zero_page(regX);
			LD(regA,memory->read_byte(addr));
			break;
				
		case 0xB6:						//LDX zpg,Y
			DEBUG_PRINT("LOAD ind y"<<endl);
			addr = zero_page(regY);
			LD(regX,memory->read_byte(addr));
			break;

		case 0xB8:
			DEBUG_PRINT("CLV"<<endl);
			regs.overflow_flag = false;
			break;

		case 0xB9:						//LDA abs,Y
			addr = absolute(regY);
			DEBUG_PRINT("LOAD "<<hex<<unsigned(addr)<<endl);

			LD(regA,memory->read_byte(addr));
			break;

		case 0xBA:						//TSX
			DEBUG_PRINT("TSX "<<endl);
			TSX();
			break;

		case 0xBC:						//LDY abs,X
			DEBUG_PRINT("LOAD abs X"<<endl);
			addr = absolute(regX);
			LD(regY,memory->read_byte(addr));
			break;

		case 0xBD:						//LDA abs,X
			DEBUG_PRINT("LOAD abs X"<<endl);
			addr = absolute(regX);
			LD(regA,memory->read_byte(addr));
			break;

		case 0xBE:						//LDX abs,X
			DEBUG_PRINT("LOAD abs Y"<<endl);
			addr = absolute(regY);
			LD(regX,memory->read_byte(addr));
			break;
		
		case 0xC0:						//CPY imm
			DEBUG_PRINT("CPY"<<endl);
			addr = immediate();
			CP(regY,addr);
			break;

		case 0xC1:						//CPY ind X
			DEBUG_PRINT("CMP"<<endl);
			addr = indirect_X();
			CP(regA,memory->read_byte(addr));
			break;	

		case 0xC4:						//CPY zpg
			DEBUG_PRINT("CPY"<<endl);
			addr = zero_page();
			CP(regY,memory->read_byte(addr));
			break;		
		
		case 0xC5:						//CPY zpg
			DEBUG_PRINT("CMP"<<endl);
			addr = zero_page();
			CMP_addr(addr);
			break;		
		
		case 0xC6:						//DEC zpg
			DEBUG_PRINT("DEC"<<endl);
			addr = zero_page();
			DEC(addr);
			break;	
		
		case 0xC8:						//INY
			DEBUG_PRINT("INY"<<endl);
			INC(regY);
			break;
		
		//TODO: CHECK!!!!
		/*case 0xC9:
			DEBUG_PRINT("CMP"<<endl);		//CMP ZERO_PAGE
			addr = zero_page();
			CMP_data(addr);
			break;*/
		
		case 0xC9:
			addr = immediate();
			DEBUG_PRINT("CMP imm "<<hex<<unsigned(addr)<<endl);
			CMP_data(addr);
			break;

		case 0xCA:						//DEX
			DEBUG_PRINT("DEX"<<endl);
			DE(regX);
			break;

		case 0xCC:
			DEBUG_PRINT("CPY"<<endl);
			addr = absolute();
			CP(regY,memory->read_byte(addr));
			break;

		case 0xCE:						//DEC ABS
			DEBUG_PRINT("CMP"<<endl);
			addr = absolute();
			DEC(addr);
			break;

		case 0xCD:						//CMP ABS
			DEBUG_PRINT("CMP"<<endl);
			addr = absolute();
			CMP_addr(addr);
			break;
		
		case 0xD0:						//BNE
			DEBUG_PRINT("BNE"<<endl);
			addr = immediate();
			BNE(addr);
			break;	
		
		case 0xD1:						//CMP
			DEBUG_PRINT("CMP"<<endl);
			addr = indirect_Y();
			CMP_addr(addr);
			break;	

		case 0xD5:						//CMP zeropage_x
			DEBUG_PRINT("CMP"<<endl);
			addr = zero_page(regX);
			CMP_addr(addr);
			break;	

		case 0xD6:						//DEC ZPG X
			DEBUG_PRINT("DEC"<<endl);
			addr = zero_page(regX);
			DEC(addr);
			break;

		case 0xD8:						//CLD
			DEBUG_PRINT("CLD"<<endl);
			regs.decimal_mode_flag = false;
			break;
		
		case 0xD9:						//CMP ABS,Y
			DEBUG_PRINT("CMP"<<endl);
			addr = absolute(regY);
			CMP_addr(addr);
			break;

		case 0xDD:						//CMP abs,X
			DEBUG_PRINT("CMP"<<endl);
			addr = absolute(regX);
			CMP_addr(addr);
			break;

		case 0xDE:						//DEC ABS,X
			DEBUG_PRINT("DEC"<<endl);
			addr = absolute(regX);
			DEC(addr);
			break;
		
		case 0xE0:						//CPX imm
			DEBUG_PRINT("CPX"<<endl);
			addr = immediate();
			CP(regX,addr);
			break;	

		case 0xE1:								//SBC (ind X)
			DEBUG_PRINT("SBC"<<endl);
			addr = indirect_X();
			SBC(memory->read_byte(addr));
			break;

		case 0xE4:
			DEBUG_PRINT("CPX"<<endl);
			addr = zero_page();
			CPX(memory->read_byte(addr));
			break;

		case 0xE6:						//INC
			addr = zero_page();
			DEBUG_PRINT("INC zpg "<<hex<<unsigned(addr)<<endl);

			INC(addr);
			break;

		case 0xE5:						//SBC
			DEBUG_PRINT("SBC"<<endl);
			addr = zero_page();
			SBC(memory->read_byte(addr));
			break;

		case 0xE8:						//INX
			DEBUG_PRINT("INX"<<endl);
			INC(regX);
			break;
		
		case 0xE9:
			DEBUG_PRINT("SBC"<<endl);
			addr = immediate();
			SBC(addr);
			break;

		case 0xEA:						//NOP
			DEBUG_PRINT("NOP"<<endl);
			break;

		case 0xEC:
			DEBUG_PRINT("CPX abs"<<endl);
			addr = absolute();
			CPX(memory->read_byte(addr));
			break;

		case 0xED:
			DEBUG_PRINT("SBC"<<endl); 	//SBC ABS
			addr = absolute();
			SBC(memory->read_byte(addr));
			break;

		case 0xEE:
			DEBUG_PRINT("INC"<<endl);
			addr = absolute();
			INC(addr);
			break;

		case 0xF0:						//BEQ rel
			DEBUG_PRINT("BEQ"<<endl);
			addr = immediate();
			BEQ(addr);
			break;

		case 0xF1:								//SBC (ind) Y
			DEBUG_PRINT("SBC"<<endl);
			addr = indirect_Y();
			SBC(memory->read_byte(addr));
			break;

		case 0xF5:								//SBC zpg X
			DEBUG_PRINT("ADC"<<endl);
			addr = zero_page(regX);
			SBC(memory->read_byte(addr));
			break;

		case 0xF6:
			DEBUG_PRINT("INC"<<endl);
			addr = zero_page(regX);
			INC(addr);
			break;

		case 0xF8:						//SED
			DEBUG_PRINT("SED"<<endl);
			regs.decimal_mode_flag = true;
			break;
		
		case 0xF9:								//SBC abs Y
			DEBUG_PRINT("SBC"<<endl);
			addr = absolute(regY);
			SBC(memory->read_byte(addr));
			break;		
		
		case 0xFE:						//INC ABS,X
			DEBUG_PRINT("INC"<<endl);
			addr = absolute(regX);
			INC(addr);
			break;

		case 0xFD:								//SBC abs X
			DEBUG_PRINT("SBC"<<endl);
			addr = absolute(regX);
			SBC(memory->read_byte(addr));
			break;

		default:
			DEBUG_PRINT("unimplemented: "<<hex<<unsigned(opcode)<<endl);
			raise(SIGTSTP);
			return false;
 	}

 	addr = 0;

  return true;

}