#ifndef _ENGINE_H
#define _ENGINE_H
#include "./include/raylib.h"
#include "./include/raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#define DA_APPEND(item, array) do{\
	if(array.count == 0){\
		array.items = (void*) malloc(sizeof(item));\
		assert(array.items != NULL); \
	}else if(array.capacity < sizeof(item) * (size_t)(array.count+1)){\
		array.items = (void*) realloc(array.items, sizeof(item) * (size_t)(array.count + 1));\
	}\
	array.items[array.count] = item;\
	array.count++;\
	array.capacity = sizeof(item) * array.count;\
}while(0)

#define DA_PUSH DA_APPEND

#define DA_POP(array) do{\
	size_t itemCap = sizeof(array.items[0]);\
	array.count -= 1;\
	array.capacity -= itemCap; \
	if(array.count > 0) \
		array.items = (void*) realloc(array.items, itemCap * array.count);\
	else free(array.items);\
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

typedef struct{
	const char *name;
	const char *value;
}constant;

typedef struct node node;
struct node{
	const char *name;
	size_t childrenCount;
	size_t constantCount;
	constant *constants;
	node **children;
};

typedef struct{
	node *items;
	size_t count;
	size_t capacity;
}nodes;

typedef struct token token;
typedef struct{
	token *items;
	size_t count;
	size_t capacity;
}tokens;

/* Warning: calling GetAllTokens() will change some values from nodes in the node array. */
/* Only call it after you've already used the node tree returned by ParseTokens()        */
extern tokens *GetAllTokens(const char *raw);
extern nodes  ParseTokens(tokens *allTokens);

#endif //_ENGINE_H
