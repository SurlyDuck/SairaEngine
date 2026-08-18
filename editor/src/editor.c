#include "editor.h"

#define MAX_INPUT_EVENT_BUFFER_LEN 32

// Locals
static SDL_Event inputEventBuffer[MAX_INPUT_EVENT_BUFFER_LEN] = {0};
static uint8_t inputEventBufferPtr = 0;
static SDL_Window   *window        = NULL;

// Globals
editor_state currentState    = {0};
TTF_Font *monoRegularLarge   = NULL;
TTF_Font *monoRegularSmall   = NULL;
TTF_Font *monoRegularMedium  = NULL;
SDL_Renderer *renderer       = NULL;

// Func foward declarations
static void InitNewState(editor_state_id newState);
static void AddInputEventBuffer(SDL_Event event);
static void ClearInputEventBuffer(void);
static void LoadFonts();
SDL_Event *GetInputEvents(void);

int main(){
	// TODO: error treatment
	if(!SDL_Init(SDL_INIT_VIDEO)){
		return SDL_APP_FAILURE;
	}
	
	if(!SDL_CreateWindowAndRenderer("Saíra editor", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)){
		return SDL_APP_FAILURE;
	}

	if(!TTF_Init()){
		return SDL_APP_FAILURE;
	}
	
	LoadFonts();

	currentState.queueID = MENU;
	bool running = true;
	while(running){ // <-- Main loop -->
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_EVENT_QUIT){
				running = false;
			  	break;
			}else if(event.type == SDL_EVENT_KEY_DOWN || SDL_EVENT_MOUSE_BUTTON_DOWN){
				AddInputEventBuffer(event);
				if(event.key.key == SDLK_ESCAPE && !IsGuiBusy()){ 
					running = false;
					break;
				}
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 18, 18, 18, SDL_ALPHA_TRANSPARENT);
		SDL_RenderClear(renderer);
		
		if(currentState.queueID != currentState.id){
			if(currentState.Exit != NULL) currentState.Exit();
			InitNewState(currentState.queueID);
		}

		currentState.queueID = currentState.Update(renderer);
				
		//SDL_RenderTexture(renderer, texture, NULL, NULL);
		//SDL_RenderTexture(renderer, texture, NULL, &(SDL_FRect){WINDOW_WIDTH/2-texture->w/2,WINDOW_HEIGHT/2 -50, texture->w, texture->h});
		SDL_RenderPresent(renderer);
		
		ClearInputEventBuffer();
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
		case LEVEL: {
			InitLevel(&currentState);
			break;
		}
		case FATAL_ERROR: exit(69);
		default: break;
	}
}

static void LoadFonts(){
	monoRegularLarge = TTF_OpenFont("./assets/LiberationMono-Regular.ttf", 30.0f);
	assert(monoRegularLarge);
	monoRegularMedium = TTF_OpenFont("./assets/LiberationMono-Regular.ttf", 20.0f);
	assert(monoRegularMedium);
	monoRegularSmall = TTF_OpenFont("./assets/LiberationMono-Regular.ttf", 15.0f);
	assert(monoRegularSmall);

}

static void AddInputEventBuffer(SDL_Event event){
	if(inputEventBufferPtr >= MAX_INPUT_EVENT_BUFFER_LEN-1) return;

	inputEventBuffer[inputEventBufferPtr++] = event;
}

static void ClearInputEventBuffer(void){
	memset(inputEventBuffer, 0, sizeof(inputEventBuffer));
	inputEventBufferPtr = 0;
}


SDL_Event *GetInputEvents(void){
	return inputEventBuffer;
	
}

SDL_Renderer *GetRenderer(){
	return renderer;
}


