#ifndef WINAPI_H
#define WINAPI_H

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#ifndef WIN32_EXTRA_LEAN
	#define WIN32_EXTRA_LEAN
#endif

#ifndef NOMINMAX
	#define NOMINMAX
#endif

#ifndef UNICODE
	#define UNICODE
#endif

#ifndef _UNICODE
	#define _UNICODE
#endif

#include <Windows.h>
#include <stdbool.h>

/**
 * @brief Marks variable as "consciously" unused, so that the compiler won't complain
 */
#define consciousUnused(x) ((void)x)

/**
 * @brief Registers win32 window class with specified parameters. Chooses
 * automatically default cursor as IDC_ARROW and loads application icon from
 * resource IDI_APPLICATION
 * 
 * @param hInst Instance handle
 * @param className Class name
 * @param windowProc Window procedure function pointer
 * @param backBrush HBRUSH object the background of the window should be painted with
 * @return true On success
 * @return false On failure
 */
bool w32_regClass(
	HINSTANCE hInst,
	LPCWSTR className,
	WNDPROC windowProc,
	HBRUSH backBrush
);

/**
 * @brief Get time in microsecond accuracy
 * 
 * @return LARGE_INTEGER Win32 time structure
 */
LARGE_INTEGER w32_getTime();


#endif
