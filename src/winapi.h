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

#define consciousUnused(x) ((void)x)

bool w32_regClass(
	HINSTANCE hInst,
	LPCWSTR className,
	WNDPROC windowProc,
	HBRUSH backBrush
);


#endif
