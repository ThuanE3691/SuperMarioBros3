#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "PlayScene.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16


#define BRICK_STATE_NORMAL	1
#define BRICK_STATE_BREAK	2
#define BRICK_STATE_TO_COIN	3
#define BRICK_STATE_POWER_UP	4
#define BRICK_STATE_BLOCK	5
#define BRICK_STATE_EMPTY_BLOCK	6

#define BRICK_BREAK_TIME_OUT	0

#define ID_SPRITE_SHAPE_BRICK	20100
#define ID_ANI_SHAPE	10001

#define SHAPE_SPEED	0.06f
#define SHAPE_GRAVITY	0.0015f

#define SHAPE_SPEED_UP	0.3f

#define SHAPE_STATE_UP	1
#define SHAPE_APPEAR_TIME_OUT	500



class CShapeBrick : public CGameObject {
private:
	float ax, ay;
	int direction;
	ULONGLONG start_appear;

public:
	CShapeBrick(float x, float y, int direction) : CGameObject(x, y) {
		this->ax = 0.0f;
		this->ay = SHAPE_GRAVITY;
		this->direction = direction;
		start_appear = -1;
		SetState(SHAPE_STATE_UP);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnNoCollision(DWORD dt);
	void SetState(int state);
};

class CBrick : public CGameObject {
private:
	ULONGLONG start_break;
public:
	CBrick(float x, float y, int type) : CGameObject(x, y) {
		start_break = -1;
		if (type != BRICK_STATE_BLOCK) SetState(BRICK_STATE_NORMAL);
		else SetState(BRICK_STATE_BLOCK);
	}

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetState(int state);
};

