#include "Pch.h"
#include "Application.h"

namespace CPPSnake
{
	Application* _app{};

	Application::~Application()
	{
		delete _snake;
		delete _grid;
		delete _gfxDevice;
		delete _appWindow;
		delete _timer;
	}

	Bool Application::initialize()
	{
		srand(timeGetTime());

		_timer = new Timer();
		if (!_timer->initialize()) return false;
		_appWindow = new ApplicationWindow();
		if (!_appWindow->initialize()) return false;
		_gfxDevice = new GfxDevice();
		if (!_gfxDevice->initialize()) return false;
		_grid = new Grid();
		if (!_grid->initialize(_gridSettings)) return false;
		_snake = new Snake();
		if (!_snake->initialize(_snakeSettings)) return false;

		ShowCursor(FALSE);

		return true;
	}

	Void Application::run()
	{
		SNK_ASSERT(_appWindow);

		MSG msg{};
		while (true)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else processFrame();
		}
	}

	Void Application::quit()
	{
		PostQuitMessage(0);
	}

	Void Application::processFrame()
	{
		_gfxDevice->clearColor(0x00000000);
		if (_gameState == GameState::Snaking)
		{
			_snake->update();
			_snake->draw();
			_grid->draw();
			drawScore();

			if (_snake->bitHisOwnTail())
			{
				_gameState = GameState::Reset;
				_resetSequence.start();
			}
		}
		else
		if (_gameState == GameState::Reset)
		{
			_snake->draw();
			_grid->draw();
			drawScore();
			_resetSequence.draw();

			if (_resetSequence.willStartToFadeIn())
			{
				_grid->initialize({});
				_snake->initialize({});
			}

			if (!_resetSequence.isPlaying()) _gameState = GameState::Snaking;
		}

		_gfxDevice->present();
	}

	Void Application::drawScore()
	{
		HDC backbufferDC = _gfxDevice->getBackbufferDC();
		std::string info = "Score: " + std::to_string(_snake->getNumFoodEaten());
		SetTextColor(backbufferDC, 0x00FFFFFF);
		TextOut(backbufferDC, 10, 10, info.c_str(), (UInt32)info.length());
	}
}