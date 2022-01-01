#ifndef PONGERR_H
#define PONGERR_H

#include "winapi.h"
#include "window.h"

typedef enum PongErr
{
	PongErr_unknown,
	PongErr_windowClass,
	PongErr_window,
	PongErr_thread,
	PongErr_dxFactory,
	PongErr_dxRT,
	PongErr_dxBrush,
	PongErr_dxGradient,
	PongErr_dxGeo,
	PongErr_dwFactory,
	PongErr_dwFont,
	PongErr_dwFontAlignment,

	PongErr_num_errs
} PongErr_e;

extern PongErr_e g_pongLastError;

extern const wchar_t * c_errorIds[PongErr_num_errs];

/**
 * @brief Normalises error id to be within error id range
 * 
 * @param errId Error id to be normalised
 * @return PongErr_e Normalised error id
 */
PongErr_e PongErr_norm(PongErr_e errId);
/**
 * @brief Returns error string from normalised error id
 * 
 * @param errId Error id to be normalised
 * @return const wchar_t* Normalised error string
 */
const wchar_t * PongErr_msg(PongErr_e errId);

/**
 * @brief Brings up message box with message from normalised error id
 * 
 * @param pong Pointer to PongWnd_t object
 * @param errId Error id to be normalised
 */
void pongErr(const PongWnd_t * restrict pong, PongErr_e errId);
/**
 * @brief Brings up message box with last occurred error message
 * 
 * @param pong Pointer to PongWnd_t object
 */
void pongLastErr(const PongWnd_t * restrict pong);


#endif
