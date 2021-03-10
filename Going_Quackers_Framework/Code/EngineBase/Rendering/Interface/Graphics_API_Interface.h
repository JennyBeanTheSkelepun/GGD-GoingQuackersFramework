#ifndef _GRAHPICS_API_INTERFACE_H_
#define _GRAPHICS_API_INTERFACE_H_

#include <string>
//- Needed to forward declare Sprite renderer to prevent issues -//
class SpriteRenderer;

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
	//virtual void SetNewActiveCamera(VirtualCamera& vCam) = 0;
	
	virtual int AddObjectToRenderLoop(SpriteRenderer* ar_component) = 0;
	virtual int RemoveObjectFromRenderLoop(int index) = 0;

	virtual int LoadTexture(std::string TextureLocation) = 0;
	virtual int RemoveTexture(int index) = 0;
	virtual void CleanUpTexturePool() = 0;

	virtual int LoadShader(std::string ShaderLocation) = 0;
	virtual int RemoveShader(int index) = 0;
	virtual void CleanUpShaderPool() = 0;

	virtual int LoadSpriteSheet(std::string SpriteSheetLocation) = 0;
	virtual int RemoveSpriteSheet(int index) = 0;
	virtual void CleanUpSpriteSheetPool() = 0;

	virtual void GraphicsAPIUpdate() = 0;
	virtual void StartAPIRenderLoop() = 0;
};

#endif _GRAPHICS_API_INTERFACE_H_
