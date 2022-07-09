#include "ColorPalette.h"
#include "Random.h"
#include "Canvas.h"
#include <vector>

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
		fireStrips.resize(_canvas->getWidth() / FireStrip::tileSize + 1);
		UINT maxNumTiles = (UINT)((_canvas->getHeight() / FireStrip::tileSize) * 0.7f);

		UINT numStrips = (UINT)fireStrips.size();
		for (UINT stripIndex = 0; stripIndex < numStrips; ++stripIndex)
		{
			fireStrips[stripIndex].headPos = Vector2(stripIndex * (float)FireStrip::tileSize + (float)(FireStrip::tileSize * 0.5f), -(float)(FireStrip::tileSize * 0.5f));
			fireStrips[stripIndex].speed = Random::range(FireStrip::minSpeed, FireStrip::maxSpeed);

			UINT numTiles = Random::range(FireStrip::minNumTiles, maxNumTiles);
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
			if ((tailPosY - (FireStrip::tileSize * 0.5f) > _canvas->getHeight()))
				strip.headPos.y = -(float)(FireStrip::tileSize * 0.5f);
		}
	}

	std::vector<FireStrip> fireStrips{};
};

FireRain _fireRain{};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_canvas = new Canvas();
	if (!_canvas->initialize()) return 0;

	_timer.init();
	_fireRain.init();

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
			_canvas->clear();
			_fireRain.update();
			_canvas->present();
		}
	}

	delete _canvas;

	return (int)msg.wParam;
}