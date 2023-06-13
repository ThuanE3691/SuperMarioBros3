#include "Goomba.h"

CGoomba::CGoomba(float x, float y,int typeGoomba):CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
	this->typeGoomba = typeGoomba;
	if (typeGoomba == GOOMBA_TYPE_NORMAL) level = 1;
	else level = 2;
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	switch(typeGoomba){
		case GOOMBA_TYPE_NORMAL:
			if (state == GOOMBA_STATE_DIE)
			{
				left = x - GOOMBA_BBOX_WIDTH/2;
				top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
				right = left + GOOMBA_BBOX_WIDTH;
				bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
			}
			else
			{ 
				left = x - GOOMBA_BBOX_WIDTH/2;
				top = y - GOOMBA_BBOX_HEIGHT/2;
				right = left + GOOMBA_BBOX_WIDTH;
				bottom = top + GOOMBA_BBOX_HEIGHT;
			}
			break;
		case GOOMBA_TYPE_RED_WING:
			if (state == GOOMBA_STATE_DIE)
			{
				left = x - GOOMBA_BBOX_WIDTH / 2;
				top = y - GOOMBA_RED_WING_BBOX_HEIGHT_DIE / 2;
				right = left + GOOMBA_BBOX_WIDTH;
				bottom = top + GOOMBA_RED_WING_BBOX_HEIGHT_DIE;
			}
			else if (level == 2)
			{
				left = x - GOOMBA_RED_WING_BBOX_WIDTH / 2;
				top = y - GOOMBA_RED_WING_BBOX_HEIGHT / 2;
				right = left + GOOMBA_RED_WING_BBOX_WIDTH;
				bottom = top + GOOMBA_RED_WING_BBOX_HEIGHT;
			}
			else {
				left = x - GOOMBA_RED_WING_BBOX_WIDTH / 2;
				top = y - GOOMBA_RED_BBOX_HEIGHT / 2;
				right = left + GOOMBA_RED_WING_BBOX_WIDTH;
				bottom = top + GOOMBA_RED_BBOX_HEIGHT;
			}
			break;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 

	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ( (state==GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


int CGoomba::GetAni() {
	float aniId = -1;
	switch(typeGoomba){
		case GOOMBA_TYPE_NORMAL:
			if (state == GOOMBA_STATE_DIE)
			{
				aniId = ID_ANI_GOOMBA_DIE;
			}
			else
				aniId = ID_ANI_GOOMBA_WALKING;
			break;
		case GOOMBA_TYPE_RED_WING:
			if (state == GOOMBA_STATE_DIE) {
				aniId = ID_ANI_GOOMBA_RED_DIE;
			}
			else if (level == 2) {
				aniId = ID_ANI_GOOMBA_RED_WING_WALK;
			}
			else
				aniId = ID_ANI_GOOMBA_RED_WALK;
			break;
	}
	return aniId;
}

void CGoomba::Render()
{
	int aniId = GetAni();
	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			switch (typeGoomba) {
			case GOOMBA_TYPE_NORMAL:
				y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
				break;
			case GOOMBA_TYPE_RED_WING:
				y += (GOOMBA_RED_WING_BBOX_HEIGHT - GOOMBA_RED_WING_BBOX_HEIGHT_DIE) / 2;
				break;
			}
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_DECREASE_LEVEL:
			level -= 1;
			break;
	}
}
