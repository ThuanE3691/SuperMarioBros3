#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.03f
#define KOOPA_ROTATE_SPEED 0.15f
#define KOOPA_TRANSFORM_SPEED	0.1f


#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 22
#define KOOPA_BBOX_HEIGHT_SHELL 14
#define KOOPA_BBOX_HEIGHT_SHELL_TRANSFORM 12


#define KOOPA_SHELL_WAIT_ROTATE_TIME_OUT 2000
#define KOOPA_SHELL_TRANSFORM_WALKING_TIME_OUT	1000

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_SHELL_IDLE	200
#define KOOPA_STATE_SHELL_ROTATE	300
#define KOOPA_STATE_SHELL_TRANSFORM_WALKING	400

#define ID_ANI_KOOPA_WALKING_LEFT 6000
#define ID_ANI_KOOPA_WALKING_RIGHT 6001
#define ID_ANI_KOOPA_SHELL_IDLE 6002
#define ID_ANI_KOOPA_SHELL_ROTATE 6003
#define ID_ANI_KOOPA_SHELL_TRANSFORM_WALKING 6004


class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG shell_wait_rotate_start;
	ULONGLONG shell_transform_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	int GetAni();

public:
	CKoopa(float x, float y);
	virtual void SetState(int state);
};