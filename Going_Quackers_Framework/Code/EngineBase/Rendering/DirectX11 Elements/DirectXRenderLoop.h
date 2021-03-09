#ifndef _DIRECT_X_RENDER_LOOP_H_
#define _DIRECT_X_RENDER_LOOP_H_

#include <vector>

#include "../../Game Systems/Components/Component.h"
#include "../../ImGui/EngineGuiClass.h"

#include "DirectXClass.h"
#include "DirectXImGui.h"
#include "DirectXCamera.h"
#include "DirectXShader.h"

class DirectXTexture;

class DirectXRenderLoop
{
public: 
	DirectXRenderLoop(DirectXClass& ar_DirectXClass);
	~DirectXRenderLoop();

	DirectXClass& mr_directXClass;
	
	bool Render(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShader& ar_Shader, DirectXTexture& ar_texture, DirectXImGui& ar_ImGui);
	void SetObjectToRender(Component& ObjToRender);

private:
	std::vector<Component&> GameObjectsToRender;

	bool EditorRender(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShader& ar_Shader, DirectXTexture& ar_texture, DirectXImGui& ar_ImGui);
	bool ActiveGameRender(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShader& ar_Shader, DirectXTexture& ar_texture, DirectXImGui& ar_ImGui);
};

#endif /* _DIRECT_X_RENDER_LOOP_H_ */
