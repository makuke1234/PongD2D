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
	ID2D1Factory * This,
	D2D1_RENDER_TARGET_PROPERTIES props,
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps,
	ID2D1HwndRenderTarget ** ppRT
);

HRESULT dxFactoryCreateRectangleGeometry(
	ID2D1Factory * This,
	D2D1_RECT_F rectangle,
	ID2D1RectangleGeometry ** ppRectGeo
);
HRESULT dxFactoryCreateEllipseGeometry(
	ID2D1Factory * This,
	D2D1_ELLIPSE ellipse,
	ID2D1EllipseGeometry ** ppRectEllipse
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

HRESULT dxRTCreateSolidColorBrush(
	ID2D1RenderTarget * This,
	D2D1_COLOR_F color,
	ID2D1SolidColorBrush ** ppBrush
);
HRESULT dxRTCreateRadialGradientBrush(
	ID2D1RenderTarget * This,
	D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES props,
	ID2D1GradientStopCollection * gradientStops,
	ID2D1RadialGradientBrush ** ppBrush
);

D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES dxD2D1RadialGradientBrushProperties(
	D2D1_POINT_2F center,
	D2D1_POINT_2F originOffset,
	FLOAT radiusx,
	FLOAT radiusy
);
HRESULT dxRTCreateGradientStopCollection(
	ID2D1RenderTarget * This,
	const D2D1_GRADIENT_STOP * gradientStops,
	UINT32 gradientStopCount,
	D2D1_GAMMA colorInterpolationGamma,
	D2D1_EXTEND_MODE extendMode,
	ID2D1GradientStopCollection ** ppGradStopCollection
);

HRESULT dxRTCreateLinearGradientBrush(
	ID2D1RenderTarget * This,
	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES props,
	ID2D1GradientStopCollection * gradientStops,
	ID2D1LinearGradientBrush ** ppBrush
);

void dxRTDrawGeometry(
	ID2D1RenderTarget * This,
	ID2D1Geometry * geometry,
	ID2D1Brush * brush,
	FLOAT strokeWidth,
	ID2D1StrokeStyle * strokeStyle
);
void dxRTFillGeometry(
	ID2D1RenderTarget * This,
	ID2D1Geometry * geometry,
	ID2D1Brush * brush,
	ID2D1Brush * opacityBrush
);

void dxRTFillRectangle(
	ID2D1RenderTarget * This,
	D2D1_RECT_F rect,
	ID2D1Brush * brush
);
void dxRTFillEllipse(
	ID2D1RenderTarget * This,
	D2D1_ELLIPSE ellipse,
	ID2D1Brush * brush
);

void dxRTDrawLine(
	ID2D1RenderTarget * This,
	D2D1_POINT_2F point0,
	D2D1_POINT_2F point1,
	ID2D1Brush * brush,
	FLOAT strokeWidth,
	ID2D1StrokeStyle * strokeStyle
);


HRESULT dxHwndRTResize(ID2D1HwndRenderTarget * This, D2D1_SIZE_U sz);

void dxRadialGradBrushSetCenter(
	ID2D1RadialGradientBrush * This,
	D2D1_POINT_2F center
);

#ifdef __cplusplus
}
#endif

#endif
