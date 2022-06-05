#pragma once
#include "player.h"
#include "invader.h"
#include "bullet.h"
#include "ufo.h"
#include <vector>

class Game
{
	typedef enum {START, PLAYING, FINAL_BOSS, DEATH, CREDITS, WIN} status_t;
	Player* player = nullptr;
	bool player_created = false;
	Invader* alien = nullptr;
	Bullet* bullet = nullptr;
	std::vector<Bullet> bullets;

	UFO* ufo = nullptr;
	UFO* u1 = nullptr;
	UFO* u2 = nullptr;
	UFO* u3 = nullptr;
	unsigned int col;

	bool takingDamage = false;
	bool stopSound = true;

	void spawnInvader();
	void spawnUFO();
	void checkUFO();
	bool checkCollision();
	status_t status = START;

	unsigned int window_width = WINDOW_WIDTH,
				 window_height = WINDOW_HEIGHT;

	void updateStartScreen();
	void updatePlayingScreen();
	void updateFinalBossScreen();
	void updateDeathScreen();
	void updateCreditsScreen();
	void updateWinScreen();

	void drawStartScreen();
	void drawPlayingScreen();
	void drawFinalBossScreen();
	void drawDeathScreen();
	void drawCreditsScreen();
	void drawWinScreen();

	float t1;
	int shootTimer = 0;
	int points;

	bool startOver = false;

public:
	void update();
	void draw();
	void init();
	unsigned int getWindowWidth() { return window_width; }
	unsigned int getWindowHeight() { return window_height; }
	float window2canvasX(float x);
	float window2canvasY(float y);
	void setWindowDimensions(unsigned int w, unsigned int h) { window_width = w; window_height = h; }
	Game();
	~Game();
};