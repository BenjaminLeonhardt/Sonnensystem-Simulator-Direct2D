#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <iostream>
#include <exception>
#include <Dwrite.h>

class Graphics{
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;
	ID2D1SolidColorBrush* brush;
	IDWriteFactory* writefactory;
	IDWriteTextFormat* tf;
public:
	Graphics();
	~Graphics();

	bool Init(HWND windowHandle);
	void BeginDraw() { renderTarget->BeginDraw(); }
	void EndDraw() { renderTarget->EndDraw(); }

	void ClearScreen(float r, float g, float b);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
	void DrawOval(float x, float y, float radius1, float radius2, float r, float g, float b, float a);
	void DrawRect(float x, float y, float height, float width, float r, float g, float b, float a);
	void DrawTextS(float x, float y, WCHAR *string);
};

