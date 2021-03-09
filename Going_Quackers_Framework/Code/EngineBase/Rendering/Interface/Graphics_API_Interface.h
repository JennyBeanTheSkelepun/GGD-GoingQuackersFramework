#ifndef _GRAHPICS_API_INTERFACE_H_
#define _GRAPHICS_API_INTERFACE_H_

#include <string>

class VirtualCamera;

class GraphicsInterface
{
public:
	//- Required Setup Elements -//
	virtual bool SetupRenderingSystem() = 0;
	virtual bool ResizeRenderingSystem() = 0;
	virtual bool SetUpImGui() = 0;

	//- Required Rendering Loop Elements -//
	virtual bool StartRenderLoop() = 0;
	virtual void AddObjectToRenderLoop(Component& ar_component) = 0;

	//- Camera Elements -//
	virtual bool SetUpCamera() = 0;
	virtual bool SetNewActiveCamera(VirtualCamera& NextActiveCamera) = 0;
	
	//- Render Texture Setup -//
	virtual bool SetupRenderTexture() = 0;

	//- Texture Loading -//
	virtual bool LoadTexture(std::string TextureLocation) = 0;
	virtual bool TileSheetLoad(std::string SpriteSheetLocation) = 0;
	
	//- Shader Loading -//
	virtual bool CreateShader(std::string ShaderLocation) = 0;
	virtual bool GetShader() = 0;

	//- Window Creation -//
	virtual bool CreateEngineWindow() = 0;
};

#endif _GRAPHICS_API_INTERFACE_H_
