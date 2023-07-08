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
	else if (state == BUTTON_STATE_JUMP_ON && start_jump_on != -1 && GetTickCount64() - start_jump_on > BUTTON_JUMP_ON_TIME_OUT) {
		ActiveEvents(EVENT_TURN_COIN_TO_BRICK);
		start_jump_on = -1;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}

bool IsInAxis(float start_axis, float end_axis, float axis_check) {
	return axis_check > start_axis && axis_check < end_axis;
}

void CButton::ActiveEvents(int events) {
	CGame* game = CGame::GetInstance();
	LPPLAYSCENE current_scene = (LPPLAYSCENE)game->GetCurrentScene();
	vector<LPGAMEOBJECT> objects = current_scene->GetObjects();

	float left_event = x - EVENT_RANGE_X;
	float right_event = x + EVENT_RANGE_X;
	float top_event = y - EVENT_RANGE_Y;
	float bottom_event = y + EVENT_RANGE_Y;


	for (int i = 0; i < objects.size(); i++) {
		float obj_x, obj_y;
		objects[i]->GetPosition(obj_x, obj_y);

		if (dynamic_cast<CBrick*>(objects[i])) {
			CBrick* brick = (CBrick*)objects[i];
			if (IsInAxis(left_event, right_event, obj_x) && IsInAxis(top_event, bottom_event, obj_y)) {
				if (events == EVENT_TURN_BRICK_TO_COIN && brick->GetState() == BRICK_STATE_NORMAL) {
					brick->SetState(BRICK_STATE_TO_COIN);
				}
				else if (events == EVENT_TURN_COIN_TO_BRICK && brick->GetState() == BRICK_STATE_TO_COIN) {
					brick->SetState(BRICK_STATE_NORMAL);
				}
			}
		}
	}
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
		ActiveEvents(EVENT_TURN_BRICK_TO_COIN);
		break;
	}
	CGameObject::SetState(state);
}