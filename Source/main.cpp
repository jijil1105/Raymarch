#include "System/Window/Window.h"
#include "Application/Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	// WICファイル読込のため、COMを初期化しておく
	auto result = CoInitializeEx(NULL, COINITBASE_MULTITHREADED);
	if (FAILED(result)) return 1;

	DXApplication dxApp(1280, 720, L"DX Sample");
	Window::Run(&dxApp, hInstance);
	return 0;
}
