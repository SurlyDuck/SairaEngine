#ifndef _ENGINE_H
#define _ENGINE_H
#include "./include/raylib.h"
#include "./include/raymath.h"
#include "./include/raymath.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint16_t    width;
	uint16_t    height;
	uint8_t     fps;
	const char *title;
}game;

typedef struct{
	const char *sheetName;
	const char *filePath;
	const char *animName;
	uint8_t frames;
	uint8_t fps;
	Vector2 n;
	Vector2 s;
	Vector2 e;
	Vector2 w;
	Vector2 ne;
	Vector2 nw;
	Vector2 se;
	Vector2 sw;
	Vector2 sheetSize;
}animation;

/********************************************* GAME *********************************************/
extern game InitGame();
extern void UpdateGame(float fps);

/********************************************* GAME *********************************************/


/******************************************** ENGINE ********************************************/
extern void FillBackground(Color color);
extern bool LoadAnimationSheets(const char **dataFile);
extern void PlayAnimation(const char *sheetTitle, const char *animName, bool repeat, bool stack, Vector2 dir);

/******************************************* ENGINE ********************************************/


#endif //_ENGINE_H
