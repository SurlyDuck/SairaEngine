#include "../saira.h"

#define GAME_WIDTH  800
#define GAME_HEIGHT 600
#define FPS         60

static const char *animationFiles[] = {
	"./game/assets/characters_sheet/char0/Default1.data",
	"./game/assets/characters_sheet/char0/Default2.data",
};

game InitGame(){
	return (game) {
		.width  = GAME_WIDTH,
		.height = GAME_HEIGHT,
		.fps        = FPS,
		.title      = "Game Title",
		.animationsDataPath = animationFiles};
}

void UpdateGame(float fps){


}
