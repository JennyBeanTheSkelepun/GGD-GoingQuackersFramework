#include <iostream>
#include "EngineMain.h"

int main()
{
	std::unique_ptr<EngineMain> Engine(new EngineMain());

	while (!Engine->IsClosed())
	{
		Engine->Update();
		Engine->Render();
	}

	return 0;
}