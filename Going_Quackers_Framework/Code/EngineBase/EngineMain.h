#ifndef ENGINE_MAIN_H
#define ENGINE_MAIN_H

#define WIN32_LEAN_AND_MEAN

#include "../../Code/EngineBase/Rendering/Graphics.h"
#include "Game Systems/Input.h"
#include "Game Systems/Time.h"

class EngineMain
{
public:
	EngineMain();
	~EngineMain();

	void Run();
	bool Initalize();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool UpdateRenderLoop();
	void InitalizeWindows(int& ai_screenWidth, int& ai_screenHeight);
	void ShutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	Input* mp_Input;
	DirectXGraphics* mp_Graphics;

	bool mb_sizeMovement;
	bool mb_minamised;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

static EngineMain* ApplicationHandle = 0;

#endif // !ENGINE_MAIN_H

