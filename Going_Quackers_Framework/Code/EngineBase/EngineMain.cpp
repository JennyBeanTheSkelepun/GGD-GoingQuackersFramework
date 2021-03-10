#include "EngineMain.h"

EngineMain::EngineMain()
{
	mp_Input = 0;

	//- Creates Singleton Instance -//
	Graphics::getInstance();
}

EngineMain::~EngineMain()
{
	delete mp_Input;
	mp_Input = nullptr;
}

bool EngineMain::Initalize()
{
	int li_screenWidth = 0, li_screenHeight = 0;
	bool result;

	mp_Input = new Input();

	if (!mp_Input) 
		return false;
	mp_Input->Initialize();

	Graphics::getInstance()->InitaliseAPIs();

	return true;
}

void EngineMain::Run()
{
	MSG l_msg;
	bool lb_done = false, lb_result;

	ZeroMemory(&l_msg, sizeof(l_msg));

	while (!lb_done)
	{
		if (PeekMessage(&l_msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&l_msg);
			DispatchMessage(&l_msg);
		}

		if (l_msg.message == WM_QUIT)
		{
			lb_done = true;
		}
		else
		{
			lb_result = UpdateRenderLoop();
			if (!lb_result)
				lb_done = true;
		}

		Time::FrameEnd();
	}

	return;
}

bool EngineMain::UpdateRenderLoop()
{
	bool lb_result; 
	if (mp_Input->isKeyDown(VK_ESCAPE))
		return false;

	//- UPDATE LOOP START-//



	Graphics::getInstance()->StartApiUpdateLoop();
	//- UPDATE LOOP END -//

	Graphics::getInstance()->StartRenderLoop();
}