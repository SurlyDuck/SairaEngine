#include "player.h"
#include "../saira.h"

#define CAMERA_SPEED  500
#define PLAYER_SPEED  100

static Vector2 playerPosition  = {-64, -64};
static Vector2 playerDirection = {0};
static Vector2 playerTarget    = {0};
static Vector2 cameraPosition  = {0};
static Vector2 cameraVelocity  = {0};
static ANIMATION_CONTAINER *playerAnim = NULL;

static inline Vector2 GetPlayerPosition();
static inline Vector2 GetPlayerPositionOffset();

void InitPlayer(){
	playerDirection = SOUTHEAST_VEC;
	playerAnim = GetAnimationContainer();
	SetAnimation(playerAnim, "Default", "Walking", true);
	MoveAnimation(playerAnim, playerPosition, playerDirection);
	cameraPosition = (Vector2){playerPosition.x+64, playerPosition.y+64};
	playerTarget = GetPlayerPositionOffset();
}

void UpdatePlayer(float deltaTime){
	cameraVelocity.x = (IsAnyActionKeyDown(RIGHT_ACTION) - IsAnyActionKeyDown(LEFT_ACTION)) * CAMERA_SPEED * deltaTime;
	cameraVelocity.y = (IsAnyActionKeyDown(DOWN_ACTION) - IsAnyActionKeyDown(UP_ACTION)) * CAMERA_SPEED * deltaTime;
	cameraPosition.x += cameraVelocity.x;
	cameraPosition.y += cameraVelocity.y;
	
	CenterCamera(cameraPosition);
	Vector2 mos = GetScreenMousePosition();
	Vector2 cam = GetCameraPosition();
	Vector2 wrd = GetWorldMousePosition();

	PrintScreenMessage((Vector2){0, 20},"Mouse position :(%2.f,%2.f)", mos.x, mos.y);
	PrintScreenMessage((Vector2){0, 40},"Camera position:(%2.f,%2.f)", cam.x, cam.y);
	PrintScreenMessage((Vector2){0, 60},"Mouse world pos:(%2.f,%2.f)", wrd.x, wrd.y);
	

	if(IsAnyActionKeyDown(ACCEPT_ACTION)){
		playerTarget = GetWorldMousePosition();
	}

	if(Vector2Distance(playerTarget, GetPlayerPositionOffset()) > 1){
		Vector2 dir = Vector2Subtract(playerTarget, GetPlayerPositionOffset());
		dir = Vector2Normalize(dir);
		//SairaLog(SAIRA_INFO, "%.2f,%.2f",playerTarget.x, playerTarget.y);
		playerDirection = dir;
		playerPosition.x+= dir.x * PLAYER_SPEED * deltaTime;
		playerPosition.y+= dir.y * PLAYER_SPEED * deltaTime;
		SetAnimation(playerAnim, "Default", "Walking", true);
	}else{
		SetAnimation(playerAnim, "Default", "Idle_Spear", true);
	}
	
	MoveAnimation(playerAnim, GetPlayerPosition(), (Vector2){roundf(playerDirection.x), roundf(playerDirection.y)});

}

static inline Vector2 GetPlayerPosition(){
	return (Vector2) {playerPosition.x, playerPosition.y};
}

static inline Vector2 GetPlayerPositionOffset(){
	return (Vector2) {playerPosition.x+64, playerPosition.y+64};
}
