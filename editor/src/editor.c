#include "editor.h"

// Globals
static SDL_Window *window            = NULL;
static SDL_Renderer *renderer        = NULL;
editor_state currentState            = {0};

// Func foward declarations
static void InitNewState(editor_state_id newState);

int main(){
	if(!SDL_Init(SDL_INIT_VIDEO)){
		return SDL_APP_FAILURE;
	}
	
	if(!SDL_CreateWindowAndRenderer("Saíra editor", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)){
		return SDL_APP_FAILURE;
	}
	
	currentState.queueID = MENU;
	bool running = true;
	while(running){ // Main loop
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_EVENT_QUIT || event.key.key == SDLK_ESCAPE){
				running = false;
			  	break;
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 18, 18, 18, SDL_ALPHA_TRANSPARENT);
		SDL_RenderClear(renderer);
		
		if(currentState.queueID != currentState.id){
			if(currentState.Exit != NULL) currentState.Exit();
			InitNewState(currentState.queueID);
		}

		currentState.queueID = currentState.Update(renderer);
		
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();

	return 0;
}

static void InitNewState(editor_state_id newState){
	switch(newState){
		case MENU:{
			InitMenu(&currentState);
			break;
		}
		default: break;
	}
}




