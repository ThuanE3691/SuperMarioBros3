#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define ID_ANI_COIN_DISAPPEAR 11100

#define COIN_NORMAL_STATE 1
#define COIN_UP_STATE 2
#define COIN_DISAPPEAR 3

#define COIN_UP_DISTANCE 40
#define COIN_SPEED_MOVE 0.25f

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

class CCoin : public CGameObject {
private:
	ULONGLONG up_start_time;
	float y_start_up;
public:
	CCoin(float x, float y) : CGameObject(x, y) { 
		y_start_up = -1;
		up_start_time = -1; 
		CGameObject::SetState(COIN_NORMAL_STATE); 
		
	}
	void SetState(int state);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};