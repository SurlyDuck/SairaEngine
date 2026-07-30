/************************************************************************** 
*									2D camera
*
***************************************************************************/

#include "saira.h"

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

Camera2D GetCamera(){
	return camera;
}
