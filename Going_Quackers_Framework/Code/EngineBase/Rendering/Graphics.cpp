#include "Graphics.h"

Graphics* Graphics::getInstance()
{
	if (SingletonInstance == 0)
		SingletonInstance = new Graphics();

	return SingletonInstance;
}

bool Graphics::InitaliseAPIs()
{
	//- Create Temp Storage for APi -//
	GraphicsInterface* lp_interface;
	
	//- Instanciate new Api -//
	lp_interface = new DirectXGraphics();

	//- Setup all elements required for engine -//
	if (!SetUpAPI(lp_interface))
	{
		throw std::runtime_error("DIRECT X API SETTUP FAILED");
		return false;
	}
	//- save to api database -//
	APIs.push_back(lp_interface);

	//- null pointer for next api -//
	lp_interface = nullptr;

	//- Duplicate code above for new API -//
	//- Template Code below -//

	//lp_interface = new YOUR API CLASS HERE(); <--------------------------------------------

	//if (!SetUpAPI(lp_interface))
	//{
	//	throw std::runtime_error("API SETTUP FAILED");
	//	return false;
	//}

	//lp_interface = nullptr;

	//- Allocate the current Grpahics Api -//
	CurrentGraphicsAPI = APIs[0];
	((DirectXGraphics*)CurrentGraphicsAPI)->CreateEngineWindow();

	return true;
}

bool Graphics::ResizeRenderingSystem()
{
	return false;
}

bool Graphics::SetUpAPI(GraphicsInterface* API)
{
	if (!API->SetupRenderingSystem())
	{
		throw std::runtime_error("RENDING API SETTUP FAILED");
		return false;
	}

	if (!API->SetUpCamera())
	{
		throw std::runtime_error("CAMERA API SETTUP FAILED");
		return false;
	}

	if (!API->SetupRenderTexture())
	{
		throw std::runtime_error("RENDER TEXTURE API SETTUP FAILED");
		return false;
	}

	if (!API->SetUpImGui())
	{
		throw std::runtime_error("IMGUI API SETTUP FAILED");
		return false;
	}

	return false;
}

bool Graphics::StartRenderLoop()
{
	return CurrentGraphicsAPI->StartRenderLoop();
}

void Graphics::AddObjectToRenderLoop(Component& ar_component)
{
	return CurrentGraphicsAPI->AddObjectToRenderLoop(ar_component);
}

bool Graphics::SetNewActiveCamera(VirtualCamera& NextActiveCamera)
{
	return CurrentGraphicsAPI->SetNewActiveCamera(NextActiveCamera);
}

bool Graphics::LoadTexture(std::string TextureLocation)
{
	return CurrentGraphicsAPI->LoadTexture(TextureLocation);
}

bool Graphics::TileSheetLoad(std::string TileSheetLocaiton)
{
	return CurrentGraphicsAPI->TileSheetLoad(TileSheetLocaiton);
}

bool Graphics::CreateShader(std::string ShaderLocation)
{
	return CurrentGraphicsAPI->CreateShader(ShaderLocation);
}

bool Graphics::GetShader()
{
	return CurrentGraphicsAPI->GetShader();
}

bool Graphics::CreateEngineWindow()
{
	return CurrentGraphicsAPI->CreateEngineWindow();
}
