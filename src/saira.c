/* Saíra main module */
/* Runs the game, update states check for errors... */

#include "saira.h"

int main(){
	game gameEnv = InitGame();
	/* resource loading */	
	LoadAnimationData(gameEnv.animationsDataPath);
	
	SetTargetFPS(gameEnv.fps);
	InitWindow(gameEnv.width, gameEnv.height, gameEnv.title);

	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(RED);
		EndDrawing();
	
	}

	return 0;
}





