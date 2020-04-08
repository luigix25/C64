#include "BusDevice.h"

BusDevice::BusDevice(uint16_t _base, uint16_t _size):base(_base),size(_size) {
    this->enabled = false;
}

void BusDevice::enable(bool status) {
    this->enabled = status;
}

uint16_t BusDevice::getBase(void) {
    return this->base;
}

uint16_t BusDevice::getSize(void) {
    return this->size;
}

bool BusDevice::isEnabled(void) {
    return this->enabled;
}