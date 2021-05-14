#include "EngineMain.h"
#include "SceneManager/SceneManager.h"
#include "../../Code/EngineBase/Rendering/Graphics.h"

EngineMain::EngineMain()
{
	

	//- Creates Singleton Instance -//
	Graphics::getInstance();
}

EngineMain::~EngineMain()
{
	

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
		gameObjects[i] = nullptr;
	}

}

bool EngineMain::Initalize()
{
	int li_screenWidth = 0, li_screenHeight = 0;
	bool result;

	

	Graphics::getInstance()->InitaliseAPIs();

	SceneManager::GetInstance()->Initialize();
	SceneManager::GetInstance()->NewScene("test", "Test Scene", "DEBUG", false);

	GameObject* mp_Model = new GameObject("Model 1");
	mp_Model->SetID("test01");
	mp_Model->AddComponent<SpriteRenderer>();
	mp_Model->GetTransform()->SetPosition(Vector2(2.0f, 0.0f));

	// Create the model object.
	GameObject* mp_Model2 = new GameObject("Model 2");
	mp_Model2->SetID("test02");
	result = mp_Model2->AddComponent<SpriteRenderer>();
	mp_Model2->GetTransform()->SetPosition(Vector2(-2.5f, 0.0f));
	mp_Model2->GetTransform()->SetLocalScale(Vector2(0.5f, 0.5f));
	mp_Model2->SetParent(mp_Model);
	
	// Create the model object.
	GameObject* mp_Model3 = new GameObject("Model 3");
	mp_Model3->SetID("test03");
	mp_Model3->AddComponent<SpriteRenderer>();
	mp_Model3->GetTransform()->SetPosition(Vector2(-5.0f, 0.0f));
	mp_Model3->GetTransform()->SetLocalScale(Vector2(0.5f, 0.5f));
	mp_Model3->SetParent(mp_Model2);

	SceneManager::GetInstance()->GetCurrentScene()->AddObject(mp_Model);
	SceneManager::GetInstance()->GetCurrentScene()->AddObject(mp_Model2);
	SceneManager::GetInstance()->GetCurrentScene()->AddObject(mp_Model3);

	SceneManager::GetInstance()->SaveCurrentScene();

	EngineGuiClass::getInstance()->InitializeObjectList(gameObjects);

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
	

	//- UPDATE LOOP START-//
	
	SceneManager::GetInstance()->Update(Time::GetDeltaTime());

	//for (size_t i = 0; i < gameObjects.size(); i++)
	//{
	//	gameObjects[i]->Update();
	//}

	SceneManager::GetInstance()->GetCurrentScene()->GetObjectByIndex(0)->GetTransform()->SetLocalRotation(
		SceneManager::GetInstance()->GetCurrentScene()->GetObjectByIndex(0)->GetTransform()->GetLocalRotation() + 20.0f * Time::GetDeltaTime());

	SceneManager::GetInstance()->GetCurrentScene()->GetObjectByIndex(2)->GetTransform()->SetLocalRotation(
		SceneManager::GetInstance()->GetCurrentScene()->GetObjectByIndex(2)->GetTransform()->GetLocalRotation() + 100.0f * Time::GetDeltaTime());

	//gameObjects[0]->GetTransform()->SetPosition(gameObjects[0]->GetTransform()->GetPosition() + Vector2(-0.1f, 0.0f) * Time::GetDeltaTime());
	//gameObjects[1]->GetTransform()->SetPosition(gameObjects[1]->GetTransform()->GetPosition() + Vector2(0.5f, 0.0f) * Time::GetDeltaTime());
	//gameObjects[0]->GetTransform()->SetLocalRotation(gameObjects[0]->GetTransform()->GetLocalRotation() + 20.0f * Time::GetDeltaTime());
	//gameObjects[1]->GetTransform()->SetLocalScale(gameObjects[1]->GetTransform()->GetLocalScale() - Vector2(0.1f, 0.1f) * Time::GetDeltaTime());
	//gameObjects[2]->GetTransform()->SetLocalRotation(gameObjects[2]->GetTransform()->GetLocalRotation() + 100.0f * Time::GetDeltaTime());


	Graphics::getInstance()->StartApiUpdateLoop();
	//- UPDATE LOOP END -//

	Graphics::getInstance()->StartRenderLoop();
	return true;
}
void EngineMain::Update()
{
	while (!mp_Input->IsKeyQueueEmpty())
	{
		KeyboardEvents keyEvent = mp_Input->readKeyQueue();
		unsigned char keycode = keyEvent.GetKey();
		std::string outputmessage = "keycode: ";
		outputmessage += keycode;
		outputmessage += "\n";
		OutputDebugStringA(outputmessage.c_str());
	}
}