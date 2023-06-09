#include "QuestionBlock.h"

CQuestionBlock::CQuestionBlock(float x, float y, vector<LPGAMEOBJECT>& objects,int type_block) : CGameObject(x, y) {
	up_start = -1;
	this->type_block = type_block;
	this->ay = 0;
	y_init = y;
	CGameObject::SetState(QUESTION_BLOCK_STATE);
	coin = NULL;
	pu = NULL;
	bu = NULL;
	switch (type_block) {
		case QBLOCK_TYPE_COIN:
			coin = new CCoin(x, y - 20);
			coin->SetState(COIN_HIDDEN_STATE);
			objects.push_back(coin);
			break;
		case QBLOCK_TYPE_POWERUP:
			pu = new CPowerUp(x, y - (QBLOCK_BBOX_HEIGHT - MUSHROOM_BBOX_HEIGHT) / 2);
			pu->SetState(POWER_UP_HIDDEN_STATE);
			objects.push_back(pu);
			break;
		case BRICK_BLOCK_STATE:
			bu = new CButton(x, y - QBLOCK_BBOX_HEIGHT);
			CGameObject::SetState(BRICK_BLOCK_STATE);
			objects.push_back(bu);
			break;
	}
}

void CQuestionBlock::OnNoCollision(DWORD dt) {
	y += vy * dt;
}

void CQuestionBlock::SetState(int state) {
	int old_state = this->state;
	CGameObject::SetState(state);

	if ((old_state == QUESTION_BLOCK_STATE || old_state == BRICK_BLOCK_STATE) && state == EMPTY_BLOCK_STATE) {
		vy = -QUESTION_BLOCK_DEFLECT_SPEED;
		ay = QUESTION_BLOCK_GRAVITY;
		up_start = GetTickCount64();
	}
	
}

void CQuestionBlock::ActiveEvents() {
	LPPLAYSCENE current_scene = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene());
	CMario* mario = (CMario*)current_scene->GetPlayer();
	int current_level = mario->GetLevel();
	switch (this->type_block) {
	case QBLOCK_TYPE_COIN:
		coin->SetState(COIN_UP_STATE);
		break;
	case QBLOCK_TYPE_POWERUP:
		if (current_level <= 1)	pu->SetState(MUSHROOM_UP_STATE);
		else pu->SetState(LEAF_UP_STATE);
		break;
	case BRICK_BLOCK_STATE:
		bu->SetState(BUTTON_STATE_APPEAR);
		break;
	}
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	vy += ay * dt;

	if (this->state == EMPTY_BLOCK_STATE) {
		if ((up_start != -1) && (GetTickCount64() - up_start > BLOCK_UP_TIME_OUT)) {
			vy = QUESTION_BLOCK_DEFLECT_SPEED;
			up_start = -1;
		}
		
		if (y > y_init && vy != 0) {
			vy = 0;
			ay = 0;
			y = y_init;
			ActiveEvents();
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CQuestionBlock::Render()
{
	int id_ani = ID_ANI_QUESTION_BLOCK;
	if (state == BRICK_BLOCK_STATE) {
		id_ani = ID_ANI_BRICK;
	}
	else if (state == EMPTY_BLOCK_STATE) {
		id_ani = ID_ANI_EMPTY_BLOCK;
	}
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(id_ani)->Render(x, y);
	RenderBoundingBox();
}

void CQuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QBLOCK_WIDTH / 2;
	t = y - QBLOCK_BBOX_HEIGHT / 2;
	r = l + QBLOCK_BBOX_WIDTH;
	b = t + QBLOCK_BBOX_HEIGHT;
}