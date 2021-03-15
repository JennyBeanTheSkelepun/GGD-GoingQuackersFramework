#ifndef ENGINE_MAIN_H
#define ENGINE_MAIN_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "Game Systems/Input.h"
#include "Game Systems/Time.h"
#include "Game Systems/GameObject.h"

class EngineMain
{
public:
	EngineMain();
	~EngineMain();

	void Run();
	bool Initalize();

private:
	bool UpdateRenderLoop();

	std::vector<GameObject*> gameObjects;
	
	Input* mp_Input;
};

#endif // !ENGINE_MAIN_H

