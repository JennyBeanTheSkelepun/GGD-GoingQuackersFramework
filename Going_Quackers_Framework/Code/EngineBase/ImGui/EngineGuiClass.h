#ifndef _ENGINE_IM_GUI_
#define _ENGINE_IM_GUI_

#include <d3d11.h>
#include <memory>

#include "../Graphics/DirectX/DirectXClass.h"

#include "ImGui SourceCode/imgui.h"
#include "ImGui SourceCode/imgui_impl_win32.h"
#include "ImGui SourceCode/imgui_impl_dx11.h"

class EngineGuiClass
{
public:
	EngineGuiClass();
	~EngineGuiClass();

	void Initalize();

	void Update(ID3D11ShaderResourceView* a_RenderTexture);
	void Render();


private:
	std::shared_ptr<DirectXClass> mp_DirectX;
	bool mb_toggleTestVar = false;
};



#endif // !_ENGINE_IM_GUI_