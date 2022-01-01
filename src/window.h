#ifndef WINDOW_H
#define WINDOW_H

#include "winapi.h"
#include "d2dwrapper.h"
#include "dwritewrapper.h"
#include "logic.h"
#include <stdbool.h>

extern bool g_pongClassRegistered;

typedef struct DxBoilerPlate
{
	// Direct2D

	ID2D1Factory * factory;

	ID2D1HwndRenderTarget * pRT;

	ID2D1SolidColorBrush * pWhiteBrush;
	
	// Ball
	ID2D1GradientStopCollection * pGradStops;
	ID2D1RadialGradientBrush * pBallBrush;

	// DirectWrite

	IUnknown * wFactory;

	IUnknown * consolas16, * consolas16Centered, * consolas24CenteredBold;
	ID2D1SolidColorBrush * pPauseBrush, * pGameOverBrush, * pWinBrush;

	// Asset creation flag
	bool assetsCreated;

} DxBoilerPlate_t;


typedef struct PongWnd
{
	HINSTANCE hInst;
	PWSTR lpCmdArgs;
	int nCmdShow;

	SIZE minSize, border;
	SIZE size;

	HWND hwnd;
	LPWSTR wndTitle;

	FLOAT dpiX, dpiY;
	DxBoilerPlate_t dx;

	PongLogic_t logic;

} PongWnd_t;



/**
 * @brief Creates DX assets
 * 
 * @param pong Pointer to PongWnd_t object
 * @return true On success
 * @return false On failure
 */
bool PongWnd_createAssets(PongWnd_t * restrict pong);
/**
 * @brief Destroys DX assets
 * 
 * @param pong Pointer to PongWnd_t object
 */
void PongWnd_freeAssets(PongWnd_t * restrict pong);

/**
 * @brief Creates PongWnd_t object
 * 
 * @param pong Pointer to PongWnd_t structure
 * @param hInst Instance handle
 * @param lpCmdArgs Command line argument vector
 * @param nCmdShow Window visibility state
 * @return true On success
 * @return false On failure
 */
bool PongWnd_create(PongWnd_t * restrict pong, HINSTANCE hInst, PWSTR lpCmdArgs, int nCmdShow);
bool PongWnd_setTitle(PongWnd_t * restrict pong, LPCWSTR title);
const wchar_t * PongWnd_getTitle(const PongWnd_t * restrict pong);
/**
 * @brief Executes window's message loop
 * 
 * @param pong Pointer to PongWnd_t structure
 * @return int wParam of last message
 */
int PongWnd_msgLoop(const PongWnd_t * pong);
/**
 * @brief Frees resources owned by PongWnd_t object
 * 
 * @param pong Pointer to PongWnd_t object
 */
void PongWnd_free(PongWnd_t * restrict pong);

/**
 * @brief Convert horizontal length from device-independent pixels to real pixels
 * 
 * @param pong Pointer to PongWnd_t structure
 * @param x device-independent length
 * @return FLOAT real length
 */
FLOAT PongWnd_dipx(const PongWnd_t * restrict pong, FLOAT x);
/**
 * @brief Convert vertical length from device-independent pixels to real pixels
 * 
 * @param pong Pointer to PongWnd_t structure
 * @param y device-independent length
 * @return FLOAT real length
 */
FLOAT PongWnd_dipy(const PongWnd_t * restrict pong, FLOAT y);
/**
 * @brief Convert horizontal length from real pixels to device-independent pixels
 * 
 * @param pong Pointer to PongWnd_t structure
 * @param x real length
 * @return FLOAT device-independent length
 */
FLOAT PongWnd_dpix(const PongWnd_t * restrict pong, FLOAT x);
/**
 * @brief Convert vertical length from real pixels to device-independent pixels
 * 
 * @param pong Pointer to PongWnd_t structure
 * @param y real length
 * @return FLOAT device-independent length
 */
FLOAT PongWnd_dpiy(const PongWnd_t * restrict pong, FLOAT y);

/**
 * @brief Calculates all DPI-specific sizes
 * 
 * @param pong Pointer to PongWnd_t object
 */
void PongWnd_calcDpiSpecific(PongWnd_t * restrict pong);

/**
 * @brief Win32 window procedure "hub" that deals with all incoming messages
 * for all windows of one windows class
 */
LRESULT CALLBACK PongWnd_winProcHub(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
/**
 * @brief OO object-specific function call
 * 
 * @param pong Pointer to PongWnd_t object
 */
LRESULT PongWnd_winProc(PongWnd_t * pong, HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

/**
 * @brief Rendering sub-method, WM_PAINT msg
 */
void PongWnd_onRender(PongWnd_t * restrict pong);
/**
 * @brief Size sub-method, WM_SIZE msg
 */
void PongWnd_onSize(PongWnd_t * restrict pong, LPARAM lp);
/**
 * @brief Active-resizing sub-method, WM_SIZING msg
 */
void PongWnd_onSizing(PongWnd_t * restrict pong, WPARAM wp, LPARAM lp);
/**
 * @brief DPI changing sub-method, WM_DPICHANGED msg
 */
void PongWnd_onDpiChanged(PongWnd_t * restrict pong, LPARAM lp);
/**
 * @brief Key press-down sub-method, WM_KEYDOWN msg
 */
void PongWnd_onKeyPress(PongWnd_t * restrict pong, WPARAM wp, LPARAM lp);
/**
 * @brief Key release sub-method, WM_KEYUP msg
 */
void PongWnd_onKeyRelease(PongWnd_t * restrict pong, WPARAM wp, LPARAM lp);

#endif
