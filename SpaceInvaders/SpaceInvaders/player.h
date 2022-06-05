#pragma once
#include "gameobject.h"
#include "config.h"

class Player : public GameObject, public Collidable
{
	float speed = 3.0f;
	float life = 1.0f;
public:
	float pos_x = CANVAS_WIDTH / 2, pos_y = CANVAS_HEIGHT - CANVAS_HEIGHT / 10;
	Player(const class Game& mygame);
	void update() override;
	void draw() override;
	void init() override;
	Disk getCollisionHull() const override;
	float getRemainingLife() const { return life; }
	void damage() { life -= 0.1f; }
	float getposX() { return pos_x; }
	float getposY() { return pos_y; }
};
