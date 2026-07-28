/* Saíra main module */
/* Runs the game, update states check for errors... */

#include "saira.h"

int main(){
	game gameEnv = InitGame();
	InitWindow(gameEnv.width, gameEnv.height, gameEnv.title);

	/* resource loading */	
	if(!LoadAnimationData(gameEnv.animationsDataPath)){
		TraceLog(LOG_ERROR, "Couldn't load animation data into memory. Some textures may not work.");
	}
	
	SetTargetFPS(gameEnv.fps);
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(WHITE);

		UpdateDrawGame(GetFrameTime());
		EndDrawing();
	
	}

	CloseWindow();

	return 0;
}

int GetDirectionFromVector(Vector2 vec){
	int x = vec.x;
	int y = vec.y;

	if(x == 0 && y < 0)  return NORTH;
	if(x == 0 && y > 0)  return SOUTH;
	if(x > 0 && y == 0)  return EAST;
	if(x < 0 && y == 0)  return WEST;
	if(x > 0 && y < 0)   return NORTHEAST;
	if(x > 0 && y > 0)   return SOUTHEAST;
	if(x < 0 && y > 0)   return SOUTHWEST;
	
	return SOUTH;
}



