#pragma once

#include "GameObject.h"
#include "AssetIDs.h"


#define LEFT_TO_RIGHT	1
#define TOP_TO_BOTTOM	2

// 
// The most popular type of object in Mario! 
// 
class CPlatform : public CGameObject
{
protected: 
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;
	int drawDirection;
public: 
	CPlatform(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end, int drawDirection = 1) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
		this->drawDirection = drawDirection;

		switch (this->spriteIdBegin) {
			case(ID_SPRITE_CLOUD_BEGIN): {
				this->SetSolid(true);
				break;
			}
			case(ID_SPRITE_BOX_PINK_BEGIN): {
				this->SetSolid(true);
				break;
			}
		}
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int IsDirectionColliable(float nx, float ny);
};

typedef CPlatform* LPPLATFORM;