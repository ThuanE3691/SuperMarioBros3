#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Piranha.h"
#include "AssetIDs.h"

#define PIPE_NORMAL	1
#define PIPE_VENUS	2

#define PIPE_WIDTH 32
#define PIPE_BBOX_WIDTH 32
#define PIPE_BBOX_HEIGHT 16

class CPipe : public CGameObject {
protected:
	int typePipe;
	int length;
	bool isHaveHole;
	CPiranha* piranha;
public:
	CPipe(float x, float y, int typePipe, int length, bool isHaveHole, vector<LPGAMEOBJECT>& objects) : CGameObject(x, y) {
		this->typePipe = typePipe; 
		this->length = length;
		this->isHaveHole = isHaveHole;
		piranha = NULL;
		switch (this->typePipe)
		{
			case PIPE_VENUS:
				piranha = new CPiranha(x - 1, y + 11, 2);
				objects.push_back(piranha);
				break;
			default:
				break;
		}
	}
	void Render();
	void RenderBoundingBox();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};