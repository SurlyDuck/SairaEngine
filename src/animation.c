/* Saíra animation system */
#include "saira.h"

#define MAX_ANIMATION_CONTAINERS 1024

typedef struct{
	const char *sheetName;
	const char *filePath; /* maybe unnecessary? */
	const char *animName;
	Texture2D spriteSheetTexture;
	uint8_t frames;
	uint8_t fps;
	uint8_t cellSize;
	Vector2 n;
	Vector2 s;
	Vector2 e;
	Vector2 w;
	Vector2 ne;
	Vector2 nw;
	Vector2 se;
	Vector2 sw;
	Vector2 sheetSize;
}animation;

typedef struct{
	animation *items;
	size_t count;
	size_t capacity;
}animations;

// Animation container representing the actual object rendered on screen
typedef struct ANIMATION_CONTAINER ANIMATION_CONTAINER;
struct ANIMATION_CONTAINER{
	animation animData;
	bool isHidden;
	bool onRepeat;
	bool isPlaying;
	uint8_t frame;
	float frameTime;
	Vector2 position;
	Vector2 direction;
};

static animations allAnimations = {0}; /* animation data array */
static ANIMATION_CONTAINER *containers = NULL;
static size_t containersCount = 0;

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
			newAnim.filePath           = allNodes[0].constants[0]->value; /* dangerous */
			newAnim.cellSize	 	 		= (uint8_t) atoi(GetConstantValue(&allNodes[0], "cell"));
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

/* TODO: this maybe too slow. Maybe implement a hash table for anim data? */
void SetAnimation(ANIMATION_CONTAINER *container, const char *sheetName, const char *animName, bool repeat){
	for(size_t i = 0; i < allAnimations.count; ++i){
		if(strcmp(allAnimations.items[i].sheetName, sheetName) == 0 &&
		strcmp(allAnimations.items[i].animName, animName) == 0){
			container->animData = allAnimations.items[i];
			container->onRepeat = repeat;
			break;
		}
	}
		
}

void MoveAnimation(ANIMATION_CONTAINER *container, Vector2 position, Vector2 direction){
	container->position  = position;
	container->direction = direction;
}

void SetAnimationFrame(ANIMATION_CONTAINER *container, size_t frame){
	container->frame = frame;
}

void PlayAnimation(ANIMATION_CONTAINER *container, bool repeat){
	container->isPlaying = true;
	container->onRepeat = repeat;
}

bool IsAnimationSheet(ANIMATION_CONTAINER *container, const char *sheetName){
	if(container->animData.sheetName == NULL) return false;
	return strcmp(container->animData.sheetName, sheetName) == 0;
}

bool IsAnimationVisible(ANIMATION_CONTAINER *container){
	return !container->isHidden;
}

void SetAnimationVisibility(ANIMATION_CONTAINER *container, bool visible){
	container->isHidden = !visible;
}

bool IsAnimationPlaying(ANIMATION_CONTAINER *container){
	return container->isPlaying;
}

bool IsAnimationName(ANIMATION_CONTAINER *container, const char *animName){
	if(container->animData.animName == NULL) return false;
	return strcmp(container->animData.animName, animName) == 0;
}

void DrawAllAnimationContainers(){
	if(containers == NULL) return;
	for(size_t i = 0; i < containersCount; ++i){
		if(!containers[i].isPlaying) continue;
		animation animData = containers[i].animData;
		
		Rectangle r = {0};
		switch(GetDirectionFromVector(containers[i].direction)){
			case NORTH: r.x = animData.n.x,  r.y = animData.n.y;  break;
			case SOUTH: r.x = animData.s.x,  r.y = animData.s.y;  break;
			case EAST: r.x = animData.e.x,  r.y = animData.e.y;  break;
			case WEST: r.x = animData.w.x,  r.y = animData.w.y;  break;
			case NORTHEAST: r.x = animData.ne.x, r.y = animData.ne.y; break;
			case NORTHWEST: r.x = animData.nw.x, r.y = animData.nw.y; break;
			case SOUTHEAST: r.x = animData.se.x, r.y = animData.se.y; break;
			case SOUTHWEST: r.x = animData.sw.x, r.y = animData.sw.y; break;
			default: return;
		}
		r.width  = animData.cellSize;
		r.height = animData.cellSize;
		r.x *= animData.cellSize;
		r.y *= animData.cellSize;
		
		containers[i].frameTime += GetFrameTime();
		if(containers[i].frameTime >= (float)(1.00f/animData.fps)) {
			containers[i].frame++;
			containers[i].frameTime = 0;
		}
		if(containers[i].frame >= animData.frames-1){
			containers[i].frame = 0;
			containers[i].frameTime = 0.00f;
		}	
		r.x += containers[i].frame * animData.cellSize;

		DrawTextureRec(animData.spriteSheetTexture, r, containers[i].position, WHITE);
	}
	
	return;
}

ANIMATION_CONTAINER* GetAnimationContainer(){
	assert(containersCount < MAX_ANIMATION_CONTAINERS && "too many animations");

	ANIMATION_CONTAINER emptyContainer = {0};
	emptyContainer.isPlaying = true;

	if(containersCount == 0){
		containers = (ANIMATION_CONTAINER*) malloc(sizeof(ANIMATION_CONTAINER) * MAX_ANIMATION_CONTAINERS);
	}
	containers[containersCount] = emptyContainer;
	containersCount++;
	return &containers[containersCount-1];
	
}

void Exit(){

}
