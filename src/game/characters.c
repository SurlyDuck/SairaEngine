#include "../saira.h"
#include "characters.h"

/* TODO: array of characters most come for the world must come from the engine */
size_t animID = 0;
Vector2 playerPos = {0, 0};
Vector2 playerDir = {1,0};


void InitCharacters(){
	animID = GetNewAnimation();
}

void UpdateCharacters(){
	PlayAnimation("Default", "Walking", true, animID, false, playerDir, playerPos);
	playerPos.x += .5;
	playerPos.y += .5;

}
