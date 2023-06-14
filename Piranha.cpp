#include "Piranha.h"

CPiranha::CPiranha(float x, float y, int typeGoomba) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	rising_start = -1;
	direction = 1;
}

void CPiranha::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PIRANHA_BBOX_WIDTH / 2;
	top = y - PIRANHA_BBOX_HEIGHT / 2;
	right = left + PIRANHA_BBOX_WIDTH;
	bottom = top + PIRANHA_BBOX_HEIGHT;
}

void CPiranha::OnNoCollision(DWORD dt)
{
	y += vy * dt;
};

void CPiranha::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CPiranha*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
		direction = -1;
	}
}

void CPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


int CPiranha::GetAni() {
	float aniId = -1;
	switch (state)
	{
	case PIRANHA_STATE_RISING:
		aniId = PIRANHA_STATE_RISING;
		break;
	case PIRANHA_STATE_SHOOT_FIRE:
		aniId = ID_ANI_PIRANHA_SHOOT_FIRE_BOTTOM_LEFT;
		break;
	case PIRANHA_STATE_HIDING:
		aniId = PIRANHA_STATE_RISING;
		break;
	default:
		break;
	}
	return aniId;
}

void CPiranha::Render()
{
	int aniId = GetAni();
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CPiranha::SetState(int state)
{
	switch (state) {
		case PIRANHA_STATE_RISING:
			vy = -PIRANHA_RISING_SPEED;
			break;
		case PIRANHA_STATE_SHOOT_FIRE:
			vy = 0;
			break;
		case PIRANHA_STATE_HIDING:
			vy = PIRANHA_RISING_SPEED;
			break;
	}
	CGameObject::SetState(state);
}
