/************************************************************************** 
*									  User interface
*
*	Draw items like buttons and screen messages, define callbacks...
*
***************************************************************************/

#include "./saira.h"
#include "./include/raylib.h"

#define MAX_DEBUG_MESSAGE 256
#define MAX_DEBUG_ITEMS   10

typedef struct{
	char text[MAX_DEBUG_MESSAGE];
	Vector2 screenPosition;
}debug_item;

static debug_item debugItems[MAX_DEBUG_ITEMS] = {0};
static uint8_t debugItemPtr = 0;

void PrintScreenMessage(Vector2 position, const char *text, ...){
	if(debugItemPtr >= MAX_DEBUG_ITEMS){
		SairaLog(SAIRA_WARNING, "Maximum number of debug items on screen.");
		return;
	}

	char buffer[MAX_DEBUG_MESSAGE] = {0};
	va_list args;
	va_start(args, text);
	vsprintf(buffer, text, args);
	
	int i = 0;
	while(buffer[i] != '\0'){
		debugItems[debugItemPtr].text[i] = buffer[i];
		i++;
	} 

	debugItems[debugItemPtr].screenPosition = position;
	debugItemPtr++;
}

void DrawDebugItems(){
	for(uint8_t i = 0; i < MAX_DEBUG_ITEMS; ++i){
		DrawText(debugItems[i].text, debugItems[i].screenPosition.x, debugItems[i].screenPosition.y, 20, GREEN); 
		
		// Clear data  once item is on screen
		memset(debugItems[i].text, '\0', MAX_DEBUG_MESSAGE);
		debugItems[i].screenPosition = (Vector2) {0};
	}
	
	debugItemPtr = 0;
}
