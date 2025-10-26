#pragma once
#include <windows.h>
#include <tchar.h>
#include "../../Application/Application.h"

class Window
{
public:
	static void Run(DXApplication* dxApp, HINSTANCE hInstance);

private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
};
