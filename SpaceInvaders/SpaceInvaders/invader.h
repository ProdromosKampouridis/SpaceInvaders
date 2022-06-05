#pragma once
#include "gameobject.h"
#include "graphics.h"
#include "config.h"

class Invader : public GameObject, public Collidable
{
	float pos_x, pos_y;
	float speed;
	float size;
	float life = 1.0f;
	graphics::Brush br;
	bool inPosition = false;

public:
	void update() override;
	void draw() override;
	void init() override;
	Invader(const class Game& mygame);
	~Invader();
	Disk getCollisionHull() const override;
	float& getPosX() { return pos_x; }
	float& getPosY() { return pos_y; }
	float getRemainingLife() const { return life; }
	void damage() { life -= 0.05f; }
	bool isReady() { return inPosition; }
};