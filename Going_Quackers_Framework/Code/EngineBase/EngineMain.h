#ifndef _ENGINE_MAIN_H_
#define _ENGINE_MAIN_H_

#include "Game Systems/Time.h"
#include "Game Systems/GameObject.h"
#include "Game Systems/Input.h"
#include "Game Systems/Debug.h"
class Rigidbody;

class EngineMain
{
public:
	EngineMain();
	~EngineMain();
	bool Initalize();
	void Run();

private:
	bool UpdateRenderLoop();
};

#endif // !_ENGINE_MAIN_H_

