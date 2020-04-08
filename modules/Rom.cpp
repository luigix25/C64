#include "Rom.h"

Rom::Rom(uint16_t _base, uint16_t _size) : BusDevice(_base, _size) {
    data = new char[_size];
}

Rom::~Rom() {
    if (data != nullptr) {
        delete[] data;
    }
}

uint8_t Rom::read(uint16_t address) {
    return data[address];
}

void Rom::write(uint16_t address, uint8_t value) {
    ;   /* well, this is a ROM read-only-memory, isn't it? */
}

void Rom::dump(void) {
    ;   // TODO to be implemented
}

bool Rom::fromDisk(const char* filename) {
    std::fstream file(filename);
    if (! file) return false;

    file.read(data, this->size);

    return true;
}
