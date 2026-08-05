/************************************************************************** 
*										2D camera
*
*	Controls a raylib's 2d camera
***************************************************************************/

#include "saira.h"
#include "./include/raylib.h"

static Camera2D camera = {0};

void InitCamera(Vector2 position){
	camera = (Camera2D){0};
	camera.target = position;
	camera.offset  = (Vector2) {GetGameWidth()*.5, GetGameHeight()*.5};
	camera.zoom = 1.0f;
}

void CenterCamera(Vector2 position){
	camera.target = position;
	camera.offset  = (Vector2) {GetGameWidth()*.5, GetGameHeight()*.5};
}

void BeginModeCamera2D(){
	BeginMode2D(camera);
}

// Offset is always half the screen texture dimensions
// so camera.target is the camera's center
Vector2 GetCameraPosition(){
	return (Vector2){camera.target.x, camera.target.y};
}
