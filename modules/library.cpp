//library.cpp
#include "library.h"

void hexDump(void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf("  %s\n", buff);

            // Output the offset.
            printf("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf(" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
            buff[i % 16] = '.';
        } else {
            buff[i % 16] = pc[i];
        }

        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf("  %s\n", buff);
}

KeyboardMatrix RowColFromScancode(uint16_t code){

	KeyboardMatrix matrix;

	switch(code){
		case SDL_SCANCODE_A:
			matrix.col = 1;
			matrix.row = 2;
			break;

		case SDL_SCANCODE_B:
			matrix.col = 3;
			matrix.row = 4;
			break;

		case SDL_SCANCODE_C:
			matrix.col = 2;
			matrix.row = 4;
			break;

		case SDL_SCANCODE_D:	
			matrix.col = 2;
			matrix.row = 2;
			break;
		case SDL_SCANCODE_E:
			matrix.col = 1;
			matrix.row = 6;
			break;
		case SDL_SCANCODE_F:
			matrix.col = 2;
			matrix.row = 5;
			break;

		case SDL_SCANCODE_G:
			matrix.col = 3;
			matrix.row = 2;
			break;

		case SDL_SCANCODE_H:
			matrix.col = 3;
			matrix.row = 5;
			break;
		case SDL_SCANCODE_I:
			matrix.col = 4;
			matrix.row = 1;
			break;

		case SDL_SCANCODE_J:
			matrix.col = 4;
			matrix.row = 2;
			break;		
		case SDL_SCANCODE_K:
			matrix.col = 4;
			matrix.row = 5;
			break;		
		case SDL_SCANCODE_L:
			matrix.col = 5;
			matrix.row = 2;
			break;		
		case SDL_SCANCODE_M:
			matrix.col = 4;
			matrix.row = 4;
			break;		
		case SDL_SCANCODE_N:
			matrix.col = 4;
			matrix.row = 7;
			break;			
		case SDL_SCANCODE_O:
			matrix.col = 4;
			matrix.row = 6;
			break;		
		case SDL_SCANCODE_P:
			matrix.col = 5;
			matrix.row = 1;
			break;

		case SDL_SCANCODE_Q:
			matrix.col = 7;
			matrix.row = 6;
			break;
					
		case SDL_SCANCODE_R:
			matrix.col = 2;
			matrix.row = 1;
			break;
		
		case SDL_SCANCODE_S:
			matrix.col = 1;
			matrix.row = 5;
			break;
		
		case SDL_SCANCODE_T:
			matrix.col = 2;
			matrix.row = 6;
			break;		

		case SDL_SCANCODE_U:
			matrix.col = 3;
			matrix.row = 6;
			break;		

		case SDL_SCANCODE_V:
			matrix.col = 3;
			matrix.row = 7;
			break;	

		case SDL_SCANCODE_Z:
			matrix.col = 1;
			matrix.row = 4;
			break;

		case SDL_SCANCODE_5:
			matrix.col = 2;
			matrix.row = 0;
			break;

		case SDL_SCANCODE_RETURN:
			matrix.col = 0;
			matrix.row = 1;
			break;

		case SDL_SCANCODE_SPACE:
			matrix.col = 7;
			matrix.row = 4;
			break;

		case SDL_SCANCODE_F1:	//Equals
			matrix.col = 6;
			matrix.row = 5;
			break;
	}

	matrix.col = getMaskForCode(matrix.col);

	matrix.row = ~(1 << matrix.row); //operazione inversa di getMaskForCode

	return matrix;

}

uint8_t getMaskForCode(uint8_t code){

	if(code == 0){
		return 0xFE;
	} else  if(code == 1){
		return 0xFD;
	} else  if(code == 2){
		return 0xFB;
	} else  if(code == 3){
		return 0xF7;
	} else  if(code == 4){
		return 0xEF;
	} else  if(code == 5){
		return 0xDF;
	} else  if(code == 6){
		return 0xBF;
	} else  if(code == 7){
		return 0x7F;
	} else
		return 0xFF;

}