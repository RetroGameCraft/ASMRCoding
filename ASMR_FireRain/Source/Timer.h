#pragma once
#include <windows.h>

class Timer
{
public:

	bool initialize();
	void tick();

	float getElapsedTime() const { return _elapsedTime; }
	float getAvgElapsedTime() const { return _avgElapsedTime; }

private:

	__int64 _freq{};
	__int64 _prevTime{};
	__int64 _currentTime{};

	static const UINT _maxNumSamples = 50;
	float _timeSamples[_maxNumSamples]{};
	UINT _numSamples{};

	float _toSeconds{};
	float _elapsedTime{};
	float _avgElapsedTime{};
};

extern Timer* _timer;