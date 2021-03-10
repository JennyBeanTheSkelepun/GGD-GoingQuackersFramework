#ifndef _GRAHPICS_API_INTERFACE_H_
#define _GRAPHICS_API_INTERFACE_H_

#include <string>
#include "../../Game Systems/Components/Component.h"

class VirtualCamera
{
public: 
	VirtualCamera() {};
	~VirtualCamera() {};
};

class GraphicsInterface
{
public:
	virtual bool InitalizeGraphicalApi() = 0;
	virtual void SetNewActiveCamera(VirtualCamera& vCam) = 0;
	virtual void AddObjectToRenderLoop(Component& ar_component) = 0;
	virtual int LoadTexture(std::string TextureLocation) = 0;
	virtual int LoadShader(std::string ShaderLocation) = 0;
	virtual int LoadSpriteSheet(std::string SpriteSheetLocation) = 0;
	virtual void GraphicsAPIUpdate() = 0;
	virtual void StartAPIRenderLoop() = 0;
};

#endif _GRAPHICS_API_INTERFACE_H_
