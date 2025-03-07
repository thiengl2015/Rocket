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

void Cenemy::Update(DWORD dt)
{
	if (Eactive)
	{
		y +=0.01f* dt;
	}
	else { return; }
}

#define ROCKET_VX 0.1f
#define ROCKET_WIDTH 14
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
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		CBullet* bullet = bulletPool->GetBullet(x, y);
		if (bullet != nullptr)
		{
			bullet->SetVy(0.3f);  // Đạn bay lên
			bullet->SetActive(true);
		}
	}
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - ROCKET_WIDTH) {
		
		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - ROCKET_WIDTH)
		{
			x = (float)(BackBufferWidth - ROCKET_WIDTH);
		}
	}
}

void CBullet::Update(DWORD dt)
{
	if (!active) return;

	y -= vy * dt;

	// Kiểm tra nếu viên đạn bay ra khỏi màn hình thì vô hiệu hóa nó
	if (y < 0) active = false;
}


CBulletPool::CBulletPool(int size, LPTEXTURE texture)
{
	poolSize = size;
	for (int i = 0; i < poolSize; i++)
	{
		CBullet* bullet = new CBullet(0,0,0,0,texture);
		bullet->SetActive(false);
		pool.push_back(bullet);
	}
}

CBullet* CBulletPool::GetBullet(float startX, float startY)
{
	for (auto bullet : pool)
	{
		if (!bullet->getActive())  // Tìm viên đạn chưa active
		{
			bullet->SetPosition(startX, startY);
			bullet->SetActive(true);
			return bullet;
		}
	}
	return nullptr; // Nếu hết đạn trong pool
}
void CBulletPool::Update(DWORD dt, vector<Cenemy*>& enemies)
{
	for (auto bullet : pool)
	{
		if (bullet->getActive())
		{
			bullet->Update(dt);

			// Kiểm tra va chạm với enemy
			for (auto it = enemies.begin(); it != enemies.end(); )
			{
				Cenemy* enemy = *it;
				if (enemy->GetEActive() && bullet->CheckCollision(bullet, enemy))
				{
					bullet->SetActive(false);  // Tắt viên đạn
					enemy->SetEActive(false);  // Tắt kẻ thù
					// Không cần xóa ngay lập tức, để trạng thái Eactive xử lý
					++it;
				}
				else
				{
					++it;
				}
			}
		}
	}
}


void CBulletPool::Render()
{
	for (auto bullet : pool)
	{
		if (bullet->getActive())
		{
			bullet->Render();
		}
	}
}
