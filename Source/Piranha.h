#pragma once
#include "GameObject.h"
#include "AnimationIDs.h"

#define PIRANHA_RISING_SPEED 0.04f

#define PIRANHA_BBOX_WIDTH 16
#define PIRANHA_BBOX_HEIGHT 33

#define PIRANHA_GREEN_BBOX_HEIGHT 16
#define PIRANHA_VENUS_BBOX_HEIGHT	16

#define VENUS_BBOX_WIDTH	16
#define VENUS_BBOX_HEIGHT	16

#define PIRANHA_WAIT_TO_SHOOT_TIME_OUT	500
#define PIRANHA_SHOOT_TIME_OUT	500
#define PIRANHA_HIDDEN_TIME_OUT	2000
#define PIRANHA_DIE_TIME_OUT	200

#define PIRANHA_STATE_RISING 100
#define PIRANHA_STATE_SHOOT_FIRE	200
#define PIRANHA_STATE_HIDING	300
#define PIRANHA_STATE_HIDDEN	400
#define PIRANHA_STATE_DIE_BY_ATTACK	500


#define PIRANHA_TYPE_PIRANHA	1
#define PIRANHA_TYPE_PIRANHA_GREEN	2
#define PIRANHA_TYPE_VENUS	3


#pragma region FIREBULLET

#define BULLET_BBOX_WIDTH	8
#define BULLET_BBOX_HEIGHT	8

#define BULLET_SPEED_X	0.04f
#define BULLET_SPEED_Y	0.04f



#pragma endregion


class CFireBullet : public CGameObject {
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; }
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

public:
	CFireBullet(float x, float y) : CGameObject(x, y) {}
	void SetDirection(float mx, float my) {

		int direction_x = x < mx ? 1 : -1;
		int direction_y = y < my ? 1 : -1;
		vx = BULLET_SPEED_X * direction_x;
		vy = BULLET_SPEED_Y * direction_y;
	}
};

class CPiranha : public CGameObject
{
protected:
	float ax;
	float ay;

	float maxY;
	float minY;

	int direction;
	int firstLoad;

	int type;

	ULONGLONG hidden_start;
	ULONGLONG shoot_start;
	ULONGLONG bullet_fire_start; // Create just for fun :>
	ULONGLONG die_start;

	CFireBullet* bullet;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; }
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void ShootMario();

	virtual int GetAni();

public:
	CPiranha(float x, float y, int type);
	virtual void SetState(int state);

};

