#ifndef WINDOW_H
#define WINDOW_H

#include "winapi.h"
#include "d2dwrapper.h"
#include <stdbool.h>

extern bool g_pongClassRegistered;

typedef struct DxBoilerPlate
{
	ID2D1Factory * factory;
	IUnknown * wFactory;

	ID2D1HwndRenderTarget * hwndRT;

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
void PongWnd_destroyAssets(PongWnd_t * restrict pong);

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

FLOAT PongWnd_dipx(const PongWnd_t * restrict pong, FLOAT x);
FLOAT PongWnd_dipy(const PongWnd_t * restrict pong, FLOAT y);
FLOAT PongWnd_dpix(const PongWnd_t * restrict pong, FLOAT x);
FLOAT PongWnd_dpiy(const PongWnd_t * restrict pong, FLOAT y);

void PongWnd_calcDpiSpecific(PongWnd_t * restrict pong);

LRESULT CALLBACK PongWnd_winProcHub(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT PongWnd_winProc(PongWnd_t * pong, HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

void PongWnd_onRender(PongWnd_t * restrict pong);
void PongWnd_onSize(PongWnd_t * restrict pong, LPARAM lp);
void PongWnd_onSizing(PongWnd_t * restrict pong, WPARAM wp, LPARAM lp);
void PongWnd_onDpiChanged(PongWnd_t * restrict pong, LPARAM lp);

#endif
