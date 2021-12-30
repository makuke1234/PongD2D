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
	ID2D1Factory * This,
	D2D1_RENDER_TARGET_PROPERTIES props,
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps,
	ID2D1HwndRenderTarget ** ppRT
)
{
	return This->CreateHwndRenderTarget(props, hwndProps, ppRT);
}

HRESULT dxFactoryCreateRectangleGeometry(
	ID2D1Factory * This,
	D2D1_RECT_F rectangle,
	ID2D1RectangleGeometry ** ppRectGeo
)
{
	return This->CreateRectangleGeometry(rectangle, ppRectGeo);
}
HRESULT dxFactoryCreateEllipseGeometry(
	ID2D1Factory * This,
	D2D1_ELLIPSE ellipse,
	ID2D1EllipseGeometry ** ppRectEllipse
)
{
	return This->CreateEllipseGeometry(ellipse, ppRectEllipse);
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
void dxRTGetDpi(const ID2D1RenderTarget * This, FLOAT * dpiX, FLOAT * dpiY)
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

HRESULT dxRTCreateSolidColorBrush(
	ID2D1RenderTarget * This,
	D2D1_COLOR_F color,
	ID2D1SolidColorBrush ** ppBrush
)
{
	return This->CreateSolidColorBrush(color, ppBrush);
}
HRESULT dxRTCreateRadialGradientBrush(
	ID2D1RenderTarget * This,
	D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES props,
	ID2D1GradientStopCollection * gradientStops,
	ID2D1RadialGradientBrush ** ppBrush
)
{
	return This->CreateRadialGradientBrush(props, gradientStops, ppBrush);
}

D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES dxD2D1RadialGradientBrushProperties(
	D2D1_POINT_2F center,
	D2D1_POINT_2F originOffset,
	FLOAT radiusx,
	FLOAT radiusy
)
{
	return D2D1::RadialGradientBrushProperties(center, originOffset, radiusx, radiusy);
}
HRESULT dxRTCreateGradientStopCollection(
	ID2D1RenderTarget * This,
	const D2D1_GRADIENT_STOP * gradientStops,
	UINT32 gradientStopCount,
	D2D1_GAMMA colorInterpolationGamma,
	D2D1_EXTEND_MODE extendMode,
	ID2D1GradientStopCollection ** ppGradStopCollection
)
{
	return This->CreateGradientStopCollection(
		gradientStops,
		gradientStopCount,
		colorInterpolationGamma,
		extendMode,
		ppGradStopCollection
	);
}

HRESULT dxRTCreateLinearGradientBrush(
	ID2D1RenderTarget * This,
	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES props,
	ID2D1GradientStopCollection * gradientStops,
	ID2D1LinearGradientBrush ** ppBrush
)
{
	return This->CreateLinearGradientBrush(props, gradientStops, ppBrush);
}

void dxRTDrawGeometry(
	ID2D1RenderTarget * This,
	ID2D1Geometry * geometry,
	ID2D1Brush * brush,
	FLOAT strokeWidth,
	ID2D1StrokeStyle * strokeStyle
)
{
	This->DrawGeometry(
		geometry,
		brush,
		strokeWidth,
		strokeStyle
	);
}
void dxRTFillGeometry(
	ID2D1RenderTarget * This,
	ID2D1Geometry * geometry,
	ID2D1Brush * brush,
	ID2D1Brush * opacityBrush
)
{
	This->FillGeometry(
		geometry,
		brush,
		opacityBrush
	);
}

void dxRTFillRectangle(
	ID2D1RenderTarget * This,
	D2D1_RECT_F rect,
	ID2D1Brush * brush
)
{
	This->FillRectangle(rect, brush);
}
void dxRTFillEllipse(
	ID2D1RenderTarget * This,
	D2D1_ELLIPSE ellipse,
	ID2D1Brush * brush
)
{
	This->FillEllipse(ellipse, brush);
}

void dxRTDrawLine(
	ID2D1RenderTarget * This,
	D2D1_POINT_2F point0,
	D2D1_POINT_2F point1,
	ID2D1Brush * brush,
	FLOAT strokeWidth,
	ID2D1StrokeStyle * strokeStyle
)
{
	This->DrawLine(point0, point1, brush, strokeWidth, strokeStyle);
}


HRESULT dxHwndRTResize(ID2D1HwndRenderTarget * This, D2D1_SIZE_U sz)
{
	return This->Resize(sz);
}

void dxRadialGradBrushSetCenter(
	ID2D1RadialGradientBrush * This,
	D2D1_POINT_2F center
)
{
	This->SetCenter(center);
}

HRESULT dxGeoCompareWithGeometry(
	const ID2D1Geometry * This,
	ID2D1Geometry * inputGeo,
	const D2D1_MATRIX_3X2_F * inputGeoTransform,
	D2D1_GEOMETRY_RELATION * relation
)
{
	return This->CompareWithGeometry(inputGeo, inputGeoTransform, relation);
}


}
