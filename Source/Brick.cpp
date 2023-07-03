#include "Brick.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	if (state == BRICK_STATE_BREAK && GetTickCount64() - start_break > BRICK_BREAK_TIME_OUT) {
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBrick::OnCollisionWith(LPCOLLISIONEVENT e){}
void CBrick::SetState(int state) {
	switch (state) {
	case BRICK_STATE_BREAK:
		start_break = GetTickCount64();
		break;
	}
	CGameObject::SetState(state);
}


void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}