#include "window.h"
#include "resource.h"
#include "pongerr.h"

#include <stdlib.h>

bool g_pongClassRegistered = false;

bool PongWnd_createAssets(PongWnd_t * restrict pong)
{
	if (pong == NULL)
	{
		return false;
	}
	else if (pong->dx.assetsCreated == true)
	{
		return true;
	}

	// Create assets here
	HRESULT hr = dxFactoryCreateHwndRenderTarget(
		pong->dx.factory,
		dxD2D1RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			dxD2D1PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_UNKNOWN),
			0.0F,
			0.0F,
			D2D1_RENDER_TARGET_USAGE_NONE,
			D2D1_FEATURE_LEVEL_DEFAULT
		),
		dxD2D1HwndRenderTargetProperties(
			pong->hwnd,
			(D2D1_SIZE_U){ .width = (UINT32)pong->size.cx, .height = (UINT32)pong->size.cy },
			D2D1_PRESENT_OPTIONS_NONE
		),
		&pong->dx.pRT
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxRT;
		return false;
	}

	// Create Brushes
	hr = dxRTCreateSolidColorBrush(
		(ID2D1RenderTarget *)pong->dx.pRT,
		(D2D1_COLOR_F){ .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f },
		&pong->dx.pWhiteBrush
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxBrush;
		return false;
	}

	D2D1_GRADIENT_STOP stops[2];
	stops[0].color = (D2D1_COLOR_F){ .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
	stops[0].position = 0.0f,
	stops[1].color = (D2D1_COLOR_F){ .r = 0.9f, .g = 0.1f, .b = 0.1f, .a = 1.0f };
	stops[1].position = 1.0f;


	hr = dxRTCreateGradientStopCollection(
		(ID2D1RenderTarget *)pong->dx.pRT,
		stops,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&pong->dx.pGradStops
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxGradient;
		return false;
	}

	hr = dxRTCreateRadialGradientBrush(
		(ID2D1RenderTarget *)pong->dx.pRT,
		dxD2D1RadialGradientBrushProperties(
			(D2D1_POINT_2F){ .x = PONG_BALL_X, .y = PONG_BALL_Y },
			(D2D1_POINT_2F){ .x = 0.0f, .y = 0.0f },
			PONG_BALL_X, PONG_BALL_Y
		),
		pong->dx.pGradStops,
		&pong->dx.pBallBrush
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxBrush;
		return false;
	}

	// Create 2 rectangles to represent the bouncing surface
	hr = dxFactoryCreateRectangleGeometry(
		pong->dx.factory,
		(D2D1_RECT_F){
			.left   = 0.0f,
			.top    = (PONG_MINH - PONG_WALL_Y) / 2.0f + pong->dx.rightWallRelPos,
			.right  = PONG_WALL_X,
			.bottom = (PONG_MINH + PONG_WALL_Y) / 2.0f + pong->dx.rightWallRelPos
		},
		&pong->dx.pLeftWallGeo
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxGeo;
		return false;
	}

	hr = dxFactoryCreateRectangleGeometry(
		pong->dx.factory,
		(D2D1_RECT_F){
			.left   = (PONG_MINW - PONG_WALL_X),
			.top    = (PONG_MINH - PONG_WALL_Y) / 2.0f + pong->dx.rightWallRelPos,
			.right  = PONG_MINW,
			.bottom = (PONG_MINH + PONG_WALL_Y) / 2.0f + pong->dx.rightWallRelPos
		},
		&pong->dx.pRightWallGeo
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxGeo;
		return false;
	}

	// Create 1 circle to represent the ball
	hr = dxFactoryCreateEllipseGeometry(
		pong->dx.factory,
		(D2D1_ELLIPSE){
			.point = {
				.x = PONG_MINW / 2.0f + pong->dx.ballRelPos.x,
				.y = PONG_MINH / 2.0f + pong->dx.ballRelPos.y
			},
			.radiusX = PONG_BALL_X,
			.radiusY = PONG_BALL_Y
		},
		&pong->dx.pBallGeo
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxGeo;
		return false;
	}

	// return successfully
	pong->dx.assetsCreated = true;
	return true;
}
void PongWnd_destroyAssets(PongWnd_t * restrict pong)
{
	if (pong == NULL || pong->dx.assetsCreated == false)
	{
		return;
	}
	pong->dx.assetsCreated = false;

	// Destroy assets here

	// Geometries
	dxSafeRelease((IUnknown **)&pong->dx.pBallGeo);
	dxSafeRelease((IUnknown **)&pong->dx.pRightWallGeo);
	dxSafeRelease((IUnknown **)&pong->dx.pLeftWallGeo);

	// Brushes
	dxSafeRelease((IUnknown **)&pong->dx.pBallBrush);
	dxSafeRelease((IUnknown **)&pong->dx.pWhiteBrush);

	// Gradient stops
	dxSafeRelease((IUnknown **)&pong->dx.pGradStops);

	// Hwnd render target
	dxSafeRelease((IUnknown **)&pong->dx.pRT);
}
void PongWnd_updateAssets(PongWnd_t * restrict pong)
{
	if (pong == NULL || pong->dx.assetsCreated == false)
	{
		return;
	}

	// Update wall positions

	// Update ball position


}

