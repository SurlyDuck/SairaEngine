#include "../saira.h"
#include "characters.h"
/* Manages most of the `characters` ai on the expedition level */

/* TODO: array of characters must come from the world data */

static Vector2 p = {0};

ANIMATION_CONTAINER *secondAnim = NULL;
ANIMATION_CONTAINER *thirdAnim = NULL;
ANIMATION_CONTAINER *fourthAnim = NULL;
float fourthRot = 0;
void InitCharacters(){
	secondAnim = GetAnimationContainer();
	thirdAnim  = GetAnimationContainer();
	fourthAnim = GetAnimationContainer();
	SetAnimation(secondAnim, "Default", "Walking", true);
	SetAnimation(thirdAnim, "Default", "Defense_spear", false);
	SetAnimation(fourthAnim, "Default", "Idle_Spear", false);
	
	MoveAnimation(secondAnim, (Vector2){GetGameWidth()-128,0}, SOUTHWEST_VEC);
	MoveAnimation(thirdAnim, (Vector2){GetGameWidth()/2-64,100}, SOUTH_VEC);
	p = EAST_VEC;
	MoveAnimation(fourthAnim, (Vector2){GetGameWidth()/2-64,0}, p);
}

void UpdateCharacters(float deltaTime){
	if (secondAnim == NULL) return;

	Vector2 sPosition = GetAnimationPosition(secondAnim);
	if(sPosition.y >= GetGameHeight()+30) {
		sPosition = (Vector2) {GetGameWidth(),-90};
		SetAnimationFrame(thirdAnim, 0);
	}
	sPosition.x -= 50 * deltaTime;
	sPosition.y += 50 * deltaTime;
	MoveAnimation(secondAnim, sPosition, SOUTHWEST_VEC);
	
	fourthRot += 400*(M_PI/180.00f) * deltaTime;
	
	MoveAnimation(fourthAnim, (Vector2){GetGameWidth()/2-64,0}, (Vector2){roundf(cosf(fourthRot)), roundf(sinf(fourthRot))});

}
