#include "Pch.h"
#include "Application.h"
using namespace CPPSnake;

Int32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, Int32 nCmdShow)
{
	_app = new Application();
	if (!_app->initialize())
	{
		MessageBox(NULL, TEXT("Failed to initialize application. The program will now exit."), TEXT("Error"), MB_OK | MB_ICONERROR);
		delete _app;
		return 0;
	}

	_app->run();
	delete _app;

	return 0;
}