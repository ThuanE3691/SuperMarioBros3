#include "Background.h"

void CBackground::Render()
{
	CSprites* s = CSprites::GetInstance();
	s->Get(this->spriteIdBackground)->Draw(x, y);
}


void CBackground::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = r = 0;
	t = b = 0;
}