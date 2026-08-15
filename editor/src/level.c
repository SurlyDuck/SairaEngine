#include "editor.h"

// Locals
editor_state_id nextState;

// Local foward declarations
editor_state_id UpdateLevel(SDL_Renderer *renderer);

void InitLevel(editor_state *state){
	state->Update  = UpdateLevel;
	state->Exit    = ExitLevel;
	state->id      = LEVEL;
	nextState      = LEVEL;
}

editor_state_id UpdateLevel(SDL_Renderer *renderer){
	
	return nextState;
}

void ExitLevel(){
// defer
}

