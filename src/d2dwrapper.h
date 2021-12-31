#ifndef D2DWRAPPER_H
#define D2DWRAPPER_H

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Safely release DirectX object
 * 
 * @param ppInstance Address of a pointer of Isomething DX object down-casted to
 * address of a pointer of IUnknown base class
 */
void dxSafeRelease(IUnknown ** ppInstance);

/**
 * @brief Creates Direct2D factory that can be used to create D2D resources
 * 
 * @param type Threading model of the factory and the resources it creates
 * @param factory Address of pointer to factory object
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dxD2D1CreateFactory(D2D1_FACTORY_TYPE type, void ** factory);
/**
 * @brief Creates DirectWrite render factory
 * 
 * @param type Value that specifies whether the factory object will be shared or isolated
 * @param factory Address of a pointer to factory object
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dxDWriteCreateFactory(enum DWRITE_FACTORY_TYPE type, IUnknown ** factory);

/**
 * @brief Creates HWND render target
 * 
 * @param This Pointer to D2D factory
 * @param props Rendering mode, pixel format, remoting options, DPI info,
 * minimum DirectX feature level
 * @param hwndProps Window handle, initial size in pixels, present options
 * @param ppRT Address of a pointer to HWND render target object
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dxFactoryCreateHwndRenderTarget(
	ID2D1Factory * This,
	D2D1_RENDER_TARGET_PROPERTIES props,
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps,
	ID2D1HwndRenderTarget ** ppRT
);

/**
 * @brief Creates rectangle geometry object
 * 
 * @param This Pointer to D2D factory
 * @param rectangle Coordinates of the rectangle geometry
 * @param ppRectGeo Address of a pointer to rectangle geometry object
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dxFactoryCreateRectangleGeometry(
	ID2D1Factory * This,
	D2D1_RECT_F rectangle,
	ID2D1RectangleGeometry ** ppRectGeo
);
/**
 * @brief Creates ellipse geometry object
 * 
 * @param This Pointer to D2D factory
 * @param ellipse Value that describes ellipse geometry parameters
 * @param ppRectEllipse Address of a pointer to ellipse geometry object
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dxFactoryCreateEllipseGeometry(
	ID2D1Factory * This,
	D2D1_ELLIPSE ellipse,
	ID2D1EllipseGeometry ** ppRectEllipse
);

/**
 * @brief Creates D2D render target properties object
 * 
 * @param type Value that spcifies whether the render target must use hardware
 * rendering of software rendering
 * @param pixelFormat Pixel format and alpha mode for the render target
 * @param dpiX Horizontal DPI of the render target
 * @param dpiY Vertical DPI of the render target
 * @param usage Specifies how the render target is remotely rendered and whether
 * it should be GDI-compatible
 * @param minLevel Minimum Direct3D feature level required for hardware rendering
 * @return D2D1_RENDER_TARGET_PROPERTIES Render target properties object
 */
D2D1_RENDER_TARGET_PROPERTIES dxD2D1RenderTargetProperties(
	D2D1_RENDER_TARGET_TYPE type,
	D2D1_PIXEL_FORMAT pixelFormat,
	FLOAT dpiX,
	FLOAT dpiY,
	D2D1_RENDER_TARGET_USAGE usage,
	D2D1_FEATURE_LEVEL minLevel
);
/**
 * @brief Creates D2D HWND render target properties object
 * 
 * @param hwnd Window handle
 * @param pixelSize Size of render target in pixels
 * @param opt D2D presenting options
 * @return D2D1_HWND_RENDER_TARGET_PROPERTIES HWND render target properties object
 */
D2D1_HWND_RENDER_TARGET_PROPERTIES dxD2D1HwndRenderTargetProperties(
	HWND hwnd,
	D2D1_SIZE_U pixelSize,
	D2D1_PRESENT_OPTIONS opt
);
/**
 * @brief Creates D2D pixel format object
 * 
 * @param dxgiFormat Value that specifies the size and arrangement of channels in each pixel
 * @param alphaMode Alpha-channel mode
 * @return D2D1_PIXEL_FORMAT Pixel format object for bitmap or render target
 */
D2D1_PIXEL_FORMAT dxD2D1PixelFormat(
	DXGI_FORMAT dxgiFormat,
	D2D1_ALPHA_MODE alphaMode
);

/**
 * @return An identity matrix
 */
D2D1_MATRIX_3X2_F dxD2D1Matrix3x2FIdentity();

