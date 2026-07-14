#ifndef _ENGINE_H
#define _ENGINE_H
#include "./include/raylib.h"
#include "./include/raymath.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint16_t    width;
	uint16_t    height;
	uint8_t     fps;
	const char *title;
	const char **animationsDataPath;
}game;

typedef struct animation animation;

/********************************************* GAME *********************************************/
extern game InitGame();
extern void UpdateGame(float fps);

/********************************************* GAME *********************************************/


/******************************************** ENGINE ********************************************/
extern void FillBackground(Color color);
extern void PlayAnimation(const char *sheetTitle, const char *animName, bool repeat, bool stack, Vector2 dir);
extern bool LoadAnimationData(const char **dataFile);
/******************************************* ENGINE ********************************************/


#endif //_ENGINE_H
