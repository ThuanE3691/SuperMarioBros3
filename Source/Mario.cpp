#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "Platform.h"
#include "QuestionBlock.h"
#include "PowerUp.h"
#include "Koopa.h"
#include "Piranha.h"
#include "Button.h"

#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (x < 10) x = 10;

	if (isHolding) {
		if (enemies && dynamic_cast<CKoopa*>(enemies)) {
			if (enemies->GetState() != KOOPA_STATE_WALKING && enemies->GetState() != KOOPA_STATE_DIE_BY_ATTACKING)
				MarioHolding();
			else {
				dynamic_cast<CKoopa*>(enemies)->SetOnHand(false);
				isHolding = false;
				if (enemies->GetState() != KOOPA_STATE_DIE_BY_ATTACKING) MarioIsAttacked();
				enemies = NULL;
			}
		}
		else {
			enemies = NULL;
		}
	}
	else{
		if (enemies && dynamic_cast<CKoopa*>(enemies)) {
			dynamic_cast<CKoopa*>(enemies)->SetOnHand(false);
			kick_start = GetTickCount64();
			SetState(MARIO_STATE_KICK);
			enemies->SetState(KOOPA_STATE_SHELL_ROTATE);
			float koopa_vx, koopa_vy;
			enemies->GetSpeed(koopa_vx, koopa_vy);
			if (nx > 0 && koopa_vx < 0) {
				enemies->SetSpeed(-koopa_vx, koopa_vy);
			}
			enemies = NULL;
		}
	}

	vy += ay * dt;
	vx += ax * dt;

	// Set max speed for mario
	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	
	if (isTransform) {
		if (GetTickCount64() - transform_start > MARIO_TRANSFORM_TIME_OUT) {
			isTransform = false;
			transform_start = -1;
			if (level == MARIO_LEVEL_SMALL) {
				level = MARIO_LEVEL_BIG;
			}
			else if (level == MARIO_LEVEL_BIG) {
				level = MARIO_LEVEL_SMALL;
			}
		}
		else this->SetState(MARIO_STATE_TRANSFORM);
	}

	if (kick_start != -1 && GetTickCount64() - kick_start > MARIO_KICK_TIME_OUT)
	{
		kick_start = -1;
	}
	else if (kick_start != -1) {
		this->SetState(MARIO_STATE_KICK);
	}

	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::MarioIsAttacked() {
	if (level > MARIO_LEVEL_SMALL)
	{
		StartUntouchable();
		isTransform = true;
		transform_start = GetTickCount64();
		this->SetState(MARIO_STATE_TRANSFORM);
	}
	else
	{
		DebugOut(L">>> Mario DIE >>> \n");
		SetState(MARIO_STATE_DIE);
	}
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;

		if (enemies) {
			if (dynamic_cast<CKoopa*>(enemies)) {
				enemies->SetSpeed(vx, vy);
			}
		}
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CPiranha*>(e->obj))
		OnCollisionWithPiranha(e);
	else if (dynamic_cast<CFireBullet*>(e->obj))
		OnCollisionWithFireBullet(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CQuestionBlock*>(e->obj))
		OnCollisionWithQuestionBlock(e);
	else if (dynamic_cast<CButton*>(e->obj))
		OnCollisionWithButton(e);
	else if (dynamic_cast<CPowerUp*>(e->obj))
		OnCollisionWithPowerUp(e);
	else if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetLevel() == 1) {
			if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_DIE_BY_ATTACK)
			{
				goomba->SetState(GOOMBA_STATE_DIE);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
		else {
			goomba->SetState(GOOMBA_DECREASE_LEVEL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0 && goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_DIE_BY_ATTACK)
		{
			MarioIsAttacked();
		}
	}
}

