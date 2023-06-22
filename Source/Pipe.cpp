#include "Pipe.h"
#include "PlayScene.h"

void CPipe::Render()
{

	CSprites* sprites = CSprites::GetInstance();
	int currentLength = 0;
	if (isHaveHole) {
		sprites->Get(ID_SPRITE_PIPE_HEAD)->Draw(x, y);
		currentLength += 1;
	}
	
	if (currentLength < length) {
		for (int i = currentLength; i <= length; i++) {
			sprites->Get(ID_SPRITE_PIPE_BODY)->Draw(x, y + i * PIPE_BBOX_HEIGHT);
		}
	}

	RenderBoundingBox();
}

void CPipe::RenderBoundingBox() {
	if (((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetIsEnableRenderBoundingBox() == true) {
		D3DXVECTOR3 p(x, y, 0);
		RECT rect;

		LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

		float l, t, r, b;

		GetBoundingBox(l, t, r, b);
		rect.left = 0;
		rect.top = 0;
		rect.right = (int)r - (int)l;
		rect.bottom = (int)b - (int)t;

		float cx, cy;
		CGame::GetInstance()->GetCamPos(cx, cy);

		float yy = y - PIPE_BBOX_HEIGHT / 2 + rect.bottom / 2;

		CGame::GetInstance()->Draw(x - cx, yy - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
	}
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PIPE_BBOX_WIDTH / 2 + 2;
	t = y - PIPE_BBOX_HEIGHT / 2;
	r = l + PIPE_BBOX_WIDTH - 2;

	int lengthActual = (isHaveHole == true) ? this->length + 1 : this->length;

	b = t + PIPE_BBOX_HEIGHT * lengthActual;
}