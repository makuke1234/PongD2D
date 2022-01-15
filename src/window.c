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

	// Create DirectWrite assets
	hr = dwFactoryCreateTextFormat(
		pong->dx.wFactory,
		L"Consolas",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"",
		&pong->dx.consolas16
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dwFont;
		return false;
	}
	hr = dwFactoryCreateTextFormat(
		pong->dx.wFactory,
		L"Consolas",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"",
		&pong->dx.consolas16Centered
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dwFont;
		return false;
	}
	hr = dwFactoryCreateTextFormat(
		pong->dx.wFactory,
		L"Consolas",
		NULL,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.0f,
		L"",
		&pong->dx.consolas24CenteredBold
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dwFont;
		return false;
	}


	// Set alignment
	hr = dwTFmtSetTextAlignment(
		pong->dx.consolas16Centered,
		DWRITE_TEXT_ALIGNMENT_CENTER
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dwFontAlignment;
		return false;
	}
	hr = dwTFmtSetTextAlignment(
		pong->dx.consolas24CenteredBold,
		DWRITE_TEXT_ALIGNMENT_CENTER
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dwFontAlignment;
		return false;
	}


	
	hr = dxRTCreateSolidColorBrush(
		(ID2D1RenderTarget *)pong->dx.pRT,
		(D2D1_COLOR_F){ .r = 0.600f, .g = 0.851f, .b = 0.918f, .a = 1.000f },
		&pong->dx.pPauseBrush
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxBrush;
		return false;
	}
	
	hr = dxRTCreateSolidColorBrush(
		(ID2D1RenderTarget *)pong->dx.pRT,
		(D2D1_COLOR_F){ .r = 0.945f, .g = 0.294f, .b = 0.329f, .a = 1.000f },
		&pong->dx.pGameOverBrush
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxBrush;
		return false;
	}

	hr = dxRTCreateSolidColorBrush(
		(ID2D1RenderTarget *)pong->dx.pRT,
		(D2D1_COLOR_F){ .r = 0.000f, .g = 0.769f, .b = 0.459f, .a = 1.000f },
		&pong->dx.pWinBrush
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxBrush;
		return false;
	}


	// Create assets for game logic
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

	// Text formats
	dxSafeRelease(&pong->dx.consolas24CenteredBold);
	dxSafeRelease(&pong->dx.consolas16Centered);
	dxSafeRelease(&pong->dx.consolas16);

	// Brushes
	dxSafeRelease((IUnknown **)&pong->dx.pWinBrush);
	dxSafeRelease((IUnknown **)&pong->dx.pGameOverBrush);
	dxSafeRelease((IUnknown **)&pong->dx.pPauseBrush);
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

	hr = dwDWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, &pong->dx.wFactory);
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
		WS_OVERLAPPEDWINDOW,
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
int PongWnd_msgLoop(PongWnd_t * pong)
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		BOOL br;
		if ((br = PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) != 0)
		{
			if (br == -1)
			{
				pongErr(pong, PongErr_unknown);
				return -1;
			}
			//TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			// render
			static LARGE_INTEGER start = { 0 };
			PongWnd_onRender(pong);
			
			LARGE_INTEGER stop;
			QueryPerformanceCounter(&stop);
			pong->logic.timing.frameTime = (float)(stop.QuadPart - start.QuadPart) / (float)10000000;

			// Notify game logic thread to calculate next frame's movement in time
			WakeConditionVariable(&pong->logic.timing.cv);

			start = stop;
		}
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

void PongWnd_calcPositions(PongWnd_t * restrict pong)
{
	FLOAT x = PongWnd_dipx(pong, PONG_MINW), y = PongWnd_dipy(pong, PONG_MINH);

	float factX = (float)pong->size.cx / x;
	float factY = (float)pong->size.cy / y;

	pong->factor = factX < factY ? factX : factY;
	pong->offsetX = PongWnd_dpix(pong, ((float)pong->size.cx - x * pong->factor) / 2.0f);
	pong->offsetY = PongWnd_dpiy(pong, ((float)pong->size.cy - y * pong->factor) / 2.0f);
}

