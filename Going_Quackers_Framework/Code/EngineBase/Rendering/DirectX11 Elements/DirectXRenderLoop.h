#ifndef _DIRECT_X_RENDER_LOOP_H_
#define _DIRECT_X_RENDER_LOOP_H_

#include <vector>

//- TEMP INCLUDE UNTILL MOVE OVER MATRIXES -//
#include "../../Game Systems/GameObject.h"
#include "../../Game Systems/Components/SpriteRenderer.h"
//class SpriteRenderer;

#include "../../ImGui/EngineGuiClass.h"
//class EngineGuiClass;
//
#include "DirectXClass.h"
//class DirectXClass;
//
#include "DirectXImGui.h"
//class DirectXImGui;
//
#include "DirectXCamera.h"
#include "DirectXShaderManager.h"
#include "DirectXTextureManager.h"

//class EngineGuiClass;
//class DirectXClass;
//class DirectXImGui;
//class DirectXCamera;
//class DirectXShaderManager;
//class DirectXTextureManager;
#include "DirectXTwoDObject.h"

class DirectXRenderLoop
{
public: 
	DirectXRenderLoop(DirectXClass* ap_DirectX);
	~DirectXRenderLoop();

	bool Render(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShaderManager& ar_Shader, DirectXTextureManager& ar_texture, DirectXImGui& ar_ImGui);	
	int SetObjectToRender(SpriteRenderer* ObjToRender);
private:
	std::vector<SpriteRenderer*> GameObjectsToRender;
	DirectXTwoDObject* mp_2DModel;

	bool EditorRender(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShaderManager& ar_Shader, DirectXTextureManager& ar_texture, DirectXImGui& ar_ImGui);
	bool ActiveGameRender(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShaderManager& ar_Shader, DirectXTextureManager& ar_texture, DirectXImGui& ar_ImGui);
};

#endif /* _DIRECT_X_RENDER_LOOP_H_ */
