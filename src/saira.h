#ifndef _ENGINE_H
#define _ENGINE_H
#include "./include/raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
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
#define TRANSPARENT 0xFE01FDFF

// 8 Directions every graphic object may face
enum{
	NORTH = 0,      
	SOUTH,      
	EAST,      
	WEST,      
	NORTHEAST,  
	NORTHWEST,  
	SOUTHEAST,  
	SOUTHWEST  
};

// 8 Directions every graphic object may face as a 2d vector
#define NORTH_VEC     (Vector2){0, -1}
#define SOUTH_VEC     (Vector2){0,  1}
#define EAST_VEC      (Vector2){1,  0}
#define WEST_VEC      (Vector2){-1, 0}
#define NORTHEAST_VEC (Vector2){1, -1}
#define NORTHWEST_VEC (Vector2){-1,-1}
#define SOUTHEAST_VEC (Vector2){1,  1}
#define SOUTHWEST_VEC (Vector2){-1, 1}


//------------------------------------------------------------------------------------
// Functions each game must implement
//------------------------------------------------------------------------------------
extern game InitGame();
extern void UpdateDrawGame(float deltaTime);

//------------------------------------------------------------------------------------
// Below this point only functions and types implemented by the engine
//------------------------------------------------------------------------------------
extern void FillBackground(size_t color);
extern void SairaPanic();
extern int GetDirectionFromVector(Vector2 vec);
extern int GetGameWidth();
extern int GetGameHeight();

//------------------------------------------------------------------------------------
// Animation system 
//------------------------------------------------------------------------------------
typedef struct ANIMATION_CONTAINER ANIMATION_CONTAINER;
extern void DrawAllAnimationContainers();
extern void SetAnimation(ANIMATION_CONTAINER *container, const char *sheetName, const char *animName, bool repeat);
extern void SetAnimationFrame(ANIMATION_CONTAINER *container, size_t frame);
extern void SetAnimationVisibility(ANIMATION_CONTAINER *container, bool visible);
extern void PlayAnimation(ANIMATION_CONTAINER *container, bool repeat);
extern void MoveAnimation(ANIMATION_CONTAINER *container, Vector2 position, Vector2 direction);
extern bool LoadAnimationData(const char **dataFile);
extern bool IsAnimationSheet(ANIMATION_CONTAINER *container, const char *sheetName);
extern bool IsAnimationName(ANIMATION_CONTAINER *container, const char *animName);
extern bool IsAnimationVisible(ANIMATION_CONTAINER *container);
extern bool IsAnimationPlaying(ANIMATION_CONTAINER *container);
extern ANIMATION_CONTAINER* GetAnimationContainer();
extern Vector2 GetAnimationPosition(ANIMATION_CONTAINER *container);

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

//------------------------------------------------------------------------------------
// Functions and types for the 2d camera
//------------------------------------------------------------------------------------
extern void InitCamera(Vector2 position);
extern void CenterCamera(Vector2 position);
extern void FlyCamera(Vector2 position, float speed);
extern void ShakeCamera(float speed, float duration);
extern void SetCameraZoom(float zoom, bool smooth);
extern void BeginModeCamera2D();


//------------------------------------------------------------------------------------
// Logging
//------------------------------------------------------------------------------------
enum{ 
    SAIRA_INFO = 0,       // Info logging, used for program execution info
    SAIRA_WARNING,        // Warning logging, used on recoverable failures
    SAIRA_ERROR,          // Error logging, used on unrecoverable failures
    SAIRA_FATAL,          // Fatal logging, used to abort program: exit(EXIT_FAILURE)
};

extern void SairaLog(int logLevel, const char *msg, ...);

#endif //_ENGINE_H
