#pragma once
#include "gameobject.h"
#include "player.h"

class Bullet : public Collidable, public Player
{
	float bullet_x, finalboss_x;
	const float bullet_w = 25;
	const float bullet_h = 50;
public:
	float bullet_y, finalboss_y;
	Bullet(const class Game& mygame);
	~Bullet();
	Disk getCollisionHull() const override;
	void update() override;
	void updatefinalbossBullet();
	void draw() override;
	void drawfinalbossBullet();
	void init() override;
	void setPos(float pos_x, float pos_y) { this->Player::pos_x = pos_x; this->Player::pos_y =pos_y; }

	Bullet& operator =(const Bullet& src);

};