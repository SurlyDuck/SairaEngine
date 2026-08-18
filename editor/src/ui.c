#include "editor.h"

typedef enum{
	BUTTON_IDLE = 0,
	BUTTON_HOVERED,
	BUTTON_PRESSED
}button_state;

typedef struct button{
	SDL_FRect rect;
	SDL_Texture *idleTexture;
	SDL_Texture *hoverTexture;
	SDL_Texture *pressedTexture;
	button_state state;
	void (*CallBack)();
}button;

typedef struct{
	button *items;
	size_t count;
	size_t capacity;
}buttons;

typedef struct label{
	SDL_FRect rect;
	SDL_Texture *texture;
}label;

typedef struct{
	label *items;
	size_t count;
	size_t capacity;
}labels;

// A window containing a list of values the user can select
// The selected value is given on callback
// Only one at each time
// values does not point to the heap
typedef struct context_menu{
	bool active;
	const char **values;
	SDL_FRect rect;
	uint8_t selected;
	origin anchor;
	void (*Callback)(const char *val);
}context_menu;

// Locals
static struct context_menu currentContextMenu = {0};
static buttons allButtons = {0};
static labels  allLabels  = {0};

// TODO: the rest of the anchors
void AlignRect(SDL_FRect *rect, origin or){
	switch(or){
		case TOP_LEFT: break; // SDL standard
		case MIDDLE_CENTER: rect->x -= rect->w/2.00f; rect->y -= rect->h/2.00f; break;
		case BOTTOM_RIGHT:  rect->x -= rect->w;       rect->y -= rect->h; break;
		default: break;
	
	}
}

void AddButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *idleTexture,
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
	
	DA_APPEND(newBtn, (&allButtons));
}

#define BTN_STANDARD_BG_COLOR 0, 255, 0, SDL_ALPHA_OPAQUE
#define BTN_STANDARD_FG_COLOR 0, 0, 255, SDL_ALPHA_OPAQUE

void AddStdButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *text, TTF_Font *font, void (*Callback)()){
	SDL_Texture *idleTexture, *hoverTexture, *pressedTexture, *fontTexture;
	SDL_FRect btnRect = {.x = x, .y = y, .w = w, .h = h };

	// Create font texture
	SDL_Surface *fontSurface = TTF_RenderText_Blended(font, text, 0, WHITE);
	fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_DestroySurface(fontSurface);
	
	// Create each button state texture as a target to render on
	idleTexture    = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	hoverTexture   = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	pressedTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	
	// Idle texture rendering
	SDL_SetRenderTarget(renderer, idleTexture);
	SDL_SetRenderDrawColor(renderer, BTN_STANDARD_BG_COLOR);
	SDL_RenderFillRect(renderer, &(SDL_FRect){.x = 0, .y = 0, .w = w, .h = h});
	SDL_SetRenderDrawColor(renderer, BTN_STANDARD_FG_COLOR);
	SDL_RenderFillRect(renderer, &(SDL_FRect){.x = 0, .y = 0, .w = w-4, .h = h-4});
	SDL_RenderTexture(renderer, fontTexture, NULL, &(SDL_FRect){w/2-fontTexture->w/2, h/2-fontTexture->h/2, fontTexture->w, fontTexture->h});
	
	// Hovering texture rendering
	SDL_SetRenderTarget(renderer, hoverTexture);
	SDL_SetRenderDrawColor(renderer, BTN_STANDARD_FG_COLOR);
	SDL_RenderFillRect(renderer, &(SDL_FRect){.x = 0, .y = 0, .w = w, .h = h});
	SDL_SetRenderDrawColor(renderer, BTN_STANDARD_BG_COLOR);
	SDL_RenderFillRect(renderer, &(SDL_FRect){.x = 0, .y = 0, .w = w-4, .h = h-4});
	SDL_RenderTexture(renderer, fontTexture, NULL, &(SDL_FRect){w/2-fontTexture->w/2, h/2-fontTexture->h/2, fontTexture->w, fontTexture->h});

	// Pressed texture rendering
	SDL_SetRenderTarget(renderer, pressedTexture);
	SDL_SetRenderDrawColor(renderer, BTN_STANDARD_FG_COLOR);
	SDL_RenderFillRect(renderer, &(SDL_FRect){.x = 0, .y = 0, .w = w, .h = h});
	SDL_RenderTexture(renderer, fontTexture, NULL, &(SDL_FRect){w/2-fontTexture->w/2+4, h/2-fontTexture->h/2+4, fontTexture->w, fontTexture->h});

	SDL_SetRenderTarget(renderer, NULL);

	SDL_DestroyTexture(fontTexture);

	button newBtn = {
		.rect = btnRect, 
		.idleTexture = idleTexture,
		.hoverTexture = hoverTexture,
		.pressedTexture = pressedTexture,
		.state = BUTTON_IDLE,
		.CallBack = Callback};

	DA_APPEND(newBtn, (&allButtons));
	
}

