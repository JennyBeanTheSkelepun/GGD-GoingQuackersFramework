#ifndef _GRAHPICS_H_
#define _GRAHPICS_H_

#include <vector>
#include <string>


#include "Rendering/Interface/Graphics_API_Interface.h"
#include "Rendering/DirectX11 Elements/DirectXGraphics.h"

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
	//- API setup -//
	bool SetUpAPI(GraphicsInterface* API);
	bool InitaliseAPIs();

	//- Acsess functions -//
	//- Required Setup Elements -//
	bool ResizeRenderingSystem();
	//- Required Rendering Loop Elements -//
	bool StartRenderLoop();
	void AddObjectToRenderLoop(Component& component);

	//- Camera Elements -//
	bool SetNewActiveCamera(VirtualCamera& NextActiveCamera);
	//- Texture Loading -//
	bool LoadTexture(std::string TextureLocation);
	bool TileSheetLoad(std::string TileSheetLocation);
	//- Shader Loading -//
	bool CreateShader(std::string ShaderLocation);
	bool GetShader();
	//- Window Creation -//
	bool CreateEngineWindow();



	//- The current Graphics Api that is being used -//
	GraphicsInterface* CurrentGraphicsAPI;
	//- All Supported Interfaces -//
	std::vector<GraphicsInterface*> APIs;
};

#endif _GRAPHICS_H_
