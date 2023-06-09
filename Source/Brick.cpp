#include "Brick.h"
#include "PlayScene.h"
#include "debug.h"

void CBrick::Render()
{
	int aniId = ID_ANI_BRICK;
	switch (state)
	{
	case BRICK_STATE_TO_COIN:
		aniId = ID_ANI_COIN_STATIC;
		break;
	default:
		break;
	}
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	if (state == BRICK_STATE_BREAK && GetTickCount64() - start_break > BRICK_BREAK_TIME_OUT) {
		isDeleted = true;
		CGame* game = CGame::GetInstance();
		LPPLAYSCENE current_scene = (LPPLAYSCENE)game->GetCurrentScene();
		current_scene->AddObject(new CShapeBrick(x + 4.0f, y + 16.0f, 1));
		current_scene->AddObject(new CShapeBrick(x + 4.0f, y - 4.0f, 1));
		current_scene->AddObject(new CShapeBrick(x - 4.0f, y + 16.0f, -1));
		current_scene->AddObject(new CShapeBrick(x - 4.0f, y - 4.0f, -1));
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBrick::SetState(int state) {
	switch (state) {
		case BRICK_STATE_BREAK:
			start_break = GetTickCount64();
			break;
	}
	CGameObject::SetState(state);
}


void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	switch (state) {
	case BRICK_STATE_TO_COIN:
		l = x - COIN_BBOX_WIDTH / 2;
		t = y - COIN_BBOX_HEIGHT / 2;
		r = l + COIN_BBOX_WIDTH;
		b = t + COIN_BBOX_HEIGHT;
		break;
	default:
		l = x - BRICK_BBOX_WIDTH / 2;
		t = y - BRICK_BBOX_HEIGHT / 2;
		r = l + BRICK_BBOX_WIDTH;
		b = t + BRICK_BBOX_HEIGHT;
		break;
	}
}


void CShapeBrick::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_SHAPE)->Render(x, y);
}

void CShapeBrick::OnNoCollision(DWORD dt) {
	DebugOut(L"Has going to no collision\n");
	x += vx * dt;
	y += vy * dt;
}

void CShapeBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vy += ay * dt;

	if (GetTickCount64() - start_appear > SHAPE_APPEAR_TIME_OUT) {
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CShapeBrick::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - SHAPE_BBOX_WIDTH / 2;
	t = x - SHAPE_BBOX_HEIGHT / 2;
	r = l + SHAPE_BBOX_WIDTH;
	b = t + SHAPE_BBOX_HEIGHT;
}

void CShapeBrick::SetState(int state) {
	switch (state) {
	case SHAPE_STATE_UP:
		vx = direction * SHAPE_SPEED;
		vy = -SHAPE_SPEED_UP;
		start_appear = GetTickCount64();
		break;
	}
	CGameObject::SetState(state);
}

