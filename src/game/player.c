#include "player.h"
#include "../saira.h"

#define CAMERA_SPEED 500

static Vector2 playerPosition  = {0};
static Vector2 playerDirection = {0};
static Vector2 cameraPosition  = {0};
static Vector2 cameraVelocity  = {0};
static ANIMATION_CONTAINER *playerAnim = NULL;

void InitPlayer(){
	playerDirection = SOUTHEAST_VEC;
	playerAnim = GetAnimationContainer();
	SetAnimation(playerAnim, "Default", "Walking", true);
	MoveAnimation(playerAnim, playerPosition, playerDirection);
	cameraPosition = (Vector2){playerPosition.x+64, playerPosition.y+64};
}

void UpdatePlayer(float deltaTime){
	//playerPosition.x += 50 * deltaTime;
	//playerPosition.y += 50 * deltaTime;
	cameraVelocity.x = (IsActionKeyDown(RIGHT_ACTION) - IsActionKeyDown(LEFT_ACTION)) * CAMERA_SPEED * deltaTime;
	cameraVelocity.y = (IsActionKeyDown(DOWN_ACTION) - IsActionKeyDown(UP_ACTION)) * CAMERA_SPEED * deltaTime;
	cameraPosition.x += cameraVelocity.x;
	cameraPosition.y += cameraVelocity.y;
	
	CenterCamera(cameraPosition);
	
	//MoveAnimation(playerAnim, playerPosition, playerDirection);
	//CenterCamera((Vector2){playerPosition.x+64, playerPosition.y+64});

}


