#ifndef RESOURCE_H
#define RESOURCE_H

#include "winapi.h"

#define PONG_CLASSNAME	L"PongD2DClass"
#define PONG_APPNAME	L"PongD2D"

#define PONG_MINW 1024.0f
#define PONG_MINH 600.0f

#define PONG_BALL_X 15.0f
#define PONG_BALL_Y 15.0f

// Ball velocity in DIPs (device-independent pixels) per second
#define PONG_BALL_VELOCITY 300.0f
#define PONG_WALL_VELOCITY 300.0f

#define PONG_WALL_X 10.0f
#define PONG_WALL_Y 80.0f

#define PONG_WALL_MAX ((PONG_MINH - PONG_WALL_Y) / 2.0f)

#define VERSION_STR "1.1.1"
#define VERSION_SEQ 1,1,1


#endif
