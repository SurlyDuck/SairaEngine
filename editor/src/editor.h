/*********************************************************************
*							Saíra engine main editor
*
* Update and initialization of functions for each mode/state
* must be declared here.
* 
* editor.c updates each state/mode using function pointers.
* A new state is assigned by returning a value from update routine.
*
* ui.c contains high level declarations for dealing with
* gui elements like buttons, dropdown menus, lists etc.
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
#define _XOPEN_SOURCE 500
#endif

#define VERSION "0.0.1"
#define RELEASE "alpha"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#define WINDOW_WIDTH  1360
#define WINDOW_HEIGHT  730

//------------------------------------------------------------------------------------
// Globals
//------------------------------------------------------------------------------------
// ...

//------------------------------------------------------------------------------------
// functions implemented in editor.c
//------------------------------------------------------------------------------------
SDL_Renderer *GetRenderer();
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
extern editor_state_id UpdateMenu(SDL_Renderer *renderer);

//------------------------------------------------------------------------------------
// ui elements
//------------------------------------------------------------------------------------
typedef enum{
	BUTTON_IDLE = 0,
	BUTTON_HOVERED,
	BUTTON_PRESSED
}button_state;

typedef struct button button;
typedef struct{
	button *items;
	size_t count;
	size_t capacity;
}buttons;

void AddButton(buttons *buttonsArray, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *filePath, void (*CallBack)());
void DrawAllButtons(buttons *buttonsArray);
void UpdateButtons(buttons *buttonsArray);

#endif // EDITOR_H