void PongWnd_toggleFullScreen(PongWnd_t * restrict pong)
{
	// Toggle fullscreen flag
	pong->isFullscreen ^= 1;

	if (pong->isFullscreen)
	{
		pong->oldSize = (SIZE){
			.cx = pong->size.cx,
			.cy = pong->size.cy
		};
		RECT r;
		GetWindowRect(pong->hwnd, &r);
		pong->oldPos = (SIZE){
			.cx = r.left,
			.cy = r.top
		};
		pong->oldStyle = (DWORD)SetWindowLongPtrW(
			pong->hwnd,
			GWL_STYLE,
			WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE
		);

		// Get screen size
		SIZE screen;
		if (w32_getScreenSize(pong->hwnd, &screen))
		{
			MoveWindow(pong->hwnd, 0, 0, screen.cx, screen.cy, TRUE);
		}
		else
		{
			g_pongLastError = PongErr_fullscreen;
			pongLastErr(pong);
		}
	}
	else
	{
		RECT r = {
			.left   = 0,
			.top    = 0,
			.right  = pong->oldSize.cx,
			.bottom = pong->oldSize.cy
		};
		SetWindowLongPtrW(
			pong->hwnd,
			GWL_STYLE,
			pong->oldStyle
		);
		AdjustWindowRect(&r, pong->oldStyle, FALSE);
		MoveWindow(
			pong->hwnd,
			pong->oldPos.cx,
			pong->oldPos.cy,
			r.right  - r.left,
			r.bottom - r.top,
			TRUE
		);
		PongWnd_calcDpiSpecific(pong);
	}
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
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		consciousUnused(hdc);
		
		// Direct2D rendering
		PongWnd_onRender(pong);


		EndPaint(hwnd, &ps);
		break;
	}
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
		PongWnd_onSize(pong);
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

	dxRTBeginDraw((ID2D1RenderTarget *)pong->dx.pRT);
	
	D2D1_MATRIX_3X2_F scale = dxD2D1Matrix3x2FScale(
		(D2D1_SIZE_F){
			.width  = pong->factor,
			.height = pong->factor
		},
		(D2D1_POINT_2F){
			.x = 0.0f,
			.y = 0.0f
		}
	);
	D2D1_MATRIX_3X2_F translate = dxD2D1Matrix3x2FTranslation(
		(D2D1_SIZE_F){
			.width  = pong->offsetX,
			.height = pong->offsetY
		}
	);

	dxRTSetTransform(
		(ID2D1RenderTarget *)pong->dx.pRT,
		dxD2D1Matrix3x2FMultiply(scale, translate)
	);
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
	wchar_t temp[256];
	wsprintfW(temp, L"%u", pong->logic.scoring.leftScore);

	dxRTDrawTextW(
		(ID2D1RenderTarget *)pong->dx.pRT,
		temp,
		(UINT32)wcslen(temp),
		pong->dx.consolas24CenteredBold,
		(D2D1_RECT_F){
			.left   = 0.0f,
			.top    = 10.0f,
			.right  = PONG_MINW / 2.0f,
			.bottom = 10.0f
		},
		(ID2D1Brush *)pong->dx.pWhiteBrush,
		D2D1_DRAW_TEXT_OPTIONS_NONE,
		DWRITE_MEASURING_MODE_NATURAL
	);

	wsprintfW(temp, L"%u", pong->logic.scoring.rightScore);

	dxRTDrawTextW(
		(ID2D1RenderTarget *)pong->dx.pRT,
		temp,
		(UINT32)wcslen(temp),
		pong->dx.consolas24CenteredBold,
		(D2D1_RECT_F){
			.left   = PONG_MINW / 2.0f,
			.top    = 10.0f,
			.right  = PONG_MINW,
			.bottom = 10.0f
		},
		(ID2D1Brush *)pong->dx.pWhiteBrush,
		D2D1_DRAW_TEXT_OPTIONS_NONE,
		DWRITE_MEASURING_MODE_NATURAL
	);


	// Draw time

	{
		float time = pong->logic.scoring.time;
		uint64_t ltime = (uint64_t)time;
		uint32_t seconds = (uint32_t)(ltime % 60ull);
		uint32_t milliseconds = (uint32_t)((time - (float)ltime) * 1000.0f);
		uint32_t hours = (uint32_t)(ltime / 3600ull);
		uint32_t minutes = (uint32_t)(ltime - (uint64_t)hours * 3600ull - seconds) / 60u;
		wsprintfW(
			temp,
			L"%.2u:%.2u:%.2u.%.3u",
			hours,
			minutes,
			seconds,
			milliseconds
		);
	}

	dxRTDrawTextW(
		(ID2D1RenderTarget *)pong->dx.pRT,
		temp,
		(UINT32)wcslen(temp),
		pong->dx.consolas16Centered,
		(D2D1_RECT_F){
			.left   = 0.0f,
			.top    = PONG_MINH - 16.f,
			.right  = PONG_MINW,
			.bottom = PONG_MINH
		},
		(ID2D1Brush *)pong->dx.pWhiteBrush,
		D2D1_DRAW_TEXT_OPTIONS_NONE,
		DWRITE_MEASURING_MODE_NATURAL
	);


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

	// Draw game over screen
	if (!pong->logic.scoring.notPaused)
	{
		const wchar_t pauseStr[] = L"The game has been paused.",
			pauseStr2[] = L"Press 'ESCAPE' to resume or 'ENTER' to restart.";

		dxRTDrawTextW(
			(ID2D1RenderTarget *)pong->dx.pRT,
			pauseStr,
			(sizeof(pauseStr) - 1) / sizeof(wchar_t),
			pong->dx.consolas24CenteredBold,
			(D2D1_RECT_F){
				.left   = 20.0f,
				.top    = 144.0f,
				.right  = PONG_MINW - 20.0f,
				.bottom = 144.0f
			},
			(ID2D1Brush *)pong->dx.pPauseBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE,
			DWRITE_MEASURING_MODE_NATURAL
		);

		dxRTDrawTextW(
			(ID2D1RenderTarget *)pong->dx.pRT,
			pauseStr2,
			(sizeof(pauseStr2) - 1) / sizeof(wchar_t),
			pong->dx.consolas24CenteredBold,
			(D2D1_RECT_F){
				.left   = 20.0f,
				.top    = 360.0f,
				.right  = PONG_MINW - 20.0f,
				.bottom = 360.0f
			},
			(ID2D1Brush *)pong->dx.pPauseBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE,
			DWRITE_MEASURING_MODE_NATURAL
		);
	}
	else if (pong->logic.scoring.mode == GameMode_gameOver)
	{
		const wchar_t goString[] = L"Game over!";
		dxRTDrawTextW(
			(ID2D1RenderTarget *)pong->dx.pRT,
			goString,
			(sizeof(goString) - 1) / sizeof(wchar_t),
			pong->dx.consolas24CenteredBold,
			(D2D1_RECT_F){
				.left   = 20.0f,
				.top    = 144.0f,
				.right  = PONG_MINW - 20.0f,
				.bottom = 144.0f
			},
			(ID2D1Brush *)pong->dx.pGameOverBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE,
			DWRITE_MEASURING_MODE_NATURAL
		);

		wsprintfW(temp, L"%s wins!", (pong->logic.scoring.winnerIdx == 1) ? L"Right" : L"Left");
		dxRTDrawTextW(
			(ID2D1RenderTarget *)pong->dx.pRT,
			temp,
			(UINT32)wcslen(temp),
			pong->dx.consolas24CenteredBold,
			(D2D1_RECT_F){
				.left   = 20.0f,
				.top    = 180.0f,
				.right  = PONG_MINW - 20.0f,
				.bottom = 180.0f
			},
			(ID2D1Brush *)pong->dx.pWinBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE,
			DWRITE_MEASURING_MODE_NATURAL
		);

		// Show instructions
		const wchar_t instStr[] = L"Press 'ENTER' to start again.";

		dxRTDrawTextW(
			(ID2D1RenderTarget *)pong->dx.pRT,
			instStr,
			(sizeof(instStr) - 1) / sizeof(wchar_t),
			pong->dx.consolas24CenteredBold,
			(D2D1_RECT_F){
				.left   = 20.0f,
				.top    = 360.0f,
				.right  = PONG_MINW - 20.0f,
				.bottom = 360.0f
			},
			(ID2D1Brush *)pong->dx.pGameOverBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE,
			DWRITE_MEASURING_MODE_NATURAL
		);
	}

	if (dxRTEndDraw((ID2D1RenderTarget *)pong->dx.pRT, NULL, NULL) == (HRESULT)D2DERR_RECREATE_TARGET)
	{
		PongWnd_freeAssets(pong);
	}
}
void PongWnd_onSize(PongWnd_t * restrict pong)
{
	if (pong->dx.pRT == NULL)
	{
		return;
	}
	RECT r;
	GetClientRect(pong->hwnd, &r);
	pong->size.cx = r.right  - r.left;
	pong->size.cy = r.bottom - r.top;

	dxHwndRTResize(
		pong->dx.pRT, (D2D1_SIZE_U){
			.width  = (UINT32)pong->size.cx,
			.height = (UINT32)pong->size.cy
		}
	);
	PongWnd_calcPositions(pong);
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
		}
		break;
	case VK_RETURN:
		if (pong->logic.scoring.mode != GameMode_normal || !pong->logic.scoring.notPaused)
		{
			PongLogic_reset(&pong->logic);
		}
		break;
	case VK_F11:
		// Act only if key was just pressed down, prevents spamming
		if ((lp & 0x40000000) == 0)
		{
			PongWnd_toggleFullScreen(pong);
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

