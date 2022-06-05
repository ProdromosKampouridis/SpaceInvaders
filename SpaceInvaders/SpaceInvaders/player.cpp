#include "player.h"
#include "graphics.h"

Player::Player(const Game& mygame)
	:GameObject(mygame)
{
}

void Player::update()
{

	if (graphics::getKeyState(graphics::SCANCODE_LEFT))
	{
		pos_x -= speed * graphics::getDeltaTime()/10.0f;
	}

	if (graphics::getKeyState(graphics::SCANCODE_RIGHT))
	{
		pos_x += speed * graphics::getDeltaTime() / 10.0f;
	}

	Disk hull = getCollisionHull();
	if (pos_x < hull.radius) pos_x = hull.radius;

	if (pos_x > CANVAS_WIDTH - hull.radius) pos_x = CANVAS_WIDTH - hull.radius;
}

void Player::draw()
{
	float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime()/10.0f);

	graphics::Brush br;
	br.outline_opacity = 0.0f;

	br.fill_opacity = 1.0f;
	br.texture = std::string(ASSET_PATH) + "Ship.png";
	graphics::drawRect(pos_x, pos_y, 100, 100, br);

	//glow colors
	br.texture = "";

	br.fill_color[0] = 1.0f;	//RED
	br.fill_color[1] = 0.5f + glow*0.5f;	//GREEN
	br.fill_color[2] = 0.0f;	//BLUE

	br.fill_secondary_color[0] = 0.3f;
	br.fill_secondary_color[1] = 0.1f;
	br.fill_secondary_color[2] = 0.0f;

	br.fill_opacity = 1.0f;
	br.fill_secondary_opacity = 0.0f;
	br.gradient = true;

	graphics::drawDisk(pos_x, pos_y + 30, 20, br);
}

void Player::init()
{
}

Disk Player::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = 30.0f; //~60/2
	return disk;
}