#include "Koopa.h"
#include "QuestionBlock.h"
#include "Mario.h"
#include "Platform.h"
#include "Piranha.h"
#include "Goomba.h"
#include "PlayScene.h"
#include "Brick.h"


CKoopa::CKoopa(float x, float y,int type) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	this->type = type;

	isOnHand = false;
	shell_wait_rotate_start = -1;
	die_by_attacking_start = -1;

	int direction = vx > 0.0 ? 1 : -1;
	if (vx == 0) direction = nx > 0 ? 1 : -1;
	wall = new CInvisibleWall(x + direction * KOOPA_BBOX_WIDTH, y + 2, KOOPA_BBOX_WIDTH, KOOPA_BBOX_HEIGHT);

	if (type != KOOPA_TYPE_GREEN_WING)	SetState(KOOPA_STATE_WALKING);
	else SetState(KOOPA_STATE_FLY);

	
	wall->SetSpeed(vx, vy);


	((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->AddObject(wall);


	/*float direction = nx > 0 ? 1 : -1;


	// ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->AddObject(wall); */
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (state)
	{
		case KOOPA_STATE_WALKING:
			left = x - KOOPA_BBOX_WIDTH / 2;
			top = y - KOOPA_BBOX_HEIGHT / 2;
			right = left + KOOPA_BBOX_WIDTH;
			bottom = top + KOOPA_BBOX_HEIGHT ;
			break;
		case KOOPA_STATE_FLY:
			left = x - KOOPA_BBOX_WIDTH / 2;
			top = y - KOOPA_BBOX_HEIGHT_FLY / 2;
			right = left + KOOPA_BBOX_WIDTH;
			bottom = top + KOOPA_BBOX_HEIGHT_FLY;
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
		case KOOPA_STATE_SHELL_TRANSFORM_WALKING:
			left = x - KOOPA_BBOX_WIDTH / 2;
			top = y - KOOPA_BBOX_HEIGHT_SHELL_TRANSFORM / 2;
			right = left + KOOPA_BBOX_WIDTH;
			bottom = top + KOOPA_BBOX_HEIGHT_SHELL_TRANSFORM;
			break;
	}
}

void CKoopa::SetInvisibleWall() {
	direction = vx > 0 ? 1 : -1;
	if (vx == 0) direction = nx > 0 ? 1 : -1;
	wall->SetPosition(x + direction * KOOPA_BBOX_WIDTH, y);
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;	
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	float left, right, top, bottom;

	e->obj->GetBoundingBox(left, top, right, bottom);

	if (state == KOOPA_STATE_SHELL_ROTATE || (isOnHand)) {
		if (dynamic_cast<CPiranha*>(e->obj)) {
			e->obj->SetState(PIRANHA_STATE_DIE_BY_ATTACK);
			if (isOnHand) {
				SetState(KOOPA_STATE_DIE_BY_ATTACKING);
			}
		}
		else if (dynamic_cast<CGoomba*>(e->obj)) {
			CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
			if (goomba->GetState() != GOOMBA_STATE_DIE_BY_ATTACK) {
				goomba->SetState(GOOMBA_STATE_DIE_BY_ATTACK);
			}
			if (isOnHand) {
				SetState(KOOPA_STATE_DIE_BY_ATTACKING);
			}
		}
		else if (!isOnHand && e->ny == 0) {
			if (dynamic_cast<CBrick*>(e->obj) && !isOnHand) {
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (brick->GetState() != BRICK_STATE_TO_COIN) {
					brick->SetState(BRICK_STATE_BREAK);
				}
			}
			else if (dynamic_cast<CQuestionBlock*>(e->obj)) {
				CQuestionBlock* qb = (CQuestionBlock*)e->obj;
				if (state == KOOPA_STATE_SHELL_ROTATE) {
					qb->SetState(EMPTY_BLOCK_STATE);
				}
			}
		}
	}

	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopa*>(e->obj)) return;

	// If go end then reverse in walking state
	/*if (state == KOOPA_STATE_WALKING) {
		if ((x < left && vx < 0) || (x > right && vx > 0)) {
			vx = -vx;
		}
	}*/

	float wx, wy;
	wall->GetPosition(wx,wy);

	if (wy - y > 10 && state == KOOPA_STATE_WALKING) {
		vx = -vx;
		SetInvisibleWall();
	}

	wall->SetSpeed(vx, 1);

	if (e->ny != 0 )
	{
		vy = 0;
		if (e->ny < 0 && state == KOOPA_STATE_FLY) {
			SetState(KOOPA_STATE_FLY);
		}
	}
	else if (e->nx != 0)
	{
		vx = -vx;
		SetInvisibleWall();
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (state == KOOPA_STATE_DIE_BY_ATTACKING && GetTickCount64() - die_by_attacking_start > KOOPA_SHELL_DIE_TIME_OUT) {
		isDeleted = true;
		wall->Delete();
		return;
	}

	if (state == KOOPA_STATE_SHELL_IDLE && shell_wait_rotate_start != -1 && GetTickCount64() - shell_wait_rotate_start > KOOPA_SHELL_WAIT_ROTATE_TIME_OUT) {
		SetState(KOOPA_STATE_SHELL_TRANSFORM_WALKING);
	}
	else if (state == KOOPA_STATE_SHELL_TRANSFORM_WALKING && shell_transform_start != -1 && GetTickCount64() - shell_transform_start > KOOPA_SHELL_TRANSFORM_WALKING_TIME_OUT) {
		SetState(KOOPA_STATE_WALKING);
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_SHELL_TRANSFORM) / 2;
	}
	else if (state == KOOPA_STATE_SHELL_TRANSFORM_WALKING) {
		vx = -vx;
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
		case KOOPA_STATE_FLY:
			if (vx < 0)
				aniId = ID_ANI_KOOPA_GREEN_FLY_LEFT;
			else
				aniId = ID_ANI_KOOPA_GREEN_FLY_RIGHT;
			break;
		case KOOPA_STATE_SHELL_IDLE:
			aniId = ID_ANI_KOOPA_SHELL_IDLE;
			break;
		case KOOPA_STATE_SHELL_ROTATE:
			aniId = ID_ANI_KOOPA_SHELL_ROTATE;
			break;
		case KOOPA_STATE_SHELL_TRANSFORM_WALKING:
			aniId = ID_ANI_KOOPA_SHELL_TRANSFORM_WALKING;
			break;
		case KOOPA_STATE_DIE_BY_ATTACKING:
			aniId = ID_ANI_KOOPA_DIE_REVERSE_SHELL;
			break;
	}

	if (type == KOOPA_TYPE_GREEN || (type == KOOPA_TYPE_GREEN_WING && state != KOOPA_STATE_FLY)) aniId += 100; // GREEN LIKE RED HAS ANIID = RED + 100
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
	switch (state)
	{
		case KOOPA_STATE_WALKING:
			shell_transform_start = -1;
			vx = -KOOPA_WALKING_SPEED;
			vy = 0;
			ay = KOOPA_GRAVITY;
			SetInvisibleWall();
			break;
		case KOOPA_STATE_FLY:
			shell_transform_start = -1;
			vx = -KOOPA_WALKING_FLOAT_SPEED;
			vy = -KOOPA_FLY_SPEED;
			ay = KOOPA_FLOAT_GRAVITY;
			break;
		case KOOPA_STATE_SHELL_IDLE:
			if (this->state == KOOPA_STATE_SHELL_ROTATE) {
				y -= 2;
			}
			vx = 0;
			shell_wait_rotate_start = GetTickCount64();
			break;
		case KOOPA_STATE_SHELL_ROTATE:
			if (this->state == KOOPA_STATE_SHELL_IDLE) {
				y -= 2;
			}
			shell_wait_rotate_start = -1;
			vx = -KOOPA_ROTATE_SPEED;
			ay = KOOPA_GRAVITY;
			break;
		case KOOPA_STATE_SHELL_TRANSFORM_WALKING:
			shell_wait_rotate_start = -1;
			shell_transform_start = GetTickCount64();
			vx = -KOOPA_TRANSFORM_SPEED;
			break;
		case KOOPA_STATE_DIE_BY_ATTACKING:
			vy = -KOOPA_DIE_REVERSE_SPEED;
			die_by_attacking_start = GetTickCount64();
			break;
	}
	CGameObject::SetState(state);
}
