//
// Created by bianc on 26-11-2018.
//

#include "bomb.h"
#include <stdint.h>

Bomb::Bomb (uint32_t placedTime, uint8_t x, uint8_t y, uint8_t blastRadius)
{

    placedTime = getPlacedTime();
    x = getx();
    y = gety();
    blastRadius = getBlastRadius();

}

// get the time of when the bomb was placed
uint32_t Bomb::getPlacedTime()
{
    return placedTime;
}

// get position x from bomb
uint8_t Bomb::getx()
{
    return x;
}

// get position y from bomb
uint8_t Bomb::gety()
{
    return y;
}

// get how many blocks a bomb can
uint8_t Bomb::getBlastRadius()
{
    return blastRadius;
}