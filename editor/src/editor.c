#include <stdio.h>

#define WINDOW_WIDTH  1360
#define WINDOW_HEIGHT  760
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
	if(!SDL_Init(SDL_INIT_VIDEO)){
		return SDL_APP_FAILURE;
	}

	if(!SDL_CreateWindowAndRenderer("Saíra editor", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MINIMIZED, &window, &renderer)){
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
	SDL_RenderPresent(renderer);
	
	SDL_FRect btn1 = {
		.x = WINDOW_WIDTH/2-50,
		.y = WINDOW_HEIGHT/4,
		.w = 100,
		.h = 100
	};

	SDL_FRect btn2 = {
		.x = WINDOW_WIDTH/2-50,
		.y = WINDOW_HEIGHT/4+120,
		.w = 100,
		.h = 100
	};

	SDL_FRect btn3 = {
		.x = WINDOW_WIDTH/2-50,
		.y = WINDOW_HEIGHT/4+240,
		.w = 100,
		.h = 100
	};

	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
	SDL_RenderFillRect(renderer, &btn1);

	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, SDL_ALPHA_TRANSPARENT);
	SDL_RenderFillRect(renderer, &btn2);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, SDL_ALPHA_TRANSPARENT);
	SDL_RenderFillRect(renderer, &btn3);

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
	switch(event->type){
		case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
		case SDL_EVENT_KEY_DOWN:{
			if(event->key.key == SDLK_ESCAPE)
				return SDL_APP_SUCCESS;
			else return SDL_APP_CONTINUE;
		}
		default: return SDL_APP_CONTINUE;
	}
	
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){

}
