#include "editor.h"

struct button{
	SDL_FRect rect;
	SDL_Texture *idleTexture;
	SDL_Texture *hoverTexture;
	SDL_Texture *pressedTexture;
	button_state state;
	void (*CallBack)();
};

struct label{
	SDL_FRect rect;
	SDL_Texture *texture;
};

void AddButton(buttons *buttonsArray, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *idleTexture,
	const char *hoverTexture, const char *pressedTexture, void (*CallBack)()){

	SDL_Surface *idleSur, *hoverSur, *pressedSur;

	idleSur      = IMG_Load(idleTexture);
	hoverSur     = IMG_Load(hoverTexture);
	pressedSur   = IMG_Load(pressedTexture);
	assert(idleSur && hoverSur && pressedSur);

	SDL_Texture *idleTex = SDL_CreateTextureFromSurface(GetRenderer(), idleSur);
	SDL_Texture *hoverTex = SDL_CreateTextureFromSurface(GetRenderer(), hoverSur);
	SDL_Texture *pressedTex = SDL_CreateTextureFromSurface(GetRenderer(), pressedSur);
	SDL_DestroySurface(idleSur);
	SDL_DestroySurface(hoverSur);
	SDL_DestroySurface(pressedSur);

	SDL_FRect btnRect = {
		.x = x,
		.y = y,
		.w = w,
		.h = h};

	button newBtn = {
		.rect = btnRect, 
		.idleTexture = idleTex,
		.hoverTexture = hoverTex,
		.pressedTexture = pressedTex,
		.state = BUTTON_IDLE,
		.CallBack = CallBack};

	DA_APPEND(newBtn, buttonsArray);
	
}

void AddLabel(const char *text, labels *labelsArray, uint16_t x, uint16_t y, TTF_Font *font, SDL_Color color, origin or){
	SDL_Surface *sur = TTF_RenderText_Blended(font, text, 0, color);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, sur);
	SDL_DestroySurface(sur);
	
	SDL_FRect rect = {
		.x = x,
		.y = y,
		.w = tex->w,
		.h = tex->h};
	
	// TODO: better to factor this since will probably be used somewhere else
	switch(or){
		case TOP_LEFT: break; // SDL standard
		case MIDDLE_CENTER: rect.x -= tex->w/2.00f; rect.y -= tex->h/2.00f; break;
		case BOTTOM_RIGHT:  rect.x -= tex->w;       rect.y -= tex->h; break;
		default: break;
	}

	label newLabel = {
		.texture = tex,
		.rect    = rect};

	DA_APPEND(newLabel, labelsArray);
	
}

// TODO: a bit of repetition; maybe a macro can replace DrawAllButtons() and DrawAllLabels()
void DrawAllButtons(buttons *buttonsArray){
	for(size_t i = 0; i < buttonsArray->count; ++i){
		SDL_Texture *btnTexture; //= buttonsArray->items[i].idleTexture;
		switch(buttonsArray->items[i].state){
			case BUTTON_HOVERED: btnTexture = buttonsArray->items[i].hoverTexture; break;
			case BUTTON_PRESSED: btnTexture = buttonsArray->items[i].pressedTexture; break;
			default: btnTexture = buttonsArray->items[i].idleTexture; break;
			
		}

		SDL_FRect    btnRect    = buttonsArray->items[i].rect;
		SDL_RenderTexture(GetRenderer(), btnTexture, NULL, &btnRect);
	}
}
void DrawAllLabels(labels *labelArray){
	for(size_t i = 0; i < labelArray->count; ++i){
		SDL_Texture *labelTexture  = labelArray->items[i].texture;
		SDL_FRect    labelRect     = labelArray->items[i].rect;
		SDL_RenderTexture(GetRenderer(), labelTexture, NULL, &labelRect);
	}
}

bool IsPointOverlayingRect(int px, int py, SDL_FRect rect){
	int centerX = rect.x + rect.w/2;
	int centerY = rect.y + rect.h/2;

	int disX = (px > centerX) ? px - centerX : centerX - px;
	int disY = (py > centerY) ? py - centerY : centerY - py;
	
	if(disX > rect.w/2 || disY > rect.h/2) 
		return false;
	else
		return true;
}

// Callbacks and states
void UpdateButtons(buttons *buttonsArray){
	SDL_Event *event = GetInputEvents();
	for(; event->type != 0; ++event){
		switch(event->type){
			case SDL_EVENT_MOUSE_BUTTON_DOWN :{
				if(event->button.button == SDL_BUTTON_LEFT){
					for(size_t i = 0; i < buttonsArray->count; ++i){
						if(IsPointOverlayingRect(event->button.x, event->button.y, buttonsArray->items[i].rect)){
							buttonsArray->items[i].state = BUTTON_PRESSED;
							break;
						}
					}
				}
				break;
			}
			case SDL_EVENT_MOUSE_BUTTON_UP :{
				if(event->button.button == SDL_BUTTON_LEFT){
					for(size_t i = 0; i < buttonsArray->count; ++i){
						if(IsPointOverlayingRect(event->button.x, event->button.y, buttonsArray->items[i].rect)){
							buttonsArray->items[i].state = BUTTON_IDLE;
							buttonsArray->items[i].CallBack();
							break;
						}
					}
				}
				break;
			}
			case SDL_EVENT_MOUSE_MOTION: {
			bool alreadyHovering = false;
			for(size_t i = 0; i < buttonsArray->count; ++i){
				if(alreadyHovering) {buttonsArray->items[i].state = BUTTON_IDLE; continue;}

				if(IsPointOverlayingRect(event->button.x, event->button.y, buttonsArray->items[i].rect)){
					buttonsArray->items[i].state = BUTTON_HOVERED;
					alreadyHovering = true;
				}else buttonsArray->items[i].state = BUTTON_IDLE;
			}
				break;
			}
			default: break;
		}
	}
}

void DestroyLabels(labels *labelArray){
 // TODO
}

void DestroyButtons(buttons *buttonArray){
 // TODO
}
