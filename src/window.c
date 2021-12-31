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

	D2D1_GRADIENT_STOP gradientStops[3];
	gradientStops[0].color = (D2D1_COLOR_F){ 1.0f, 1.0f, 1.0f, 1.0f };
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = (D2D1_COLOR_F){ 0.5f, 0.5f, 0.5f, 1.0f };
	gradientStops[1].position = 0.6f;
	gradientStops[2].color = (D2D1_COLOR_F){ 0.2f, 0.2f, 0.2f, 1.0f };
	gradientStops[2].position = 1.0f;

	hr = dxRTCreateGradientStopCollection(
		(ID2D1RenderTarget *)pong->dx.pRT,
		gradientStops,
		3,
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
			pong->logic.scoring.absBall,
			(D2D1_POINT_2F){ .x = 5.0f, .y = -2.0f },
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

	if (PongLogic_createAssets(&pong->logic) == false)
	{
		return false;
	}

	// return successfully
	pong->dx.assetsCreated = true;
	return true;
}
void PongWnd_freeAssets(PongWnd_t * restrict pong)
{
	if (pong == NULL)
	{
		return;
	}
	pong->dx.assetsCreated = false;

	// Destroy assets here
	PongLogic_freeAssets(&pong->logic);

	// Brushes
	dxSafeRelease((IUnknown **)&pong->dx.pBallBrush);
	dxSafeRelease((IUnknown **)&pong->dx.pWhiteBrush);

	// Gradient stops
	dxSafeRelease((IUnknown **)&pong->dx.pGradStops);

	// Hwnd render target
	dxSafeRelease((IUnknown **)&pong->dx.pRT);
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

	PongLogic_create(&pong->logic, pong);

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
	PongWnd_freeAssets(pong);
	dxSafeRelease(&pong->dx.wFactory);
	dxSafeRelease((IUnknown **)&pong->dx.factory);

	PongLogic_free(&pong->logic);
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
	case WM_KEYDOWN:
		PongWnd_onKeyPress(pong, wp, lp);
		break;
	case WM_KEYUP:
		PongWnd_onKeyRelease(pong, wp, lp);
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


	// Draw line in the center
	dxRTDrawLine(
		(ID2D1RenderTarget *)pong->dx.pRT,
		(D2D1_POINT_2F){ .x = PONG_MINW / 2.0f - 0.5f, .y = 0.0f },
		(D2D1_POINT_2F){ .x = PONG_MINW / 2.0f + 0.5f, .y = PONG_MINH },
		(ID2D1Brush *)pong->dx.pWhiteBrush,
		1.0f,
		NULL
	);

	// Draw scores


	// Draw walls
	dxRTFillRectangle(
		(ID2D1RenderTarget *)pong->dx.pRT,
		pong->logic.scoring.absLeftPad,
		(ID2D1Brush *)pong->dx.pWhiteBrush
	);
	dxRTFillRectangle(
		(ID2D1RenderTarget *)pong->dx.pRT,
		pong->logic.scoring.absRightPad,
		(ID2D1Brush *)pong->dx.pWhiteBrush
	);

	// Update ball brush center
	dxRadialGradBrushSetCenter(
		pong->dx.pBallBrush,
		(D2D1_POINT_2F){
			.x = pong->logic.scoring.absBall.x + 5.0f,
			.y = pong->logic.scoring.absBall.y - 2.0f 
		}
	);

	// Draw ball
	dxRTFillEllipse(
		(ID2D1RenderTarget *)pong->dx.pRT,
		(D2D1_ELLIPSE){
			.point   = pong->logic.scoring.absBall,
			.radiusX = PONG_BALL_X,
			.radiusY = PONG_BALL_Y
		},
		(ID2D1Brush *)pong->dx.pBallBrush
	);

	if (dxRTEndDraw((ID2D1RenderTarget *)pong->dx.pRT, NULL, NULL) == (HRESULT)D2DERR_RECREATE_TARGET)
	{
		PongWnd_freeAssets(pong);
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
	PongWnd_freeAssets(pong);
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
void PongWnd_onKeyPress(PongWnd_t * restrict pong, WPARAM wp, LPARAM lp)
{
	consciousUnused(lp);

	switch (wp)
	{
	case VK_ESCAPE:
		if (pong->logic.scoring.mode == GameMode_normal)
		{
			pong->logic.scoring.notPaused ^= 1;
			InvalidateRect(pong->hwnd, NULL, FALSE);
		}
		break;
	// Left pad up
	case L'W':
		pong->logic.scoring.relLeftPad = clamp(pong->logic.scoring.relLeftPad - 1.0f, -PONG_WALL_MAX, PONG_WALL_MAX);
		break;
	// Left pad down
	case L'S':
		pong->logic.scoring.relLeftPad = clamp(pong->logic.scoring.relLeftPad + 1.0f, -PONG_WALL_MAX, PONG_WALL_MAX);
		break;
	// Right pad up
	case VK_UP:
		pong->logic.scoring.relRightPad = clamp(pong->logic.scoring.relRightPad - 1.0f, -PONG_WALL_MAX, PONG_WALL_MAX);
		break;
	// Right pad down
	case VK_DOWN:
		pong->logic.scoring.relRightPad = clamp(pong->logic.scoring.relRightPad + 1.0f, -PONG_WALL_MAX, PONG_WALL_MAX);
		break;
	case VK_RETURN:
		if (pong->logic.scoring.mode != GameMode_normal || !pong->logic.scoring.notPaused)
		{
			PongLogic_reset(&pong->logic);
		}
		break;
	}
}
void PongWnd_onKeyRelease(PongWnd_t * restrict pong, WPARAM wp, LPARAM lp)
{
	consciousUnused(pong);
	consciousUnused(wp);
	consciousUnused(lp);
	// Do nothing
}