bool PongWnd_create(PongWnd_t * restrict pong, HINSTANCE hInst, PWSTR lpCmdArgs, int nCmdShow)
{
	if (pong == NULL)
	{
		return false;
	}

	// Clear memory first and initialise some members
	*pong = (PongWnd_t){
		.hInst     = hInst,
		.lpCmdArgs = lpCmdArgs,
		.nCmdShow  = nCmdShow
	};

	// init d2d factory
	HRESULT hr = dxD2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, (void **)&pong->dx.factory);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxFactory;
		return false;
	}

	hr = dxDWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, &pong->dx.wFactory);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dwFactory;
		return false;
	}

	// try to register class
	if (g_pongClassRegistered == false)
	{
		g_pongClassRegistered = w32_regClass(
			pong->hInst,
			PONG_CLASSNAME,
			&PongWnd_winProcHub,
			CreateSolidBrush(RGB(240, 240, 240))
		);
	}

	if (g_pongClassRegistered == false)
	{
		g_pongLastError = PongErr_windowClass;
		return false;
	}

	// Get DPI
	dxFactoryGetDesktopDpi(pong->dx.factory, &pong->dpiX, &pong->dpiY);
	
	// try to create a window
	pong->hwnd = CreateWindowExW(
		0,
		PONG_CLASSNAME,
		PONG_APPNAME,
		WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_SIZEBOX),
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInst,
		pong
	);
	if (pong->hwnd == NULL)
	{
		g_pongLastError = PongErr_window;
		return false;
	}

	PongWnd_calcDpiSpecific(pong);

	SetWindowPos(
		pong->hwnd,
		NULL,
		0,
		0,
		pong->minSize.cx,
		pong->minSize.cy,
		SWP_NOZORDER | SWP_NOMOVE
	);

	if (PongWnd_createAssets(pong) == false)
	{
		return false;
	}

	ShowWindow(pong->hwnd, nCmdShow);
	UpdateWindow(pong->hwnd);

	return true;
}
bool PongWnd_setTitle(PongWnd_t * restrict pong, LPCWSTR title)
{
	if (pong == NULL || title == NULL || pong->hwnd == NULL)
	{
		return false;
	}

	size_t len = wcslen(title) + 1;
	pong->wndTitle = realloc(pong->wndTitle, sizeof(wchar_t) * len);

	if (pong->wndTitle == NULL)
	{
		return false;
	}

	memcpy(pong->wndTitle, title, sizeof(wchar_t) * len);

	return SetWindowTextW(pong->hwnd, pong->wndTitle) != 0;
}
const wchar_t * PongWnd_getTitle(const PongWnd_t * restrict pong)
{
	if (pong == NULL || pong->wndTitle == NULL)
	{
		return PONG_APPNAME;
	}
	else
	{
		return pong->wndTitle;
	}
}
int PongWnd_msgLoop(const PongWnd_t * pong)
{
	MSG msg;
	BOOL br;
	while ((br = GetMessageW(&msg, NULL, 0, 0)) != 0)
	{
		if (br == -1)
		{
			pongErr(pong, PongErr_unknown);
			return -1;
		}
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return (int)msg.wParam;
}
void PongWnd_free(PongWnd_t * restrict pong)
{
	if (pong->wndTitle != NULL)
	{
		free(pong->wndTitle);
		pong->wndTitle = NULL;
	}
	PongWnd_destroyAssets(pong);
	dxSafeRelease(&pong->dx.wFactory);
	dxSafeRelease((IUnknown **)&pong->dx.factory);
}

FLOAT PongWnd_dipx(const PongWnd_t * restrict pong, FLOAT x)
{
	return x * pong->dpiX / 96.0F;
}
FLOAT PongWnd_dipy(const PongWnd_t * restrict pong, FLOAT y)
{
	return y * pong->dpiY / 96.0F;
}
FLOAT PongWnd_dpix(const PongWnd_t * restrict pong, FLOAT x)
{
	return x * 96.0F / pong->dpiX;
}
FLOAT PongWnd_dpiy(const PongWnd_t * restrict pong, FLOAT y)
{
	return y * 96.0F / pong->dpiY;
}


void PongWnd_calcDpiSpecific(PongWnd_t * restrict pong)
{
	RECT r, cr;
	GetWindowRect(pong->hwnd, &r);
	GetClientRect(pong->hwnd, &cr);
	pong->border.cx = (r.right  - r.left) - (cr.right  - cr.left);
	pong->border.cy = (r.bottom - r.top)  - (cr.bottom - cr.top);

	pong->minSize.cx = (LONG)PongWnd_dipx(pong, PONG_MINW) + pong->border.cx + 1;
	pong->minSize.cy = (LONG)PongWnd_dipy(pong, PONG_MINH) + pong->border.cy + 1;
}

LRESULT CALLBACK PongWnd_winProcHub(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	PongWnd_t * self = NULL;
	if (msg == WM_NCCREATE)
	{
		LPCREATESTRUCTW lpcs = (LPCREATESTRUCTW)lp;
		self = lpcs->lpCreateParams;
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)self);
	}
	else
	{
		self = (PongWnd_t *)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
	}

	if (self != NULL)
	{
		return PongWnd_winProc(self, hwnd, msg, wp, lp);
	}
	else
	{
		return DefWindowProc(hwnd, msg, wp, lp);
	}
}
LRESULT PongWnd_winProc(PongWnd_t * pong, HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_PAINT:
		PongWnd_onRender(pong);
		break;
	case WM_SIZING:
		PongWnd_onSizing(pong, wp, lp);
		break;
	case WM_SIZE:
		PongWnd_onSize(pong, lp);
		break;
	case WM_DPICHANGED:
		PongWnd_onDpiChanged(pong, lp);
		break;
	case WM_CREATE:

		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		pong->hwnd = NULL;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hwnd, msg, wp, lp);
	}

	return 0;
}

