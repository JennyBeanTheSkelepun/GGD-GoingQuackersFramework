#include "Graphics.h"
#include "../../EngineBase/Game Systems/Components/SpriteRenderer.h"
#include "../../EngineBase/Game Systems/Components/VirtualCamera.h"

Graphics* Graphics::SingletonInstance = nullptr;

Graphics* Graphics::getInstance()
{
	if (SingletonInstance == nullptr)
		SingletonInstance = new Graphics();

	return SingletonInstance;
}

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::InitaliseAPIs()
{
	//- Create Temp Storage for APi -//
	GraphicsInterface* lp_interface;
	
	//- Instanciate new Api -//
	lp_interface = reinterpret_cast<GraphicsInterface*>(new DirectXGraphics());

	//- Setup all elements required for engine -//
	if (!(reinterpret_cast<DirectXGraphics*>(lp_interface)->InitalizeGraphicalApi()))
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

	return true;
}

void Graphics::ResizeRenderingSystem()
{

}

void Graphics::StartApiUpdateLoop()
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->GraphicsAPIUpdate();
		break;
	}
}


void Graphics::StartRenderLoop()
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->StartAPIRenderLoop();
		break;
	}
}


int Graphics::AddObjectToApiRenderILoop(SpriteRenderer* component)
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->AddObjectToRenderLoop(component);
		break;
	}
}


void Graphics::SetNewActiveCamera(VirtualCamera* NextActiveCamera)
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->SetNewActiveCamera(NextActiveCamera);
		break;
	}
}

VirtualCamera* Graphics::GetActiveCamera()
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->GetActiveCamera();
		break;
	}
	return nullptr;
}

void Graphics::NullVirtualCamera()
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->NullVirtualCamera();
		break;
	}
}

int Graphics::LoadTexture(std::string TextureLocation)
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->LoadTexture(TextureLocation);
		break;
	}
}


int Graphics::LoadTileSheet(std::string TileSheetLocaiton)
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->LoadSpriteSheet(TileSheetLocaiton);
		break;
	}
}


int Graphics::LoadShader(std::wstring ShaderLocation)
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->LoadShader(ShaderLocation);
		break;
	}
}

int Graphics::RemoveObjectFromRenderLoop(int ai_ID)
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->RemoveObjectFromRenderLoop(ai_ID);
		break;
	}
}

int Graphics::RemoveTexture(int index)
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->RemoveTexture(index);
		break;
	}
}

int Graphics::RemoveTileSheet(int index)
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->RemoveSpriteSheet(index);
		break;
	}
}

int Graphics::RemoveShader(int index)
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->RemoveShader(index);
		break;
	}
}

Vector2 Graphics::GetWindowDimensions()
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->GetWindowDimensions();
		break;
	}
}

LineRenderer* Graphics::AddLineRenderer(LineRenderer* toAdd)
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->AddLineRenderer(toAdd);
		break;
	}
}

LineRenderer* Graphics::RemoveLineRenderer(LineRenderer* toRemove)
{
	switch (CurrentApi) {
	case API_TYPE::DIRECT_X_11:
		return reinterpret_cast<DirectXGraphics*>(CurrentGraphicsAPI)->RemoveLineRenderer(toRemove);
		break;
	}
}
