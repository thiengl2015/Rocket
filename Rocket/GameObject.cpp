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
	if (x <= 0 || x >= BackBufferWidth - MARIO_WIDTH) {
		
		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - MARIO_WIDTH)
		{
			x = (float)(BackBufferWidth - MARIO_WIDTH);
		}
	}
}
void CBullet::Update(DWORD dt)
{	

	if (!active) return; // Nếu viên đạn không hoạt động thì không cập nhật

	y -= vy * dt; // Bay lên trên

	// Kiểm tra nếu đạn bay ra khỏi màn hình thì đánh dấu là không hoạt động
	if (y < 0)
	{
		active = false;
	}
}
