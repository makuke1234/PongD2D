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

bool PongDx_createAssets(DxBoilerPlate_t * plate);
void PongDx_destroyAssets(DxBoilerPlate_t * plate);

typedef struct PongWnd
{
	HINSTANCE hInst;
	PWSTR lpCmdArgs;
	int nCmdShow;

	SIZE minSize;
	SIZE size;

	HWND hwnd;
	LPWSTR wndTitle;

	FLOAT dpiX, dpiY;
	DxBoilerPlate_t dx;

} PongWnd_t;

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
bool PongWnd_create(PongWnd_t * pong, HINSTANCE hInst, PWSTR lpCmdArgs, int nCmdShow);
bool PongWnd_setTitle(PongWnd_t * pong, LPCWSTR title);
const wchar_t * PongWnd_getTitle(PongWnd_t * pong);
/**
 * @brief Executes window's message loop
 * 
 * @param pong Pointer to PongWnd_t structure
 * @return int wParam of last message
 */
int PongWnd_msgLoop(PongWnd_t * pong);
/**
 * @brief Frees resources owned by PongWnd_t object
 * 
 * @param pong Pointer to PongWnd_t object
 */
void PongWnd_free(PongWnd_t * pong);

LRESULT CALLBACK PongWnd_winProcHub(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT PongWnd_winProc(PongWnd_t * pong, HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

void PongWnd_onRender(PongWnd_t * pong);
void PongWnd_onSize(PongWnd_t * pong, LPARAM lp);
void PongWnd_onSizing(PongWnd_t * pong, WPARAM wp, LPARAM lp);
void PongWnd_onDpiChanged(PongWnd_t * pong, LPARAM lp);

#endif
