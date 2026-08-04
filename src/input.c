/************************************************************************** 
*									  Input Module
*
*	Handle user input as actions...
*
***************************************************************************/

#include "./include/raylib.h"
#include "saira.h"

#define MAX_KEYS_PER_ACTION  8
#define MAX_ACTIONS        256

typedef struct{
	input_action id;
	KeyboardKey *keyboardKeys;
	MouseButton *mouseButtons;
	uint8_t keyboardKeysCount;
	uint8_t mouseButtonsCount;
}action;

action *actionsArray = NULL;

void InitInput(){
	assert(ACTION_COUNT < MAX_ACTIONS && "Too many actions!");
	actionsArray = (action*) malloc(sizeof(action) * ACTION_COUNT);

	for(uint8_t i = 0; i < ACTION_COUNT; ++i){
		actionsArray[i] = (action){0};
		actionsArray[i].id = i;
	}
}

void AddKeyAction(input_action action, int key, bool mouse){
	if(actionsArray == NULL){
		SairaLog(SAIRA_FATAL, "Input not initialized properly!");
		return;
	}
	
	for(uint8_t i = 0; i < ACTION_COUNT; ++i){
		if(action == actionsArray[i].id){
			if(!mouse){
				actionsArray[i].keyboardKeysCount++;
				actionsArray[i].keyboardKeys = realloc(actionsArray[i].keyboardKeys, sizeof(KeyboardKey) * actionsArray[i].keyboardKeysCount);
				actionsArray[i].keyboardKeys[actionsArray[i].keyboardKeysCount-1] = key;
			}
			break;
		}
	}
	
}

void UpdateKeyAction(input_action action, int oldKey, int newKey){

}

bool IsActionKeyDown(input_action action){
	switch(action){
		case UP_ACTION: return IsKeyDown(KEY_W);
		case DOWN_ACTION: return IsKeyDown(KEY_S);
		case RIGHT_ACTION: return IsKeyDown(KEY_D);
		case LEFT_ACTION: return IsKeyDown(KEY_A);
		default: return false;
	}

	return false;
}

