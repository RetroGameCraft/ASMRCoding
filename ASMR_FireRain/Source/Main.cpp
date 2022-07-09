#include "ColorPalette.h"
#include "Random.h"
#include "Canvas.h"
#include <vector>
#include "Timer.h"
#include "FireRain.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_timer = new Timer();
	if (!_timer->initialize()) return 0;

	_canvas = new Canvas();
	if (!_canvas->initialize()) return 0;

	_fireRain = new FireRain();
	if (!_fireRain->initialize()) return 0;

	MSG msg{};
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			_timer->tick();
			_canvas->clear();
			_fireRain->update();
			_fireRain->draw();
			_canvas->present();
		}
	}

	delete _fireRain;
	delete _canvas;
	delete _timer;

	return (int)msg.wParam;
}