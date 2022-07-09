#include "Pch.h"
#include "Snake.h"
#include "Timer.h"
#include "Grid.h"
#include "ApplicationWindow.h"
#include "GfxDevice.h"

namespace CPPSnake
{
	Snake* _snake{};

	Bool Snake::initialize(const SnakeSettings& settings)
	{
		_settings = settings;
		if (_settings.moveTimeStep <= 0.0f) _settings.moveTimeStep = 0.12f;
		if (_settings.minMoveTimeStep <= 0.0f) _settings.minMoveTimeStep = 0.033f;
		if (_settings.minMoveTimeStep > _settings.moveTimeStep) _settings.minMoveTimeStep = _settings.moveTimeStep;

		_moveDirection = (Direction2D)(rand() % 4);
		_length = 1;

		_bodyCoords.numItems = 0;
		if (!_bodyCoords.capacity) _bodyCoords.grow(100);
		_bodyCoords.add({(Int32)((_grid->getNumCellsX() - 1) * 0.5f), (Int32)((_grid->getNumCellsY() - 1) * 0.5f) });
		_grid->setHasSnake(_bodyCoords[0].x, _bodyCoords[0].y, true);

		_timeSinceLastMove = _timer->getCurrentTime();
		_moveTimeStep = _settings.moveTimeStep;
		_bitHisOwnTail = false;
		_numFoodEaten = 0;

		return true;
	}

	Void Snake::update()
	{
		if (_bitHisOwnTail) return;

		Float currentTime = _timer->getCurrentTime();
		if ((currentTime - _timeSinceLastMove) >= _moveTimeStep)
		{
			_timeSinceLastMove = currentTime;

			_grid->setHasSnake(_bodyCoords[_length - 1].x, _bodyCoords[_length - 1].y, false);
			for (UInt32 i = _length - 1; i > 0; --i)
				_bodyCoords[i] = _bodyCoords[i - 1];

			switch (_moveDirection)
			{
			case Direction2D::Left:

				_bodyCoords[0].x -= 1;
				if (_bodyCoords[0].x < 0) _bodyCoords[0].x = _grid->getNumCellsX() - 1;
				break;

			case Direction2D::Up:

				_bodyCoords[0].y -= 1;
				if (_bodyCoords[0].y < 0) _bodyCoords[0].y = _grid->getNumCellsY() - 1;
				break;

			case Direction2D::Right:

				_bodyCoords[0].x += 1;
				if (_bodyCoords[0].x == _grid->getNumCellsX()) _bodyCoords[0].x = 0;
				break;

			case Direction2D::Down:

				_bodyCoords[0].y += 1;
				if (_bodyCoords[0].y == _grid->getNumCellsY()) _bodyCoords[0].y = 0;
				break;
			}
			if (_grid->hasSnake(_bodyCoords[0].x, _bodyCoords[0].y))
			{
				_bitHisOwnTail = true;
				return;
			}
			_grid->setHasSnake(_bodyCoords[0].x, _bodyCoords[0].y, true);

			if (_grid->hasFood((UInt32)_bodyCoords[0].x, (UInt32)_bodyCoords[0].y))
			{
				++_length;
				++_numFoodEaten;

				const UInt32 maxFoodEatenRef = 80;
				if (_numFoodEaten <= maxFoodEatenRef)
				{
					Float t = _numFoodEaten / (Float)maxFoodEatenRef;
					_moveTimeStep = _settings.moveTimeStep + t * (_settings.minMoveTimeStep - _settings.moveTimeStep);
				}

				if (_bodyCoords.capacity == _bodyCoords.numItems) _bodyCoords.grow(10);
				_bodyCoords.add(_bodyCoords[_length - 2]);

				if (_snake->getLength() != _grid->getTotalNumCells())
					_grid->generateFood();
			}
		}
	}

	Void Snake::draw()
	{
		BGRA32 headColor = _settings.headColor;
		BGRA32 tailColor = _settings.tailColor;

		Coord2I32 coord{};
		Coord2I32 topLeft{};

		for (UInt32 i = 1; i < _bodyCoords.numItems; ++i)
		{
			coord = _bodyCoords[i];
			topLeft = _grid->calcCellTopLeft((UInt32)coord.x, (UInt32)coord.y);
			_gfxDevice->drawSquare(topLeft, _grid->getCellSize(), BGRA32::lerp(headColor, tailColor, (i + 1) / (Float)(_bodyCoords.numItems)));
		}

		coord = _bodyCoords[0];
		topLeft = _grid->calcCellTopLeft((UInt32)coord.x, (UInt32)coord.y);
		_gfxDevice->drawSquare(topLeft, _grid->getCellSize(), headColor.value);
	}

	Void Snake::setMoveDirection(Direction2D direction)
	{
		if (_moveDirection == direction) return;

		if (_length > 1)
		{
			if (direction == Direction2D::Left && _moveDirection == Direction2D::Right) return;
			if (direction == Direction2D::Right && _moveDirection == Direction2D::Left) return;
			if (direction == Direction2D::Up && _moveDirection == Direction2D::Down) return;
			if (direction == Direction2D::Down && _moveDirection == Direction2D::Up) return;
		}

		_moveDirection = direction;
	}
}