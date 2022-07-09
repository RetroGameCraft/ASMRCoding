#pragma once
#include "Core.h"
#include "ApplicationWindow.h"

namespace CPPSnake
{
	class GfxDevice : public IApplicationWindowListener
	{
		friend class Application;

	private:

		GfxDevice();
		~GfxDevice();

		Bool initialize();

	public:

		Void clearColor(UInt32 color);
		Void present();

		Void drawSquare(const Coord2I32& topLeft, UInt32 size, UInt32 color);
		Void drawTransparentQuad(const Coord2I32& topLeft, UInt32 width, UInt32 height, UInt32 color);

		UInt32* getColorBuffer() { return _colorBuffer; }
		UInt32 getBufferWidth() const { return _bufferWidth; }
		UInt32 getBufferHeight() const { return _bufferHeight; }
		HDC getBackbufferDC() const { return _backbufferDC; }

	private:

		Bool createBackbuffer();
		Void onAppWindowResized(const Size2UI32& newClientSize, Bool isMinimized) override;

	private:

		HBITMAP _backbuffer = NULL;
		HBITMAP _backbufferOut = NULL;
		HDC _backbufferDC = NULL;
		HDC _wndClientDC = NULL;

		UInt32* _colorBuffer{};
		UInt32 _bufferWidth{};
		UInt32 _bufferHeight{};
	};

	extern GfxDevice* _gfxDevice;
}