#pragma once
#include "GameObject.h"

#define PIRANHA_RISING_SPEED 0.02f

#define PIRANHA_BBOX_WIDTH 16
#define PIRANHA_BBOX_HEIGHT 33

#define PIRANHA_STATE_RISING 100
#define PIRANHA_STATE_SHOOT_FIRE	200
#define PIRANHA_STATE_HIDING	300
#define PIRANHA_STATE_HIDDEN	400

#define ID_ANI_PIRANHA_SHOOT_FIRE_TOP_LEFT	7001
#define ID_ANI_PIRANHA_SHOOT_FIRE_TOP_RIGHT	7002
#define ID_ANI_PIRANHA_SHOOT_FIRE_BOTTOM_LEFT	7003
#define ID_ANI_PIRANHA_SHOOT_FIRE_BOTTOM_RIGHT	7004

class CPiranha : public CGameObject
{
protected:
	float ax;
	float ay;

	float maxY;

	int direction;

	ULONGLONG rising_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual int GetAni();

public:
	CPiranha(float x, float y);
	virtual void SetState(int state);

};