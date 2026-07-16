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

/* TODO: Move this into saira.h */
#define DA_APPEND(item, array) do{ \
	\
}while(0)

/* TODO: Maybe move all this into a parsing and tokenizer module */
typedef enum{
	definition = 0,
	variable
}token_type;

typedef struct{
	token_type type;
	const char *text;
}token;

typedef struct{
	token  *items;
	size_t capacity;
	size_t count;
}tokens;

static bool ParseAnimationData(FILE *data, const char *filePath){
	bool onDefinition = false;
	size_t line       = 1;
	size_t cursor     = 1;
	char c;
	while(fread(&c, sizeof(char), 1, data)){
		if(c == '['){
			if(!onDefinition){
				TraceLog(LOG_WARNING, "Found definition");
				onDefinition = true;
				continue;
			}else {
				TraceLog(LOG_ERROR, "Nested definitions not supported yet. %lu:%lu", line, cursor);
				return false;
			}
		}else if(c == ']'){
			if(!onDefinition){
				TraceLog(LOG_ERROR, "Closing of unknown definition. %lu:%lu", line, cursor);
				return false;
			}else{
				
			}
		}else{

		}

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
