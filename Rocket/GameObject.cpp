#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"

/*
	Initialize game object 
*/
CGameObject::CGameObject(float x, float y, LPTEXTURE tex)
{
	this->x = x;
	this->y = y;
	this->texture = tex;
}

void CGameObject::Render()
{
	CGame::GetInstance()->Draw(x, y, texture);
}

CGameObject::~CGameObject()
{
	if (texture != NULL) delete texture;
}

#define MARIO_VX 0.1f
#define MARIO_WIDTH 14

void Crocket::Update(DWORD dt)
{
	if (GetAsyncKeyState('A') & 0x8000) // Nhấn A -> Di chuyển trái
		x -= vx * dt;
	if (GetAsyncKeyState('D') & 0x8000) // Nhấn D -> Di chuyển phải
		x += vx * dt;
	if (GetAsyncKeyState('W') & 0x8000) // Nhấn W -> Di chuyển lên
		y -= vx * dt;       // Trục y ngược chiều (0 ở trên cùng)
	if (GetAsyncKeyState('S') & 0x8000) // Nhấn S -> Di chuyển xuống
		y += vx * dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		CBullet* bullet = bulletPool->GetBullet(x, y);
	}

	bulletPool->Update(dt);
}
CBullet::CBullet(float startX, float startY)
{
	this->x = startX;
	this->y = startY;
	this->vy = -0.2f; // Đạn bay lên trên
	this->active = true;
}

void CBullet::Update(DWORD dt)
{
	if (!active) return;

	y += vy * dt;

	if (y < 0) // Ra khỏi màn hình thì vô hiệu hóa
	{
		active = false;
	}
}

CBulletPool::CBulletPool(int size)
{
	poolSize = size;
	for (int i = 0; i < poolSize; i++)
	{
		CBullet* bullet = new CBullet(0, 0);
		bullet->active = false;
		pool.push_back(bullet);
	}
}

CBullet* CBulletPool::GetBullet(float startX, float startY)
{
	for (auto bullet : pool)
	{
		if (!bullet->active)  // Tìm viên đạn chưa active
		{
			bullet->x = startX;
			bullet->y = startY;
			bullet->active = true;
			return bullet;
		}
	}
	return nullptr; // Nếu hết đạn trong pool
}

void CBulletPool::Update(DWORD dt)
{
	for (auto bullet : pool)
	{
		if (bullet->active)
		{
			bullet->Update(dt);
		}
	}
}

void CBulletPool::Render()
{
	for (auto bullet : pool)
	{
		if (bullet->active)
		{
			bullet->Render();
		}
	}
}

CBulletPool::~CBulletPool()
{
	for (auto bullet : pool)
	{
		delete bullet;
	}
	pool.clear();
}