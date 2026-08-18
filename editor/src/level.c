#include "editor.h"

// Locals
editor_state_id nextState;

// Local foward declarations
static void _OnButtonBack();
static void _OnButtonGridSize();
static void _OnButtonLoadTileset();
static void _OnChangeGridSize(const char *newVal);

void ExitLevel();
editor_state_id UpdateLevel(SDL_Renderer *renderer);

const char *gridValues[] = {
	"8x8",
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
	"Load Tileset",
	"Delete Tileset",
	NULL
};

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

}

editor_state_id UpdateLevel(SDL_Renderer *renderer){
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
}

