#pragma once
#define DIRECTINPUT_VERSION 0x0800 

#include <utility>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib") 

#include <cstdint>
#include <windows.h>

// 仮想キーコードの列挙体
enum class Key : uint16_t
{
	LBUTTON = 0x01,      // 左マウスボタン
	RBUTTON = 0x02,      // 右マウスボタン
	CANCEL = 0x03,       // 中断
	MBUTTON = 0x04,      // 中央マウスボタン
	BUTTON1 = 0x05,     // サイドボタン 1
	BUTTON2 = 0x06,     // サイドボタン 2
	BACK = 0x08,         // バックスペース
	TAB = 0x09,          // タブ
	CLEAR = 0x0C,        // CLEAR
	RETURN = 0x0D,       // ENTER
	SHIFT = 0x10,        // SHIFT
	CONTROL = 0x11,      // CTRL
	MENU = 0x12,         // ALT
	PAUSE = 0x13,        // PAUSE
	CAPITAL = 0x14,      // CAPS LOCK
	ESCAPE = 0x1B,       // ESC
	SPACE = 0x20,        // スペース
	PRIOR = 0x21,        // PAGE UP
	NEXT = 0x22,         // PAGE DOWN
	END = 0x23,          // END
	HOME = 0x24,         // HOME
	LEFT = 0x25,         // 左
	UP = 0x26,           // 上
	RIGHT = 0x27,        // 右
	DOWN = 0x28,         // 下
	SELECT = 0x29,       // SELECT
	PRINT = 0x2A,        // PRINT
	EXECUTE = 0x2B,      // EXECUTE
	SNAPSHOT = 0x2C,     // PRINT SCREEN
	INSERT = 0x2D,       // INSERT
	DEL = 0x2E,       // DELETE
	HELP = 0x2F,         // HELP
	K0 = 0x30,            // 0
	K1 = 0x31,            // 1
	K2 = 0x32,            // 2
	K3 = 0x33,            // 3
	K4 = 0x34,            // 4
	K5 = 0x35,            // 5
	K6 = 0x36,            // 6
	K7 = 0x37,            // 7
	K8 = 0x38,            // 8
	K9 = 0x39,            // 9
	A = 0x41,            // A
	B = 0x42,            // B
	C = 0x43,            // C
	D = 0x44,            // D
	E = 0x45,            // E
	F = 0x46,            // F
	G = 0x47,            // G
	H = 0x48,            // H
	I = 0x49,            // I
	J = 0x4A,            // J
	K = 0x4B,            // K
	L = 0x4C,            // L
	M = 0x4D,            // M
	N = 0x4E,            // N
	O = 0x4F,            // O
	P = 0x50,            // P
	Q = 0x51,            // Q
	R = 0x52,            // R
	S = 0x53,            // S
	T = 0x54,            // T
	U = 0x55,            // U
	V = 0x56,            // V
	W = 0x57,            // W
	X = 0x58,            // X
	Y = 0x59,            // Y
	Z = 0x5A,            // Z
	LWIN = 0x5B,         // 左ウィンドウキー
	RWIN = 0x5C,         // 右ウィンドウキー
	APPS = 0x5D,         // アプリケーションメニューキー
	SLEEP = 0x5F,        // スリープ
	NUMPAD0 = 0x60,      // 数字キー 0
	NUMPAD1 = 0x61,      // 数字キー 1
	NUMPAD2 = 0x62,      // 数字キー 2
	NUMPAD3 = 0x63,      // 数字キー 3
	NUMPAD4 = 0x64,      // 数字キー 4
	NUMPAD5 = 0x65,      // 数字キー 5
	NUMPAD6 = 0x66,      // 数字キー 6
	NUMPAD7 = 0x67,      // 数字キー 7
	NUMPAD8 = 0x68,      // 数字キー 8
	NUMPAD9 = 0x69,      // 数字キー 9
	MULTIPLY = 0x6A,     // *
	ADD = 0x6B,          // +
	SEPARATOR = 0x6C,    // |
	SUBTRACT = 0x6D,     // -
	DECIMAL = 0x6E,      // .
	DIVIDE = 0x6F,       // /
	F1 = 0x70,           // F1
	F2 = 0x71,           // F2
	F3 = 0x72,           // F3
	F4 = 0x73,           // F4
	F5 = 0x74,           // F5
	F6 = 0x75,           // F6
	F7 = 0x76,           // F7
	F8 = 0x77,           // F8
	F9 = 0x78,           // F9
	F10 = 0x79,          // F10
	F11 = 0x7A,          // F11
	F12 = 0x7B,          // F12
	F13 = 0x7C,          // F13
	F14 = 0x7D,          // F14
	F15 = 0x7E,          // F15
	F16 = 0x7F,          // F16
	F17 = 0x80,          // F17
	F18 = 0x81,          // F18
	F19 = 0x82,          // F19
	F20 = 0x83,          // F20
	F21 = 0x84,          // F21
	F22 = 0x85,          // F22
	F23 = 0x86,          // F23
	F24 = 0x87,          // F24
	NUMLOCK = 0x90,      // NUM LOCK
	SCROLL = 0x91,       // SCROLL LOCK
	LSHIFT = 0xA0,       // 左 SHIFT
	RSHIFT = 0xA1,       // 右 SHIFT
	LCONTROL = 0xA2,     // 左 CTRL
	RCONTROL = 0xA3,     // 右 CTRL
	LMENU = 0xA4,        // 左 ALT
	RMENU = 0xA5,        // 右 ALT
	BROWSER_BACK = 0xA6, // ブラウザ バック
	BROWSER_FORWARD = 0xA7, // ブラウザ フォワード
	BROWSER_REFRESH = 0xA8, // ブラウザ 更新
	BROWSER_STOP = 0xA9, // ブラウザ 停止
	BROWSER_SEARCH = 0xAA, // ブラウザ 検索
	BROWSER_FAVORITES = 0xAB, // ブラウザ お気に入り
	BROWSER_HOME = 0xAC, // ブラウザ ホーム
	VOLUME_MUTE = 0xAD, // 音量 ミュート
	VOLUME_DOWN = 0xAE, // 音量 ダウン
	VOLUME_UP = 0xAF, // 音量 アップ
	MEDIA_NEXT_TRACK = 0xB0, // メディア 次のトラック
	MEDIA_PREV_TRACK = 0xB1, // メディア 前のトラック
	MEDIA_STOP = 0xB2, // メディア 停止
	MEDIA_PLAY_PAUSE = 0xB3, // メディア 再生/一時停止
	LAUNCH_MAIL = 0xB4, // メール起動
	LAUNCH_MEDIA_SELECT = 0xB5, // メディア選択
	LAUNCH_APP1 = 0xB6, // アプリケーション1起動
	LAUNCH_APP2 = 0xB7, // アプリケーション2起動
	OEM_1 = 0xBA, // ; : (英語キーボードの場合)
	OEM_PLUS = 0xBB, // + (英語キーボードの場合)
	OEM_COMMA = 0xBC, // , < (英語キーボードの場合)
	OEM_MINUS = 0xBD, // - _ (英語キーボードの場合)
	OEM_PERIOD = 0xBE, // . > (英語キーボードの場合)
	OEM_2 = 0xBF, // / ? (英語キーボードの場合)
	OEM_3 = 0xC0, // ` ~ (英語キーボードの場合)
	OEM_4 = 0xDB, // [ { (英語キーボードの場合)
	OEM_5 = 0xDC, // \ | (英語キーボードの場合)
	OEM_6 = 0xDD, // ] } (英語キーボードの場合)
	OEM_7 = 0xDE, // ' " (英語キーボードの場合)
	OEM_8 = 0xDF, // OEM 8 (英語キーボードの場合)
	OEM_102 = 0xE2, // < > (英語キーボードの場合)
	PROCESSKEY = 0xE5, // PROCESSKEY
	PACKET = 0xE7, // PACKET
	ATTN = 0xF6, // ATTN
	CRSEL = 0xF7, // CRSEL
	EXSEL = 0xF8, // EXSEL
	EREOF = 0xF9, // EREOF
	PLAY = 0xFA, // PLAY
	ZOOM = 0xFB, // ZOOM
	NONAME = 0xFC, // NONAME
	PA1 = 0xFD, // PA1
	OEM_CLEAR = 0xFE // OEM CLEAR
};

class Input
{
public:
	static bool Initialize();
	static void Shutdown();
	static void Update();
	static void PostUpdate();
	static bool IsKeyPressed(Key key);
	static bool IsMouseButtonPressed(int button);
	static std::pair<float, float> GetMouseDelta();

private:
	static std::pair<float, float> mouseDelta;
	static bool keyState[256];

	Input() = delete;
	~Input() = delete;

	static LRESULT CALLBACK InputProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};
