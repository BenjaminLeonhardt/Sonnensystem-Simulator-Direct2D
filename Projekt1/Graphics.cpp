#include "Graphics.h"


using namespace std;

Graphics::Graphics(){
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;
}


Graphics::~Graphics(){
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
	if (brush) brush->Release();
	if (writefactory) writefactory->Release();
	if (tf)tf->Release();
}

bool Graphics::Init(HWND windowHandle) {
	
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	

	
	if (res != S_OK) { return false; }
	RECT rect;
	GetClientRect(windowHandle, &rect);

	factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget);

	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&writefactory)
	);

	if (res != S_OK) return false;

	res = renderTarget->CreateSolidColorBrush(D2D1::ColorF(0,0,0,0), &brush);
	

	res = writefactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16.0f, L"en-us", &tf);
	if (res != S_OK) return false;
	tf->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	return true;

}

void Graphics::ClearScreen(float r, float g, float b) {
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a) {
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	
	renderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush);
	
}

void Graphics::DrawOval(float x, float y, float radius1, float radius2, float r, float g, float b, float a) {
	brush->SetColor(D2D1::ColorF(r, g, b, a));

	renderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius1, radius2), brush);

}

void Graphics::DrawRect(float x, float y, float height, float width, float r, float g, float b, float a) {
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	D2D1_RECT_F* rect;
	rect = new D2D1_RECT_F();
	rect->bottom = y + height;
	rect->top = y;
	rect->left = x;
	rect->right = x + width;
	renderTarget->FillRectangle(rect, brush);
	brush->SetColor(D2D1::ColorF(0.2f, 0.5f, 0.6f, 10.0f));
	renderTarget->DrawRectangle(rect, brush,1.0f,0);
	delete rect;
}

void Graphics::DrawTextS(float x, float y, WCHAR *string) {
	brush->SetColor(D2D1::ColorF(0.2f, 0.5f, 0.6f, 10.0f));
	
	D2D1_RECT_F* rect;
	rect = new D2D1_RECT_F();
	rect->bottom=y+20;
	rect->top=y+36;
	rect->left=x-700;
	rect->right=x+700;

	renderTarget->DrawTextA(string, wcslen(string), tf, rect, brush, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
	delete rect;

}

