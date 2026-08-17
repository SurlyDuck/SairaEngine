#include "editor.h"

// Locals
editor_state_id nextState;

// Local foward declarations
static void _OnButtonBack();
static void _OnButtonGridSize();
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

void InitLevel(editor_state *state){
	state->Update  = UpdateLevel;
	state->Exit    = ExitLevel;
	state->id      = LEVEL;
	nextState      = LEVEL;
		
	AddStdButton(0, 0, 30, 40, "<<", monoRegularMedium, _OnButtonBack);
	AddStdButton(100, 0, 92, 40, "TILESET", monoRegularMedium, _OnButtonBack);
	AddStdButton(200, 0, 92, 40, "MAP", monoRegularMedium, _OnButtonBack);
	AddStdButton(300, 0, 92, 40, "OUT", monoRegularMedium, _OnButtonBack);
	AddStdButton(400, 0, 92, 40, "SIZE", monoRegularMedium, _OnButtonGridSize);

}

editor_state_id UpdateLevel(SDL_Renderer *renderer){
	UpdateGuiElements();
	return nextState;
}

static void _OnChangeGridSize(const char *newVal){
	printf("update grid size!\n");
}

static void _OnButtonBack(){
	nextState = MENU;
}


static void _OnButtonGridSize(){
	ShowMenuList(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 200, 200, gridValues, _OnChangeGridSize, MIDDLE_CENTER);
}

void ExitLevel(){
	DestroyGuiElements();
}

