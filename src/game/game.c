#include "../saira.h"

#define GAME_WIDTH  512
#define GAME_HEIGHT 288
#define FPS         60

static const char *animationFiles[] = {
	"./game/assets/characters_sheet/char0/Default1.data",
	"./game/assets/characters_sheet/char0/Default2.data",
	NULL
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
