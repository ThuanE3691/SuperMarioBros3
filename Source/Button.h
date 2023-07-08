#pragma once
#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AnimationIDs.h"
#include "PlayScene.h"

#define BUTTON_WIDTH 16
#define BUTTON_BBOX_WIDTH 16
#define BUTTON_BBOX_HEIGHT 16

#define BUTTON_STATE_APPEAR	1
#define BUTTON_STATE_NORMAL	2
#define BUTTON_STATE_JUMP_ON	3
#define BUTTON_STATE_HIDDEN	4

#define BUTTON_APPEAR_TIME_OUT	200
#define BUTTON_JUMP_ON_TIME_OUT	6000

#define EVENT_TURN_BRICK_TO_COIN	1
#define EVENT_TURN_COIN_TO_BRICK	2

#define EVENT_RANGE_X	100
#define EVENT_RANGE_Y	100


class CButton : public CGameObject {
private:
	ULONGLONG start_appear;
	ULONGLONG start_jump_on;
public:
	CButton(float x, float y) : CGameObject(x, y) {
		start_appear = -1;
		start_jump_on = -1;
		SetState(BUTTON_STATE_HIDDEN);
	}

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	void ActiveEvents(int event);
};

