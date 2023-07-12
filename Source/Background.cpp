#include "Background.h"

void CBackground::Render()
{
	CSprites* s = CSprites::GetInstance();
	s->Get(this->spriteIdBackground)->Draw(x, y);

	// Draw only 1 object
	if (this->length == 1) return;

	// Draw LEFT TO RIGHT
	if (this->drawDirection == 1) {

		float xx = x + this->cell;

		for (int i = 1; i < this->length; i++)
		{
			s->Get(this->spriteIdBackground)->Draw(xx, y);
			xx += this->cell;
		}
	} // DRAW TOP TO BOTTOM
	else {
		float yy = y + this->cell;
		for (int i = 1; i < this->length; i++)
		{
			s->Get(this->spriteIdBackground)->Draw(x, yy);
			yy += this->cell;
		}
	}

}


void CBackground::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = r = 0;
	t = b = 0;
}