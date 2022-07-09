#include "Timer.h"

Timer* _timer{};

bool Timer::initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&_freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&_currentTime);
	_prevTime = _currentTime;
	_toSeconds = 1.0f / (float)_freq;

	return true;
}

void Timer::tick()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&_currentTime);
	_elapsedTime = (_currentTime - _prevTime) * _toSeconds;
	_prevTime = _currentTime;

	if (_elapsedTime < 1.0f)
	{
		if (_numSamples == _maxNumSamples)
		{
			memmove(&_timeSamples[1], &_timeSamples[0], sizeof(float) * (_maxNumSamples - 1));
			_timeSamples[0] = _elapsedTime;
		}
		else _timeSamples[_numSamples++] = _elapsedTime;
	}

	_avgElapsedTime = 0.0f;
	for (UINT i = 0; i < _numSamples; ++i)
		_avgElapsedTime += _timeSamples[i];

	_avgElapsedTime /= (float)_numSamples;
}
