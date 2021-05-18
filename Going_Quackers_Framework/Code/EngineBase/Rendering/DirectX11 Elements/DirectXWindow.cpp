#include "DirectXWindow.h"
#include "DirectXGraphics.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

DirectXWindow::DirectXWindow(DirectXGraphics* ap_DirectX)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	mi_width = desktop.right;
	mi_height = desktop.bottom;

	mp_DirectX = ap_DirectX;
}

DirectXWindow::~DirectXWindow()
{
}

bool DirectXWindow::SetupWindow()
{
	InitalizeWindows(mi_width, mi_height);
	return true; //<-----------------------------------------------TODO Change interface for all commands to relevent elements
}

LRESULT CALLBACK DirectXWindow::MessageHandler(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam)
{
	switch (uint)
	{
		case WM_SIZE:
		{
			if (wParam == SIZE_MINIMIZED)
			{
				mb_minamised = true;
				return 0;
			}
			else if (mb_minamised)
			{
				mb_minamised = false;
				return 0;
			}
		}

		case WM_ENTERSIZEMOVE:
		{
			mb_sizeMovement = true;
			return 0;
		}

		case WM_EXITSIZEMOVE:
		{
			mb_sizeMovement = false;
			//mp_DirectX->ResizeWindowCall();
			
			return 0;
		}

		case WM_GETMINMAXINFO:
		{
			auto info = reinterpret_cast<MINMAXINFO*>(lParam);
			info->ptMinTrackSize.x = 320;
			info->ptMinTrackSize.y = 200;
			return 0;
		}


		case WM_KEYDOWN:
			//mp_Input->KeyDown((unsigned int)wParam);
			return 0;
			break;

		case WM_KEYUP:
			//mp_Input->KeyUp((unsigned int)wParam);
			return 0;
			break;

		default:
			return DefWindowProc(hwnd, uint, wParam, lParam);
			break;
	}
}

void DirectXWindow::InitalizeWindows(int& ai_screenWidth, int& ai_screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int li_posX, li_posY;

	ApplicationHandle = this;

	m_hInstance = GetModuleHandle(NULL);

	m_applicationName = L"Going Quackers Engine";

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

	ai_screenWidth = mi_width;
	ai_screenHeight = mi_height;

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
		li_posX = (GetSystemMetrics(SM_CXSCREEN) - mi_width) / 2;
		li_posY = (GetSystemMetrics(SM_CYSCREEN) - mi_height) / 2;
	}

	m_hwnd = CreateWindowEx(0, m_applicationName, m_applicationName,
		WS_OVERLAPPEDWINDOW,
		li_posX, li_posY, mi_width, mi_height, NULL, NULL, m_hInstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(true);

	return;
}

void DirectXWindow::ShutdownWindows()
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