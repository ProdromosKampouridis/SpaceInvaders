#pragma once
#include "gameobject.h"
#include "graphics.h"

class UFO : public GameObject, public Collidable
{
	float pos_x;
	float speed;
	float size;
	graphics::Brush br;
	int hp, hpmax;
	unsigned int points;

	bool active = true;
	int type = rand() % 5 + 1;
	int place = rand() % 3 + 1;
	std::string png = std::to_string(type);

public:
	float pos_y;
	void update() override;
	void draw() override;
	void init() override;
	UFO(const class Game& mygame);
	~UFO();
	Disk getCollisionHull() const override;
	bool isActive() { return active; }
	int getType() { return type; }
};