/**
 * @brief Get desktop DPI settings from D2D factory
 * 
 * @param This Pointer to D2D factory object
 * @param dpiX Pointer to floating-point horizontal DPI of the desktop, must be pre-allocated
 * @param dpiY Pointer to floating-point vertical DPI of the desktop, must be pre-allocated
 */
void dxFactoryGetDesktopDpi(ID2D1Factory * This, FLOAT * dpiX, FLOAT * dpiY);
/**
 * @brief Returns D2D render target's DPI
 * 
 * @param This Pointer to D2D render target object
 * @param dpiX Pointer to floating-point horizontal DPI of the desktop, must be pre-allocated
 * @param dpiY Pointer to floating-point vertical DPI of the desktop, must be pre-allocated
 */
void dxRTGetDpi(const ID2D1RenderTarget * This, FLOAT * dpiX, FLOAT * dpiY);
/**
 * @brief Initiates drawing on this render target
 * 
 * @param This Pointer to D2D render target
 */
void dxRTBeginDraw(ID2D1RenderTarget * This);
/**
 * @brief Applies the specified transform to the render target
 * 
 * @param This Pointer to the D2D render target
 * @param transform The transform to apply to the target
 */
void dxRTSetTransform(ID2D1RenderTarget * This, D2D1_MATRIX_3X2_F transform);
/**
 * @brief Clears the drawing area to the specified color
 * 
 * @param This Pointer to the D2D render target
 * @param color The color to which the drawing area is cleared
 */
void dxRTClear(ID2D1RenderTarget * This, D2D1_COLOR_F color);
/**
 * @brief Ends drawing operations on the render target and indicates the current
 * error state and 
 * 
 * @param This Pointer to the D2D render target
 * @param tag1 Contains the tag for drawing operations that caused errors,
 * 0 if there were no errors, can be NULL
 * @param tag2 Contains the tag for drawing operations that caused errors,
 * 0 if there were no errors, can be NULL
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dxRTEndDraw(ID2D1RenderTarget * This, D2D1_TAG * tag1, D2D1_TAG * tag2);

/**
 * @brief Creates new D2D solid color brush
 * 
 * @param This Pointer to the D2D render target
 * @param color RGBA color of the brush
 * @param ppBrush Address of a pointer to the brush
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dxRTCreateSolidColorBrush(
	ID2D1RenderTarget * This,
	D2D1_COLOR_F color,
	ID2D1SolidColorBrush ** ppBrush
);
/**
 * @brief Creates new D2D radial gradient brush
 * 
 * @param This Pointer to the D2D render target
 * @param props The center, gradient origin offset, xy-radius of the gradient
 * @param gradientStops A collection of D2D1_GRADIENT_STOP structures that describe
 * the colors in the gradient and their locations along the gradient
 * @param ppBrush Address of a pointer to the brush
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dxRTCreateRadialGradientBrush(
	ID2D1RenderTarget * This,
	D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES props,
	ID2D1GradientStopCollection * gradientStops,
	ID2D1RadialGradientBrush ** ppBrush
);

/**
 * @brief Creates radial gradient brush properties structure
 * 
 * @param center In brush's coordinate space, the center of gradient ellipse
 * @param originOffset In brush's coordinate space, the offset of gradient
 * origin relative to the center
 * @param radiusx In brush's coordinate space, the horizontal radius of the gradient ellipse
 * @param radiusy In brush's coordinate space, the vertical radius of the gradient ellipse
 * @return D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES Properties structure
 */
D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES dxD2D1RadialGradientBrushProperties(
	D2D1_POINT_2F center,
	D2D1_POINT_2F originOffset,
	FLOAT radiusx,
	FLOAT radiusy
);
/**
 * @brief Creates D2D gradient stop collection object
 * 
 * @param This Pointer to D2D render target
 * @param gradientStops A pointer to an array of D2D1_GRADIENT_STOP structures
 * @param gradientStopCount Number of gradient stops
 * @param colorInterpolationGamma The space in which color interpolation between the
 * gradient stops it performed
 * @param extendMode The behaviour of the gradient outside the gradient range
 * @param ppGradStopCollection Address to a pointer of gradient stop collection object
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dxRTCreateGradientStopCollection(
	ID2D1RenderTarget * This,
	const D2D1_GRADIENT_STOP * gradientStops,
	UINT32 gradientStopCount,
	D2D1_GAMMA colorInterpolationGamma,
	D2D1_EXTEND_MODE extendMode,
	ID2D1GradientStopCollection ** ppGradStopCollection
);

/**
 * @brief Crates D2D linear gradient brush
 * 
 * @param This Pointer to D2D render target
 * @param props The start and end points of the gradient
 * @param gradientStops A collection of D2D1_GRADIENT_STOP structures that describe
 * the colors in the gradient and their locations along the gradient
 * @param ppBrush Address of a pointer to D2D linear gradient brush object
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dxRTCreateLinearGradientBrush(
	ID2D1RenderTarget * This,
	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES props,
	ID2D1GradientStopCollection * gradientStops,
	ID2D1LinearGradientBrush ** ppBrush
);

/**
 * @brief Draws a D2D geometry object outline on D2D render target
 * 
 * @param This Pointer to D2D render target
 * @param geometry Pointer to D2D geometry object to be drawn
 * @param brush Pointer to D2D brush to be drawn with
 * @param strokeWidth Line width
 * @param strokeStyle D2D stroke style, NULL if no style should be applied
 */
