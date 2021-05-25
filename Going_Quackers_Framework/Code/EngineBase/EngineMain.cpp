#include "EngineMain.h"
#include "../../Code/EngineBase/Rendering/Graphics.h"
#include "SceneManager/SceneManager.h"

EngineMain::EngineMain()
{
	Input::getInstance();

	//- Creates Singleton Instance -//
	Graphics::getInstance();
}

EngineMain::~EngineMain()
{
	// Delete objects
	for (size_t i = 0; i < SceneManager::GetInstance()->GetCurrentScene()->GetSceneObjects().size(); i++)
	{
		SceneManager::GetInstance()->GetCurrentScene()->DeleteObjectAtIndex(i);
	}
}

bool EngineMain::Initalize()
{
	int li_screenWidth = 0, li_screenHeight = 0;
	bool result;

	Graphics::getInstance()->InitaliseAPIs();
	SceneManager::GetInstance()->Initialize();
	SceneManager::GetInstance()->ChangeScene("default", false);

	EngineGuiClass::getInstance()->InitializeObjectList(SceneManager::GetInstance()->GetCurrentScene()->GetSceneObjectsList());

	return true;
}

void EngineMain::Run()
{
	bool exit = false;
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
	if (Input::getInstance()->isKeyHeldDown(KeyCode::ESCAPE))
		return false;

	//- UPDATE LOOP START-//

	for (size_t i = 0; i < SceneManager::GetInstance()->GetCurrentScene()->GetSceneObjects().size(); i++)
	{
		if (SceneManager::GetInstance()->GetCurrentScene()->GetObjectByIndex(i)->ShouldDestroy())
		{
			SceneManager::GetInstance()->GetCurrentScene()->DeleteObjectAtIndex(i);
			break;
		}

		SceneManager::GetInstance()->GetCurrentScene()->GetObjectByIndex(i)->Update();
	}

	if (Input::getInstance()->isKeyHeldDown(KeyCode::A))
	{
		Debug::getInstance()->Log("you did it2");
	}
	if (Input::getInstance()->isKeyHeldDown(KeyCode::D))
	{
		Debug::getInstance()->Log("you did it1");
	}

	Input::getInstance()->Update();
	Graphics::getInstance()->StartApiUpdateLoop();
	//- UPDATE LOOP END -//

	Graphics::getInstance()->StartRenderLoop();
	return true;
}
