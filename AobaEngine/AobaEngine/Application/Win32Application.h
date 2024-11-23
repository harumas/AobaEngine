#pragma once
#include <functional>
#include <memory>
#include <windows.h>
#include <WinUser.h>

#include "AppContext.h"

struct AppHandle
{
	HWND hwnd;
	HINSTANCE hInstance;
};

class Win32Application
{
public:
	using UpdateCall = std::function<void()>;
	using DisposeCall = std::function<void()>;

	Win32Application(HINSTANCE hInstance, const std::shared_ptr<IAppContext>& appContext, const UpdateCall& updateCall, const DisposeCall& disposeCall);
	AppHandle InitWindow();
	void Run();

private:
	HINSTANCE hInstance;
	WNDCLASSEX windowClass = {};
	UpdateCall updateCall;
	DisposeCall disposeCall;

	std::shared_ptr<IAppContext> appContext;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
};
