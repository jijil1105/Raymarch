#include "Window.h"
#include "../Timer/Timer.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void Window::Run(DXApplication* dxApp, HINSTANCE hInstance)
{
	// ウィンドウクラス生成
	WNDCLASSEX windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = _T("DXSampleClass");
	RegisterClassEx(&windowClass);

	// ウィンドウサイズの調整
	RECT windowRect = { 0, 0, dxApp->GetWindowWidth(), dxApp->GetWindowHeight() };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクトの生成
	HWND hwnd = CreateWindow(
		windowClass.lpszClassName,
		dxApp->GetTitle(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	Timer time;
	time.Init();

	// アプリケーション初期化
	dxApp->OnInit(hwnd, time.GetElapsedTime(), dxApp->GetWindowWidth(), dxApp->GetWindowHeight());

	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);

	PlaySound(TEXT("Assets/Music/ooooinst.wav"), NULL, SND_FILENAME | SND_ASYNC);

	// メインループ
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// アプリケーション更新
		dxApp->OnUpdate(time.GetElapsedTime());
		dxApp->OnRender();
	}

	PlaySound(NULL, NULL, 0);

	// アプリケーション終了
	dxApp->OnDestroy();

	// クラスを登録解除する
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
	return 0;
}
