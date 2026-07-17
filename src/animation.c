/* Saíra animation Module */
#include "saira.h"

struct animation{
	const char *sheetName;
	const char *filePath;
	const char *animName;
	bool isPlaying;
	uint8_t frames;
	uint8_t fps;
	Vector2 n;
	Vector2 s;
	Vector2 e;
	Vector2 w;
	Vector2 ne;
	Vector2 nw;
	Vector2 se;
	Vector2 sw;
	Vector2 sheetSize;
};

typedef struct{
	animation *items;
	size_t length;
	size_t capacity;
}animations;

/*TODO: custom error messages */
static bool ParseAnimationData(FILE *data, const char *filePath){
	const char **rawTest = NULL;
	tokens *allTokens = GetAllTokens(rawTest);
	nodes nodesArray = ParseTokens(allTokens);
	animations animationsArray = {};
	if (nodesArray.count != 0){
		animation newAnim = {};
		/* nodesArray.items[0] = root */
		newAnim.sheetName = nodesArray.items[0].name; 
		newAnim.filePath  = nodesArray.items[0].constants[0]->name;
		
		if(nodesArray.items[0].childrenCount == 0) return false;

		for(size_t i = 0; i < nodesArray.items[0].childrenCount; ++i){
			if(strcmp(nodesArray.items[0].children[i]->name, "animation") == 0){
				/* create new animation here */
				/* ...items[0].children[i]->constants[0]->name == ´name´  */
				/* ...items[0].children[i]->constants[1]->name == ´frames´  */
			}
		}
	}else{
		TraceLog(LOG_ERROR, "Parser for animation data file did not return any data!");
		return false;
	}
	
	return true;
}

bool LoadAnimationData(const char **dataFile){
	size_t i = 0;
	while(dataFile[i] != NULL){
		FILE *data = fopen(dataFile[i], "rb");	
		if(data == NULL){
			TraceLog(LOG_ERROR, "Couldn't load animation data file %s: %s", dataFile[i], strerror(errno));
			return false;		
		}
		
		if(!ParseAnimationData(data, dataFile[i])){
			TraceLog(LOG_ERROR, "Couldn't parse animation data file %s", dataFile[i]);
			return false;		
		}

		fclose(data);
		i++;
	}

	return true;
}
