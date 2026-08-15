#include "editor.h"

#define WHITE (SDL_Color){255, 255, 255, SDL_ALPHA_TRANSPARENT}

// locals
static buttons allButtons        = {0};
static labels  allLabels         = {0};
static editor_state_id nextState = MENU;

// local forward declaration
static editor_state_id UpdateMenu(SDL_Renderer *renderer);
static void _OnLevelClick();
static void _OnWorldClick();
static void _OnTimelineClick();

void InitMenu(editor_state *state){
	state->Update  = UpdateMenu;
	state->Exit    = ExitMenu;
	state->id      = MENU;
	nextState      = MENU;

	// Buttons
	AddButton(&allButtons, WINDOW_WIDTH/2-64-128-32, WINDOW_HEIGHT/2-64, 128, 128, "./assets/spoon.png", 
				 "./assets/spoonHover.png", "./assets/spoonPressed.png", _OnLevelClick);

	AddButton(&allButtons, WINDOW_WIDTH/2-64, WINDOW_HEIGHT/2-64, 128, 128, "./assets/globe.png", 
	"./assets/globeHover.png", "./assets/globePressed.png", _OnWorldClick);

	AddButton(&allButtons, WINDOW_WIDTH/2-64+128+32, WINDOW_HEIGHT/2-64, 128, 128, "./assets/clock.png", 
	"./assets/clockHover.png", "./assets/clockPressed.png", _OnTimelineClick);
	
	// Labels
	AddLabel("not centered", &allLabels, WINDOW_WIDTH/2, 0, monoRegularLarge, WHITE, 0);
	AddLabel("centered", &allLabels, WINDOW_WIDTH/2, 40, monoRegularLarge, WHITE, MIDDLE_CENTER);
	AddLabel(VERSION RELEASE, &allLabels, WINDOW_WIDTH, WINDOW_HEIGHT, monoRegularSmall, WHITE, BOTTOM_RIGHT);

	//SDL_Surface *titleSurface = TTF_RenderText_Blended(monoRegularLarge, "Hello", 0, WHITE);
	//menuTitle = SDL_CreateTextureFromSurface(GetRenderer(), titleSurface);
	//SDL_DestroySurface(titleSurface);

}

// TODO: renderer is now a global; no use of it as a parameter
editor_state_id UpdateMenu(SDL_Renderer *renderer){
	SDL_FRect r = {
		.x = 0,
		.y = 0,
		.w = WINDOW_WIDTH,
		.h = WINDOW_HEIGHT
	};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_TRANSPARENT);
	SDL_RenderRect(renderer, &r);
	
	//SDL_RenderTexture(GetRenderer(), menuTitle, NULL, &(SDL_FRect){WINDOW_WIDTH/2-menuTitle->w/2, 
	//WINDOW_HEIGHT/2 - 50, menuTitle->w, menuTitle->h});

	DrawAllButtons(&allButtons);
	UpdateButtons(&allButtons);
	
	DrawAllLabels(&allLabels);

	return nextState;
}

static void _OnLevelClick(){
	printf("level click\n");
	nextState = LEVEL;
}

static void _OnWorldClick(){
	printf("world click\n");
}

static void _OnTimelineClick(){
	printf("timeline click\n");
}



void ExitMenu(){
	printf("exiting menu... \n");
	DestroyButtons(&allButtons);
	DestroyLabels(&allLabels);
}

