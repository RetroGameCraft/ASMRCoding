#pragma once
#include "Core.h"

namespace CPPSnake
{
	class ResetSequence
	{
		friend class Application;

	private:

		ResetSequence() {}
		~ResetSequence() {}

		Void start();
		Void draw();

	public:

		Bool isPlaying() const { return _isPlaying; }
		Bool isFadingOut() const { return _isFadingOut; }
		Bool willStartToFadeIn() const { return _willStartToFadeIn; }

	private:

		Bool _isPlaying{};
		Bool _isFadingOut{};
		Bool _willStartToFadeIn{};
		Float _startTime{};
	};
}