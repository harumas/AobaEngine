#include "Win32Application.h"
#include <tchar.h>

#include "Helper.h"

Win32Application::Win32Application(HINSTANCE hInstance,
	const std::shared_ptr<IAppContext>& appContext,
	const UpdateCall& updateCall,
	const DisposeCall& disposeCall)
	: hInstance(hInstance),
	updateCall(updateCall),
	disposeCall(disposeCall),
	appContext(appContext)
{
	// COMの初期化
	ThrowIfFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED));
}

AppHandle Win32Application::InitWindow()
{
	// ウィンドウクラス生成
	windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = _T("AobaEngine");
	RegisterClassEx(&windowClass);

	// ウィンドウサイズの調整
	RECT windowRect = { 0, 0, static_cast<LONG>(appContext->GetWindowWidth()), static_cast<LONG>(appContext->GetWindowHeight()) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクトの生成
	HWND hwnd = CreateWindow(
		windowClass.lpszClassName,
		appContext->GetTitle().c_str(),
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

	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);

	return { hwnd,hInstance };
}

void Win32Application::Run()
{
	// メインループ
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// アプリケーション更新
			updateCall();
		}
	}

	// アプリケーション終了
	disposeCall();

	// クラスを登録解除する
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

LRESULT CALLBACK Win32Application::WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
}