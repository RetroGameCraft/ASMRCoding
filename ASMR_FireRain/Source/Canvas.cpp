#include "Canvas.h"

Canvas* _canvas{};

Canvas::~Canvas()
{
	if (_clearBrush) DeleteObject(_clearBrush);
	if (_clearPen) DeleteObject(_clearPen);

	if (_backbuffer)
	{
		SelectObject(_backbufferDC, _bmpSelectOut);
		DeleteObject(_backbuffer);
		DeleteDC(_backbufferDC);
	}

	if (_wndHandle) DestroyWindow(_wndHandle);
}

bool Canvas::initialize()
{
	WNDCLASS wc{};
	wc.lpfnWndProc = staticWindowProc;
	wc.lpszClassName = TEXT("FireRain Canvas");
	wc.style = CS_OWNDC;
	if (!RegisterClass(&wc)) return false;

	_width = GetSystemMetrics(SM_CXSCREEN);
	_height = GetSystemMetrics(SM_CYSCREEN);

	_wndHandle = CreateWindowEx(0, wc.lpszClassName, nullptr, WS_POPUP,
		0, 0, _width, _height, NULL, NULL, NULL, this);
	if (!_wndHandle) return false;

	ShowWindow(_wndHandle, SW_NORMAL);
	_clientDC = GetDC(_wndHandle);

	if (!createClearPen() || 
		!createClearBrush() || 
		!createBackbuffer()) return false;

	return true;
}

void Canvas::clear()
{
	HPEN penOut = (HPEN)SelectObject(_backbufferDC, _clearPen);
	HBRUSH brushOut = (HBRUSH)SelectObject(_backbufferDC, _clearBrush);
	Rectangle(_backbufferDC, 0, 0, _width, _height);
	SelectObject(_backbufferDC, penOut);
	SelectObject(_backbufferDC, brushOut);
}

void Canvas::present()
{
	BitBlt(_clientDC, 0, 0, _width, _height, _backbufferDC, 0, 0, SRCCOPY);
}

bool Canvas::createClearBrush()
{
	LOGBRUSH lb{};
	lb.lbColor = 0;
	lb.lbStyle = BS_SOLID;
	_clearBrush = CreateBrushIndirect(&lb);
	if (!_clearBrush) return false;

	return true;
}

bool Canvas::createClearPen()
{
	LOGPEN lp{};
	lp.lopnStyle = PS_NULL;
	_clearPen = CreatePenIndirect(&lp);
	if (!_clearPen) return false;

	return true;
}

bool Canvas::createBackbuffer()
{
	if (_backbuffer)
	{
		SelectObject(_backbufferDC, _bmpSelectOut);
		DeleteObject(_backbuffer);
		DeleteDC(_backbufferDC);
	}

	_backbufferDC = CreateCompatibleDC(_clientDC);
	if (!_backbufferDC) return false;

	_backbuffer = CreateCompatibleBitmap(_clientDC, _width, _height);
	if (!_backbuffer)
	{
		DeleteDC(_backbufferDC);
		return false;
	}

	_bmpSelectOut = (HBITMAP)SelectObject(_backbufferDC, _backbuffer);
	return true;
}

LRESULT Canvas::staticWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE) SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);

	Canvas* canvas = (Canvas*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (canvas) return canvas->windowProc(hWnd, msg, wParam, lParam);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Canvas::windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:

		_width = LOWORD(lParam);
		_height = HIWORD(lParam);
		return 0;

	case WM_KEYDOWN:

		if (wParam == VK_ESCAPE) SendMessage(hWnd, WM_CLOSE, 0, 0);
		return 0;

	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;

	default:

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
