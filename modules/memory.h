//memory

#include "library.h"

class Memory{

	public:
		uint8_t read_byte(uint16_t);
		Memory();

	private:

		uint8_t memory[sixtyfourK];

		uint8_t basic[eightK]
		uint8_t kernal[eightK];

		uint8_t charset[fourK];

};