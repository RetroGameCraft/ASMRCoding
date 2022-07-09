#pragma once
#include "Core.h"
#include <vector>

namespace CPPSnake
{
	struct SnakeSettings
	{	
		UInt32 headColor = 0x0000FF00;
		UInt32 tailColor = 0x00008000;
		Float moveTimeStep = 0.12f;
		Float minMoveTimeStep = 0.033f;
	};

	class Snake
	{
		friend class Application;

	private:

		Snake() {}
		~Snake() {}

		Bool initialize(const SnakeSettings& settings);

		Void update();
		Void draw();

	public:

		Void setMoveDirection(Direction2D direction);

		UInt64 getNumFoodEaten() const { return _numFoodEaten; }
		UInt32 getLength() const { return _length; }
		Bool bitHisOwnTail() const { return _bitHisOwnTail; }

	private:

		SnakeSettings _settings{};
		UInt32 _length = 1;
		Float _timeSinceLastMove{};
		Direction2D _moveDirection{};
		UInt64 _numFoodEaten{};
		Float _moveTimeStep{};
		Bool _bitHisOwnTail{};
		MallocBuffer<Coord2I32> _bodyCoords{};
	};

	extern Snake* _snake;
}