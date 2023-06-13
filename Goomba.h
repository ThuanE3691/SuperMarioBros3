#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.03f

#define GOOMBA_TYPE_NORMAL	1
#define GOOMBA_TYPE_RED_WING	2

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 10
#define GOOMBA_BBOX_HEIGHT_DIE 5

#define GOOMBA_RED_WING_BBOX_WIDTH	16
#define GOOMBA_RED_WING_BBOX_HEIGHT 15
#define GOOMBA_RED_BBOX_HEIGHT	12
#define GOOMBA_RED_WING_BBOX_HEIGHT_DIE	5

#define GOOMBA_DIE_TIMEOUT 200

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_DECREASE_LEVEL 300

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001

#define ID_ANI_GOOMBA_RED_WING_WALK	5011
#define ID_ANI_GOOMBA_RED_WING_WALK_PREPARE_TO_FLY	5012
#define ID_ANI_GOOMBA_RED_WING_FLY	5013

#define ID_ANI_GOOMBA_RED_WALK	5021
#define ID_ANI_GOOMBA_RED_DIE	5031

class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 

	int typeGoomba;
	int level;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual int GetAni();

public: 	
	CGoomba(float x, float y, int typeGoomba);
	virtual void SetState(int state);

	virtual int GetLevel() { return level; }
};