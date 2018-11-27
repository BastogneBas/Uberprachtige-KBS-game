//
// Created by bianc on 26-11-2018.
//
#include <stdint.h>

#ifndef BOMB_H
#define BOMB_H

class Bomb{
public:
    Bomb (uint32_t placedTime, uint8_t x, uint8_t y, uint8_t blastRadius = 3);
    uint32_t getPlacedTime();
    uint8_t getx();
    uint8_t gety();
    uint8_t getBlastRadius();

private:
    uint32_t placedTime;
    uint8_t x;
    uint8_t y;
    uint8_t blastRadius;
};

#endif //BOMB_H
