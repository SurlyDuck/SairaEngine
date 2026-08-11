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
	

	SDL_Surface *surface = IMG_Load("./assets/duck.png");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

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
			
		SDL_RenderTexture(renderer, texture, NULL, NULL);
		
		SDL_RenderPresent(renderer);
		
		// TODO: proper fps target
		usleep(16 * 1000);
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
		case FATAL_ERROR: exit(69);
		default: break;
	}
}

SDL_Renderer *GetRenderer(){
	return renderer;
}


