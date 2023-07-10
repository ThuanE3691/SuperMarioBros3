#include "PowerUp.h"
#include "Mario.h"

CPowerUp::CPowerUp(float x, float y) : CGameObject(x,y) {
	this->ax = 0;
	this->ay = 0;
	y_target = -1;
	x_init = x;
	x_target = -1;
	SetState(POWER_UP_HIDDEN_STATE);
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

void CPowerUp::DirectionOfPowerUp() {
	LPPLAYSCENE current_scene = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene());
	CMario* mario = (CMario*) current_scene->GetPlayer();
	float m_x, m_y;// Mario x,y
	mario->GetPosition(m_x, m_y);
	if (x > m_x) {
		power_up_direction = -power_up_direction;
	}
}

void CPowerUp::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CPowerUp*>(e->obj)) return;

	if (state == MUSHROOM_UP_STATE) return;

	if (dynamic_cast<CMario*>(e->obj)) {
		DirectionOfPowerUp();
		return;
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

void CPowerUp::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state != POWER_UP_HIDDEN_STATE) {
		vy += ay * dt;
		vx += ax * dt;
	}

	if (y_target != -1 && y <= y_target) {
		switch (state)
		{
		case MUSHROOM_UP_STATE:
			SetState(MUSHROOM_WALKING_STATE);
			break;
		case LEAF_UP_STATE:
			SetState(LEAF_DOWN_STATE);
			DirectionOfPowerUp();
			vx = vx * power_up_direction;
			break;
		default:
			break;
		}
	}

	if (state == LEAF_DOWN_STATE) {
		if (vx > 0 && x >= x_target) {
			vx = -vx;
			vy = 0;
		}
		else if (vx < 0 && x <= x_init) {
			vx = -vx;
			vy = 0;
		}
	}

	
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CPowerUp::Render()
{
	int aniId = ID_ANI_MUSHROOM;

	if (state == LEAF_UP_STATE) {
		aniId = ID_ANI_LEAF_FLY_RIGHT;
	}
	else if (state == LEAF_DOWN_STATE) {
		if (vx < 0)
			aniId = ID_ANI_LEAF_FLY_LEFT;
		else
			aniId = ID_ANI_LEAF_FLY_RIGHT;
	}

	if (state != POWER_UP_HIDDEN_STATE) {
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}
	RenderBoundingBox();
}

void CPowerUp::SetState(int state)
{
	CGameObject::SetState(state);
	
	switch (state)
	{
		case POWER_UP_HIDDEN_STATE:
			vx = 0;
			vy = 0;
			ax = 0;
			ay = 0;
			break;
		case MUSHROOM_WALKING_STATE:
			vx = -MUSHROOM_WALKING_SPEED * power_up_direction;
			ay = MUSHROOM_GRAVITY;
			break;
		case MUSHROOM_UP_STATE:
			ay = 0;
			vx = 0;
			vy = -MUSHROOM_UP_SPEED;
			y_target = y - MUSHROOM_BBOX_HEIGHT;
			break;
		case LEAF_UP_STATE:
			vy = -LEAF_SPEED_UP;
			y_target = y - 3 * LEAF_BBOX_HEIGHT;
			x_target = x + 2 * 16;
			ay = 0;
			vx = 0;
			break;
		case LEAF_DOWN_STATE:
			vy = 0;
			vx = -LEAF_FLOAT_SPEED;
			ay = LEAF_GRAVITY;
			break;
	}
}
