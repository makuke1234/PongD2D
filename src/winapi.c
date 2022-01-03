#include "winapi.h"

bool w32_regClass(
	HINSTANCE hInst,
	LPCWSTR className,
	WNDPROC windowProc,
	HBRUSH backBrush
)
{
	WNDCLASSEXW wc = { .cbSize = sizeof(WNDCLASSEXW) };

	wc.lpfnWndProc   = windowProc;
	wc.hInstance     = hInst;
	wc.hIcon         = LoadIconW(hInst, IDI_APPLICATION);
	wc.hCursor       = LoadCursorW(NULL, IDC_ARROW);
	wc.hbrBackground = backBrush;
	wc.lpszClassName = className;
	wc.hIconSm       = wc.hIcon;

	return RegisterClassExW(&wc) != 0;
}

LARGE_INTEGER w32_getTime()
{
	LARGE_INTEGER time;
	if (QueryPerformanceCounter(&time) == FALSE)
	{
		return (LARGE_INTEGER){ 0 };
	}
	else
	{
		return time;
	}
}
