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

static animations animationsArray = {};

static bool AppendAnimation(){
	
	return true;
}

static bool ParseAnimationData(FILE *data, const char *filePath){
	//Break it into tokens first.
	//[name] = identifier
	//´value = a´ = variables
	//after the first two identifiers, just iterate until you find [END]


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
