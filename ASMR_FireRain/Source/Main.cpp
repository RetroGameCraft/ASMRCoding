#include "ColorPalette.h"
#include <vector>

UINT _canvasWidth{};
UINT _canvasHeight{};

float random01() { return rand() / (float)RAND_MAX; }
float randomRange(float minVal, float maxVal) { float t = random01(); return minVal + (maxVal - minVal) * t; }
UINT randomRange(UINT minVal, UINT maxVal) { float t = random01(); return (UINT)((float)minVal + (float)(maxVal - minVal) * t); }

struct Timer
{
	void init()
	{
		QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		prevTime = currentTime;
		toSeconds = 1.0f / (float)freq;
	}

	void tick()
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		elapsedTime = (currentTime - prevTime) * toSeconds;
		prevTime = currentTime;
	}

	__int64 freq{};
	__int64 prevTime{};
	__int64 currentTime{};
	float elapsedTime;
	float toSeconds{};
};

Timer _timer{};

struct Vector2
{
	Vector2() {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}

	float x{}, y{};
};

struct FireTile
{
	~FireTile()
	{
		if (brush) DeleteObject(brush);
	}
	HBRUSH brush{};
};

struct FireStrip
{
	Vector2 headPos{};
	std::vector<FireTile> tiles{};
	float speed;

	inline static const float minSpeed = 200.0f;
	inline static const float maxSpeed = 300.0f;
	inline static const UINT minNumTiles = 20;
	inline static const UINT tileSize = 32;
};

struct FireRain
{
	void init()
	{
		fireStrips.resize(_canvasWidth / FireStrip::tileSize + 1);
		UINT maxNumTiles = (UINT)((_canvasHeight / FireStrip::tileSize) * 0.7f);

		UINT numStrips = (UINT)fireStrips.size();
		for (UINT stripIndex = 0; stripIndex < numStrips; ++stripIndex)
		{
			fireStrips[stripIndex].headPos = Vector2(stripIndex * (float)FireStrip::tileSize + (float)(FireStrip::tileSize * 0.5f), -(float)(FireStrip::tileSize * 0.5f));
			fireStrips[stripIndex].speed = randomRange(FireStrip::minSpeed, FireStrip::maxSpeed);

			UINT numTiles = randomRange(FireStrip::minNumTiles, maxNumTiles);
			fireStrips[stripIndex].tiles.resize(numTiles);
			for (UINT tileIndex = 0; tileIndex < numTiles; ++tileIndex)
			{
				UINT colorIndex = _numFireColors - 1 - (UINT)((tileIndex / (float)(numTiles - 1)) * (float)(_numFireColors - 1));
				LOGBRUSH lb{};
				lb.lbColor = _fireColors[colorIndex];
				lb.lbStyle = BS_SOLID;
				fireStrips[stripIndex].tiles[tileIndex].brush = CreateBrushIndirect(&lb);
			}
		}
	}

	void update()
	{
		for (auto& strip : fireStrips)
		{
			strip.headPos.y += strip.speed * _timer.elapsedTime;
			float tailPosY = strip.headPos.y - (float)(((UINT)strip.tiles.size() - 1) * FireStrip::tileSize);
			if ((tailPosY - (FireStrip::tileSize * 0.5f) > _canvasHeight))
				strip.headPos.y = -(float)(FireStrip::tileSize * 0.5f);
		}
	}

	std::vector<FireStrip> fireStrips{};
};

FireRain _fireRain{};

LRESULT CALLBACK windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc{};
	wc.lpfnWndProc = windowProc;
	wc.lpszClassName = TEXT("Fire Rain");
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	_canvasWidth = (UINT)GetSystemMetrics(SM_CXSCREEN);
	_canvasHeight = (UINT)GetSystemMetrics(SM_CYSCREEN);
	HWND hWnd = CreateWindowEx(0, wc.lpszClassName, wc.lpszClassName, WS_POPUP,
		0, 0, _canvasWidth, _canvasHeight, NULL, NULL, NULL, nullptr);

	_timer.init();
	_fireRain.init();

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
			_timer.tick();
			_fireRain.update();

			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc{};
	PAINTSTRUCT ps{};
	UINT stripIndex{};
	UINT numFireStrips = (UINT)_fireRain.fireStrips.size();

	switch (msg)
	{
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:

		if (wParam == VK_ESCAPE) SendMessage(hWnd, WM_CLOSE, 0, 0);
		return 0;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		for (stripIndex = 0; stripIndex < numFireStrips; ++stripIndex)
		{
			FireStrip* fireStrip = &_fireRain.fireStrips[stripIndex];
			UINT numTiles = (UINT)fireStrip->tiles.size();
			for (UINT tileIndex = 0; tileIndex < numTiles; ++tileIndex)
			{
				int xPos = (int)fireStrip->headPos.x;
				int yPos = (int)(fireStrip->headPos.y - tileIndex * FireStrip::tileSize);
				int left = xPos - (int)(FireStrip::tileSize * 0.5f);
				int top = yPos - (int)(FireStrip::tileSize * 0.5f);
				int right = xPos + (int)(FireStrip::tileSize * 0.5f);
				int bottom = yPos + (int)(FireStrip::tileSize * 0.5f);
				HBRUSH brushOut = (HBRUSH)SelectObject(hdc, fireStrip->tiles[tileIndex].brush);
				Rectangle(hdc, left, top, right, bottom);
				SelectObject(hdc, brushOut);
			}
		}

		EndPaint(hWnd, &ps);
		return 0;

	default:

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}