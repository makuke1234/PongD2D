#ifndef PONGERR_H
#define PONGERR_H

#include "winapi.h"
#include "window.h"

typedef enum PongErr
{
	PongErr_unknown,
	PongErr_windowClass,
	PongErr_window,
	PongErr_dxFactory,
	PongErr_dwFactory,
	PongErr_dxRT,

	PongErr_num_errs
} PongErr_e;

extern PongErr_e g_pongLastError;

extern const wchar_t * c_errorIds[PongErr_num_errs];

PongErr_e PongErr_norm(PongErr_e errId);
const wchar_t * PongErr_msg(PongErr_e errId);

void pongErr(const PongWnd_t * restrict pong, PongErr_e errId);
void pongLastErr(const PongWnd_t * restrict pong);


#endif
