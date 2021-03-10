#ifndef _DIRECT_X_RENDER_LOOP_H_
#define _DIRECT_X_RENDER_LOOP_H_

#include <vector>

#include "../../Game Systems/Components/SpriteRenderer.h"
#include "../../ImGui/EngineGuiClass.h"

#include "DirectXClass.h"
#include "DirectXImGui.h"
#include "DirectXCamera.h"
#include "DirectXShaderManager.h"
#include "DirectXTextureManager.h"

#include "../../Game Systems/Components/Sprite.h"

class DirectXRenderLoop
{
public: 
	DirectXRenderLoop(DirectXClass& ar_DirectXClass);
	~DirectXRenderLoop();

	DirectXClass& mr_directXClass;
	
	bool Render(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShaderManager& ar_Shader,
				DirectXTextureManager& ar_texture, DirectXImGui& ar_ImGui);
	
	void SetObjectToRender(SpriteRenderer* ObjToRender);

private:
	std::vector<SpriteRenderer*> GameObjectsToRender;

	bool EditorRender(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShaderManager& ar_Shader,
					  DirectXTextureManager& ar_texture, DirectXImGui& ar_ImGui);

	bool ActiveGameRender(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShaderManager& ar_Shader, 
					  DirectXTextureManager& ar_texture, DirectXImGui& ar_ImGui);
};

#endif /* _DIRECT_X_RENDER_LOOP_H_ */
