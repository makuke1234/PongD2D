#include "pongerr.h"

const wchar_t * c_errorIds[PongErr_num_errs] = {
	L"Unknown error occurred!",
	L"Error registering window class!",
	L"Error creating window!",
	L"Error creating new thread!"
	L"Error creating D2D factory!",
	L"Error creating D2D render target!",
	L"Error creating D2D brush!",
	L"Error creating D2D gradient!",
	L"Error creating D2D geometry!",
	L"Error creating DWrite factory!",
	L"Error creating DWrite font!",
	L"Error settings DWrite font alignment!",
	L"Error going to fullscreen!",
};

PongErr_e g_pongLastError = PongErr_unknown;


PongErr_e PongErr_norm(PongErr_e errId)
{
	if (errId >= PongErr_num_errs)
	{
		return PongErr_unknown;
	}
	else
	{
		return errId;
	}
}
const wchar_t * PongErr_msg(PongErr_e errId)
{
	return c_errorIds[PongErr_norm(errId)];
}

void pongErr(const PongWnd_t * restrict pong, PongErr_e errId)
{
	MessageBoxW(
		pong->hwnd,
		PongErr_msg(errId),
		PongWnd_getTitle(pong),
		MB_ICONERROR | MB_OK
	);
}
void pongLastErr(const PongWnd_t * restrict pong)
{
	pongErr(pong, g_pongLastError);
}
