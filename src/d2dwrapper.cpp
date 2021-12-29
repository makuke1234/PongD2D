#include "d2dwrapper.h"

extern "C"
{

void dxSafeRelease(IUnknown ** ppInstance)
{
	if (ppInstance != nullptr && (*ppInstance) != nullptr) [[likely]]
	{
		(*ppInstance)->Release();
		(*ppInstance) = nullptr;
	}
}

HRESULT dxD2D1CreateFactory(D2D1_FACTORY_TYPE type, void ** factory)
{
	return ::D2D1CreateFactory(type, IID_ID2D1Factory, factory);
}

HRESULT dxDWriteCreateFactory(enum DWRITE_FACTORY_TYPE type, IUnknown ** factory)
{
	return ::DWriteCreateFactory(type, __uuidof(IDWriteFactory), factory);
}

HRESULT dxFactoryCreateHwndRenderTarget(
	ID2D1Factory * factory,
	D2D1_RENDER_TARGET_PROPERTIES props,
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps,
	ID2D1HwndRenderTarget ** ppRT
)
{
	return factory->CreateHwndRenderTarget(props, hwndProps, ppRT);
}

D2D1_RENDER_TARGET_PROPERTIES dxD2D1RenderTargetProperties(
	D2D1_RENDER_TARGET_TYPE type,
	D2D1_PIXEL_FORMAT pixelFormat,
	FLOAT dpiX,
	FLOAT dpiY,
	D2D1_RENDER_TARGET_USAGE usage,
	D2D1_FEATURE_LEVEL minLevel
)
{
	return D2D1::RenderTargetProperties(
		type,
		pixelFormat,
		dpiX,
		dpiY,
		usage,
		minLevel
	);
}
D2D1_HWND_RENDER_TARGET_PROPERTIES dxD2D1HwndRenderTargetProperties(
	HWND hwnd,
	D2D1_SIZE_U pixelSize,
	D2D1_PRESENT_OPTIONS opt
)
{
	return D2D1::HwndRenderTargetProperties(hwnd, pixelSize, opt);
}
D2D1_PIXEL_FORMAT dxD2D1PixelFormat(
	DXGI_FORMAT dxgiFormat,
	D2D1_ALPHA_MODE alphaMode
)
{
	return D2D1::PixelFormat(dxgiFormat, alphaMode);
}

D2D1_MATRIX_3X2_F dxD2D1Matrix3x2FIdentity()
{
	return D2D1::Matrix3x2F::Identity();
}


void dxFactoryGetDesktopDpi(ID2D1Factory * This, FLOAT * dpiX, FLOAT * dpiY)
{
	This->GetDesktopDpi(dpiX, dpiY);
}
void dxRTGetDpi(ID2D1RenderTarget * This, FLOAT * dpiX, FLOAT * dpiY)
{
	This->GetDpi(dpiX, dpiY);
}
void dxRTBeginDraw(ID2D1RenderTarget * This)
{
	This->BeginDraw();
}
void dxRTSetTransform(ID2D1RenderTarget * This, D2D1_MATRIX_3X2_F identity)
{
	This->SetTransform(identity);
}
void dxRTClear(ID2D1RenderTarget * This, D2D1_COLOR_F color)
{
	This->Clear(color);
}
HRESULT dxRTEndDraw(ID2D1RenderTarget * This)
{
	return This->EndDraw();
}

HRESULT dxHwndRTResize(ID2D1HwndRenderTarget * This, D2D1_SIZE_U sz)
{
	return This->Resize(sz);
}

}