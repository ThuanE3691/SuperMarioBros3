#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Piranha.h"

#define PIPE_NORMAL	1
#define PIPE_VENUS	2

#define ID_ANI_PIPE 15000
#define PIPE_WIDTH 32
#define PIPE_BBOX_WIDTH 32
#define PIPE_BBOX_HEIGHT 48

class CPipe : public CGameObject {
protected:
	int typePipe;
	CPiranha* piranha;
public:
	CPipe(float x, float y, int typePipe, vector<LPGAMEOBJECT>& objects) : CGameObject(x, y) {
		this->typePipe = typePipe; 
		switch (this->typePipe)
		{
		case PIPE_NORMAL:
			piranha = NULL;
			break;
		case PIPE_VENUS:
			piranha = new CPiranha(x - 1, y - (PIPE_BBOX_HEIGHT - PIRANHA_BBOX_HEIGHT) / 2);
			objects.push_back(piranha);
			break;
		default:
			break;
		}
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};