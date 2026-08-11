/*********************************************************************
*								Editor UI elements
**********************************************************************/
#include "editor.h"

struct button{
	SDL_FRect rect;
	SDL_Texture *texture;
	button_state state;
	void (*CallBack)();
};

void AddButton(buttons *buttonsArray, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *filePath, void (*CallBack)()){
	// TODO: move this to a macro at editor.h
	if(buttonsArray->count == 0){
		buttonsArray->items = (button*) malloc(sizeof(button) * 10);
		buttonsArray->capacity = 10 * sizeof(button);
	}else if(buttonsArray->capacity < sizeof(button) * (buttonsArray->count+1)){
		buttonsArray->items = (button*) realloc(buttonsArray->items, sizeof(button) * (buttonsArray->capacity+10));
		buttonsArray->capacity += 10 * sizeof(button);
	}
	
	SDL_Surface *sur = IMG_Load(filePath);
	assert(sur != NULL);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(GetRenderer(), sur);
	assert(tex != NULL);

	SDL_FRect btnRect = {
		.x = x,
		.y = y,
		.w = w,
		.h = h};

	button newBtn = {
		.rect = btnRect, 
		.texture = tex,
		.state = BUTTON_IDLE,
		.CallBack = CallBack};

	buttonsArray->items[buttonsArray->count] = newBtn;
	buttonsArray->count++; 
	
}

void DrawAllButtons(buttons *buttonsArray){
	for(size_t i = 0; i < buttonsArray->count; ++i){
		SDL_Texture *btnTexture = buttonsArray->items[i].texture;
		SDL_FRect    btnRect    = buttonsArray->items[i].rect;
		SDL_RenderTexture(GetRenderer(), btnTexture, NULL, &btnRect);
	}
}

void UpdateButtons(buttons *buttonsArray){
	
}
