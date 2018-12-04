#include <Arduino.h>
#include <stdint.h>
#include "../../staticDefinitions.cpp"

#ifndef PEEP_H
#define PEEP_H

class Peep
{
public:
    Peep(uint16_t x, uint16_t y);
    uint8_t getx();
    uint8_t gety();

private:
    uint16_t x;
    uint16_t y;
};

#endif PEEP_H
