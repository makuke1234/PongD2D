#ifndef LOGIC_H
#define LOGIC_H

#include "winapi.h"
#include "d2dwrapper.h"

typedef struct PongLogic
{
	ID2D1RenderTarget * pRT;
	ID2D1RectangleGeometry * pUpWallGeo, * pDownWallGeo;
	ID2D1RectangleGeometry * pLeftWallGeo, * pRightWallGeo;

	bool assetsCreated;

	float leftPadRelPos, rightPadRelPos;
	D2D1_POINT_2F ballRelPos;

	D2D1_RECT_F absLeftPad, absRightPad;
	D2D1_POINT_2F absBall;

	float ballAngle;

	HANDLE logicThread;

} PongLogic_t;

bool PongLogic_create(PongLogic_t * restrict logic);
void PongLogic_free(PongLogic_t * restrict logic);

bool PongLogic_createAssets(PongLogic_t * restrict logic, ID2D1RenderTarget * pRT);
void PongLogic_freeAssets(PongLogic_t * restrict logic);

void PongLogic_calcAbsLeftPad(PongLogic_t * logic);
void PongLogic_calcAbsRightPad(PongLogic_t * logic);
void PongLogic_calcAbsBall(PongLogic_t * logic);

#endif
