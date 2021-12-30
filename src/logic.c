#include "logic.h"
#include "resource.h"
#include "pongerr.h"

#include <time.h>

DWORD WINAPI PongLogic_thread(LPVOID param)
{
	PongLogic_t * logic = param;

	while (logic->killThreadFlag == false)
	{
		while (!logic->scoring.notPaused)
		{
			Sleep(20);
			if (logic->killThreadFlag)
			{
				goto PongLogic_thread_finish;
			}
		}

		clock_t start = clock();
		Sleep(20);
		float delta = (float)(clock() - start) / (float)CLOCKS_PER_SEC;

		logic->scoring.time += delta;

		switch (logic->scoring.mode)
		{
		case GameMode_normal:
		{
			break;
		}
		case GameMode_gameOver:
		{

			break;
		}
		}

		// Update screen
		InvalidateRect(logic->pong->hwnd, NULL, FALSE);
	}

PongLogic_thread_finish: ;

	return 0;
}

bool PongLogic_create(PongLogic_t * restrict logic, PongWnd_t * pong)
{
	if (logic == NULL)
	{
		return false;
	}

	*logic = (PongLogic_t){ 0 };
	logic->pong = pong;

	PongLogic_calcAbsLeftPad(logic);
	PongLogic_calcAbsRightPad(logic);
	PongLogic_calcAbsBall(logic);

	// try to create thread
	logic->logicThread = CreateThread(
		NULL,
		256 * sizeof(size_t),
		&PongLogic_thread,
		logic,
		0,
		NULL
	);

	if (logic->logicThread == NULL)
	{
		g_pongLastError = PongErr_thread;
		return false;
	}

	return true;
}
void PongLogic_free(PongLogic_t * restrict logic)
{
	if (logic == NULL)
	{
		return;
	}

	if (logic->logicThread != NULL)
	{
		logic->killThreadFlag = true;
		WaitForSingleObject(logic->logicThread, INFINITE);
	}

	PongLogic_freeAssets(logic);
}

bool PongLogic_createAssets(PongLogic_t * restrict logic)
{
	if (logic == NULL)
	{
		return false;
	}
	else if (logic->assetsCreated == true)
	{
		return true;
	}

	// Create geometry
	HRESULT hr = dxFactoryCreateRectangleGeometry(
		logic->pong->dx.factory,
		(D2D1_RECT_F){
			.left   = 0.0f,
			.top    = 0.0f,
			.right  = PONG_MINW,
			.bottom = 0.0f
		},
		&logic->pUpWallGeo
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxGeo;
		return false;
	}

	hr = dxFactoryCreateRectangleGeometry(
		logic->pong->dx.factory,
		(D2D1_RECT_F){
			.left   = 0.0f,
			.top    = PONG_MINH,
			.right  = PONG_MINW,
			.bottom = PONG_MINH
		},
		&logic->pDownWallGeo
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxGeo;
		return false;
	}

	hr = dxFactoryCreateRectangleGeometry(
		logic->pong->dx.factory,
		(D2D1_RECT_F){
			.left   = 0.0f,
			.top    = 0.0f,
			.right  = 0.0f,
			.bottom = PONG_MINH
		},
		&logic->pLeftWallGeo
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxGeo;
		return false;
	}

	hr = dxFactoryCreateRectangleGeometry(
		logic->pong->dx.factory,
		(D2D1_RECT_F){
			.left   = PONG_MINW,
			.top    = 0.0f,
			.right  = PONG_MINW,
			.bottom = PONG_MINH
		},
		&logic->pRightWallGeo
	);
	if (FAILED(hr))
	{
		g_pongLastError = PongErr_dxGeo;
		return false;
	}

	logic->assetsCreated = true;
	return true;
}
void PongLogic_freeAssets(PongLogic_t * restrict logic)
{
	if (logic == NULL)
	{
		return;
	}
	logic->assetsCreated = false;

	dxSafeRelease((IUnknown **)&logic->pUpWallGeo);
	dxSafeRelease((IUnknown **)&logic->pDownWallGeo);
	dxSafeRelease((IUnknown **)&logic->pLeftWallGeo);
	dxSafeRelease((IUnknown **)&logic->pRightWallGeo);
}

void PongLogic_calcAbsLeftPad(PongLogic_t * restrict logic)
{
	if (logic == NULL)
	{
		return;
	}
	logic->scoring.absLeftPad = (D2D1_RECT_F){
		.left   = 0.0f,
		.top    = (PONG_MINH - PONG_WALL_Y) / 2.0f + logic->scoring.relLeftPad,
		.right  = PONG_WALL_X,
		.bottom = (PONG_MINH + PONG_WALL_Y) / 2.0f + logic->scoring.relLeftPad,
	};
}
void PongLogic_calcAbsRightPad(PongLogic_t * restrict logic)
{
	if (logic == NULL)
	{
		return;
	}
	logic->scoring.absRightPad = (D2D1_RECT_F){
		.left   = PONG_MINW - PONG_WALL_X,
		.top    = (PONG_MINH - PONG_WALL_Y) / 2.0f + logic->scoring.relRightPad,
		.right  = PONG_MINW,
		.bottom = (PONG_MINH + PONG_WALL_Y) / 2.0f + logic->scoring.relRightPad
	};
}
void PongLogic_calcAbsBall(PongLogic_t * restrict logic)
{
	if (logic == NULL)
	{
		return;
	}
	logic->scoring.absBall = (D2D1_POINT_2F){
		.x = PONG_MINW / 2.0f,
		.y = PONG_MINH / 2.0f
	};
}

void PongLogic_reset(PongLogic_t * restrict logic)
{
	if (logic == NULL)
	{
		return;
	}

	// Reset scoring
	logic->scoring = (Scoring_t){ 0 };
}