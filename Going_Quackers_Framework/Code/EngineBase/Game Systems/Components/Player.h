#ifndef PLAYER_H
#define PLAYER_H
#pragma once

#include "Component.h"
#include "C:\Users\m013002i\Documents\GitHub\GGD-GoingQuackersFramework\Going_Quackers_Framework\Code\EngineBase\Game Systems\GameObject.h"

class Player : public Component
{
public:
	Player(GameObject* owner);
	bool grabbed;
private:
	void GrabWall();
};

#endif // !PLAYER_H