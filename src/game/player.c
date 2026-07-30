#include "player.h"
#include "../saira.h"

static Vector2 playerPosition = {0};
static Vector2 playerDirection = {0};
static ANIMATION_CONTAINER *playerAnim = NULL;

void InitPlayer(){
	playerDirection = SOUTHEAST_VEC;
	playerAnim = GetAnimationContainer();
	SetAnimation(playerAnim, "Default", "Walking", true);
	MoveAnimation(playerAnim, playerPosition, playerDirection);
}

void UpdatePlayer(float deltaTime){
	playerPosition.x += 50 * deltaTime;
	playerPosition.y += 50 * deltaTime;
	
	MoveAnimation(playerAnim, playerPosition, playerDirection);
	CenterCamera((Vector2){playerPosition.x+64, playerPosition.y+64});

}
