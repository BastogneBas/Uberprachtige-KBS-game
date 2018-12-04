#include <Arduino.h>
#include "../../staticDefinitions.cpp"
#include "Peep.h"

Peep::Peep(uint16_t x, uint16_t y)
{
    x = getx();
    y = gety();
}

// get position x from peep
uint8_t Peep::getx()
{
    return x;
}

// get position y from peep
uint8_t Peep::gety()
{
    return y;
}