void CMario::MarioHolding() {
	int direction = (vx >= 0) ? 1 : -1;
	if (vx == 0) direction = (nx >= 0) ? 1 : -1;
	dynamic_cast<CKoopa*>(enemies)->SetOnHand(true);
	enemies->SetSpeed(vx, vy);
	switch (level) {
	case MARIO_LEVEL_SMALL:
		if (direction == 1) {
			if (vx == 0)
				enemies->SetPosition(x + direction * (MARIO_SMALL_BBOX_WIDTH + KOOPA_BBOX_WIDTH + 2) / 2, y);
			else
				enemies->SetPosition(x + direction * (MARIO_SMALL_BBOX_WIDTH + KOOPA_BBOX_WIDTH + 8) / 2, y);
		}
		else {
			if (vx == 0)
				enemies->SetPosition(x + direction * (MARIO_SMALL_BBOX_WIDTH + KOOPA_BBOX_WIDTH - 2) / 2, y);
			else
				enemies->SetPosition(x + direction * (MARIO_SMALL_BBOX_WIDTH + KOOPA_BBOX_WIDTH + 6) / 2, y);
		}
		break;
	case MARIO_LEVEL_BIG:
		if (direction == 1) {
			if (vx == 0)
				enemies->SetPosition(x + direction * (MARIO_BIG_BBOX_WIDTH + KOOPA_BBOX_WIDTH - 3) / 2, y - 1);
			else
				enemies->SetPosition(x + direction * (MARIO_BIG_BBOX_WIDTH + KOOPA_BBOX_WIDTH + 5) / 2, y - 1);
		}
		else {
			if (vx == 0)
				enemies->SetPosition(x + direction * (MARIO_BIG_BBOX_WIDTH + KOOPA_BBOX_WIDTH - 6) / 2, y - 2);
			else
				enemies->SetPosition(x + direction * (MARIO_BIG_BBOX_WIDTH + KOOPA_BBOX_WIDTH + 3) / 2, y - 2);
		}
		break;
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e) {
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	if (koopa->GetState() == KOOPA_STATE_DIE_BY_ATTACKING) return;

	// jump on top >> koopa transform to shell and deflect a bit 
	if (e->ny < 0)
	{
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		if (koopa->GetState() != KOOPA_STATE_SHELL_ROTATE) {
			if (koopa->GetState() != KOOPA_STATE_SHELL_IDLE)
			{
				koopa->SetState(KOOPA_STATE_SHELL_IDLE);
			}
			else if (koopa->GetState() == KOOPA_STATE_SHELL_IDLE) {
				kick_start = GetTickCount64();
				SetState(MARIO_STATE_KICK);
				koopa->SetState(KOOPA_STATE_SHELL_ROTATE);
			}
		}
		else {
			koopa->SetState(KOOPA_STATE_SHELL_IDLE);
		}
	} // Kick shell
	else if (e->nx != 0 && koopa->GetState() == KOOPA_STATE_SHELL_IDLE) {
		if (handingMode == false) {
			koopa->SetOnHand(false);
			float vx, vy;
			kick_start = GetTickCount64();
			SetState(MARIO_STATE_KICK);
			koopa->SetState(KOOPA_STATE_SHELL_ROTATE);
			koopa->GetSpeed(vx, vy);
			if (e->nx < 0) {
				if (vx < 0) koopa->SetSpeed(-vx, vy);
			}
			else if (e->nx > 0) {
				if (vx > 0) koopa->SetSpeed(-vx, vy);
			}
		}
		else {
			enemies = koopa;
			isHolding = true;
			MarioHolding();
		}
	}
	else  // hit by koopa
	{
		if (untouchable == 0)
		{
			if (koopa->GetState() != KOOPA_STATE_SHELL_IDLE && koopa->GetState() != KOOPA_STATE_SHELL_TRANSFORM_WALKING) {
				MarioIsAttacked();
			}
		}
	}
	
}

void CMario::OnCollisionWithPiranha(LPCOLLISIONEVENT e) {
	if (untouchable == 0) {
		if (e->obj->GetState() != PIRANHA_STATE_DIE_BY_ATTACK)
			MarioIsAttacked();
	}
}

void CMario::OnCollisionWithFireBullet(LPCOLLISIONEVENT e) {
	if (untouchable == 0) {
		e->obj->Delete();
		MarioIsAttacked();
	}
}

void CMario::OnCollisionWithButton(LPCOLLISIONEVENT e) {
	if (e->ny < 0) {
		e->obj->SetState(BUTTON_STATE_JUMP_ON);
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	int coin_state = e->obj->GetState();
	if (coin_state == COIN_NORMAL_STATE){
		e->obj->Delete();
		coin++;
	}
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e) {
	CQuestionBlock* qb = (CQuestionBlock*)e->obj;
	if (e->ny > 0) {
		if (qb->GetState() != EMPTY_BLOCK_STATE) {
			if (qb->GetTypeBlock() == QBLOCK_TYPE_COIN)
				coin++;
			else {
				
			}
		}
		qb->SetState(EMPTY_BLOCK_STATE);
	}
}

void CMario::OnCollisionWithPowerUp(LPCOLLISIONEVENT e) {
	CPowerUp* pu = (CPowerUp*)e->obj;
	if (pu->GetState() == MUSHROOM_WALKING_STATE) {
		if (level == MARIO_LEVEL_SMALL) {
			isTransform = true;
			transform_start = GetTickCount64();
			this->SetState(MARIO_STATE_TRANSFORM);
			SetLevel(MARIO_LEVEL_BIG);
		}
		pu->Delete();
	}
}

void CMario::OnCollisionWithBrick(LPCOLLISIONEVENT e) {
	CBrick* brick = (CBrick*)e->obj;
	if (brick->GetState() == BRICK_STATE_TO_COIN) {
		brick->Delete();
		coin++;
	}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (this->state == MARIO_STATE_TRANSFORM) {
		if (nx > 0)
			aniId = ID_ANI_MARIO_SMALL_TRANSFORM_TO_BIG_RIGHT;
		else
			aniId = ID_ANI_MARIO_SMALL_TRANSFORM_TO_BIG_LEFT;
	}
	else if (this->state == MARIO_STATE_KICK) {
		if (nx > 0)
			aniId = ID_ANI_MARIO_SMALL_KICK_RIGHT;
		else
			aniId = ID_ANI_MARIO_SMALL_KICK_LEFT;
	}
	else if (isHolding) {
		if (vx == 0) {
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_HOLDING_RIGHT_IDLE;
			else
				aniId = ID_ANI_MARIO_SMALL_HOLDING_LEFT_IDLE;
		}
		else if (vx > 0)
		{
			if (ax < 0)
				aniId = ID_ANI_MARIO_SMALL_HOLDING_RIGHT_IDLE;
			else if (ax == MARIO_ACCEL_RUN_X)
				aniId = ID_ANI_MARIO_SMALL_HOLDING_RIGHT_RUN;
			else if (ax == MARIO_ACCEL_WALK_X)
				aniId = ID_ANI_MARIO_SMALL_HOLDING_RIGHT_WALK;
		}
		else // vx < 0
		{
			if (ax > 0)
				aniId = ID_ANI_MARIO_SMALL_HOLDING_LEFT_IDLE;
			else if (ax == -MARIO_ACCEL_RUN_X)
				aniId = ID_ANI_MARIO_SMALL_HOLDING_LEFT_RUN;
			else if (ax == -MARIO_ACCEL_WALK_X)
				aniId = ID_ANI_MARIO_SMALL_HOLDING_LEFT_WALK;
		}
	}
	else if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else {
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else {
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}
		}
	}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (this->state == MARIO_STATE_TRANSFORM) {
		if (nx > 0)
			aniId = ID_ANI_MARIO_BIG_TRANSFORM_TO_SMALL_RIGHT;
		else
			aniId = ID_ANI_MARIO_BIG_TRANSFORM_TO_SMALL_LEFT;
	}
	else if (this->state == MARIO_STATE_KICK) {
		if (nx > 0)
			aniId = ID_ANI_MARIO_BIG_KICK_RIGHT;
		else
			aniId = ID_ANI_MARIO_BIG_KICK_LEFT;
	}
	else if (isHolding) {
		if (vx == 0) {
			if (nx > 0)
				aniId = ID_ANI_MARIO_BIG_HOLDING_RIGHT_IDLE;
			else
				aniId = ID_ANI_MARIO_BIG_HOLDING_LEFT_IDLE;
		}
		else if (vx > 0)
		{
			if (ax < 0)
				aniId = ID_ANI_MARIO_BIG_HOLDING_RIGHT_IDLE;
			else if (ax == MARIO_ACCEL_RUN_X)
				aniId = ID_ANI_MARIO_BIG_HOLDING_RIGHT_RUN;
			else if (ax == MARIO_ACCEL_WALK_X)
				aniId = ID_ANI_MARIO_BIG_HOLDING_RIGHT_WALK;
		}
		else // vx < 0
		{
			if (ax > 0)
				aniId = ID_ANI_MARIO_BIG_HOLDING_LEFT_IDLE;
			else if (ax == -MARIO_ACCEL_RUN_X)
				aniId = ID_ANI_MARIO_BIG_HOLDING_LEFT_RUN;
			else if (ax == -MARIO_ACCEL_WALK_X)
				aniId = ID_ANI_MARIO_BIG_HOLDING_LEFT_WALK;
		}
	}
	else if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();

	if (untouchable && GetTickCount64() % 2 == 0)
		animations->Get(aniId)->Render(x, y);
	else if (!untouchable) 
		animations->Get(aniId)->Render(x, y);


	RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 
	CGameObject::SetState(state);


	switch (this->state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_KICK:
		vx = 0.0f;
		// ax = 0.0f;
		break;

	case MARIO_STATE_HOLDING:
		isHolding = true;
		break;

	case MARIO_STATE_HOLDING_RELEASE:
		isHolding = false;
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_TRANSFORM:
		vy = 0;
		vx = 0;
		ax = 0;
		break;
	}


}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL && isTransform == false)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	else if (this->level == MARIO_LEVEL_SMALL && isTransform == true) {
		y -=  MARIO_SMALL_BBOX_HEIGHT / 2;
	}
	if (!isTransform) {
		level = l;
	}
}

