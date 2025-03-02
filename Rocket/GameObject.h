#pragma once

#include <Windows.h>
#include <d3dx10.h>

#include "Texture.h"

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

class Cenamy : public CGameObject
{
public:
	Cenamy(float x, float y, LPTEXTURE texture): CGameObject(x,y,texture) {}
	void Update(DWORD dt);
};

class CBullet : public Crocket
{
public:
	float GetVy() { return vy; }
	void SetVy(float vy) { this->vy = vy; }
	CBullet(float x, float y, float vx, float vy, LPTEXTURE texture) : Crocket(x, y, 0, vy, texture) {};
	void Update(DWORD dt);
};