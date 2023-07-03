#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16


#define BRICK_STATE_NORMAL	1
#define BRICK_STATE_BREAK	2
#define BRICK_STATE_TO_COIN	3
#define BRICK_STATE_POWER_UP	4

#define BRICK_BREAK_TIME_OUT	200

#define ID_SPRITE_SHAPE_BRICK	20100
#define SHAPE_SPEED	0.3f

#define SHAPE_GRAVITY	0.0002f

class CBrick : public CGameObject {
private:
	ULONGLONG start_break;
public:
	CBrick(float x, float y) : CGameObject(x, y) {
		start_break = -1;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void OnCollisionWith(LPCOLLISIONEVENT e);
	void SetState(int state);
};

class CShapeBrick : public CGameObject {
public:
	CShapeBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};