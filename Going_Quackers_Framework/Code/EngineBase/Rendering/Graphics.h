#ifndef _GRAHPICS_H_
#define _GRAHPICS_H_

#include <vector>
#include <string>

//- Need to forward declare Interface as its included in DirectXGraphics -//
class SpriteRenderer;

#include "DirectX11 Elements/DirectXGraphics.h"

enum API_TYPE {
	DIRECT_X_11 = 0
};

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
	~Graphics();

//- Class Functions -//
public:
	bool InitaliseAPIs();

	void ResizeRenderingSystem();
	void StartApiUpdateLoop();  
	void StartRenderLoop();
	
	int AddObjectToApiRenderILoop(SpriteRenderer* component);
	int RemoveObjectFromRenderLoop(int ai_ID);
	  
	//void SetNewActiveCamera(VirtualCamera& NextActiveCamera);
	  
	int LoadTexture(std::string TextureLocation);  
	int RemoveTexture(int index);
	  
	int LoadTileSheet(std::string TileSheetLocation);
	int RemoveTileSheet(int index);
	
	int LoadShader(std::wstring ShaderLocation);
	int RemoveShader(int index);

	API_TYPE CurrentApi = API_TYPE::DIRECT_X_11;

	//- The current Graphics Api that is being used -//
	GraphicsInterface* CurrentGraphicsAPI;
	//- All Supported Interfaces -//
	std::vector<GraphicsInterface*> APIs;
};

#endif _GRAPHICS_H_
