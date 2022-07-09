#pragma once
#include "ApplicationWindow.h"
#include "GfxDevice.h"
#include "Grid.h"
#include "Snake.h"
#include "ResetSequence.h"

namespace CPPSnake
{
	enum class GameState
	{
		Snaking = 0,
		Reset
	};

	class Application
	{
	public:

		Application() {}
		~Application();

		Bool initialize();
		Void run();
		Void quit();

	private:

		Void processFrame();
		Void drawScore();

	private:

		GameState _gameState = GameState::Snaking;
		ResetSequence _resetSequence{};
		SnakeSettings _snakeSettings{};
		GridSettings _gridSettings{};
	};

	extern Application* _app;
}