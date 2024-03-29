#ifndef _DIRECTX_IMGUI_H_
#define _DIRECTX_IMGUI_H_

#include "../../ImGui/EngineGuiClass.h"
#include "DirectXClass.h"

#include "../../ImGui/ImGui SourceCode/imgui.h"
#include "../../ImGui/ImGui SourceCode/imgui_impl_win32.h"
#include "../../ImGui/ImGui SourceCode/imgui_impl_dx11.h"

class DirectXImGui
{
public:
	DirectXImGui(HWND hwnd, DirectXClass* mp_DirectX);
	~DirectXImGui();

	bool Update(ID3D11ShaderResourceView* ap_renderTexture, float width, float height);
	bool Render();
private:
	float m_scale;

};

#endif /* _DIRECTX_IMGUI_H_ */

