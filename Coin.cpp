#include "Coin.h"


void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state == COIN_UP_STATE) {
		if (y_start_up - y > COIN_UP_DISTANCE) {
			vy = -vy;
		}
		else if (vy > 0 && y + 5 > y_start_up) {
			CGameObject::SetState(COIN_DISAPPEAR);
			return;
		}
		y += vy * dt;
	}
	else if (state == COIN_DISAPPEAR) {
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
}

void CCoin::Render()
{
	int id_ani = ID_ANI_COIN;
	if (state == COIN_DISAPPEAR) {
		id_ani = ID_ANI_COIN_DISAPPEAR;
	}
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(id_ani)->Render(x, y);

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
		up_start_time = GetTickCount64();
	}
}