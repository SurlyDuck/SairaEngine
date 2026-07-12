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
	uint8*_t frames;
	uint8_t fps;
	Vector2 sheetSize;
	Vector2 n;
	Vector2 s;
	Vector2 e;
	Vector2 w;
	Vector2 ne;
	Vector2 nw;
	Vector2 se;
	Vector2 sw;
}animation;

/********************************************* GAME *********************************************/
extern game InitGame();
extern void UpdateGame(float fps);

/********************************************* GAME *********************************************/


/******************************************** ENGINE ********************************************/
extern void FillBackground(Color color);
extern bool LoadAnimationSheets(const char **dataFile);
extern animation GetAnimation(const char *sheetName);
extern void PlayAnimation(animation anim, const char *animName, bool repeat, bool stack, Vector2 pos);

/******************************************* ENGINE ********************************************/


#endif //_ENGINE_H
