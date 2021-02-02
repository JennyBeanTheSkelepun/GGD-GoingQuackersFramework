#include "Code/EngineBase/EngineMain.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	std::unique_ptr<EngineMain> Engine(new EngineMain());
	if (!Engine) 
		return 0;
	
	bool result = Engine->Initalize();
	if (result)
		Engine->Run();

	Engine->ShutDown();

	return 0;
}