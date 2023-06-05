#include "Koopa.h"

CKoopa::CKoopa(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	die_start = -1;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (state)
	{
		case KOOPA_STATE_DIE:
			left = x - KOOPA_BBOX_WIDTH / 2;
			top = y - KOOPA_BBOX_HEIGHT_DIE / 2;
			right = left + KOOPA_BBOX_WIDTH;
			bottom = top + KOOPA_BBOX_HEIGHT_DIE;
			break;
		case KOOPA_STATE_WALKING:
			left = x - KOOPA_BBOX_WIDTH / 2;
			top = y - KOOPA_BBOX_HEIGHT / 2;
			right = left + KOOPA_BBOX_WIDTH;
			bottom = top + KOOPA_BBOX_HEIGHT;
			break;
		case KOOPA_STATE_SHELL_IDLE:
			left = x - KOOPA_BBOX_WIDTH / 2;
			top = y - KOOPA_BBOX_HEIGHT_SHELL / 2;
			right = left + KOOPA_BBOX_WIDTH;
			bottom = top + KOOPA_BBOX_HEIGHT_SHELL;
			break;
		case KOOPA_STATE_SHELL_ROTATE:
			left = x - KOOPA_BBOX_WIDTH / 2;
			top = y - KOOPA_BBOX_HEIGHT_SHELL / 2;
			right = left + KOOPA_BBOX_WIDTH;
			bottom = top + KOOPA_BBOX_HEIGHT_SHELL;
			break;
	}
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopa*>(e->obj)) return;

	float left, right, top, bottom;

	e->obj->GetBoundingBox(left, top, right, bottom);

	// If go end then reverse in walking state
	if (state == KOOPA_STATE_WALKING) {
		if ((x < left && vx < 0) || (x > right && vx > 0)) {
			vx = -vx;
		}
	}

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == KOOPA_STATE_DIE) && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

int CKoopa::GetAni() {
	int aniId = -1;
	switch (state) {
		case KOOPA_STATE_WALKING:
			if (vx < 0)
				aniId = ID_ANI_KOOPA_WALKING_LEFT;
			else 
				aniId = ID_ANI_KOOPA_WALKING_RIGHT;
			break;
		case KOOPA_STATE_SHELL_IDLE:
			aniId = ID_ANI_KOOPA_SHELL_IDLE;
			break;
		case KOOPA_STATE_SHELL_ROTATE:
			aniId = ID_ANI_KOOPA_SHELL_ROTATE;
			break;
	}
	return aniId;
}

void CKoopa::Render()
{
	int aniId = GetAni();

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_DIE:
		die_start = GetTickCount64();
		y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case KOOPA_STATE_WALKING:
		vx = -KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_SHELL_IDLE:
		vx = 0;
		break;
	case KOOPA_STATE_SHELL_ROTATE:
		vx = -KOOPA_ROTATE_SPEED;
		break;
	}
}
