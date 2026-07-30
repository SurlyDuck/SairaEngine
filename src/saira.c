/************************************************************************** 
*									Saíra main module 
*	Initialize and updates the game, render frames, updates user options...
*
***************************************************************************/

#include "saira.h"

void DisplayDebugInformation();

static game gameEnv = {0};

int main(){
	gameEnv = InitGame();
	//SetConfigFlags(FLAG_FULLSCREEN_MODE);
	InitWindow(gameEnv.width, gameEnv.height, gameEnv.title);

	/* resource loading */	
	if(!LoadAnimationData(gameEnv.animationsDataPath)){
		TraceLog(LOG_ERROR, "Couldn't load animation data into memory. Some textures may not work.");
	}
	
	SetTargetFPS(gameEnv.fps);

	RenderTexture mainFrameBuffer = {0};
	mainFrameBuffer = LoadRenderTexture(gameEnv.width, gameEnv.height);
	//SetTextureFilter(mainFrameBuffer.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureFilter(mainFrameBuffer.texture, TEXTURE_FILTER_POINT);
	float widthScale  = (float)(GetScreenWidth()) /  (float)(gameEnv.width);
	float heightScale = (float)(GetScreenHeight()) / (float)(gameEnv.height);
	float windowScale = (widthScale < heightScale) ? widthScale : heightScale;
	
	InitCamera((Vector2){0, 0});

	while(!WindowShouldClose()){
		BeginTextureMode(mainFrameBuffer);
			BeginMode2D(GetCamera());	
			ClearBackground(WHITE);
			UpdateDrawGame(GetFrameTime());
			EndMode2D();

		EndTextureMode();
	
		Rectangle gameSourceRect = {
		.x = 0.00f,
		.y = 0.00f,
		.width  = mainFrameBuffer.texture.width,
		.height = -mainFrameBuffer.texture.height, /* go fuck urself opengl */};
			
		Rectangle screenDestRect = {
		.x      = (float)(GetScreenWidth()  - (float)(windowScale * gameEnv.width)) * .5f,
		.y      = (float)(GetScreenHeight() - (float)(windowScale * gameEnv.height))* .5f,
		.width  = (float)(gameEnv.width  * windowScale),
		.height = (float)(gameEnv.height * windowScale)};

		BeginDrawing();
			ClearBackground(PINK);
			DrawTexturePro(mainFrameBuffer.texture, gameSourceRect, screenDestRect, (Vector2){0,0}, 0.00f, WHITE);
			DisplayDebugInformation();	

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
	if(x < 0 && y < 0)   return NORTHWEST;
	
	return SOUTH;
}

int GetGameWidth(){
	return gameEnv.width;
}

int GetGameHeight(){
	return gameEnv.height;
}

void DisplayDebugInformation(){
	DrawFPS(0,0);
}
