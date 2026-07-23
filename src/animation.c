/* Saíra animation Module */
#include "saira.h"

struct animation{
	const char *sheetName;
	const char *filePath; /* maybe unnecessary? */
	const char *animName;
	Texture2D SpriteSheetTexture;
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
	size_t count;
	size_t capacity;
}animations;

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
	animations animationsArray = {};
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
		
		Texture2D animationTexture  = LoadTexture(allNodes[0].constants[0]->value);
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
			newAnim.SpriteSheetTexture = animationTexture;
			newAnim.sheetSize          = (Vector2) {animationTexture.width, animationTexture.height};
			TraceLog(LOG_ERROR, "%f,%f", newAnim.sheetSize.x, newAnim.sheetSize.y);
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
