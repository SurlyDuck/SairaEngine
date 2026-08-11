/*********************************************************************
*							Saíra engine main editor
*
* Update and initialization of functions for for each mode/state
* must be declared here.
* 
* editor.c updates each state/mode using function pointers.
* A new state is assigned by returning a value from update routine.
*
* ui.h contains high level declarations for dealing with
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

#include <stdlib.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include "ui.h"
#define WINDOW_WIDTH  1360
#define WINDOW_HEIGHT  760

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

#endif // EDITOR_H
