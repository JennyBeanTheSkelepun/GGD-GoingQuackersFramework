#ifndef _ENGINE_IM_GUI_
#define _ENGINE_IM_GUI_

#include <d3d11.h>
#include <memory>
#include <string>

//- only knows windows Elements of Imgui to prevent anyone from messing up Api template code -//
#include "ImGui SourceCode/imgui.h"
#include "ImGui SourceCode/imgui_impl_win32.h"

class Graphics;
class GameObject;

class EngineGuiClass
{
//- Singleton Decliration -//
public:
	static EngineGuiClass* getInstance();

	EngineGuiClass(EngineGuiClass const&) = delete;
	void operator=(EngineGuiClass const&) = delete;

private:
	static EngineGuiClass* SingletonInstance;
	EngineGuiClass();
	~EngineGuiClass();

	void Update(ID3D11ShaderResourceView* a_RenderTexture, GameObject* obj);
	void GameUpdate();
	void EditorUpdate(ID3D11ShaderResourceView* a_RenderTexture, GameObject* obj);
	void Render();

	const char* BoolToString(bool Input);

	inline bool IsInPlayMode() { return mb_playGame; }

private:
	bool mb_maxEditor = false;
	bool mb_maxOnPlay = false;
	bool mb_playGame = false;
	bool mb_closeEditor = false;
};

#endif // !_ENGINE_IM_GUI_