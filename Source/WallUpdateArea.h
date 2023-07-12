#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define WALL_UPDATE_BBOX_WIDTH	16
#define WALL_UPDATE_BBOX_HEIGHT	9999

class CWallUpdateArea : public CGameObject {
public:
	CWallUpdateArea(float x, float y) : CGameObject(x, y) {
		
	}
	void Render() { RenderBoundingBox(); }
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	int IsBlocking() { return 0; }
	int IsCollidable() { return 0; }
};