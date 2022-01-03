#include "logic.h"
#include "resource.h"
#include "pongerr.h"

#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

DWORD WINAPI PongLogic_thread(LPVOID param)
{
	PongLogic_t * logic = param;

	while (logic->killThreadFlag == false)
	{
		// Wait for time to come over
		EnterCriticalSection(&logic->timing.crit);

		SleepConditionVariableCS(&logic->timing.cv, &logic->timing.crit, INFINITE);
		float delta = logic->timing.frameTime;

		LeaveCriticalSection(&logic->timing.crit);


		if (logic->requestReset)
		{
			logic->requestReset = false;
			EnterCriticalSection(&logic->timing.critR);

			WakeConditionVariable(&logic->timing.cvR);
			SleepConditionVariableCS(&logic->timing.cvR, &logic->timing.critR, INFINITE);

			LeaveCriticalSection(&logic->timing.critR);
		}

		if (!logic->scoring.notPaused)
		{
			continue;
		}


		switch (logic->scoring.mode)
		{
		case GameMode_normal:
		{
			// Always keep positive angle
			if (logic->scoring.ballAngle < 0.0f)
			{
				logic->scoring.ballAngle += (float)M_PI * 2.0f;
			}

			if (logic->scoring.ballAngle > (float)M_PI * 1.5f)
			{
				logic->scoring.ballAngle = clamp(logic->scoring.ballAngle, 1.75f * (float)M_PI, 2.0f * (float)M_PI);
			}
			else if (logic->scoring.ballAngle > (float)M_PI)
			{
				logic->scoring.ballAngle = clamp(logic->scoring.ballAngle, (float)M_PI, 1.25f * (float)M_PI);
			}
			else if (logic->scoring.ballAngle > (float)M_PI * 0.5f)
			{
				logic->scoring.ballAngle = clamp(logic->scoring.ballAngle, (float)M_PI * 0.75f, (float)M_PI);
			}
			else
			{
				logic->scoring.ballAngle = clamp(logic->scoring.ballAngle, 0.0f, (float)M_PI * 0.25f);
			}

			float dxWall = 0.9f * logic->scoring.ballSpeed * delta;
			logic->scoring.relLeftPadPrev  = logic->scoring.relLeftPadPrev;
			logic->scoring.relRightPadPrev = logic->scoring.relRightPadPrev;
			
			// Left pad up
			if (GetAsyncKeyState(L'W') & 0x8000)
			{
				logic->scoring.relLeftPad = clamp(logic->scoring.relLeftPad - dxWall, -PONG_WALL_MAX, PONG_WALL_MAX);
			}
			// Left pad down
			else if (GetAsyncKeyState(L'S') & 0x8000)
			{
				logic->scoring.relLeftPad = clamp(logic->scoring.relLeftPad + dxWall, -PONG_WALL_MAX, PONG_WALL_MAX);
			}
			// Right pad up
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				logic->scoring.relRightPad = clamp(logic->scoring.relRightPad - dxWall, -PONG_WALL_MAX, PONG_WALL_MAX);
			}
			// Right pad down
			else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				logic->scoring.relRightPad = clamp(logic->scoring.relRightPad + dxWall, -PONG_WALL_MAX, PONG_WALL_MAX);
			}


			logic->scoring.time += delta;
			// Create left & right pad geometries, ball geometry

			PongLogic_calcAbsLeftPad(logic);
			PongLogic_calcAbsRightPad(logic);

			// Move ball in direction given to it

			float vx =  logic->scoring.ballSpeed * cosf(logic->scoring.ballAngle);
			float vy = -logic->scoring.ballSpeed * sinf(logic->scoring.ballAngle);

			float dx = delta * vx;
			float dy = delta * vy;

			logic->scoring.absBall.x = clamp(logic->scoring.absBall.x + dx, PONG_BALL_X, PONG_MINW - PONG_BALL_X);
			logic->scoring.absBall.y = clamp(logic->scoring.absBall.y + dy, PONG_BALL_Y, PONG_MINH - PONG_BALL_Y);

			ID2D1RectangleGeometry * plPadGeo, * prPadGeo;
			ID2D1EllipseGeometry * pBallGeo;

			HRESULT hr = dxFactoryCreateRectangleGeometry(
				logic->pong->dx.factory,
				logic->scoring.absLeftPad,
				&plPadGeo
			);
			if (FAILED(hr))
			{
				goto PongLogic_thread_release_rsc;
			}

			hr = dxFactoryCreateRectangleGeometry(
				logic->pong->dx.factory,
				logic->scoring.absRightPad,
				&prPadGeo
			);
			if (FAILED(hr))
			{
				goto PongLogic_thread_release_rsc;
			}

			hr = dxFactoryCreateEllipseGeometry(
				logic->pong->dx.factory,
				(D2D1_ELLIPSE){
					.point   = logic->scoring.absBall,
					.radiusX = PONG_BALL_X,
					.radiusY = PONG_BALL_Y
				},
				&pBallGeo
			);
			if (FAILED(hr))
			{
				goto PongLogic_thread_release_rsc;
			}

			// Check for "bad" collisions
			bool collides = false;

			D2D1_GEOMETRY_RELATION geoRel;
			
			// If no "bad" collisions, check for "good"
			// If ball bounces off roof
			hr = dxGeoCompareWithGeometry(
				(ID2D1Geometry *)pBallGeo,
				(ID2D1Geometry *)logic->pUpWallGeo,
				NULL,
				&geoRel
			);
			if (FAILED(hr))
			{
				goto PongLogic_thread_release_rsc;
			}
			else if (geoRel > D2D1_GEOMETRY_RELATION_DISJOINT)
			{
				logic->scoring.ballAngle = -logic->scoring.ballAngle;	// -angle
				goto PongLogic_thread_release_rsc;
			}

			// If ball bounces off floor
			hr = dxGeoCompareWithGeometry(
				(ID2D1Geometry *)pBallGeo,
				(ID2D1Geometry *)logic->pDownWallGeo,
				NULL,
				&geoRel
			);
			if (FAILED(hr))
			{
				goto PongLogic_thread_release_rsc;
			}
			else if (geoRel > D2D1_GEOMETRY_RELATION_DISJOINT)
			{
				logic->scoring.ballAngle = -logic->scoring.ballAngle;
				goto PongLogic_thread_release_rsc;
			}

			// If ball bounces off left pad
			hr = dxGeoCompareWithGeometry(
				(ID2D1Geometry *)pBallGeo,
				(ID2D1Geometry *)plPadGeo,
				NULL,
				&geoRel
			);
			if (FAILED(hr))
			{
				goto PongLogic_thread_release_rsc;
			}
			else if (geoRel > D2D1_GEOMETRY_RELATION_DISJOINT)
			{
				logic->scoring.ballAngle = fmodf((float)M_PI - logic->scoring.ballAngle, 2.0f * (float)M_PI);	// 180 - angle
				float percentAngle = logic->scoring.absBall.x;
				logic->scoring.absBall.x = PONG_WALL_X + PONG_BALL_X;
				percentAngle = clamp(fabsf(logic->scoring.absBall.x - percentAngle) / PONG_BALL_X, 0.0f, 0.75f);

				float paddleSpeed = (logic->scoring.relLeftPad - logic->scoring.relLeftPadPrev) / delta;

				logic->scoring.ballAngle += (PongRng_rand_normf(&logic->rng, 10000) * 2.0f - 1.0f) * (float)M_PI / 6.0f;

				logic->scoring.ballSpeed = fabsf(logic->scoring.ballSpeed + percentAngle * paddleSpeed * (PongRng_rand_normf(&logic->rng, 10000) * 2.0f - 1.0f));

				goto PongLogic_thread_release_rsc;
			}

			// If ball bounces of right pad
			hr = dxGeoCompareWithGeometry(
				(ID2D1Geometry *)pBallGeo,
				(ID2D1Geometry *)prPadGeo,
				NULL,
				&geoRel
			);
			if (FAILED(hr))
			{
				goto PongLogic_thread_release_rsc;
			}
			else if (geoRel > D2D1_GEOMETRY_RELATION_DISJOINT)
			{
				logic->scoring.ballAngle = fmodf((float)M_PI - logic->scoring.ballAngle, 2.0f * (float)M_PI);	// 180 - angle
				float percentAngle = logic->scoring.absBall.x;
				logic->scoring.absBall.x = PONG_MINW - (PONG_WALL_X + PONG_BALL_X);
				percentAngle = clamp(fabsf(logic->scoring.absBall.x - percentAngle) / PONG_BALL_X, 0.0f, 0.75f);

				// Calculate moving speed of right paddle
				float paddleSpeed = (logic->scoring.relRightPad - logic->scoring.relRightPadPrev) / delta;

				logic->scoring.ballAngle += (PongRng_rand_normf(&logic->rng, 10000) * 2.0f - 1.0f) * (float)M_PI / 6.0f;

				logic->scoring.ballSpeed = fabsf(logic->scoring.ballSpeed + percentAngle * paddleSpeed * (PongRng_rand_normf(&logic->rng, 10000) * 2.0f - 1.0f));

				goto PongLogic_thread_release_rsc;
			}

			logic->scoring.ballSpeed = clamp(logic->scoring.ballSpeed, 0.75f * PONG_BALL_VELOCITY, 2.5f * PONG_BALL_VELOCITY);

			// If no "good" collisions, check for "bad"

			hr = dxGeoCompareWithGeometry(
				(ID2D1Geometry *)pBallGeo,
				(ID2D1Geometry *)logic->pLeftWallGeo,
				NULL,
				&geoRel
			);
			if (FAILED(hr))
			{
				goto PongLogic_thread_release_rsc;
			}
			else if (geoRel > D2D1_GEOMETRY_RELATION_DISJOINT)
			{
				++logic->scoring.rightScore;
				collides = true;
			}

			if (!collides)
			{
				hr = dxGeoCompareWithGeometry(
					(ID2D1Geometry *)pBallGeo,
					(ID2D1Geometry *)logic->pRightWallGeo,
					NULL,
					&geoRel
				);
				if (FAILED(hr))
				{
					goto PongLogic_thread_release_rsc;
				}
				else if (geoRel > D2D1_GEOMETRY_RELATION_DISJOINT)
				{
					++logic->scoring.leftScore;
					collides = true;
				}
			}

			if (collides)
			{
				if (logic->scoring.leftScore >= 10 || logic->scoring.rightScore >= 10)
				{
					if (logic->scoring.rightScore > logic->scoring.leftScore)
					{
						logic->scoring.winnerIdx = 1;
					}
					logic->scoring.mode = GameMode_gameOver;
				}
				else
				{
					PongLogic_calcAbsBall(logic);
					logic->scoring.notPaused = false;
				}
			}

		PongLogic_thread_release_rsc: ;
			// Release resources
			dxSafeRelease((IUnknown **)&plPadGeo);
			dxSafeRelease((IUnknown **)&prPadGeo);
			dxSafeRelease((IUnknown **)&pBallGeo);

			break;
		}
		case GameMode_gameOver:
			break;
		}

		// Screen will be updated automatically
	}

	logic->logicThread = NULL;
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

	// Init rng
	logic->rng = PongRng_init((uint64_t)time(NULL));

	PongLogic_reset(logic);

	// Create critical section and condition variable
	InitializeCriticalSection(&logic->timing.crit);
	InitializeConditionVariable(&logic->timing.cv);

	InitializeCriticalSection(&logic->timing.critR);
	InitializeConditionVariable(&logic->timing.cvR);

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
		WakeConditionVariable(&logic->timing.cv);
		WaitForSingleObject(logic->logicThread, INFINITE);
	}

	// Delete critical section
	DeleteCriticalSection(&logic->timing.crit);
	DeleteCriticalSection(&logic->timing.critR);

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

