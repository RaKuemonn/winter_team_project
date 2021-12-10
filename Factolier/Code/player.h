#pragma once
#include "model.h"
#include "character.h"

class Player : public Character
{
public:
    Player();
    ~Player();

private:
    Model *player = nullptr;

};