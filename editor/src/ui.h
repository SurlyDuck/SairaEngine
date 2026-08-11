/*********************************************************************
*								Editor UI elements
**********************************************************************/


#ifndef UI_H
#define UI_H
typedef enum{
	IDLE = 0,
	HOVERED,
	PRESSED
}button_state;

typedef struct{
	SDL_FRect rect;
	SDL_Texture *texture;
	button_state state;
}button;

typedef struct{
	button items;
	size_t count;
	size_t capacity;
}buttons;

#endif // UI_H
