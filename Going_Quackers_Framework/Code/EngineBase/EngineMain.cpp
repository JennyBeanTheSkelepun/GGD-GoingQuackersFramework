#include "EngineMain.h"
#include <tchar.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EngineMain::EngineMain()
{
	mp_Input = 0;
	mp_Graphics = 0;
}

EngineMain::EngineMain(const EngineMain& ar_EngineMain)
{

}

EngineMain::~EngineMain()
{

}

bool EngineMain::Initalize()
{
	int li_screenWidth = 0, li_screenHeight = 0;
	bool result;

	InitalizeWindows(li_screenWidth, li_screenHeight);

	mp_Input = std::make_unique<Input>();
	if (!mp_Input) 
		return false;
	mp_Input->Initialize();

	mp_Graphics = std::make_unique<Graphics>();
	if (!mp_Graphics) 
		return false;
	
	result = mp_Graphics->Initialize(li_screenWidth, li_screenHeight, m_hwnd);
	if (!result) 
		return false;

	return true;
}

void EngineMain::ShutDown()
{
	if (mp_Graphics) 
		return mp_Graphics->Shutdown();
	
	ShutdownWindows();
	return;
}

void EngineMain::Run()
{
	MSG l_msg;
	bool lb_done = false, lb_result;

	ZeroMemory(&l_msg, sizeof(l_msg));

	while (!lb_done)
	{
		if (PeekMessage(&l_msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&l_msg);
			DispatchMessage(&l_msg);
		}

		if (l_msg.message == WM_QUIT)
		{
			lb_done = true;
		}
		else
		{
			lb_result = UpdateRenderLoop();
			if (!lb_result)
				lb_done = true;
		}
	}

	return;
}

bool EngineMain::UpdateRenderLoop()
{
	bool lb_result; 
	if (mp_Input->isKeyDown(VK_ESCAPE))
		return false;

	mp_Graphics->Update();
	lb_result = mp_Graphics->Render();
	if (!lb_result)
		return false;

	return true;
}

LRESULT CALLBACK EngineMain::MessageHandler(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam)
{
	switch (uint)
	{
	case WM_KEYDOWN:
		mp_Input->KeyDown((unsigned int) wParam);
		return 0;
		break;

	case WM_KEYUP:
		mp_Input->KeyUp((unsigned int) wParam);
		return 0;
		break;

	default:
		return DefWindowProc(hwnd, uint, wParam, lParam);
		break;
	}
}

void EngineMain::InitalizeWindows(int& ai_screenWidth, int& ai_screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int li_posX, li_posY;

	ApplicationHandle = this;

	m_hInstance = GetModuleHandle(NULL);

	m_applicationName = L"Going Quakers Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	ai_screenWidth = GetSystemMetrics(SM_CXSCREEN);
	ai_screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)ai_screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)ai_screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		li_posX = li_posY = 0;
	}
	else
	{
		ai_screenWidth = 1280;
		ai_screenHeight = 720;

		li_posX = (GetSystemMetrics(SM_CXSCREEN) - ai_screenWidth) / 2;
		li_posY = (GetSystemMetrics(SM_CYSCREEN) - ai_screenHeight) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		li_posX, li_posY, ai_screenWidth, ai_screenHeight, NULL, NULL, m_hInstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(true);

	ai_screenHeight -= 49; //<------------------------- IGNORE ITS A DUCTTAPE FIX AT 6PM
	ai_screenWidth -= 10; //<-------------------------- IGNORE ITS A DUCTTAPE FIX AT 6PM

	return;
}

void EngineMain::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hInstance = NULL;
	ApplicationHandle = NULL;
	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMessage, wParam, lParam))
	{
		return true;
	}

	switch (uMessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, uMessage, wParam, lParam);
		}
	}
}