void AddLabel(const char *text, uint16_t x, uint16_t y, TTF_Font *font, SDL_Color color, origin or){
	SDL_Surface *sur = TTF_RenderText_Blended(font, text, 0, color);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, sur);
	SDL_DestroySurface(sur);
	
	SDL_FRect rect = {
		.x = x,
		.y = y,
		.w = tex->w,
		.h = tex->h};
		
	AlignRect(&rect, or);

	label newLabel = {
		.texture = tex,
		.rect    = rect};

	DA_APPEND(newLabel, (&allLabels));
}

void ShowContextMenu(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *list[], void (*Callback)(const char *val), origin or){
		SDL_FRect rect = {.x = x, .y = y, .w = w, .h = h};
		AlignRect(&rect, or);
		currentContextMenu = (context_menu) {
		.rect = rect,
		.active = true,
		.selected = 0,
		.Callback = Callback,
		.values = list,
		.anchor = or};
}

// TODO: a bit of repetition; maybe a macro can replace all drawing subroutines
static void DrawAllButtons(){
	for(size_t i = 0; i < allButtons.count; ++i){
		SDL_Texture *btnTexture; //= buttonsArray->items[i].idleTexture;
		switch(allButtons.items[i].state){
			case BUTTON_HOVERED: btnTexture = allButtons.items[i].hoverTexture; break;
			case BUTTON_PRESSED: btnTexture = allButtons.items[i].pressedTexture; break;
			default:             btnTexture = allButtons.items[i].idleTexture; break;
			
		}

		SDL_FRect    btnRect    = allButtons.items[i].rect;
		SDL_RenderTexture(renderer, btnTexture, NULL, &btnRect);
	}
}

void DrawAllLabels(){
	for(size_t i = 0; i < allLabels.count; ++i){
		SDL_Texture *labelTexture  = allLabels.items[i].texture;
		SDL_FRect    labelRect     = allLabels.items[i].rect;
		SDL_RenderTexture(GetRenderer(), labelTexture, NULL, &labelRect);
	}
}

