#ifndef _DIRECT_X_RENDER_LOOP_H_
#define _DIRECT_X_RENDER_LOOP_H_

#include <vector>

#include "../../Game Systems/GameObject.h"
#include "../../Game Systems/Components/SpriteRenderer.h"
#include "../../Game Systems/Components/LineRenderer.h"

#include "../../ImGui/EngineGuiClass.h"
#include "DirectXClass.h"
#include "DirectXImGui.h"
#include "DirectXCamera.h"
#include "DirectXShaderManager.h"
#include "DirectXTextureManager.h"
#include "DirectXTwoDObject.h"

class DirectXLineRendererObject;

class DirectXRenderLoop
{
public: 
	DirectXRenderLoop(DirectXClass* ap_DirectX);
	~DirectXRenderLoop();

	bool Render(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShaderManager& ar_Shader, DirectXTextureManager& ar_texture, DirectXImGui& ar_ImGui);	
	int SetObjectToRender(SpriteRenderer* ap_objToRender);
	int RemoveObjectToRenderLoop(int ai_index);

	void AddLineRenderer(LineRenderer* lineRenderer);
	LineRenderer* RemoveLineRenderer(LineRenderer* lineRenderer);

private:
	std::vector<SpriteRenderer*> m_gameObjectsToRender;
	DirectXTwoDObject* mp_2DModel;

	std::vector<LineRenderer*> m_linesToRender;
	DirectXLineRendererObject* mp_Line;

	bool EditorRender(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShaderManager& ar_Shader, DirectXTextureManager& ar_texture, DirectXImGui& ar_ImGui);
	bool ActiveGameRender(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShaderManager& ar_Shader, DirectXTextureManager& ar_texture, DirectXImGui& ar_ImGui);
};

#endif /* _DIRECT_X_RENDER_LOOP_H_ */
