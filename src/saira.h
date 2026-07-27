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

//------------------------------------------------------------------------------------
// Macros and types for dealing with dynamic arrays
//------------------------------------------------------------------------------------
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
	void  **items;
	size_t count;
	size_t capacity;
}ptr_da;

// Game data for each game
typedef struct {
	uint16_t    width;
	uint16_t    height;
	uint8_t     fps;
	const char *title;
	const char **animationsDataPath;
}game;

// The background color used for every sprite
#define TRANSPARENT GetColor(0xFE01FDFF)

//------------------------------------------------------------------------------------
// Functions each game must implement
//------------------------------------------------------------------------------------
extern game InitGame();
extern void UpdateDrawGame(float fps);

//------------------------------------------------------------------------------------
// Below this point only functions and types implemented by the engine
//------------------------------------------------------------------------------------
extern void FillBackground(Color color);

//------------------------------------------------------------------------------------
// Animation system 
//------------------------------------------------------------------------------------
extern void PlayAnimation(const char *sheetName, const char *animName, bool repeat, size_t animID, bool stack, 
Vector2 dir, Vector2 pos);
extern void DrawAllAnimations();
extern bool LoadAnimationData(const char **dataFile);
extern bool IsAnimationPlaying(const char *sheetName, const char *animName, size_t animID);
extern size_t GetNewAnimation();

//------------------------------------------------------------------------------------
// Functions and types for tokenizing and parsing of .sgd files
//------------------------------------------------------------------------------------
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

typedef struct token token;
typedef struct{
	token *items;
	size_t count;
	size_t capacity;
}tokens;

extern tokens *GetAllTokens(const char *raw);
extern node   *ParseTokens(tokens *allTokens);
extern ptr_da GetNodes(const char *nodeName, node *searchArray);
extern const char *GetConstantValue(node *searchNode, const char *constantName);

#endif //_ENGINE_H
