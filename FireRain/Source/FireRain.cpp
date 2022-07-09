#include "FireRain.h"
#include "Random.h"
#include "Timer.h"

FireRain* _fireRain{};

FireRain::~FireRain()
{
	if (_fireTilePen) DeleteObject(_fireTilePen);

	for (UINT i = 0; i < _numFireColors; ++i)
	{
		if (_fireBrushes[i]) DeleteObject(_fireBrushes[i]);
	}
}

bool FireRain::initialize()
{
	LOGPEN lp{};
	lp.lopnStyle = PS_NULL;
	_fireTilePen = CreatePenIndirect(&lp);
	if (!_fireTilePen) return false;

	LOGBRUSH lb{};
	lb.lbStyle = BS_SOLID;

	for (UINT i = 0; i < _numFireColors; ++i)
	{
		lb.lbColor = _fireColors[i];
		_fireBrushes[i] = CreateBrushIndirect(&lb);
		if (!_fireBrushes[i]) return false;
	}

	_fireStrips.resize(_canvas->getWidth() / _fireTileSize + 1);
	UINT maxNumFireTiles = (UINT)((_canvas->getHeight() / _fireTileSize) * 0.7f);
	if (maxNumFireTiles <= _minNumFireTiles) maxNumFireTiles = _minNumFireTiles + 1;

	UINT numStrips = (UINT)_fireStrips.size();
	for (UINT stripIndex = 0; stripIndex < numStrips; ++stripIndex)
	{
		_fireStrips[stripIndex].headPosX = stripIndex * (float)_fireTileSize + (float)(_fireTileSize * 0.5f);
		_fireStrips[stripIndex].headPosY = -(float)(_fireTileSize * 0.5f);
		_fireStrips[stripIndex].speed = Random::range(_minFallSpeed, _maxFallSpeed);

		UINT numTiles = Random::range(_minNumFireTiles, maxNumFireTiles);
		_fireStrips[stripIndex].tiles.resize(numTiles);
		for (UINT tileIndex = 0; tileIndex < numTiles; ++tileIndex)
		{
			UINT colorIndex = _numFireColors - 1 - (UINT)((tileIndex / (float)(numTiles - 1)) * (float)(_numFireColors - 1));
			_fireStrips[stripIndex].tiles[tileIndex] = _fireBrushes[colorIndex];
			if (!_fireStrips[stripIndex].tiles[tileIndex]) return false;
		}
	}

	return true;
}

void FireRain::update()
{
	for (auto& strip : _fireStrips)
	{
		strip.headPosY += strip.speed * _timer->getAvgElapsedTime();
		float tailPosY = strip.headPosY - (float)(((UINT)strip.tiles.size() - 1) * _fireTileSize);
		if ((tailPosY - (_fireTileSize * 0.5f) > _canvas->getHeight()))
			strip.headPosY = -(float)(_fireTileSize * 0.5f);
	}
}

void FireRain::draw()
{
	HDC hdc = _canvas->getDrawDC();
	HPEN penOut = (HPEN)SelectObject(hdc, _fireTilePen);
	int tileSizeAdd = _tileGapsEnabled ? 0 : 1;

	UINT numStrips = (UINT)_fireStrips.size();
	for (UINT stripIndex = 0; stripIndex < numStrips; ++stripIndex)
	{
		FireStrip* strip = &_fireStrips[stripIndex];
		UINT numTiles = (UINT)strip->tiles.size();
		for (UINT tileIndex = 0; tileIndex < numTiles; ++tileIndex)
		{
			float x = strip->headPosX;
			float y = strip->headPosY - tileIndex * (float)(_fireTileSize);
			int left = (int)(x - (float)(_fireTileSize * 0.5f));
			int right = left + _fireTileSize + tileSizeAdd;
			int top = (int)(y - (float)(_fireTileSize * 0.5f));
			int bottom = top + _fireTileSize + tileSizeAdd;

			HBRUSH brushOut = (HBRUSH)SelectObject(hdc, strip->tiles[tileIndex]);
			Rectangle(hdc, left, top, right, bottom);
			SelectObject(hdc, brushOut);
		}
	}

	SelectObject(hdc, penOut);
}