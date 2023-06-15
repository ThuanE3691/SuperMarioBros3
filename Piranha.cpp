#include "Piranha.h"
#include "Mario.h"
#include "PlayScene.h"



void CFireBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BULLET_BBOX_WIDTH / 2;
	top = y - BULLET_BBOX_HEIGHT / 2;
	right = left + BULLET_BBOX_WIDTH;
	bottom = top + BULLET_BBOX_HEIGHT;
}

void CFireBullet::OnNoCollision(DWORD dt)
{	
	x += vx * dt;
	y += vy * dt;
}

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	float start_cx, start_cy, bbf_width, bbf_height;
	bbf_width = game->GetBackBufferWidth();
	bbf_height = game->GetBackBufferHeight();
	
	game->GetCamPos(start_cx, start_cy);
	float end_cx = start_cx + bbf_width;
	float end_cy = start_cy + bbf_height;

	if (!((x > start_cx && x < end_cx) && (y > start_cy && y < end_cy))) {
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CFireBullet::Render() {
	int aniId = ID_ANI_FIRE_BULLET_RIGHT;
	if (vx < 0)
		aniId = ID_ANI_FIRE_BULLET_LEFT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}


CPiranha::CPiranha(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	this->maxY = y - PIRANHA_BBOX_HEIGHT;
	this->minY = y;
	bullet = NULL;
	bullet_fire_start = -1;
	hidden_start = -1;
	direction = 1;
	firstLoad = true;
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
	y += vy * dt;
};

void CPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	bool inCamArea = false;
	CGame* game = CGame::GetInstance();
	float start_cx, cy, bbf_width;
	game->GetCamPos(start_cx, cy);
	bbf_width = game->GetBackBufferWidth();
	float end_cx = start_cx + bbf_width;
	float left, top, right, bottom;
	GetBoundingBox(left, top, right, bottom);
	if (left > start_cx && right < end_cx) {
		inCamArea = true;
	}

	DebugOut(L"[INFO] - IN CAMERA AREA: %d - STATE: %d - VY : %f\n", inCamArea, state, vy);
	
	if (inCamArea) {

		if (state == PIRANHA_STATE_HIDDEN) {
			if (firstLoad || (hidden_start != -1 && GetTickCount64() - hidden_start > PIRANHA_HIDDEN_TIME_OUT)) {
				hidden_start = -1;
				firstLoad = false;
				SetState(PIRANHA_STATE_RISING);
			}
			
		}

		if (state == PIRANHA_STATE_RISING && y < maxY) {
			SetState(PIRANHA_STATE_SHOOT_FIRE);
		}

		if (state == PIRANHA_STATE_HIDING && y > minY) {
			SetState(PIRANHA_STATE_HIDDEN);
			hidden_start = GetTickCount64();
		}

		if (state == PIRANHA_STATE_SHOOT_FIRE && GetTickCount64() - bullet_fire_start > PIRANHA_SHOOT_TIME_OUT) {
			SetState(PIRANHA_STATE_HIDING);
			bullet_fire_start = -1;
		}

		// JUST FOR FUN ONLY
		/*if (state == PIRANHA_STATE_SHOOT_FIRE && bullet != NULL && GetTickCount64() - bullet_fire_start > 200) {
			ShootMario();
			bullet_fire_start = GetTickCount64();
		}*/
	}
	else {
		firstLoad = true;
		if (state != PIRANHA_STATE_HIDDEN) {
			if (y < minY) {
				SetState(PIRANHA_STATE_HIDING);
			}
			else {
				SetState(PIRANHA_STATE_HIDDEN);
			}
		}
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
	
	// Position where bullet appear
	bullet = new CFireBullet(x + 2, y - BULLET_BBOX_HEIGHT);
	current_scene->AddObject(bullet);
	
	bullet->SetDirection(mx,my);
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
			// IMPORTANT THE CODE BELOW IS JUST FOR FUN

			// BEGIN
			bullet_fire_start = GetTickCount64();

			// END
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
