#include "editor.h"

const char *gridValues[] = {
	"16x16",
	"32x32",
	"64x64",
	"128x128",
	"256x256",
	"512x512",
	"1024x1024",
	NULL
};

const char *directories[] = {
	"Load Walls",
	"Load floors",
	NULL
};

// Locals
editor_state_id nextState;
static uint16_t gridWidth         = 16;
static uint16_t gridHeight        = 16;
static uint16_t tileWidth         = 128;
static uint16_t tileHeight        = 64;
static SDL_Texture *gridTargetTexture  = NULL;
static SDL_FRect gridScreenRect = {0};
static SDL_FRect gridTargetRect = {0};

// Local foward declarations
static void _OnButtonBack();
static void _OnButtonGridSize();
static void _OnButtonLoadTileset();
static void _OnChangeGridSize(const char *newVal);

void ExitLevel();
editor_state_id UpdateLevel(SDL_Renderer *renderer);


void InitLevel(editor_state *state){
	state->Update  = UpdateLevel;
	state->Exit    = ExitLevel;
	state->id      = LEVEL;
	nextState      = LEVEL;
		
	AddStdButton(0, 0, 30, 40, "<<", monoRegularMedium, _OnButtonBack);
	AddStdButton(100, 0, 92, 40, "TILESET", monoRegularMedium, _OnButtonLoadTileset);
	AddStdButton(200, 0, 92, 40, "MAP", monoRegularMedium, _OnButtonBack);
	AddStdButton(300, 0, 92, 40, "OUT", monoRegularMedium, _OnButtonBack);
	AddStdButton(400, 0, 92, 40, "SIZE", monoRegularMedium, _OnButtonGridSize);
	
	gridTargetRect.w = WINDOW_WIDTH;
	gridTargetRect.h = WINDOW_HEIGHT - 200;
	gridTargetRect.x = 0;
	gridTargetRect.y = 0;

	gridScreenRect.w = WINDOW_WIDTH;
	gridScreenRect.h = WINDOW_HEIGHT - 200;
	gridScreenRect.y = 200;
	
	int textureWidth = gridWidth * tileWidth;
	int textureHeight = gridHeight * tileHeight;
	gridTargetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textureWidth, textureHeight);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_SetRenderTarget(renderer, gridTargetTexture); // Starts rendering the grid
	SDL_RenderRect(renderer, &(SDL_FRect){0,0,100,100});
	SDL_SetRenderTarget(renderer, NULL); // Stops rendering the grid
}

float y = 0;
editor_state_id UpdateLevel(SDL_Renderer *renderer){
	// Screen margins
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderRect(renderer, &gridScreenRect);
	
	SDL_RenderTexture(renderer, gridTargetTexture, &gridTargetRect, &gridScreenRect);
	
	y +=.01;
	gridTargetRect.y += y;
	gridTargetRect.x += y;

	UpdateGuiElements();
	return nextState;
}

static void _OnChangeGridSize(const char *newVal){
	printf("update grid size to: %s \n", newVal);
}

static void _OnButtonBack(){
	nextState = MENU;
}


static void _OnButtonLoadTileset(){
	ShowContextMenu(100, 48, 200, 200, directories, _OnChangeGridSize, TOP_LEFT);
}

static void _OnButtonGridSize(){
	ShowContextMenu(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 200, 200, gridValues, _OnChangeGridSize, MIDDLE_CENTER);
}

void ExitLevel(){
	DestroyGuiElements();
	SDL_DestroyTexture(gridTargetTexture);
}

