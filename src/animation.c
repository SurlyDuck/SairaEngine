/* Saíra animation system */
#include "saira.h"

#define MAX_ANIMATION_OBJECTS 1024

typedef struct{
	const char *sheetName;
	const char *filePath; /* maybe unnecessary? */
	const char *animName;
	Texture2D spriteSheetTexture;
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
} animation;

typedef struct{
	animation *items;
	size_t count;
	size_t capacity;
}animations;

// The actual animation object on screen
typedef struct {
	bool isEnabled;
	bool onRepeat;
	size_t frame;
	animation animData;
	Vector2 position;
	Vector2 direction;
}animation_object;

typedef struct{
	size_t *items;
	size_t count;
	size_t capacity;
}animation_ids;

static animation_ids idsArray   = {0};
static animations allAnimations = {0}; /* animation data array */
static animation_object animationObjectArray[MAX_ANIMATION_OBJECTS] = {0};

static bool AppendAnimationsData(FILE *data){
	char *rawTest = NULL;
	assert(fseek(data, 0, SEEK_END) == 0);
	long fsize = ftell(data);
	rawTest = (char*) malloc((fsize+1) * sizeof(char));
	assert(rawTest != NULL);
	rewind(data);
	fread((void*)rawTest, sizeof(char), fsize, data);
	rawTest[fsize] = '\0';

	tokens *allTokens = GetAllTokens( (const char*) rawTest);
	node   *allNodes  = ParseTokens(allTokens);

	if (allNodes != NULL){
		if(allNodes[0].childrenCount == 0 || allNodes[0].constantCount == 0){
			TraceLog(LOG_ERROR, "Parser returned invalid data from animation file");
		  	return false;
		}
		ptr_da animationNodes = GetNodes("animation", allNodes);
		if(animationNodes.count <= 0){
			TraceLog(LOG_ERROR, "No animation nodes found");
			return false;
		}
		
		/* TODO: move this to a function in saira.c */
		Image rawImage = LoadImage(allNodes[0].constants[0]->value);
		ImageFormat(&rawImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
		ImageColorReplace(&rawImage, TRANSPARENT, BLANK);
		Texture2D animationTexture  = LoadTextureFromImage(rawImage);

		for(size_t i = 0; i < animationNodes.count; ++i){
			/* create new animation here */
			/* assuming one root for animation file (allNodes[0]) */
			/* also only one sprite sheet per file */
			/* TODO: more error handling */
			animation newAnim          = {};
			node *nodeAnim             = animationNodes.items[i];
			newAnim.sheetName          = allNodes[0].name;
			newAnim.filePath           = allNodes[0].constants[0]->value;
			newAnim.animName           = GetConstantValue(nodeAnim, "name");
			newAnim.frames             = (uint8_t) strtoull(GetConstantValue(nodeAnim, "frames"), NULL, 10);
			newAnim.fps                = (uint8_t) strtoull(GetConstantValue(nodeAnim, "fps"), NULL, 10);
			newAnim.spriteSheetTexture = animationTexture;
			newAnim.sheetSize          = (Vector2) {animationTexture.width, animationTexture.height};
			
			if(nodeAnim->childrenCount != 8){
				TraceLog(LOG_ERROR, "Each animation needs 8 coordinates");
				return false;
			}

			/* N, S, E, W, NE, NW, SE, SW coordinates */
			newAnim.n.x  = strtoull(GetConstantValue(nodeAnim->children[0], "x"), NULL, 10);
			newAnim.n.y  = strtoull(GetConstantValue(nodeAnim->children[0], "y"), NULL, 10);
			newAnim.s.x  = strtoull(GetConstantValue(nodeAnim->children[1], "x"), NULL, 10);
			newAnim.s.y  = strtoull(GetConstantValue(nodeAnim->children[1], "y"), NULL, 10);
			newAnim.e.x  = strtoull(GetConstantValue(nodeAnim->children[2], "x"), NULL, 10);
			newAnim.e.y  = strtoull(GetConstantValue(nodeAnim->children[2], "y"), NULL, 10);
			newAnim.w.x  = strtoull(GetConstantValue(nodeAnim->children[3], "x"), NULL, 10);
			newAnim.w.y  = strtoull(GetConstantValue(nodeAnim->children[3], "y"), NULL, 10);
			newAnim.ne.x = strtoull(GetConstantValue(nodeAnim->children[4], "x"), NULL, 10);
			newAnim.ne.y = strtoull(GetConstantValue(nodeAnim->children[4], "y"), NULL, 10);
			newAnim.nw.x = strtoull(GetConstantValue(nodeAnim->children[5], "x"), NULL, 10);
			newAnim.nw.y = strtoull(GetConstantValue(nodeAnim->children[5], "y"), NULL, 10);
			newAnim.se.x = strtoull(GetConstantValue(nodeAnim->children[6], "x"), NULL, 10);
			newAnim.se.y = strtoull(GetConstantValue(nodeAnim->children[6], "y"), NULL, 10);
			newAnim.sw.x = strtoull(GetConstantValue(nodeAnim->children[7], "x"), NULL, 10);
			newAnim.sw.y = strtoull(GetConstantValue(nodeAnim->children[7], "y"), NULL, 10);

			/* TODO: hash table with DA_APPEND_AT */
			DA_APPEND(newAnim, allAnimations);
		}	
	}else{
		TraceLog(LOG_ERROR, "Parser for animation data file did not return any data.");
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
		
		if(!AppendAnimationsData(data)){
			TraceLog(LOG_ERROR, "Couldn't parse animation data file %s", dataFile[i]);
			return false;		
		}

		fclose(data);
		i++;
	}

	return true;
}

void PlayAnimation(const char *sheetName, const char *animName, bool repeat, size_t animID, bool stack, Vector2 dir, Vector2 pos){
	assert(animationObjectArray[animID].isEnabled && "animation not enabled or freed");

	if(!animationObjectArray[animID].animData.animName == NULL){
		if(strcmp(animationObjectArray[animID].animData.animName, animName) == 0){
			/* already playing, only update direction, position and repeat */
			animationObjectArray[animID].onRepeat = repeat;
			animationObjectArray[animID].position = pos;
			animationObjectArray[animID].direction = dir;

			return;
		}

	}
	
	for(size_t i = 0; i < allAnimations.count; ++i){
		if(strcmp(allAnimations.items[i].animName,   animName) == 0 &&
			strcmp(allAnimations.items[i].sheetName, sheetName) == 0){
			animationObjectArray[animID].animData = allAnimations.items[i];
			animationObjectArray[animID].position = pos;
			animationObjectArray[animID].position = dir;
		animationObjectArray[animID].onRepeat = repeat;
		}
	}
	
}

bool IsAnimationPlaying(const char *sheetName, const char *animName, size_t animID){
	if(strcmp(animationObjectArray[animID].animData.sheetName,sheetName) == 0 &&
		strcmp(animationObjectArray[animID].animData.animName,animName)   == 0){
		return true;
	}
	return false;
}

void DrawAllAnimations(){
	for(size_t i = 0; i < idsArray.count; ++i){
		size_t id = idsArray.items[i];
		Rectangle rec = {
			.x = animationObjectArray[id].animData.se.x * 128,
			.y = animationObjectArray[id].animData.se.y * 128,
			.width = 128,
			.height = 128
		};
		DrawTextureRec(animationObjectArray[id].animData.spriteSheetTexture, rec, animationObjectArray[id].position, WHITE);
	}
}

size_t GetNewAnimation(){
	size_t newID = 0;
	if(idsArray.count == 0){
		DA_APPEND(newID, idsArray);
		animationObjectArray[newID].isEnabled = true;
	}else{
		bool foundSlot = false;
		for(size_t i = 0; i < MAX_ANIMATION_OBJECTS; ++i){
			if(!animationObjectArray[i].isEnabled){
				foundSlot = true;
				newID = i;
				DA_APPEND(newID, idsArray);
			}
		}
		assert(foundSlot && "Too many animations!");
	}

	return newID;
}

void RemoveAnimation(size_t id){
/*TODO*/
}
