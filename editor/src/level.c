#include "editor.h"
#include "./nfd/nfd.h"

#define CAMERA_SPEED 900

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
	"Walls",
	"Floors",
	"Misc.",
	NULL
};

// Locals
editor_state_id nextState;
static uint16_t gridWidth      = 16; // Maybe move grid variables into a `world` structure?
static uint16_t gridHeight     = 16;
static uint16_t tileWidth      = 128;
static uint16_t tileHeight     = 64; 
static uint16_t textureWidth;
static uint16_t textureHeight;
static SDL_Texture *gridTargetTexture  = NULL;
static SDL_FRect gridScreenRect = {0};
static SDL_FRect gridTargetRect = {0};

// Local foward declarations
static void DrawTile(uint16_t x, uint16_t y, uint8_t w, uint8_t h);
static void DrawGrid();
static void _OnButtonBack();
static void _OnButtonGridSize();
static void _OnButtonLoadTileset();
static void _OnChangeGridSize(const char *newVal);
static void _OnLoadTileset(const char *newVal);
static bool IsMouseBeingDragged(float *mouseXMotion, float *mouseYMotion);

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
	
	textureWidth = gridWidth * tileWidth + tileWidth * 2;
	textureHeight = gridHeight * tileHeight + tileHeight * 2;
	gridTargetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textureWidth, textureHeight);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_SetRenderTarget(renderer, gridTargetTexture); // Starts rendering the grid
	DrawGrid();
	SDL_SetRenderTarget(renderer, NULL); // Stops rendering the grid

	// Essentialy, this is the camera 
	gridTargetRect.x = textureWidth/2 - gridTargetRect.w/2;
}

editor_state_id UpdateLevel(SDL_Renderer *renderer){
	// Camera moving
	const bool *kstate = SDL_GetKeyboardState(NULL);
	SDL_FRect cpyTemp = gridTargetRect;
	gridTargetRect.y += (kstate[SDL_SCANCODE_S] - kstate[SDL_SCANCODE_W]) * CAMERA_SPEED * 16/1000;
	gridTargetRect.x += (kstate[SDL_SCANCODE_D] - kstate[SDL_SCANCODE_A]) * CAMERA_SPEED * 16/1000;

	float mouseXMotion, mouseYMotion;
	if(IsMouseBeingDragged(&mouseXMotion, &mouseYMotion)){

		gridTargetRect.x += -mouseXMotion * CAMERA_SPEED/8.0f * 16.0f/1000.0f;
		gridTargetRect.y += -mouseYMotion * CAMERA_SPEED/8.0f * 16.0f/1000.0f;
	}

	if(gridTargetRect.x+gridTargetRect.w >= textureWidth || gridTargetRect.x < 0) gridTargetRect.x = cpyTemp.x;
	if(gridTargetRect.y+gridTargetRect.y >= textureHeight || gridTargetRect.y < 0) gridTargetRect.y = cpyTemp.y;
	
	// Screen margins
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderRect(renderer, &gridScreenRect);
	
	SDL_RenderTexture(renderer, gridTargetTexture, &gridTargetRect, &gridScreenRect);

	UpdateGuiElements();
	return nextState;
}

static void DrawTile(uint16_t x, uint16_t y, uint8_t w, uint8_t h){
	SDL_RenderLine(renderer, w/2-1 + x, h-1 + y, x, h/2 + y);
	SDL_RenderLine(renderer, w/2 + x, h-1 + y, w-1 + x, h/2 + y);
	SDL_RenderLine(renderer, w/2-1 + x, y, x, h/2 + y);
	SDL_RenderLine(renderer, w/2 + x, y, w-1 + x, h/2 + y);

}

static void DrawGrid(){
	uint16_t tileStartX = textureWidth/2 - tileWidth/2;
	uint16_t tileStartY = tileHeight;
	for(uint16_t x = 0; x < gridWidth; ++x){
		for(uint16_t y = 0; y < gridHeight; ++y){
			uint16_t px = (x-y) * (tileWidth/2) + tileStartX;
			uint16_t py = (x+y) * (tileHeight/2) + tileStartY;

			DrawTile(px, py, tileWidth, tileHeight);
		}
		
	}
}

// TODO: maybe move this somewhere else?
static bool IsMouseBeingDragged(float *mouseXMotion, float *mouseYMotion){
	bool isMouseMoving, isDragBtnPressed;
	isMouseMoving = isDragBtnPressed = false;
	SDL_Event *event = GetInputEvents();

	for(; event->type != 0; ++event){
		switch(event->type){
			case SDL_EVENT_MOUSE_MOTION :{
				*mouseXMotion = event->motion.xrel;
				*mouseYMotion = event->motion.yrel;
				isMouseMoving = true;
				break;
			}
		}
	}
	
	if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_MIDDLE){
		isDragBtnPressed = true;
	}
	
	return isMouseMoving && isDragBtnPressed;
}

static void _OnChangeGridSize(const char *newVal){
	printf("update grid size to: %s \n", newVal);
}

static void _OnLoadTileset(const char *newVal){
	printf("Load %s\n",newVal);

	nfdchar_t *outPath = NULL;
	nfdchar_t *filter  = "png,jpeg,gif";
	nfdresult_t result = NFD_OpenDialog(filter, NULL, &outPath);

	if(result == NFD_OKAY){
		printf("opened: %s \n", outPath);
		free(outPath);
	}else if(result == NFD_CANCEL){
		printf("cancelled\n");
	}else{
		printf("Error: %s\n", NFD_GetError());
	}
                                    
                                    
}

static void _OnButtonBack(){
	nextState = MENU;
}

static void _OnButtonLoadTileset(){
	ShowContextMenu(100, 48, 200, 200, directories, _OnLoadTileset, TOP_LEFT);
}

static void _OnButtonGridSize(){
	ShowContextMenu(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 200, 200, gridValues, _OnChangeGridSize, MIDDLE_CENTER);
}

void ExitLevel(){
	DestroyGuiElements();
	SDL_DestroyTexture(gridTargetTexture);
}

