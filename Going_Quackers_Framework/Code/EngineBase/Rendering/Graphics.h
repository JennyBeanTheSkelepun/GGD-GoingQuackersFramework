#ifndef _GRAHPICS_H_
#define _GRAHPICS_H_

#include <vector>
#include <string>

#include "Interface/Graphics_API_Interface.h"
#include "DirectX11 Elements/DirectXGraphics.h"

// TODO CHANGE FROM TEMPLATE INTO WHAT I HAVE FOR COMPONENET SYSTEM

class Graphics
{
//- Singleton Decliration -//
public:
	static Graphics* getInstance();

	Graphics(Graphics const&) = delete;
	void operator=(Graphics const&) = delete;

private:
	static Graphics* SingletonInstance;
	Graphics();

//- Class Functions -//
public:
	bool InitaliseAPIs();
	
	template <class C>
	void ResizeRenderingSystem();
	
	template <class C>
	void StartApiUpdateLoop();
	
	template <class C>
	void StartRenderLoop();
	
	template <class C>
	int AddObjectToRenderLoop(SpriteRenderer* component);

	template <class C>
	int RemoveObjectFromRenderLoop(int ai_ID);
	
	template <class C>
	void SetNewActiveCamera(VirtualCamera& NextActiveCamera);
	
	template <class C>
	int LoadTexture(std::string TextureLocation);

	template <class C>
	int RemoveTexture(int index);
	
	template <class C>
	int LoadTileSheet(std::string TileSheetLocation);

	template <class C>
	int RemoveTileSheet(int index);
	
	template <class C>
	int LoadShader(std::string ShaderLocation);
	
	template <class C>
	int RemoveShader(int index);

	//- The current Graphics Api that is being used -//
	GraphicsInterface* CurrentGraphicsAPI;
	//- All Supported Interfaces -//
	std::vector<GraphicsInterface*> APIs;
};

template <class C>
void Graphics::ResizeRenderingSystem()
{
}

template <class C>
void Graphics::StartApiUpdateLoop()
{
	((C*)CurrentGraphicsAPI)->GraphicsAPIUpdate();
}

template <class C>
void Graphics::StartRenderLoop()
{
	((C*)CurrentGraphicsAPI)->StartAPIRenderLoop();
}

template <class C>
int Graphics::AddObjectToRenderLoop(SpriteRenderer* ar_component)
{
	return ((C*)CurrentGraphicsAPI)->AddObjectToRenderLoop(ar_component);
}

template <class C>
void Graphics::SetNewActiveCamera(VirtualCamera& NextActiveCamera)
{
	return ((C*)CurrentGraphicsAPI)->SetNewActiveCamera(NextActiveCamera);
}

template <class C>
int Graphics::LoadTexture(std::string TextureLocation)
{
	return ((C*)CurrentGraphicsAPI)->LoadTexture(TextureLocation);
}

template <class C>
int Graphics::LoadTileSheet(std::string TileSheetLocaiton)
{
	return ((C*)CurrentGraphicsAPI)->LoadSpriteSheet(TileSheetLocaiton);
}

template <class C>
int Graphics::LoadShader(std::string ShaderLocation)
{
	return ((C*)CurrentGraphicsAPI)->LoadShader(ShaderLocation);
}


template<class C>
inline int Graphics::RemoveObjectFromRenderLoop(int ai_ID)
{
	return ((C*)CurrentGraphicsAPI)->LoadShader(ai_ID)
}

template<class C>
inline int Graphics::RemoveTexture(int index)
{
	return ((C*)CurrentGraphicsAPI)->RemoveTexture(index);
}

template<class C>
inline int Graphics::RemoveTileSheet(int index)
{
	return ((C*)CurrentGraphicsAPI)->RemoveTileSheet(index);
}

template<class C>
inline int Graphics::RemoveShader(int index)
{
	return ((C*)CurrentGraphicsAPI)->RemoveShader(index);
}

#endif _GRAPHICS_H_
