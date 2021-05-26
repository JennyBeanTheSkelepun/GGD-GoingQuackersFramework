#ifndef _DIRECT_X_WINDOW_H_
#define _DIRECT_X_WINDOW_H_

#define WIN32_LEAN_AND_MEAN

#define FULL_SCREEN false

#include "../../Game Systems/Input.h"
#include "../../ImGui/ImGui SourceCode/imgui.h"
#include <Windows.h>

class DirectXGraphics;

class DirectXWindow
{
public:
	DirectXWindow(DirectXGraphics* ap_DirectX);
	~DirectXWindow();

	bool SetupWindow();
	LRESULT MessageHandler(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam);
	void InitalizeWindows(int& ai_screenWidth, int& ai_screenHeight);

	void ShutdownWindows();

	bool mb_minamised;
	bool mb_sizeMovement;

	HWND m_hwnd;
	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;

	DirectXGraphics* mp_DirectX;

	int mi_width;
	int mi_height;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

static DirectXWindow* ApplicationHandle = 0;

#endif /* _DIRECT_X_WINDOW_H_ */
