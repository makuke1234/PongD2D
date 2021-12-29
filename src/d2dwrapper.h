#ifndef D2DWRAPPER_H
#define D2DWRAPPER_H

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#ifdef __cplusplus
extern "C"
{
#endif

void dxSafeRelease(IUnknown ** ppInstance);

HRESULT dxD2D1CreateFactory(D2D1_FACTORY_TYPE type, void ** factory);
HRESULT dxDWriteCreateFactory(enum DWRITE_FACTORY_TYPE type, IUnknown ** factory);

HRESULT dxFactoryCreateHwndRenderTarget(
	ID2D1Factory * factory,
	D2D1_RENDER_TARGET_PROPERTIES props,
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps,
	ID2D1HwndRenderTarget ** ppRT
);

D2D1_RENDER_TARGET_PROPERTIES dxD2D1RenderTargetProperties(
	D2D1_RENDER_TARGET_TYPE type,
	D2D1_PIXEL_FORMAT pixelFormat,
	FLOAT dpiX,
	FLOAT dpiY,
	D2D1_RENDER_TARGET_USAGE usage,
	D2D1_FEATURE_LEVEL minLevel
);
D2D1_HWND_RENDER_TARGET_PROPERTIES dxD2D1HwndRenderTargetProperties(
	HWND hwnd,
	D2D1_SIZE_U pixelSize,
	D2D1_PRESENT_OPTIONS opt
);
D2D1_PIXEL_FORMAT dxD2D1PixelFormat(
	DXGI_FORMAT dxgiFormat,
	D2D1_ALPHA_MODE alphaMode
);

D2D1_MATRIX_3X2_F dxD2D1Matrix3x2FIdentity();

void dxFactoryGetDesktopDpi(ID2D1Factory * This, FLOAT * dpiX, FLOAT * dpiY);
void dxRTGetDpi(ID2D1RenderTarget * This, FLOAT * dpiX, FLOAT * dpiY);
void dxRTBeginDraw(ID2D1RenderTarget * This);
void dxRTSetTransform(ID2D1RenderTarget * This, D2D1_MATRIX_3X2_F identity);
void dxRTClear(ID2D1RenderTarget * This, D2D1_COLOR_F color);
HRESULT dxRTEndDraw(ID2D1RenderTarget * This);

HRESULT dxHwndRTResize(ID2D1HwndRenderTarget * This, D2D1_SIZE_U sz);

#ifdef __cplusplus
}
#endif

#endif
