#include "./engine/include/raylib.h"

int main(){
	
	SetTargetFPS(30);
	InitWindow(800,600, "People");

	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(RED);
		EndDrawing();	
	}

	CloseWindow();
	
	return 0;
}
