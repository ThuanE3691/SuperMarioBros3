#include "QuestionBlock.h"

void CQuestionBlock::SetState(int state) {
	int old_state = this->state;
	CGameObject::SetState(state);

	if (old_state == QUESTION_BLOCK_STATE && state == EMPTY_BLOCK_STATE) {
		coin = new CCoin(x, y - 20);
		coin->SetState(COIN_UP_STATE);
		y -= BLOCK_UP_DISTANCE;
		up_start = GetTickCount64();
	}
	
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	if ((this->state == EMPTY_BLOCK_STATE) && (up_start != -1) && (GetTickCount64() - up_start > BLOCK_UP_TIME_OUT)) {
		y += BLOCK_UP_DISTANCE;
		up_start = -1;
		coObjects->push_back(coin);
	}

	if (this->coin) {
		coin->Update(dt, coObjects);
	}

	CGameObject::Update(dt, coObjects);
}

void CQuestionBlock::Render()
{
	int id_ani = ID_ANI_QUESTION_BLOCK;
	if (state == EMPTY_BLOCK_STATE) {
		id_ani = ID_ANI_EMPTY_BLOCK;
	}
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(id_ani)->Render(x, y);
	if (this->coin && this->coin->IsDeleted() == false) {
		coin->Render();
	}
	//RenderBoundingBox();
}

void CQuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QBLOCK_WIDTH / 2;
	t = y - QBLOCK_BBOX_HEIGHT / 2;
	r = l + QBLOCK_BBOX_WIDTH;
	b = t + QBLOCK_BBOX_HEIGHT;
}