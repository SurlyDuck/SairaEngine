/* Saíra main module */
/* Runs the game, update states check for errors... */

#include "saira.h"

int main(){
	game gameEnv = InitGame();
	/* resource loading */	
	if(!LoadAnimationData(gameEnv.animationsDataPath)){
		TraceLog(LOG_ERROR, "Couldn't load animation data into memory. Some textures may not work.");
	}
	
	SetTargetFPS(gameEnv.fps);
	InitWindow(gameEnv.width, gameEnv.height, gameEnv.title);

	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(RED);
		EndDrawing();
	
	}

	return 0;
}





