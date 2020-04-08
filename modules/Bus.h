#pragma once

#include "BusDevice.h"
#include <vector>
using namespace std;

class Bus {
    private:
        vector<BusDevice*> devices;
    public:
        bool addDevice(BusDevice* dev);
        bool delDevice(BusDevice* dev);

        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);
};