/*********************************************************************
*							Saíra engine main editor
*
* Update and initialization of functions for each mode/state
* must be declared here.
* 
* editor.c updates each state/mode using function pointers.
* A new state is assigned by returning a value from update routine.
*
* ui.c contains low level implementations for dealing with
* gui elements like buttons, dropdown menus, labels etc.
*
* Modes
*		- Starting: goes straight to one of the modes below
*		- Menu: main menu to choose between modes/states
*		- Level: isometric tile level editor
*		- World: top-down open world editor
*		- timeline: Event and quests editor for the open world
*
**********************************************************************/
#ifndef _EDITOR_H
#define _EDITOR_H

#ifndef WIN_32
#define _XOPEN_SOURCE 600
#endif

#define VERSION "0.0.1"
#define RELEASE "alpha"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_process.h>

#define WINDOW_WIDTH  1360
#define WINDOW_HEIGHT  730

#define WHITE (SDL_Color){255, 255, 255, SDL_ALPHA_OPAQUE}
#define RED (SDL_Color){255, 0, 0, SDL_ALPHA_OPAQUE}

//------------------------------------------------------------------------------------
// Macros for dealing with dynamic arrays
//------------------------------------------------------------------------------------
#define DA_PUSH(item,array) DA_APPEND(item, array)
#define DA_APPEND(item, array) do {\
	if(array->count == 0){\
		array->items = (void*) malloc(sizeof(item) * 8);\
		array->capacity = 8 * sizeof(item);\
	}else if(array->capacity < sizeof(item) * (array->count+1)){\
		array->items = (void*) realloc(array->items, sizeof(item) * (array->capacity+8));\
		array->capacity += 8 * sizeof(item);\
	}\
	\
	array->items[array->count] = item;\
	array->count++;\
}while(0)
// TODO: DA_POP

#define DA_CLEAR(array) do{\
	free(array->items);\
	array->items = NULL;\
	array->count = 0;\
	array->capacity = 0;\
}while(0)

//------------------------------------------------------------------------------------
// Globals
//------------------------------------------------------------------------------------
extern TTF_Font *monoRegularLarge;
extern TTF_Font *monoRegularMedium;
extern TTF_Font *monoRegularSmall;
extern SDL_Renderer *renderer;

//------------------------------------------------------------------------------------
// functions implemented in editor.c
//------------------------------------------------------------------------------------
SDL_Renderer *GetRenderer(); // deprecated
SDL_Event *GetInputEvents(void);

//------------------------------------------------------------------------------------
// Types used to change between states
//------------------------------------------------------------------------------------
typedef enum{
	STARTING = 0,
	MENU,
	LEVEL,
	WORLD,
	TIMELINE,
	FATAL_ERROR
}editor_state_id;

typedef struct{
	editor_state_id (*Update)();
	void (*Exit)();
	editor_state_id id;
	editor_state_id queueID;
}editor_state;

//------------------------------------------------------------------------------------
// Main menu functions
//------------------------------------------------------------------------------------
extern void ExitMenu();
extern void InitMenu(editor_state *state);

//------------------------------------------------------------------------------------
// Level editor functions
//------------------------------------------------------------------------------------
extern void ExitLevel();
extern void InitLevel(editor_state *state);


//------------------------------------------------------------------------------------
// gui elements
//------------------------------------------------------------------------------------
typedef enum{
	TOP_LEFT = 0,
	TOP_CENTER,
	TOP_RIGHT,
	MIDDLE_LEFT,
	MIDDLE_CENTER,
	MIDDLE_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_CENTER,
	BOTTOM_RIGHT,
}origin;

typedef struct CONTAINER CONTAINER;

extern CONTAINER *InitContainer();
extern void UpdateContainer(CONTAINER *container);
extern void DestroyContainer(CONTAINER *container);
extern void AddStdButtonToContainer(CONTAINER *container);
extern void AddLabelButtonToContainer(CONTAINER *container);
extern void AddButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *idleTexture,const char *hoverTexture, const char *pressedTexture, void (*CallBack)());
extern void AddStdButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *text, TTF_Font *font, void (*Callback)());
extern void AddLabel(const char *text, uint16_t x, uint16_t y, TTF_Font *font, SDL_Color color, origin or);
extern void UpdateGuiElements();
extern void DestroyGuiElements();
extern void ShowContextMenu(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *list[], void (*Callback)(const char *val), origin or);
extern bool IsGuiBusy();

#endif // EDITOR_H
