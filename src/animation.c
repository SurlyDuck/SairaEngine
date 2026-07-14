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

bool LoadAnimationData(const char **dataFile){
	
}
