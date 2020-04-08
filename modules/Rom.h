#pragma once

#include <stdint.h>
#include <fstream>

#include "BusDevice.h"

class Rom : public BusDevice {
    private:
        char* data = nullptr;
    public:
        Rom(uint16_t _base, uint16_t _size);
        ~Rom();
        virtual uint8_t read(uint16_t address);
        virtual void write(uint16_t address, uint8_t value);
        virtual void dump(void);

        bool fromDisk(const char* filename);

};