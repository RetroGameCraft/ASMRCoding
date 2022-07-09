#include "FireRain.h"

FireRain* _fireRain{};

FireRain::~FireRain()
{
	for (UINT i = 0; i < _numFireColors; ++i)
	{
		if (_fireBrushes[i]) DeleteObject(_fireBrushes[i]);
	}
}

bool FireRain::initialize()
{
	LOGBRUSH lb{};
	lb.lbStyle = BS_SOLID;

	for (UINT i = 0; i < _numFireColors; ++i)
	{
		lb.lbColor = _fireColors[i];
		_fireBrushes[i] = CreateBrushIndirect(&lb);
		if (!_fireBrushes[i]) return false;
	}
}

void FireRain::update()
{
}

void FireRain::draw()
{
}
