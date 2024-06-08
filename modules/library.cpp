//library.cpp
#include "library.h"

void hexDump(void *addr, uint16_t len)
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


uint8_t* readBinFile(const string& filename, streampos &size){

	ifstream file(filename,ios::in | ios::binary | ios::ate);
	//streampos size;

	if (file.is_open())
	{

		size = file.tellg();

		uint8_t *rom = new uint8_t[size];

		file.seekg (0, ios::beg);
		file.read ((char*)rom, size);
		file.close();
		return rom;
	}

	return nullptr;

}
