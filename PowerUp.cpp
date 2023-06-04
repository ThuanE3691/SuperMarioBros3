#include "PowerUp.h"

CPowerUp::CPowerUp(float x, float y) : CGameObject(x,y) {
	this->ax = 0;
	this->ay = MUSHROOM_GRAVITY;
	SetState(MUSHROOM_WALKING_STATE);
}

void CPowerUp::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MUSHROOM_BBOX_WIDTH / 2;
	top = y - MUSHROOM_BBOX_HEIGHT / 2;
	right = left + MUSHROOM_BBOX_WIDTH;
	bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CPowerUp::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CPowerUp::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CPowerUp*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CPowerUp::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CPowerUp::Render()
{
	int aniId = ID_ANI_MUSHROOM;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CPowerUp::SetState(int state)
{
	CGameObject::SetState(state);
	
	switch (state)
	{
		case MUSHROOM_WALKING_STATE:
			vx = -MUSHROOM_WALKING_SPEED;
			break;
	}
}