/************************************************************************** 
*									Saíra main module 
*	Initialize and updates the game, render frames, load user options, 
*	implement logging and miscellaneous stuff not worth a module of their own.
*
*
*	NOTES
*		- The logging is implemented here. SairaLogV is the callback
*		for Raylib's TraceLog and SairaLog is called everywhere else
***************************************************************************/

#include "saira.h"
#include "./include/raylib.h"

void DisplayDebugInformation(); 
static void SairaLogV(int logLevel, const char *text, va_list args);
static void LoadStandardKeys();

static game gameEnv = {0};

int main(){
	gameEnv = InitGame();
	TraceLogCallback callback = SairaLogV;
	SetTraceLogCallback(callback);
	//SetConfigFlags(FLAG_FULLSCREEN_MODE);
	InitWindow(gameEnv.width, gameEnv.height, gameEnv.title);
	InitInput();
	LoadStandardKeys();

	/* resource loading */	
	if(!LoadAnimationData(gameEnv.animationsDataPath)){
		SairaLog(SAIRA_ERROR, "Couldn't load animation data into memory. Some textures may not work.");
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

			BeginModeCamera2D();
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

// TODO: better to load this from a file
// TODO: once the user change keys save to another file
static void LoadStandardKeys(){
	AddKeyAction(UP_ACTION, KEY_W, false);
	AddKeyAction(UP_ACTION, KEY_UP, false);
	AddKeyAction(DOWN_ACTION, KEY_S, false);
	AddKeyAction(DOWN_ACTION, KEY_DOWN, false);

	AddKeyAction(RIGHT_ACTION, KEY_D, false);
	AddKeyAction(RIGHT_ACTION, KEY_RIGHT, false);
	AddKeyAction(LEFT_ACTION,  KEY_A, false);
	AddKeyAction(LEFT_ACTION,  KEY_LEFT, false);
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

#define LOG_MAX_LENGTH 256
static void SairaLogV(int logLevel, const char *text, va_list args){
	char buffer[LOG_MAX_LENGTH] = {0};
	switch(logLevel){
		case LOG_ERROR: strcat(buffer,"ERROR: "); break;
		case LOG_INFO: strcat(buffer,"INFO: "); break;
		case LOG_WARNING: strcat(buffer,"WARNING: "); break;
		case LOG_FATAL: strcat(buffer,"FATAL: "); break;
		default: return; // Ignore everything else
	}
	
	printf("%s", buffer);
	vfprintf(stdout, text, args);
	printf("\n");
	fflush(stdout);

	if(logLevel == LOG_FATAL){
		SairaPanic();
	}

	/* TODO: GUI popup */
	/* TODO: logging file */
}

void SairaLog(int logLevel, const char *msg, ...){
	switch(logLevel){
		case SAIRA_ERROR: logLevel = LOG_ERROR; break;
		case SAIRA_INFO: logLevel = LOG_INFO; break;
		case SAIRA_WARNING: logLevel = LOG_WARNING; break;
		case SAIRA_FATAL: logLevel = LOG_FATAL; break;
		default: return;
	}
	va_list args;
	va_start(args, msg);
	//char text[256];
	//vsprintf(text, msg, args);
	//TraceLog(logLevel, text);
	SairaLogV(logLevel, msg, args);
	va_end(args);

}

void SairaPanic(){
	exit(69);
}