void PongLogic_reset(PongLogic_t * logic)
{
	if (logic == NULL)
	{
		return;
	}

	if (logic->logicThread != NULL)
	{
		EnterCriticalSection(&logic->timing.critR);
		logic->requestReset = true;
		WakeConditionVariable(&logic->timing.cv);

		SleepConditionVariableCS(&logic->timing.cvR, &logic->timing.critR, INFINITE);
	}

	// Reset scoring
	logic->scoring = (Scoring_t){ 0 };
	// Semi random initial ball angle
	logic->scoring.ballAngle = (float)M_PI / 4.0f + ((float)M_PI / 6.0f) * (PongRng_rand_normf(&logic->rng, 10000ULL) * 2.0f - 1.0f); // 45 degrees give or take
	logic->scoring.ballSpeed = PONG_BALL_VELOCITY + (PONG_BALL_VELOCITY / 2.0f) * (PongRng_rand_normf(&logic->rng, 10000) * 2.0f - 1.0f);

	PongLogic_calcAbsLeftPad(logic);
	PongLogic_calcAbsRightPad(logic);
	PongLogic_calcAbsBall(logic);

	if (logic->logicThread != NULL)
	{
		LeaveCriticalSection(&logic->timing.critR);
		WakeConditionVariable(&logic->timing.cvR);
	}
}

float clamp(float value, float min, float max)
{
	if (value < min)
	{
		return min;
	}
	else if (value > max)
	{
		return max;
	}
	else
	{
		return value;
	}
}