void PongWnd_onRender(PongWnd_t * restrict pong)
{
	if (PongWnd_createAssets(pong) == false)
	{
		return;
	}

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(pong->hwnd, &ps);
	consciousUnused(hdc);

	dxRTBeginDraw((ID2D1RenderTarget *)pong->dx.pRT);
	dxRTSetTransform((ID2D1RenderTarget *)pong->dx.pRT, dxD2D1Matrix3x2FIdentity());
	// Draw black background
	dxRTClear((ID2D1RenderTarget *)pong->dx.pRT, (D2D1_COLOR_F){ .r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f });


	// Update geometry locations
	//PongWnd_updateAssets(pong);

	// Draw all geometries

	// Walls
	dxRTFillGeometry(
		(ID2D1RenderTarget *)pong->dx.pRT,
		(ID2D1Geometry *)pong->dx.pLeftWallGeo,
		(ID2D1Brush *)pong->dx.pWhiteBrush,
		NULL
	);
	dxRTFillGeometry(
		(ID2D1RenderTarget *)pong->dx.pRT,
		(ID2D1Geometry *)pong->dx.pRightWallGeo,
		(ID2D1Brush *)pong->dx.pWhiteBrush,
		NULL
	);

	// Ball
	dxRTFillGeometry(
		(ID2D1RenderTarget *)pong->dx.pRT,
		(ID2D1Geometry *)pong->dx.pBallGeo,
		(ID2D1Brush *)pong->dx.pBallBrush,
		NULL
	);


	if (dxRTEndDraw((ID2D1RenderTarget *)pong->dx.pRT) == (HRESULT)D2DERR_RECREATE_TARGET)
	{
		PongWnd_destroyAssets(pong);
	}

	EndPaint(pong->hwnd, &ps);
}
void PongWnd_onSize(PongWnd_t * restrict pong, LPARAM lp)
{
	if (pong->dx.pRT == NULL)
	{
		return;
	}
	pong->size.cx = LOWORD(lp);
	pong->size.cy = HIWORD(lp);

	dxHwndRTResize(pong->dx.pRT, (D2D1_SIZE_U){ .width = (UINT32)pong->size.cx, .height = (UINT32)pong->size.cy });
	
	// Recreate assets
	PongWnd_destroyAssets(pong);
	PongWnd_createAssets(pong);
}
void PongWnd_onSizing(PongWnd_t * restrict pong, WPARAM wp, LPARAM lp)
{
	// Make sure that window is not resizable
	RECT * r = (RECT *)lp;
	SIZE tempMSz = pong->minSize;
	// x-coordinate
	switch (wp)
	{
	case WMSZ_LEFT:
	case WMSZ_TOPLEFT:
	case WMSZ_BOTTOMLEFT:
		if ((r->right - r->left) < tempMSz.cx)
		{
			r->left = r->right - tempMSz.cx;
		}
		break;
	case WMSZ_RIGHT:
	case WMSZ_TOPRIGHT:
	case WMSZ_BOTTOMRIGHT:
		if ((r->right - r->left) < tempMSz.cx)
		{
			r->right = r->left + tempMSz.cx;
		}
		break;
	}

	// y-coordinate
	switch (wp)
	{
	case WMSZ_TOP:
	case WMSZ_TOPLEFT:
	case WMSZ_TOPRIGHT:
		if ((r->bottom - r->top) < tempMSz.cy)
		{
			r->top = r->bottom - tempMSz.cy;
		}
		break;
	case WMSZ_BOTTOM:
	case WMSZ_BOTTOMLEFT:
	case WMSZ_BOTTOMRIGHT:
		if ((r->bottom - r->top) < tempMSz.cy)
		{
			r->bottom = r->top + tempMSz.cy;
		}
		break;
	}
}
void PongWnd_onDpiChanged(PongWnd_t * restrict pong, LPARAM lp)
{
	if (pong->dx.pRT == NULL)
	{
		return;
	}

	dxRTGetDpi((ID2D1RenderTarget *)pong->dx.pRT, &pong->dpiX, &pong->dpiY);

	RECT * newr = (RECT *)lp;
	SetWindowPos(
		pong->hwnd,
		NULL,
		newr->left, newr->right,
		newr->right - newr->left, newr->bottom - newr->top,
		SWP_NOZORDER
	);

	PongWnd_calcDpiSpecific(pong);
}
