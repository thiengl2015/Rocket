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
	bool CheckCollision(CGameObject* obj1, CGameObject* obj2)
	{
		float obj1_left = obj1->GetX();
		float obj1_right = obj1->GetX() + 16; // Giả sử kích thước 16x16
		float obj1_top = obj1->GetY();
		float obj1_bottom = obj1->GetY() + 16;

		float obj2_left = obj2->GetX();
		float obj2_right = obj2->GetX() + 16;
		float obj2_top = obj2->GetY();
		float obj2_bottom = obj2->GetY() + 16;

		return (obj1_right > obj2_left &&
			obj1_left < obj2_right &&
			obj1_bottom > obj2_top &&
			obj1_top < obj2_bottom);
	}
	~CGameObject();
};
typedef CGameObject * LPGAMEOBJECT;



class CBrick : public CGameObject
{
public:
	CBrick(float x, float y, LPTEXTURE texture): CGameObject(x,y,texture) {}
	void Update(DWORD dt) {}; 
};
class Cenemy : public CGameObject
{
private:
	bool Eactive;
public:
	Cenemy(float x, float y, LPTEXTURE texture) : CGameObject(x, y, texture) 
	{
		Eactive = true;
	}
	void Render() override
	{
		if (Eactive)  // Chỉ vẽ nếu kẻ thù còn sống
			CGameObject::Render();
	}
	bool GetEActive() { return Eactive; }
	Cenemy() : CGameObject(0, 0, nullptr), Eactive(true) {}
	void SetEActive(bool k) { Eactive = k; }
	void Update(DWORD dt) ;
};


class CBulletPool;
class Crocket : public CGameObject
{
protected:
	float vx;
	float vy;
	CBulletPool* bulletPool;
public: 
	Crocket(float x, float y, float vx, float vy, LPTEXTURE texture, CBulletPool* pool)
		: CGameObject(x, y, texture), bulletPool(pool)
	{
		this->vx = vx;
		this->vy = vy;
	};
	void Update(DWORD dt);
};


class CBullet : public CGameObject
{
protected:
	float vx;
	float vy;
	bool active;
public:
	CBullet(float x, float y, float vx, float vy, LPTEXTURE texture)
		: CGameObject(x, y, texture)
	{
		this->vx = vx;
		this->vy = vy;
		active = false;
	};

	CBullet()
		: CGameObject(0, 0, nullptr), vx(0), vy(0), active(false) {}

	bool getActive() { return active; }
	void SetActive(bool x) { active = x; }
	void SetVy(float vy) { this->vy = vy; }
	void Update(DWORD dt) override; ;
};

class CBulletPool
{
private:
	vector<CBullet*> pool;
	int poolSize;
public:
	CBulletPool(int size, LPTEXTURE texture);
	CBullet* GetBullet(float startX, float startY);
	int GetSize() { return poolSize; }
	vector<CBullet*> Getpool() { return pool; }
	void Update(DWORD dt, vector<Cenemy*>& enemys);
	void Render();
	~CBulletPool() {
		for (CBullet* bullet : pool) {
			delete bullet;
		}
		pool.clear();
	}
};