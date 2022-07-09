#include "Pch.h"
#include "ResetSequence.h"
#include "Timer.h"
#include "Grid.h"
#include "ApplicationWindow.h"

namespace CPPSnake
{
	Void ResetSequence::start()
	{
		if (_isPlaying) return;

		_isPlaying = true;
		_isFadingOut = true;
		_startTime = _timer->getCurrentTime();
	}

	Void ResetSequence::draw()
	{
		if (!_isPlaying) return;

		const Float fadeDuration = 2.0f;
		BGRA32 startColor = BGRA32(0, 0, 0, 0);
		BGRA32 endColor = BGRA32(0, 0, 0, 255);
		UInt32 currentColor;

		if (_isFadingOut)
		{
			Float fadeTime = _timer->getCurrentTime() - _startTime;
			Float t = fadeTime / fadeDuration;
			if (t > 1.0f)
			{
				_isFadingOut = false;
				_willStartToFadeIn = true;
				t = 1.0f;
			}

			currentColor = BGRA32::lerp(startColor, endColor, t);
		}
		else
		{
			_willStartToFadeIn = false;

			Float fadeTime = _timer->getCurrentTime() - _startTime - fadeDuration;
			Float t = fadeTime / fadeDuration;
			if (t > 1.0f)
			{
				_isPlaying = false;
				t = 1.0f;
			}

			currentColor = BGRA32::lerp(endColor, startColor, t);
		}

		_gfxDevice->drawTransparentQuad({0, 0}, _appWindow->getClientWidth(), _appWindow->getClientHeight(), currentColor);
	}
}