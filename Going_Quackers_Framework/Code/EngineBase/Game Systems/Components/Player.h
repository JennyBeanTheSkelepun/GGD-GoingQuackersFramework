#ifndef PLAYER_H
#define PLAYER_H
#pragma once

#include "Component.h"

class Player : public Component
{
public:
	Player(GameObject* owner);
};

#endif // !PLAYER_H