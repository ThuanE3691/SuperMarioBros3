#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Piranha.h"
#include "AssetIDs.h"

#define PIPE_NORMAL	1
#define PIPE_PIRANHA_RED	2
#define PIPE_PIRANHA_GREEN	3
#define PIPE_VENUS_GREEN	4


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
			case PIPE_PIRANHA_RED:
				piranha = new CPiranha(x - 1, y + PIRANHA_BBOX_HEIGHT / 3, PIRANHA_TYPE_PIRANHA);
				objects.push_back(piranha);
				break;
			case PIPE_PIRANHA_GREEN:
				piranha = new CPiranha(x - 1, y + 1, PIRANHA_TYPE_PIRANHA_GREEN);
				objects.push_back(piranha);
				break;
			case PIPE_VENUS_GREEN:
				piranha = new CPiranha(x - 1, y - 2, PIRANHA_TYPE_VENUS);
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