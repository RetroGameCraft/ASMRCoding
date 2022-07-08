#include <windows.h>

struct Vector2
{
	Vector2() {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}

	float x{}, y{};
};

struct FireStrip
{
	Vector2 headPos{};
	UINT numTiles{};
};

COLORREF _fireColors[] =
{
	RGB(7, 7, 7),
	RGB(31, 7, 7),
	RGB(47, 15, 7),
	RGB(71, 15, 7),
	RGB(87, 23, 7),
	RGB(103, 31, 7),
	RGB(119, 31, 7),
	RGB(143, 39, 7),
	RGB(159, 47, 7),
	RGB(175, 63, 7),
	RGB(191, 71, 7),
	RGB(199, 71, 7),
	RGB(223, 79, 7),
	RGB(223, 87, 7),
	RGB(223, 87, 7),
	RGB(215, 95, 7),
	RGB(215, 103, 15),
	RGB(207, 111, 15),
	RGB(207, 119, 15),
	RGB(207, 127, 15),
	RGB(207, 135, 23),
	RGB(199, 135, 23),
	RGB(199, 143, 23),
	RGB(199, 151, 31),
	RGB(191, 159, 31),
	RGB(191, 159, 31),
	RGB(191, 159, 31),
	RGB(191, 167, 39),
	RGB(191, 167, 39),
	RGB(191, 175, 47),
	RGB(183, 175, 47),
	RGB(183, 183, 47),
	RGB(183, 183, 55),
	RGB(207, 207, 111),
	RGB(223, 223, 159),
	RGB(239, 239, 199),
	RGB(255, 255, 255)
};
constexpr UINT _numFireColors = sizeof(_fireColors) / sizeof(COLORREF);
constexpr UINT _fireTileSize = 32;

UINT _canvasWidth{};
UINT _canvasHeight{};

LRESULT CALLBACK windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc{};
	wc.lpfnWndProc = windowProc;
	wc.lpszClassName = TEXT("Fire Rain");
	RegisterClass(&wc);

	_canvasWidth = (UINT)GetSystemMetrics(SM_CXSCREEN);
	_canvasHeight = (UINT)GetSystemMetrics(SM_CYSCREEN);
	HWND hWnd = CreateWindowEx(0, wc.lpszClassName, wc.lpszClassName, WS_POPUP,
		0, 0, _canvasWidth, _canvasHeight, NULL, NULL, NULL, nullptr);

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	MSG msg{};
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

		}
	}
}