void dxRTDrawGeometry(
	ID2D1RenderTarget * This,
	ID2D1Geometry * geometry,
	ID2D1Brush * brush,
	FLOAT strokeWidth,
	ID2D1StrokeStyle * strokeStyle
);
/**
 * @brief Draws a filled D2D geometry object on D2D render target
 * 
 * @param This Pointer to D2D render target
 * @param geometry Pointer to D2D geometry object to be drawn
 * @param brush Pointer to D2D brush to be drawn with
 * @param opacityBrush Pointer to D2D brush that should determine the opacity
 * of the drawn geometry, can be NULL, if no opacity mask should be applied
 */
void dxRTFillGeometry(
	ID2D1RenderTarget * This,
	ID2D1Geometry * geometry,
	ID2D1Brush * brush,
	ID2D1Brush * opacityBrush
);

/**
 * @brief Draws a filled rectangle on D2D render target
 * 
 * @param This Pointer to D2D render target
 * @param rect Coordinates of the rectangle to be drawn
 * @param brush Pointer to D2D brush to be drawn with
 */
void dxRTFillRectangle(
	ID2D1RenderTarget * This,
	D2D1_RECT_F rect,
	ID2D1Brush * brush
);
/**
 * @brief Draws a filled ellipse on D2D render target
 * 
 * @param This Pointer to D2D render target
 * @param ellipse Value that describes ellipse parameters
 * @param brush Pointer to D2D brush to be drawn with
 */
void dxRTFillEllipse(
	ID2D1RenderTarget * This,
	D2D1_ELLIPSE ellipse,
	ID2D1Brush * brush
);

/**
 * @brief Draws a line on D2D render target
 * 
 * @param This Pointer to D2D render target
 * @param point0 Start-point of the line
 * @param point1 End-point of the line
 * @param brush Pointer to D2D brush to be drawn with
 * @param strokeWidth Line width
 * @param strokeStyle D2D stroke style, NULL if no style should be applied
 */
void dxRTDrawLine(
	ID2D1RenderTarget * This,
	D2D1_POINT_2F point0,
	D2D1_POINT_2F point1,
	ID2D1Brush * brush,
	FLOAT strokeWidth,
	ID2D1StrokeStyle * strokeStyle
);

/**
 * @brief Resizes D2D HWND render target
 * 
 * @param This Pointer to D2D HWND render target
 * @param sz New size of HWND render target in pixels
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dxHwndRTResize(ID2D1HwndRenderTarget * This, D2D1_SIZE_U sz);

/**
 * @brief Sets new center coordinates of radial gradient brush 
 * 
 * @param This Pointer to D2D radial gradient brush object
 * @param center New center coordinates in brush's coordinate space of the brush
 */
void dxRadialGradBrushSetCenter(
	ID2D1RadialGradientBrush * This,
	D2D1_POINT_2F center
);

/**
 * @brief Compares 2 D2D geometries with each other
 * 
 * @param This Pointer to D2D geometry object to be compared
 * @param inputGeo Pointer to the second D2D geometry object ot be compared against
 * @param inputGeoTransform A transform to be applied to the second geometry object,
 * NULL if no transform should be applied
 * @param relation Pointer to D2D1_GEOMETRY_RELATION holding the result of the comparison
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dxGeoCompareWithGeometry(
	const ID2D1Geometry * This,
	ID2D1Geometry * inputGeo,
	const D2D1_MATRIX_3X2_F * inputGeoTransform,
	D2D1_GEOMETRY_RELATION * relation
);

#ifdef __cplusplus
}
#endif

#endif
