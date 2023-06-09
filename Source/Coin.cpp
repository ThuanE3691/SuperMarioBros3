#include "Coin.h"


void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state == COIN_UP_STATE) {
		if (y_start_up - y > COIN_UP_DISTANCE) {
			vy = -vy;
		}
		else if (vy > 0 && y + 2 > y_start_up) {
			CGameObject::SetState(COIN_DISAPPEAR);
			disappear_time = GetTickCount64();
			return;
		}
		y += vy * dt;
	}
	else if (state == COIN_DISAPPEAR && GetTickCount64() - disappear_time > COIN_DISAPPEAR_TIME_ANIMATION) {
		isDeleted = true;
		return;
	}

	if (state == COIN_HIDDEN_STATE) {
		CGameObject::Update(dt, coObjects);
	}
}

void CCoin::Render()
{
	int id_ani = ID_ANI_COIN;
	if (state == COIN_DISAPPEAR) {
		id_ani = ID_ANI_COIN_DISAPPEAR;
	}

	if (state != COIN_HIDDEN_STATE) {
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(id_ani)->Render(x, y);
	}
	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoin::SetState(int state) {
	CGameObject::SetState(state);
	if (state == COIN_UP_STATE) {
		y_start_up = y;
		vy = -COIN_SPEED_MOVE;
	}
}