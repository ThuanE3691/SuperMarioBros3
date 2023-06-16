#include "Goomba.h"
#include "debug.h"

CGoomba::CGoomba(float x, float y,int typeGoomba):CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	fly_start = -1;
	jump_count = 0;
	direction = 1;
	SetState(GOOMBA_STATE_WALKING);
	this->typeGoomba = typeGoomba;
	if (typeGoomba == GOOMBA_TYPE_NORMAL) level = 1;
	else {
		level = 2;
		walk_start = GetTickCount64();
	}
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
				if (state == GOOMBA_STATE_WALKING || state == GOOMBA_STATE_JUMPING_WALK) {
					left = x - GOOMBA_RED_WING_BBOX_WIDTH / 2;
					top = y - GOOMBA_RED_WING_CLOSE_BBOX_HEIGHT / 2;
					right = left + GOOMBA_RED_WING_BBOX_WIDTH;
					bottom = top + GOOMBA_RED_WING_CLOSE_BBOX_HEIGHT;
				}
				else{
					left = x - GOOMBA_RED_WING_BBOX_WIDTH / 2;
					top = y - GOOMBA_RED_WING_BBOX_HEIGHT / 2;
					right = left + GOOMBA_RED_WING_BBOX_WIDTH;
					bottom = top + GOOMBA_RED_WING_BBOX_HEIGHT;
				}
			}
			// WALK WITH NO WING
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
		if (e->ny < 0) {
			// Is on platform
			if (state == GOOMBA_STATE_JUMPING) {
				SetState(GOOMBA_STATE_JUMPING_WALK);
				walk_start = GetTickCount64();
				// vy = -GOOMBA_JUMP_SPEED;
			}
			else if (state == GOOMBA_STATE_FLY) {
				SetState(GOOMBA_STATE_WALKING);
			}
		}
	}
	else if (e->nx != 0)
	{
		vx = -vx;
		direction = -1;
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

	if ((state == GOOMBA_STATE_DIE_BY_ATTACK) && (GetTickCount64() - die_start > GOOMBA_DIE_BY_ATTACK_TIMEOUT)) {
		isDeleted = true;
		return;
	}

	if (level == 2) {
		DebugOut(L"[INFO] GOOMBA - STATE: %d - VX: %f - VY: %f\n", state, vx, vy);
		if (state == GOOMBA_STATE_WALKING && GetTickCount64() - walk_start > GOOMBA_RED_WING_WALK_TIMEOUT) {
			walk_start = -1;
			SetState(GOOMBA_STATE_JUMPING);
		}
		else if (state == GOOMBA_STATE_JUMPING || state == GOOMBA_STATE_JUMPING_WALK) {
			if (jump_count >= 4) {
				jump_count = 0;
				walk_start = -1;
				SetState(GOOMBA_STATE_FLY);
			}
			else if (state == GOOMBA_STATE_JUMPING_WALK && GetTickCount64() - walk_start > GOOMBA_RED_WING_JUMP_TIMEOUT) {
				walk_start = -1;
				SetState(GOOMBA_STATE_JUMPING);
			}
		}
	}
	

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


int CGoomba::GetAni() {
	float aniId = -1;
	switch(typeGoomba){
		case GOOMBA_TYPE_NORMAL:
			if (state == GOOMBA_STATE_DIE) aniId = ID_ANI_GOOMBA_DIE;
			else if (state == GOOMBA_STATE_DIE_BY_ATTACK) aniId = ID_ANI_GOOMBA_DIE_BY_ATTACK;
			else aniId = ID_ANI_GOOMBA_WALKING;
			break;
		case GOOMBA_TYPE_RED_WING:
			if (state == GOOMBA_STATE_DIE) {
				aniId = ID_ANI_GOOMBA_RED_DIE;
			}
			else if (state == GOOMBA_STATE_DIE_BY_ATTACK) {
				if (level == 1) {
					aniId = ID_ANI_GOOMBA_RED_DIE_BY_ATTACK;
				}
				else
					aniId = ID_ANI_GOOMBA_RED_WING_DIE_BY_ATTACK;
			}
			else if (level == 2) {
				if (state == GOOMBA_STATE_JUMPING)
					aniId = ID_ANI_GOOMBA_RED_WING_JUMP;
				else if (state == GOOMBA_STATE_FLY)
					aniId = ID_ANI_GOOMBA_RED_WING_FLY;
				else
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
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			switch (typeGoomba) {
			case GOOMBA_TYPE_NORMAL:
				y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
				break;
			case GOOMBA_TYPE_RED_WING:
				y += (GOOMBA_RED_BBOX_HEIGHT - GOOMBA_RED_WING_BBOX_HEIGHT_DIE) / 2;
				break;
			}
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED * direction;
			ay = GOOMBA_GRAVITY;
			if (level == 2) {
				walk_start = GetTickCount64();
			}
			break;
		case GOOMBA_DECREASE_LEVEL:
			level -= 1;
			SetState(GOOMBA_STATE_WALKING);
			return;
		case GOOMBA_STATE_JUMPING:
			y -= (GOOMBA_RED_WING_BBOX_HEIGHT - GOOMBA_RED_WING_CLOSE_BBOX_HEIGHT) / 2;
			vy = -GOOMBA_JUMP_SPEED;
			jump_start = GetTickCount64();
			jump_count += 1;
			break;
		case GOOMBA_STATE_JUMPING_WALK:
			// y += (GOOMBA_RED_WING_BBOX_HEIGHT - GOOMBA_RED_WING_CLOSE_BBOX_HEIGHT) / 2;
			break;
		case GOOMBA_STATE_FLY:
			vy = -GOOMBA_FLY_SPEED;
			ay = GOOMBA_GRAVITY / 2;
			break;
		case GOOMBA_STATE_DIE_BY_ATTACK:
			vy = -GOOMBA_DIE_BY_ATTACK_SPEED;
			die_start = GetTickCount64();
			break;
	}
	CGameObject::SetState(state);

}
