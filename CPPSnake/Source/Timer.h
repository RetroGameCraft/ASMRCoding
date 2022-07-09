#pragma once
#include "Core.h"
#include <windows.h>

namespace CPPSnake
{
	class Timer
	{
		friend class Application;

	private:

		Timer() {}
		~Timer() {}

		Bool initialize();

	public:

		Float getCurrentTime()
		{
			UInt64 ticks{};
			QueryPerformanceCounter((LARGE_INTEGER*)&ticks);
			return ticks * _toSeconds;
		}

	private:

		Float _toSeconds{};
	};

	extern Timer* _timer;
}