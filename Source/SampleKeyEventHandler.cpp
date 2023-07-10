#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	LPPLAYSCENE currentScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario *)currentScene->GetPlayer();

	bool enableRenderBoundingBox = currentScene->GetIsEnableRenderBoundingBox();

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACOON);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_NUMPAD1: // Print the current position and speed of mario
		float x, y, vx, vy;
		int	nx;
		mario->GetPosition(x, y);
		mario->GetSpeed(vx, vy);
		nx = mario->GetNx();
		DebugOut(L"[INFO] MARIO - [POSITION] X: %f, Y: %f - [SPEED] VX: %f, VY = %f, NX = %f\n",x,y,vx,vy,nx);
		break;
	case DIK_EQUALS: // Turn on/off rendering bounding box
		currentScene->SetIsEnableRenderBoundingBox(!enableRenderBoundingBox);
		break;
	case DIK_R: // reset
		CGame::GetInstance()->GetCurrentScene()->Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	case DIK_A: {
		mario->SetHandingMode(false);
		mario->SetState(MARIO_STATE_HOLDING_RELEASE);
	}
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();


	if (game->IsKeyDown(DIK_A)) {
		mario->SetHandingMode(true);

	}
	 
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A)) {
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		}
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A)) {
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		}
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}