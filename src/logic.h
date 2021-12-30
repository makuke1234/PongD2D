#ifndef LOGIC_H
#define LOGIC_H

#include "winapi.h"
#include "d2dwrapper.h"

#include <stdint.h>

typedef enum GameMode
{
	GameMode_normal,
	GameMode_gameOver

} GameMode_e;

typedef struct Scoring
{
	float relLeftPad, relRightPad;

	D2D1_RECT_F absLeftPad, absRightPad;
	D2D1_POINT_2F absBall;

	float ballAngle;

	GameMode_e mode;
	bool notPaused;

	float time;
	uint32_t leftScore, rightScore;

} Scoring_t;

struct PongWnd;

typedef struct PongLogic
{
	struct PongWnd * pong;
	ID2D1RectangleGeometry * pUpWallGeo, * pDownWallGeo;
	ID2D1RectangleGeometry * pLeftWallGeo, * pRightWallGeo;

	bool assetsCreated;

	Scoring_t scoring;

	HANDLE logicThread;
	bool killThreadFlag;

} PongLogic_t;

DWORD WINAPI PongLogic_thread(LPVOID param);

bool PongLogic_create(PongLogic_t * restrict logic, struct PongWnd * hwnd);
void PongLogic_free(PongLogic_t * restrict logic);

bool PongLogic_createAssets(PongLogic_t * restrict logic);
void PongLogic_freeAssets(PongLogic_t * restrict logic);

void PongLogic_calcAbsLeftPad(PongLogic_t * restrict logic);
void PongLogic_calcAbsRightPad(PongLogic_t * restrict logic);
void PongLogic_calcAbsBall(PongLogic_t * restrict logic);

void PongLogic_reset(PongLogic_t * restrict logic);

float clamp(float value, float min, float max);

#endif
