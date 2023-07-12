#pragma once

#include "WallUpdateArea.h"
#include "Platform.h"

void CWallUpdateArea::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - WALL_UPDATE_BBOX_WIDTH / 2;
	t = y - WALL_UPDATE_BBOX_HEIGHT / 2;
	r = l + WALL_UPDATE_BBOX_WIDTH;
	b = t + WALL_UPDATE_BBOX_HEIGHT;
}
