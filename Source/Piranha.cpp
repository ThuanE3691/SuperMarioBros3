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
	float start_cx, start_cy;
	int bbf_width, bbf_height;
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


CPiranha::CPiranha(float x, float y,int type = 1) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	this->type = type;

	switch (type)
	{
	case PIRANHA_TYPE_PIRANHA:
		this->maxY = y - PIRANHA_BBOX_HEIGHT;
		this->minY = y + PIRANHA_BBOX_HEIGHT / 2;
		break;
	case PIRANHA_TYPE_PIRANHA_GREEN:
		this->maxY = y - PIRANHA_GREEN_BBOX_HEIGHT;
		this->minY = y + PIRANHA_GREEN_BBOX_HEIGHT / 2;
		break;
	case PIRANHA_TYPE_VENUS:
		this->maxY = y - PIRANHA_VENUS_BBOX_HEIGHT;
		this->minY = y + PIRANHA_VENUS_BBOX_HEIGHT / 2;
		break;
	}

	bullet = NULL;
	bullet_fire_start = -1;
	hidden_start = -1;
	shoot_start = -1;
	direction = 1;
	firstLoad = true;
	SetState(PIRANHA_STATE_HIDDEN);
}

void CPiranha::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (type) {
	case PIRANHA_TYPE_PIRANHA:
		left = x - PIRANHA_BBOX_WIDTH / 2;
		top = y - PIRANHA_BBOX_HEIGHT / 2;
		right = left + PIRANHA_BBOX_WIDTH;
		bottom = top + PIRANHA_BBOX_HEIGHT;
		break;
	case PIRANHA_TYPE_PIRANHA_GREEN:
		left = x - PIRANHA_BBOX_WIDTH / 2;
		top = y - PIRANHA_GREEN_BBOX_HEIGHT / 2;
		right = left + PIRANHA_BBOX_WIDTH;
		bottom = top + PIRANHA_GREEN_BBOX_HEIGHT;
		break;
	case PIRANHA_TYPE_VENUS:
		left = x - PIRANHA_BBOX_WIDTH / 2;
		top = y - PIRANHA_VENUS_BBOX_HEIGHT / 2;
		right = left + PIRANHA_BBOX_WIDTH;
		bottom = top + PIRANHA_VENUS_BBOX_HEIGHT;
		break;
	}
}

void CPiranha::OnNoCollision(DWORD dt)
{
	y += vy * dt;
};

void CPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	if (state == PIRANHA_STATE_DIE_BY_ATTACK && GetTickCount64() - die_start > PIRANHA_DIE_TIME_OUT) {
		isDeleted = true;
		return;
	}

	bool inCamArea = false;
	CGame* game = CGame::GetInstance();
	float start_cx, cy;
	int	bbf_width;
	game->GetCamPos(start_cx, cy);
	bbf_width = game->GetBackBufferWidth();
	float end_cx = start_cx + bbf_width;
	float left, top, right, bottom;
	GetBoundingBox(left, top, right, bottom);
	if (left > start_cx && right < end_cx) {
		inCamArea = true;
	}

	if (inCamArea) {

		if (state == PIRANHA_STATE_HIDDEN) {
			if (firstLoad || (hidden_start != -1 && GetTickCount64() - hidden_start > PIRANHA_HIDDEN_TIME_OUT)) {
				hidden_start = -1;
				firstLoad = false;
				SetState(PIRANHA_STATE_RISING);
			}
			
		}

		if (state == PIRANHA_STATE_RISING && y <= maxY) {
			if (type != PIRANHA_TYPE_VENUS)	SetState(PIRANHA_STATE_SHOOT_FIRE);
			else {
				SetState(PIRANHA_STATE_WAIT);
			}
		}

		if (state == PIRANHA_STATE_HIDING && y >= minY) {
			SetState(PIRANHA_STATE_HIDDEN);
			hidden_start = GetTickCount64();
		}

		if (state == PIRANHA_STATE_SHOOT_FIRE) {
			if (shoot_start != -1 && GetTickCount64() - shoot_start > PIRANHA_WAIT_TO_SHOOT_TIME_OUT) {
				shoot_start = -1;
				ShootMario();
				bullet_fire_start = GetTickCount64();
			}
			else if (bullet_fire_start != -1 && GetTickCount64() - bullet_fire_start > PIRANHA_SHOOT_TIME_OUT) {
				SetState(PIRANHA_STATE_HIDING);
				bullet_fire_start = -1;
			}
		}

		if (state == PIRANHA_STATE_WAIT) {
			if (wait_start != -1 && GetTickCount64() - wait_start > PIRANHA_WAIT_TIME_OUT) {
				wait_start = -1;
				SetState(PIRANHA_STATE_HIDING);
			}
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
			if (y <= minY) {
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
	
	int aniId = -1;
	switch (state)
	{
		case PIRANHA_STATE_RISING:
			if (type != PIRANHA_TYPE_VENUS) {
				if (x > mx)
					aniId = ID_ANI_PIRANHA_MOVE_HEAD_TOP_LEFT;
				else
					aniId = ID_ANI_PIRANHA_MOVE_HEAD_TOP_RIGHT;
			}
			else {
				aniId = ID_ANI_VENUS_MOVE;
			}
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
			if (type != PIRANHA_TYPE_VENUS) {
				if (x < mx) {
					// MARIO IN BOTTOM RIGHT OF PIRANHA
					if (y < my) {
						aniId = ID_ANI_PIRANHA_MOVE_HEAD_BOTTOM_RIGHT;
					}
					else
						aniId = ID_ANI_PIRANHA_MOVE_HEAD_TOP_RIGHT;
				} // MARIO IN LEFT OF PIRANHA
				else {
					if (y < my) {
						aniId = ID_ANI_PIRANHA_MOVE_HEAD_BOTTOM_LEFT;
					}
					else
						aniId = ID_ANI_PIRANHA_MOVE_HEAD_TOP_LEFT;
				}
			}
			else {
				aniId = ID_ANI_VENUS_MOVE;
			}
			break;
		case PIRANHA_STATE_DIE_BY_ATTACK:
			aniId = ID_ANI_PIRANHA_DIE_BY_ATTACK;
			break;
		case PIRANHA_STATE_WAIT:
			aniId = ID_ANI_VENUS_MOVE;
			break;
		default:
			break;
	}

	if (type == PIRANHA_TYPE_PIRANHA_GREEN && state != PIRANHA_STATE_DIE_BY_ATTACK) {
		aniId += 100;
	}
	return aniId;
}

void CPiranha::Render()
{
	int aniId = GetAni();

	if (state != PIRANHA_STATE_HIDDEN)	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
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
			shoot_start = GetTickCount64();
			// IMPORTANT THE CODE BELOW IS JUST FOR FUN

			// BEGIN
			// bullet_fire_start = GetTickCount64();

			// END
			break;
		case PIRANHA_STATE_HIDING:
			vy = PIRANHA_RISING_SPEED;
			break;
		case PIRANHA_STATE_HIDDEN:
			vy = 0;
			break;
		case PIRANHA_STATE_DIE_BY_ATTACK:
			vy = 0;
			die_start = GetTickCount64();
			break;
		case PIRANHA_STATE_WAIT:
			vy = 0;
			wait_start = GetTickCount64();
			break;
	}
	CGameObject::SetState(state);
}
