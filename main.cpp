#include "library.h"

using namespace std;


uint8_t memory[sixtyfourK];
registers regs;

char convert(char);
void decode(uint8_t);
int main(){

	ifstream file(ROM_CHIPS,ios::in | ios::binary | ios::ate);
	streampos size;

  	if (file.is_open())
  	{

  		uint8_t *rom = new uint8_t[sixteenK];

	    size = file.tellg();
        file.seekg (0, ios::beg);
    	file.read ((char*)rom, size);
    	file.close();

    	memcpy(memory+BASIC_START,rom,eigthK);
    	memcpy(memory+KERNAL_START,rom+eigthK,eigthK);

    	delete[] rom;

  	}

  	//hexDump(memory+BASIC_START,2);

  	decode(memory[BASIC_START]);

}


void decode(uint8_t opcode){

  uint8_t high = opcode & 0xFF00;
  uint8_t low = opcode & 0x00FF;

  switch(opcode){
    case 0x0000:        //BRK
      return;
    case 0x0001:        //ORA indirect with X


  }


}

/*char convert(char c){
	cout<<"ciao"<<endl;

	if(c >= 65 && c <= 90){					//lower case letters
		cout<<"lower case";
		c+= (97-65);
	} else if(c>=193 && c<= 218){
		cout<<"upper case";
		c-= (193-65);
	}

	return c;


}*/