#include "bullet.h"
#include "graphics.h"
#include "player.h"
#include "gameobject.h"
#include "util.h"



Bullet::Bullet(const class Game& mygame)
	:Player(mygame)
{
	init();
}


Bullet::~Bullet()
{
}

Disk Bullet::getCollisionHull() const
{
	Disk disk;
	disk.cx = bullet_x;
	disk.cy = bullet_y;
	disk.radius = bullet_h * 0.30f;
	return disk;
}

void Bullet::update()
{
	float speed = 0.0000002f;

	if (bullet_y < 0)
		return;

	setPos(bullet_x, bullet_y);
}

void Bullet::draw()
{
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	br.texture = std::string(ASSET_PATH) + "shot1.png";

	setPos(Player::pos_x, Player::pos_y);
	bullet_x = Player::pos_x;
	bullet_y = Player::pos_y - 30;

	graphics::drawRect(bullet_x, bullet_y, bullet_w, bullet_h, br);

	//collision
	//br.outline_opacity = 1.0f;
	//br.texture = "";
	//br.fill_color[0] = 0.3f;
	//br.fill_color[1] = 1.0f;
	//br.fill_color[2] = 0.3f;
	//br.fill_opacity = 0.3f;
	//br.gradient = false;
	//Disk hull = getCollisionHull();
	//graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);
}

void Bullet::init()
{
}

Bullet& Bullet::operator=(const Bullet& src)
{
	return *this;
}