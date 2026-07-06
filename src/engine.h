#ifndef _ENGINE_H
#define _ENGINE_H
#include "./include/raylib.h"
#include "./include/raymath.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint16_t    width;
	uint16_t    height;
	uint8_t     fps;
	const char *title;
}game;


/********************************************* GAME *********************************************/
extern game InitGame();
extern void UpdateGame(float fps);

/********************************************* GAME *********************************************/


/******************************************** ENGINE ********************************************/
extern void FillBackground(Color color);

/******************************************** ENGINE ********************************************/


#endif //_ENGINE_H
