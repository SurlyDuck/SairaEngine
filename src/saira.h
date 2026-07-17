	#ifndef _ENGINE_H
	#define _ENGINE_H
	#include "./include/raylib.h"
	#include "./include/raymath.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdbool.h>
	#include <stdint.h>
	#include <string.h>
	#include <errno.h>

	#define DA_APPEND(item, array) do{ \
		\
	}while(0)

	typedef struct {
		uint16_t    width;
		uint16_t    height;
		uint8_t     fps;
		const char *title;
		const char **animationsDataPath;
	}game;


	/********************************************* GAME *********************************************/
	extern game InitGame();
	extern void UpdateGame(float fps);


	/******************************************** ENGINE ********************************************/
	extern void FillBackground(Color color);


	/* ------- Saíra animation system ------- */
	typedef struct animation animation;
	extern void PlayAnimation(const char *sheetTitle, const char *animName, bool repeat, bool stack, Vector2 dir);
	extern bool LoadAnimationData(const char **dataFile);

/* ------- sgd parsing library -------*/
typedef struct tokens tokens;

typedef struct{
	const char *name;
	const char *value;
}constant;

typedef struct node node;
struct node{
	const char *name;
	size_t childrenCount;
	size_t constantCount;
	constant **constants;
	node **children;
};

typedef struct{
	node *items;
	size_t count;
	size_t capacity;
}nodes;

extern tokens *GetAllTokens(const char **raw);
extern nodes  ParseTokens(tokens *allTokens);


#endif //_ENGINE_H
