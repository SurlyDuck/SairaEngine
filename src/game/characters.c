#include "../saira.h"
#include "characters.h"
/* Manages most of the `characters` ai on the expedition level */

/* TODO: array of characters must come from the world data */

/*TODO: is better to handle the player somewhere else */
static Vector2 playerDirection = {0};
static Vector2 playerPosition  = {10,10};

ANIMATION_CONTAINER *anim = NULL;
void InitCharacters(){
	playerDirection = SOUTHEAST_VEC;
	anim = GetAnimationContainer();
	SetAnimation(anim, "Default", "Walking", true);
}

void UpdateCharacters(float deltaTime){
	if (anim == NULL) return;
	/* TODO: no need of so many checks, lots of things are default. Just doing to test the api */
	if(!IsAnimationSheet(anim, "Default") || !IsAnimationName(anim, "Walking")){
		SetAnimation(anim, "Default", "Walking", true);
		SetAnimationFrame(anim, 0);
	}
	
	if(!IsAnimationVisible(anim)){
		SetAnimationVisibility(anim, true);
	}

	if(!IsAnimationPlaying(anim)){
		PlayAnimation(anim, true);
	}

	MoveAnimation(anim, playerPosition, playerDirection);
	playerPosition.x += 50 * deltaTime;
	playerPosition.y += 50 * deltaTime;
	
	if(playerPosition.y >= 432) playerPosition = (Vector2) {-90,-90};

}
