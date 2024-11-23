#include "Input.h"
#include <windows.h>
#include <libloaderapi.h>
#include <utility>
#include <vector>

bool Input::keyState[];
std::pair<float, float> Input::mouseDelta = {};

bool Input::Initialize()
{
	WNDCLASS wc{};
	wc.lpfnWndProc = &InputProc;
	wc.hInstance = ::GetModuleHandle(nullptr);
	wc.lpszClassName = TEXT("Input");
	RegisterClass(&wc);
	HWND hwnd = CreateWindow(wc.lpszClassName, nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, wc.hInstance, nullptr);

	RAWINPUTDEVICE rid[2]{};
	// mouse
	rid[0].usUsagePage = 0x01;
	rid[0].usUsage = 0x02;
	rid[0].dwFlags = RIDEV_INPUTSINK;
	rid[0].hwndTarget = hwnd;

	// keyboard
	rid[1].usUsagePage = 0x01;
	rid[1].usUsage = 0x06;
	rid[1].dwFlags = RIDEV_INPUTSINK;
	rid[1].hwndTarget = hwnd;

	RegisterRawInputDevices(rid, std::size(rid), sizeof(RAWINPUTDEVICE));

	return true;
}

void Input::Shutdown()
{
}

void Input::Update()
{
}

void Input::PostUpdate()
{
	mouseDelta.first = 0.0;
	mouseDelta.second = 0.0;
}

bool Input::IsKeyPressed(Key key)
{
	return keyState[static_cast<int>(key)];
}

bool Input::IsMouseButtonPressed(int button)
{
	return true;
}

std::pair<float, float> Input::GetMouseDelta()
{
	return mouseDelta;
}

LRESULT Input::InputProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (Msg == WM_INPUT)
	{
		UINT dwSize = 0;

		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

		if (dwSize > 0)
		{
			std::vector<BYTE> rawdata(dwSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.data(), &dwSize, sizeof(RAWINPUTHEADER)) == dwSize)
			{
				RAWINPUT* rawInput = reinterpret_cast<RAWINPUT*>(rawdata.data());

				if (rawInput->header.dwType == RIM_TYPEMOUSE)
				{
					int dx = rawInput->data.mouse.lLastX;
					int dy = rawInput->data.mouse.lLastY;

					mouseDelta.first = dx;
					mouseDelta.second = dy;
				}

				if (rawInput->header.dwType == RIM_TYPEKEYBOARD)
				{
					USHORT key = rawInput->data.keyboard.VKey;
					bool isHold = (rawInput->data.keyboard.Flags & RI_KEY_BREAK) == 0;

					keyState[key] = isHold;
				}
			}
		}

		return 0;
	}

	return ::DefWindowProc(hWnd, Msg, wParam, lParam);
}
