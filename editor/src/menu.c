#include "editor.h"

buttons allButtons = {0};

void ButtonCallback(){
	printf("A callback has been called!\n");
}

void InitMenu(editor_state *state){
	state->Update  = UpdateMenu;
	state->Exit    = ExitMenu;
	state->id      = MENU;
	
	// Buttons
	AddButton(&allButtons, WINDOW_WIDTH/2-32-72, WINDOW_HEIGHT/2, 64, 64, "./assets/duck.png", ButtonCallback);
	AddButton(&allButtons, WINDOW_WIDTH/2-32, WINDOW_HEIGHT/2, 64, 64, "./assets/duck.png", ButtonCallback);
	AddButton(&allButtons, WINDOW_WIDTH/2-32+72, WINDOW_HEIGHT/2, 64, 64, "./assets/duck.png", ButtonCallback);

}

editor_state_id UpdateMenu(SDL_Renderer *renderer){
	SDL_FRect r = {
		.x = 0,
		.y = 0,
		.w = WINDOW_WIDTH,
		.h = WINDOW_HEIGHT
	};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_TRANSPARENT);
	SDL_RenderRect(renderer, &r);
	

	DrawAllButtons(&allButtons);
	UpdateButtons(&allButtons);
	
	return MENU;
}

void ExitMenu(){

}

