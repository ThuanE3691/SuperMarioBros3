#include "Button.h"

void CButton::Render() {
	int aniId = ID_ANI_BUTTON_NORMAL;
	switch (state)
	{
	case BUTTON_STATE_APPEAR:
		aniId = ID_ANI_BUTTON_APPEAR;
		break;
	case BUTTON_STATE_JUMP_ON:
		aniId = ID_ANI_BUTTON_JUMP_ON;
		break;
	default:
		break;
	}

	if (state != BUTTON_STATE_HIDDEN) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}
void CButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	if (state == BUTTON_STATE_APPEAR && GetTickCount64() - start_appear > BUTTON_APPEAR_TIME_OUT) {
		SetState(BUTTON_STATE_NORMAL);
	}
	else if (state == BUTTON_STATE_JUMP_ON && GetTickCount64() - start_jump_on > BUTTON_JUMP_ON_TIME_OUT) {

	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}
void CButton::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	switch (state)
	{
	case BUTTON_STATE_JUMP_ON:
		left = right = top = bottom = 0;
		break;
	case BUTTON_STATE_HIDDEN:
		left = right = top = bottom = 0;
		break;
	default: // STATE NORMAL AND APPEAR
		left = x - BUTTON_BBOX_WIDTH / 2;
		top = y - BUTTON_BBOX_HEIGHT / 2;
		right = left + BUTTON_BBOX_WIDTH;
		bottom = top + BUTTON_BBOX_HEIGHT;
		break;
	}
}
void CButton::SetState(int state) {
	switch (state) {
	case BUTTON_STATE_APPEAR:
		start_appear = GetTickCount64();
		break;
	case BUTTON_STATE_JUMP_ON:
		y += BUTTON_BBOX_HEIGHT / 4 + 1;
		start_jump_on = GetTickCount64();
		break;
	}
	CGameObject::SetState(state);
}