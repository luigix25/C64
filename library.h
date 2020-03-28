#include <iostream>
#include<fstream>
#include <string.h>
#include <stdint.h>

using namespace std;


#define ROM_CHIPS "kernel_and_basic.bin"

#define eigthK 8192
#define sixteenK  16384
#define sixtyfourK 65536

#define KERNAL_START 0xE000
#define KERNAL_END 0xFFFF

#define BASIC_START 0xA000
#define BASIC_END 0xBFFF

#define ZERO_START 0x0000
#define ZERO_END 0x00FF

#define STACK_START 0x0100
#define STACK_END 0x01FF

#define RESET_routine 0xFCE2


void hexDump(void*, int);

void loadKernalAndBasic(uint8_t *,const char*);

enum register_name
{
	regA,regX,regY
	
};

struct registers{
	uint8_t reg[3];

	/*uint8_t regA;
	uint8_t regX;
	uint8_t regY;
*/
	uint16_t SP;
	uint16_t PC;


	bool interrupt_flag;
	bool decimal_mode_flag;

	uint8_t Flags;
};