#define MENULIST_BG_COLOR 0, 255, 0,   SDL_ALPHA_OPAQUE
#define MENULIST_FG_COLOR 0, 0, 255,   SDL_ALPHA_OPAQUE
#define MENULIST_SL_COLOR 155, 155, 155, SDL_ALPHA_OPAQUE
#define MENULIST_PADDING 20 + 4
void DrawContextMenu(){
	if(currentContextMenu.active){
		SDL_SetRenderDrawColor(renderer, MENULIST_BG_COLOR);
		SDL_RenderFillRect(renderer, &currentContextMenu.rect);

		SDL_SetRenderDrawColor(renderer, MENULIST_FG_COLOR);
		SDL_RenderFillRect(renderer, &(SDL_FRect){currentContextMenu.rect.x, currentContextMenu.rect.y, currentContextMenu.rect.w-4, currentContextMenu.rect.h-4});
	
		int i = 0;
		while(currentContextMenu.values[i] != NULL){ // Very dangerous
			SDL_Surface *sur = TTF_RenderText_Blended(monoRegularSmall, currentContextMenu.values[i], 0, WHITE);
			SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, sur);
			SDL_DestroySurface(sur);

			if(currentContextMenu.selected == i){
				SDL_SetRenderDrawColor(renderer, MENULIST_SL_COLOR);
				SDL_RenderFillRect(renderer, &(SDL_FRect){currentContextMenu.rect.x, currentContextMenu.rect.y + i *MENULIST_PADDING, currentContextMenu.rect.w, 15});
			}
		
			SDL_RenderTexture(renderer, tex, NULL, &(SDL_FRect){currentContextMenu.rect.x + currentContextMenu.rect.w/2 - tex->w/2, currentContextMenu.rect.y + i* MENULIST_PADDING, tex->w, tex->h});
			SDL_DestroyTexture(tex);
			i++;
		}		
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


void UpdateGuiElements(){
	// Drawing subroutines
	DrawAllButtons();
	DrawAllLabels();
	DrawContextMenu();

	SDL_Event *event = GetInputEvents();
	for(; event->type != 0; ++event){
		switch(event->type){
			case SDL_EVENT_MOUSE_BUTTON_DOWN :{
				if(event->button.button == SDL_BUTTON_LEFT){
					for(size_t i = 0; i < allButtons.count; ++i){ // Buttons
						if(IsPointOverlayingRect(event->button.x, event->button.y, allButtons.items[i].rect)){
							allButtons.items[i].state = BUTTON_PRESSED;
							break;
						}
					}
					
					if(currentContextMenu.active){ // Context menu
						currentContextMenu.active = false;
						if(IsPointOverlayingRect(event->button.x, event->button.y, currentContextMenu.rect)){
							currentContextMenu.Callback(currentContextMenu.values[currentContextMenu.selected]);
						}					
					}
				}
				break;
			}
			case SDL_EVENT_MOUSE_BUTTON_UP :{
				if(event->button.button == SDL_BUTTON_LEFT){
					for(size_t i = 0; i < allButtons.count; ++i){ // Buttons
						if(IsPointOverlayingRect(event->button.x, event->button.y, allButtons.items[i].rect)){
							allButtons.items[i].state = BUTTON_HOVERED;
							allButtons.items[i].CallBack();
							break;
						}
					}
				}
				break;
			}
			case SDL_EVENT_MOUSE_MOTION: {
				bool alreadyHovering = false;
				for(size_t i = 0; i < allButtons.count; ++i){ // Buttons
					if(alreadyHovering) {allButtons.items[i].state = BUTTON_IDLE; continue;}

					if(IsPointOverlayingRect(event->button.x, event->button.y, allButtons.items[i].rect)){
						allButtons.items[i].state = BUTTON_HOVERED;
						alreadyHovering = true;
					}else allButtons.items[i].state = BUTTON_IDLE;
				}
				
				if(currentContextMenu.active){  // Context menu
					float itemHeight = TTF_GetFontSize(monoRegularSmall); // Values in the list use this font
					if(IsPointOverlayingRect(event->button.x, event->button.y, currentContextMenu.rect)){
						for(int i = 0; currentContextMenu.values[i] != NULL; ++i){
							float itemTop = currentContextMenu.rect.y + i * MENULIST_PADDING;
							
							if(event->button.y >= itemTop && event->button.y <= itemTop + itemHeight){
								currentContextMenu.selected = i;
								break;
							}
						}					
					}							
				}
					break;
				}
			default: break;
		}
	}

}

bool IsGuiBusy(){
	return currentContextMenu.active;
}

void DestroyGuiElements(){
	// Buttons
	for(size_t i = 0; i < allButtons.count; ++i){
		SDL_DestroyTexture(allButtons.items[i].idleTexture);
		SDL_DestroyTexture(allButtons.items[i].hoverTexture);
		SDL_DestroyTexture(allButtons.items[i].pressedTexture);
	}
	DA_CLEAR((&allButtons));

	// Labels
	for(size_t i = 0; i < allLabels.count; ++i){
		SDL_DestroyTexture(allLabels.items[i].texture);
	}
	DA_CLEAR((&allLabels));

	// Menu list
	currentContextMenu.active = false;
}
