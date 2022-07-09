#include "ColorPalette.h"
#include "Random.h"
#include "Canvas.h"
#include <vector>
#include "Timer.h"
#include "FireRain.h"

struct FireStrip
{
	float headPosX{};
	float headPosY{};
	//std::vector<FireTile> tiles{};
	float speed;

	inline static const float minSpeed = 200.0f;
	inline static const float maxSpeed = 300.0f;
	inline static const UINT minNumTiles = 20;
	inline static const UINT tileSize = 32;
};

/*
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
			strip.headPos.y += strip.speed * _timer->elapsedTime;
			float tailPosY = strip.headPos.y - (float)(((UINT)strip.tiles.size() - 1) * FireStrip::tileSize);
			if ((tailPosY - (FireStrip::tileSize * 0.5f) > _canvas->getHeight()))
				strip.headPos.y = -(float)(FireStrip::tileSize * 0.5f);
		}
	}

	std::vector<FireStrip> fireStrips{};
};*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_timer = new Timer();
	if (!_timer->initialize()) return 0;

	_canvas = new Canvas();
	if (!_canvas->initialize()) return 0;

	_fireRain = new FireRain();
	if (!_fireRain->initialize()) return 0;

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
			_timer->tick();
			_canvas->clear();
			_fireRain->update();
			_fireRain->draw();
			_canvas->present();
		}
	}

	delete _fireRain;
	delete _canvas;
	delete _timer;

	return (int)msg.wParam;
}