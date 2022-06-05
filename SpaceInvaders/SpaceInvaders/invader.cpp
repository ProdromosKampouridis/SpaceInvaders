#include "invader.h"
#include "graphics.h"
#include "config.h"

void Invader::update()
{
	if (pos_y >= 0.0f)
	{
		pos_y = 0.0f;
		inPosition = true;
	}

	pos_y += speed * graphics::getGlobalTime() / 4000.f;

}

void Invader::draw()
{
	br.fill_opacity = 1.0f;
	br.texture = std::string(ASSET_PATH) + "Ship6.png";
	graphics::drawRect(pos_x, pos_y, size, size, br);
}

void Invader::init()
{
	speed = 0.1f;
	pos_x = CANVAS_WIDTH / 2;
	pos_y = - CANVAS_HEIGHT;
	size = 1000;
	br.outline_opacity = 0.0f;
}

Invader::Invader(const class Game& mygame)
	: GameObject(mygame)
{
	init();
}

Invader::~Invader()
{
}

Disk Invader::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size*0.4f; //~100/2
	return disk;
}
