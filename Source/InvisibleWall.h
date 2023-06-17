#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class CInvisibleWall : public CGameObject {
protected:

public:
	CInvisibleWall(float x, float y) : CGameObject(x, y) {}
	void Render() { RenderBoundingBox(); }
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};