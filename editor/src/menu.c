#include "editor.h"

void InitMenu(editor_state *state){
	state->Update  = UpdateMenu;
	state->Exit    = ExitMenu;
	state->id      = MENU;

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

	
	return MENU;
}

void ExitMenu(){

}

