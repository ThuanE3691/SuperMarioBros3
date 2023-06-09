#pragma once
#include "GameObject.h"
#include "AnimationIDs.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.03f
#define GOOMBA_JUMP_SPEED	0.1f
#define GOOMBA_FLY_SPEED	0.3f
#define GOOMBA_DIE_BY_ATTACK_SPEED	0.4f

#define GOOMBA_TYPE_NORMAL	1
#define GOOMBA_TYPE_RED_WING	2

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 10
#define GOOMBA_BBOX_HEIGHT_DIE 5

#define GOOMBA_RED_WING_BBOX_WIDTH	16
#define GOOMBA_RED_WING_BBOX_HEIGHT	20
#define GOOMBA_RED_WING_CLOSE_BBOX_HEIGHT 14
#define GOOMBA_RED_WING_OPEN_BBOX_HEIGHT 20
#define GOOMBA_RED_BBOX_HEIGHT	10
#define GOOMBA_RED_WING_BBOX_HEIGHT_DIE	5

#define GOOMBA_DIE_TIMEOUT 200
#define GOOMBA_DIE_BY_ATTACK_TIMEOUT	1000
#define GOOMBA_RED_WING_WALK_TIMEOUT	1000
#define GOOMBA_RED_WING_JUMP_TIMEOUT	200

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_DECREASE_LEVEL 300
#define GOOMBA_STATE_WING_CLOSE 400
#define GOOMBA_STATE_WING_OPEN 500
#define GOOMBA_STATE_JUMPING 600
#define GOOMBA_STATE_JUMPING_WALK	650
#define GOOMBA_STATE_FLY	700
#define GOOMBA_STATE_DIE_BY_ATTACK	800



class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 

	int direction;

	int typeGoomba;
	int level;

	int jump_count;

	ULONGLONG die_start;
	ULONGLONG walk_start;

	ULONGLONG jump_start;
	ULONGLONG fly_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return state != GOOMBA_STATE_DIE_BY_ATTACK; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual int GetAni();

public: 	
	CGoomba(float x, float y, int typeGoomba);
	virtual void SetState(int state);

	virtual int GetLevel() { return level; }
};