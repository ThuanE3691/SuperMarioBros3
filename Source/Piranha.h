#pragma once
#include "GameObject.h"

#define PIRANHA_RISING_SPEED 0.04f

#define PIRANHA_BBOX_WIDTH 16
#define PIRANHA_BBOX_HEIGHT 33

#define PIRANHA_WAIT_TO_SHOOT_TIME_OUT	500
#define PIRANHA_SHOOT_TIME_OUT	500
#define PIRANHA_HIDDEN_TIME_OUT	2000
#define PIRANHA_DIE_TIME_OUT	200

#define PIRANHA_STATE_RISING 100
#define PIRANHA_STATE_SHOOT_FIRE	200
#define PIRANHA_STATE_HIDING	300
#define PIRANHA_STATE_HIDDEN	400
#define PIRANHA_STATE_DIE_BY_ATTACK	500

#define ID_ANI_PIRANHA_SHOOT_FIRE_TOP_LEFT	7001
#define ID_ANI_PIRANHA_SHOOT_FIRE_TOP_RIGHT	7002
#define ID_ANI_PIRANHA_SHOOT_FIRE_BOTTOM_LEFT	7003
#define ID_ANI_PIRANHA_SHOOT_FIRE_BOTTOM_RIGHT	7004
#define ID_ANI_PIRANHA_DIE_BY_ATTACK	7005


#pragma region FIREBULLET

#define BULLET_BBOX_WIDTH	8
#define BULLET_BBOX_HEIGHT	8

#define BULLET_SPEED_X	0.04f
#define BULLET_SPEED_Y	0.04f

#define ID_ANI_FIRE_BULLET_LEFT	7010
#define ID_ANI_FIRE_BULLET_RIGHT	7011

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

		float direction_x = x < mx ? 1 : -1;
		float direction_y = y < my ? 1 : -1;
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
	CPiranha(float x, float y);
	virtual void SetState(int state);

};

