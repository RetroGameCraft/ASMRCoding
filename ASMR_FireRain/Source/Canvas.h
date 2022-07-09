#pragma once
#include <windows.h>

class Canvas
{
public:

	Canvas(){}
	~Canvas();

	bool initialize();
	void clear();
	void present();

	UINT getWidth() const { return _width; }
	UINT getHeight() const { return _height; }

private:

	bool createClearBrush();
	bool createClearPen();
	bool createBackbuffer();

	static LRESULT CALLBACK staticWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

	HWND _wndHandle{};
	HDC _clientDC{};
	HBITMAP _bmpSelectOut{};
	HBITMAP _backbuffer{};
	HDC _backbufferDC{};
	HBRUSH _clearBrush{};
	HPEN _clearPen{};

	UINT _width{};
	UINT _height{};
};

extern Canvas* _canvas;