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
	
	if(!mouse){
		assert(actionsArray[action].keyboardKeysCount < MAX_KEYS_PER_ACTION && "Too many keys for a single action");
		size_t c = ++actionsArray[action].keyboardKeysCount;
		actionsArray[action].keyboardKeys = (KeyboardKey*) realloc(actionsArray[action].keyboardKeys, sizeof(KeyboardKey) * c);
		if(c < MAX_KEYS_PER_ACTION)
			actionsArray[action].keyboardKeys[c] = KEY_NULL;
		actionsArray[action].keyboardKeys[c-1] = key;
	}
	// TODO: mouse key actions
	
}

bool IsAnyActionKeyDown(input_action action){
	for(uint8_t i = 0; i < actionsArray[action].keyboardKeysCount; ++i){
		if(IsKeyDown(actionsArray[action].keyboardKeys[i])) return true;
	}
	for(uint8_t i = 0; i < actionsArray[action].mouseButtonsCount; ++i){
		if(IsMouseButtonDown(actionsArray[action].mouseButtons[i])) return true;
	}

	return false;
}

Vector2 GetScreenMousePosition(){
	return GetMousePosition();
}
