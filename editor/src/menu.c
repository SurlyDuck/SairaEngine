#include "editor.h"

buttons allButtons = {0};

void ButtonCallback(){
	printf("A callback has been called!");
}

void InitMenu(editor_state *state){
	state->Update  = UpdateMenu;
	state->Exit    = ExitMenu;
	state->id      = MENU;
	
	// Buttons
	AddButton(&allButtons, 0, 0, 64, 64, "./assets/duck.png", ButtonCallback);
	AddButton(&allButtons, 64, 0, 64, 64, "./assets/duck.png", ButtonCallback);
	AddButton(&allButtons, 128, 0, 64, 64, "./assets/duck.png", ButtonCallback);

}

editor_state_id UpdateMenu(SDL_Renderer *renderer){
	SDL_FRect r = {
		.x = 0,
		.y = 0,
		.w = WINDOW_WIDTH,
		.h = 200
	};
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_TRANSPARENT);
	SDL_RenderFillRect(renderer, &r);
	
	DrawAllButtons(&allButtons);
	UpdateButtons(&allButtons);
	
	return MENU;
}

void ExitMenu(){

}

