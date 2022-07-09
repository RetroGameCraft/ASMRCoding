#pragma once
#include "Canvas.h"
#include "Timer.h"
#include <vector>

class FireRain
{
public:

	~FireRain();

	bool initialize();
	void update();
	void draw();
	
	void setTileGapsEnabled(bool enabled) { _tileGapsEnabled = enabled; }

private:

	struct FireStrip
	{
		float headPosX{};
		float headPosY{};
		float speed{};
		std::vector<HBRUSH> tiles{};
	};

	const UINT _numFireColors = 37;
	const COLORREF _fireColors[37] =
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
	HBRUSH _fireBrushes[37]{};
	HPEN _fireTilePen{};
	bool _tileGapsEnabled{};
	std::vector<FireStrip> _fireStrips{};

	inline static const float _minFallSpeed = 200.0f;
	inline static const float _maxFallSpeed = 300.0f;
	inline static const UINT _minNumFireTiles = 20;
	inline static const UINT _fireTileSize = 10;
};

extern FireRain* _fireRain;