#include "logic.h"
#include "resource.h"

bool PongLogic_create(PongLogic_t * restrict logic)
{
	if (logic == NULL)
	{
		return false;
	}

	*logic = (PongLogic_t){ 0 };

	PongLogic_calcAbsLeftPad(logic);
	PongLogic_calcAbsRightPad(logic);
	PongLogic_calcAbsBall(logic);

	return true;
}
void PongLogic_free(PongLogic_t * restrict logic)
{
	if (logic == NULL)
	{
		return;
	}

	PongLogic_freeAssets(logic);
}

bool PongLogic_createAssets(PongLogic_t * restrict logic, ID2D1RenderTarget * pRT)
{
	if (logic == NULL)
	{
		return false;
	}
	else if (logic->assetsCreated == true)
	{
		return true;
	}

	logic->pRT = pRT;




	logic->assetsCreated = true;
	return true;
}
void PongLogic_freeAssets(PongLogic_t * restrict logic)
{
	if (logic == NULL || logic->assetsCreated == false)
	{
		return;
	}
	logic->assetsCreated = false;


}

void PongLogic_calcAbsLeftPad(PongLogic_t * logic)
{
	if (logic == NULL)
	{
		return;
	}
	logic->absLeftPad = (D2D1_RECT_F){
		.left   = 0.0f,
		.top    = (PONG_MINH - PONG_WALL_Y) / 2.0f + logic->relLeftPad,
		.right  = PONG_WALL_X,
		.bottom = (PONG_MINH + PONG_WALL_Y) / 2.0f + logic->relLeftPad,
	};
}
void PongLogic_calcAbsRightPad(PongLogic_t * logic)
{
	if (logic == NULL)
	{
		return;
	}
	logic->absRightPad = (D2D1_RECT_F){
		.left   = PONG_MINW - PONG_WALL_X,
		.top    = (PONG_MINH - PONG_WALL_Y) / 2.0f + logic->relRightPad,
		.right  = PONG_MINW,
		.bottom = (PONG_MINH + PONG_WALL_Y) / 2.0f + logic->relRightPad
	};
}
void PongLogic_calcAbsBall(PongLogic_t * logic)
{
	if (logic == NULL)
	{
		return;
	}
	logic->absBall = (D2D1_POINT_2F){
		.x = PONG_MINW / 2.0f,
		.y = PONG_MINH / 2.0f
	};
}
