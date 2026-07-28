#include "../saira.h"
#include "characters.h"

#define GAME_WIDTH  768
#define GAME_HEIGHT 432
#define FPS         60

typedef enum{
	NONE = 0,
	MENU,
	EXPEDITION,
	EXITING
}game_state;

game_state currentGameState = NONE;

static const char *animationFiles[] = {
	"./game/assets/characters_sheet/char0/Default3.sgd",
	NULL
};

game_state UpdateDrawState(float deltaTime);

game InitGame(){
	return (game) {
		.width  = GAME_WIDTH,
		.height = GAME_HEIGHT,
		.fps        = FPS,
		.title      = "Game Title",
		.animationsDataPath = animationFiles};
}

void UpdateDrawGame(float deltaTime){
	game_state newState = UpdateDrawState(deltaTime);
	if(newState == currentGameState) return;

	switch(newState){
		case EXPEDITION:{
			InitCharacters();
			currentGameState = newState;
			break;
		}
		default: break;
	}
}

game_state UpdateDrawState(float deltaTime){
	switch(currentGameState){
		case NONE: return EXPEDITION; /* game first state */
		case MENU: return EXPEDITION; /* TODO */
		case EXPEDITION:{
			UpdateCharacters(deltaTime);
			
			DrawAllAnimationContainers();
			return EXPEDITION;
		}
		default: break;
	}
	
	return NONE;
}

