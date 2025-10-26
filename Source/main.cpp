#include "System/Window/Window.h"
#include "Application/Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	// WIC�t�@�C���Ǎ��̂��߁ACOM�����������Ă���
	auto result = CoInitializeEx(NULL, COINITBASE_MULTITHREADED);
	if (FAILED(result)) return 1;

	DXApplication dxApp(1280, 720, L"DX Sample");
	Window::Run(&dxApp, hInstance);
	return 0;
}
