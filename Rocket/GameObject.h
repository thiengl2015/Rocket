#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>
#include "Texture.h"

using namespace std;
class CGameObject
{
protected: 
	float x; 
	float y;

	// This should be a pointer to an object containing all graphic/sound/audio assets for rendering this object. 
	// For now, just a pointer to a single texture
	LPTEXTURE texture;			
public: 
	void SetPosition(float x, float y) { this->x = x; this->y = y; }
	float GetX() { return x; }
	float GetY() { return y; }

	CGameObject(float x = 0.0f, float y=0.0f, LPTEXTURE texture = NULL);

	virtual void Update(DWORD dt) = 0;
	virtual void Render();

	~CGameObject();
};
typedef CGameObject * LPGAMEOBJECT;

class CBrick : public CGameObject
{
public:
	CBrick(float x, float y, LPTEXTURE texture): CGameObject(x,y,texture) {}
	void Update(DWORD dt) {}; 
};
class Cenamy : public CGameObject
{
public:
	Cenamy(float x, float y, LPTEXTURE texture) : CGameObject(x, y, texture) {}
	void Update(DWORD dt);
};

class Crocket : public CGameObject
{
protected:
	float vx;
	float vy;
public: 
	Crocket(float x, float y, float vx, float vy, LPTEXTURE texture) :CGameObject(x, y, texture)
	{
		this->vx = vx;
		this->vy = vy;
	};
	void Update(DWORD dt);
};


class CBullet : public Crocket
{
protected:
	bool active;
public:
	CBullet(float x, float y, float vx, float vy, LPTEXTURE texture) : Crocket(x, y, 0, vy, texture)
	{
		active = false;
	};
	CBullet() : Crocket(0, 0, 0, 0, nullptr), active(false) {}

	bool getActive() { return active; }
	void SetActive(bool x) { active = x; }
	void SetVy(float vy) { this->vy = vy; }
	
	void Update(DWORD dt);
};

class CBulletPool
{
private:
	vector<CBullet*> pool;
	int poolSize;
public:
	CBulletPool(int size, LPTEXTURE texture);
	CBullet* GetBullet(float startX, float startY);
	void Update(DWORD dt);
	void Render();
};