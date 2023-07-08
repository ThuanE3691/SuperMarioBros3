#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"

//#include "Koopas.h"


class CPlayScene: public CScene
{
protected: 

	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void _IsInCamera(LPGAMEOBJECT obj);

	void LoadAssets(LPCWSTR assetFile);

	bool isEnableRenderBoundingBox;
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void Reload();

	LPGAMEOBJECT GetPlayer() { return player; }

	vector<LPGAMEOBJECT> GetObjects() { return objects; }
	void AddObject(CGameObject* obj);

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	void SetIsEnableRenderBoundingBox(bool enableRender) { this->isEnableRenderBoundingBox = enableRender; }
	bool GetIsEnableRenderBoundingBox() { return isEnableRenderBoundingBox; }
};

typedef CPlayScene* LPPLAYSCENE;

