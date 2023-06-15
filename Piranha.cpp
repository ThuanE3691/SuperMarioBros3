#include "Piranha.h"
#include "Mario.h"
#include "PlayScene.h"

CFireBullet::CFireBullet(float x, float y, float target_x, float target_y) : CGameObject(x, y) {
	this->target_x = target_x;
	this->target_y = target_y;
}

void CFireBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BULLET_BBOX_WIDTH / 2;
	top = y - BULLET_BBOX_HEIGHT / 2;
	right = left + BULLET_BBOX_WIDTH;
	bottom = top + BULLET_BBOX_HEIGHT;
}

void CFireBullet::OnNoCollision(DWORD dt)
{	
	return;
}

void CFireBullet::Render() {
	int aniId = ID_ANI_FIRE_BULLET_RIGHT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}


CPiranha::CPiranha(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	this->maxY = y - PIRANHA_BBOX_HEIGHT;
	bullet = NULL;
	rising_start = -1;
	direction = 1;
	SetState(PIRANHA_STATE_HIDDEN);
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
	if (y >= maxY) {
		y += vy * dt;
	}
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

	if (state == PIRANHA_STATE_HIDDEN) {
		CGame* game = CGame::GetInstance();
		float start_cx, cy, bbf_width;
		game->GetCamPos(start_cx, cy);
		bbf_width = game->GetBackBufferWidth();
		float end_cx = start_cx + bbf_width;
		float left, top, right, bottom;
		GetBoundingBox(left, top, right, bottom);
		if (left > start_cx && right < end_cx) {
			SetState(PIRANHA_STATE_RISING);
		}
	}

	if (state == PIRANHA_STATE_RISING && y < maxY) {
		SetState(PIRANHA_STATE_SHOOT_FIRE);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


int CPiranha::GetAni() {


	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	float mx, my;
	mario->GetPosition(mx, my);
	
	int aniId = ID_ANI_PIRANHA_SHOOT_FIRE_TOP_LEFT;
	switch (state)
	{
		case PIRANHA_STATE_RISING:
			if (x > mx)
				aniId = ID_ANI_PIRANHA_SHOOT_FIRE_TOP_LEFT;
			else
				aniId = ID_ANI_PIRANHA_SHOOT_FIRE_TOP_RIGHT;
			break;
		case PIRANHA_STATE_SHOOT_FIRE:
			// MARIO IN RIGHT OF PIRANHA
			if (x < mx) {
				// MARIO IN BOTTOM RIGHT OF PIRANHA
				if (y < my) {
					aniId = ID_ANI_PIRANHA_SHOOT_FIRE_BOTTOM_RIGHT;
				}
				else
					aniId = ID_ANI_PIRANHA_SHOOT_FIRE_TOP_RIGHT;
			} // MARIO IN LEFT OF PIRANHA
			else {
				if (y < my) {
					aniId = ID_ANI_PIRANHA_SHOOT_FIRE_BOTTOM_LEFT;
				}
				else
					aniId = ID_ANI_PIRANHA_SHOOT_FIRE_TOP_LEFT;
			}
			break;
		case PIRANHA_STATE_HIDING:
			aniId = ID_ANI_PIRANHA_SHOOT_FIRE_BOTTOM_LEFT;
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

void CPiranha::ShootMario() {
	CGame* game = CGame::GetInstance();
	LPPLAYSCENE current_scene = (LPPLAYSCENE)game->GetCurrentScene();
	CMario* mario = (CMario*)current_scene->GetPlayer();

	float mx, my;
	mario->GetPosition(mx, my);

	float left, top, right, bottom;
	GetBoundingBox(left, top, right, bottom);

	// bullet = new CFireBullet(x, y - BULLET_BBOX_HEIGHT, mx, my);
	bullet = new CFireBullet(x, y - PIRANHA_BBOX_HEIGHT * 2, mx, my);
	current_scene->AddObject(bullet);
}

void CPiranha::SetState(int state)
{
	switch (state) {
		case PIRANHA_STATE_RISING:
			vy = -PIRANHA_RISING_SPEED;
			break;
		case PIRANHA_STATE_SHOOT_FIRE:
			vy = 0;
			ShootMario();
			break;
		case PIRANHA_STATE_HIDING:
			vy = PIRANHA_RISING_SPEED;
			break;
		case PIRANHA_STATE_HIDDEN:
			vy = 0;
			break;
	}
	CGameObject::SetState(state);
}
