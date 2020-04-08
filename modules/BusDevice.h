#pragma once

#include <stdint.h>

class BusDevice {
    protected:
        const uint16_t base;
        const uint16_t size;
        bool enabled;
    public:
        BusDevice(uint16_t base_address, uint16_t size);

        void enable(bool status);
        bool isEnabled(void);

        uint16_t getBase(void);
        uint16_t getSize(void);

        virtual uint8_t read(uint16_t address) = 0;
        virtual void write(uint16_t address, uint8_t value) = 0;
        virtual void dump(void) = 0;
};