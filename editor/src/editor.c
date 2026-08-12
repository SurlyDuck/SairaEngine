#include "editor.h"

#define MAX_INPUT_EVENT_BUFFER_LEN 32

// Locals
static SDL_Event inputEventBuffer[MAX_INPUT_EVENT_BUFFER_LEN] = {0};
static uint8_t inputEventBufferPtr = 0;
static SDL_Window   *window        = NULL;
static SDL_Renderer *renderer      = NULL;

// Globals
editor_state currentState = {0};
TTF_Font *font    = NULL;
SDL_Surface *text    = NULL;

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
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text);
	//SDL_Surface *surface = IMG_Load("./assets/duck.png");
	//SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	currentState.queueID = MENU;
	bool running = true;
	while(running){ // <-- Main loop -->
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_EVENT_QUIT || event.key.key == SDLK_ESCAPE){
				running = false;
			  	break;
			}else if(event.type == SDL_EVENT_KEY_DOWN || SDL_EVENT_MOUSE_BUTTON_DOWN){
				AddInputEventBuffer(event);
			}else continue;
		}
		
		SDL_SetRenderDrawColor(renderer, 18, 18, 18, SDL_ALPHA_TRANSPARENT);
		SDL_RenderClear(renderer);
		
		if(currentState.queueID != currentState.id){
			if(currentState.Exit != NULL) currentState.Exit();
			InitNewState(currentState.queueID);
		}

		currentState.queueID = currentState.Update(renderer);
				
		//SDL_RenderTexture(renderer, texture, NULL, NULL);
		SDL_RenderTexture(renderer, texture, NULL, NULL);
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
		case FATAL_ERROR: exit(69);
		default: break;
	}
}

// Just testing text rendering on this mess of an api
static void LoadFonts(){
	FILE *fontFile    = NULL;
	size_t fileLen    = 0;
	const char *raw   = NULL;

	fontFile = fopen("./assets/LiberationMono-Regular.ttf", "rb");
	assert(fontFile != NULL);

	fseek(fontFile, 0, SEEK_END);
	fileLen = ftell(fontFile);
	assert(fileLen > 0);
	fseek(fontFile, 0, SEEK_SET);
		
	raw = (const char*) malloc(fileLen);
	size_t res = fread((char*)raw, 1, fileLen, fontFile);
	assert(res != 0);
	
	font = TTF_OpenFontIO(SDL_IOFromMem((void*)raw, fileLen), true, 10.0f);
	assert(font);

	text = TTF_RenderText_Blended(font, "jesus please  work aarrgghhh", 0, (SDL_Color) { 255, 0, 0, SDL_ALPHA_TRANSPARENT});
	
	fclose(fontFile);

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


