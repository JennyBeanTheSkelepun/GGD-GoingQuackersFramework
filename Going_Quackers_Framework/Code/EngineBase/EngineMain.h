#ifndef ENGINE_MAIN_H
#define ENGINE_MAIN_H

#define WIN32_LEAN_AND_MEAN

#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Time/Time.h"

class EngineMain
{
public:
	EngineMain();
	EngineMain(const EngineMain& ar_EngineMain);
	~EngineMain();

	void ShutDown();
	void Run();
	bool Initalize();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:

	bool UpdateRenderLoop();
	void InitalizeWindows(int&, int&);
	void ShutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	std::unique_ptr<Input> mp_Input;
	std::unique_ptr<Graphics> mp_Graphics;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

static EngineMain* ApplicationHandle = 0;

#endif // !ENGINE_MAIN_H

