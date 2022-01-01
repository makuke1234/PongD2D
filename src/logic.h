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
	uint8_t winnerIdx;

} Scoring_t;

struct PongWnd;

typedef struct PongLogic
{
	struct PongWnd * pong;
	ID2D1RectangleGeometry * pUpWallGeo, * pDownWallGeo;
	ID2D1RectangleGeometry * pLeftWallGeo, * pRightWallGeo;

	bool assetsCreated;

	Scoring_t scoring;
	volatile bool requestReset;

	HANDLE logicThread;
	bool killThreadFlag;

} PongLogic_t;

/**
 * @brief Thread method that deals with all active game logic
 * 
 * @param param Pointer to PongLogic_t object
 * @return DWORD Always 0
 */
DWORD WINAPI PongLogic_thread(LPVOID param);

/**
 * @brief Creates PongLogic_t object
 * 
 * @param logic Pointer to PongLogic_t structure
 * @param pong Pointer to PongWnd structure
 * @return true On success
 * @return false On failure
 */
bool PongLogic_create(PongLogic_t * restrict logic, struct PongWnd * pong);
/**
 * @brief Frees resources owned by PongLogic_t object
 * 
 * @param logic Pointer to PongLogic_t object
 */
void PongLogic_free(PongLogic_t * restrict logic);

/**
 * @brief Creates D2D assets
 * 
 * @param logic Pointer to PongLogic_t object
 * @return true On success
 * @return false On failure
 */
bool PongLogic_createAssets(PongLogic_t * restrict logic);
/**
 * @brief Destryos D2D assets
 * 
 * @param logic Pointer to PongLogic_t object
 */
void PongLogic_freeAssets(PongLogic_t * restrict logic);

/**
 * @brief Calculates absolute size rectangle of left pad
 * 
 * @param logic Pointer to PongLogic_t object
 */
void PongLogic_calcAbsLeftPad(PongLogic_t * restrict logic);
/**
 * @brief Calculates absolute size rectangle of right pad
 * 
 * @param logic Pointer to PongLogic_t object
 */
void PongLogic_calcAbsRightPad(PongLogic_t * restrict logic);
/**
 * @brief Calculates absolute location of ball ellipse
 * 
 * @param logic Pointer to PongLogic_t object
 */
void PongLogic_calcAbsBall(PongLogic_t * restrict logic);

/**
 * @brief Resets game logic to default state
 * 
 * @param logic Pointer to PongLogic_t object
 */
void PongLogic_reset(PongLogic_t * logic);

/**
 * @brief "Clamps" a floating-point value to be within min and max
 * 
 * @param value Floating-point value to be clamped
 * @param min Minimum value
 * @param max Maximum value
 * @return float Clamped value
 */
float clamp(float value, float min, float max);

#endif
