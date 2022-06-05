#include "game.h"
#include "graphics.h"
#include "config.h"

void Game::spawnInvader()
{
	if(!alien)
		alien = new Invader(*this);
}

void Game::spawnUFO()
{
	if (!ufo && status != FINAL_BOSS)
		ufo = new UFO(*this);

	if (!u1 && status == FINAL_BOSS)
	{
		u1 = new UFO(*this);
	}

	if (!u2 && status == FINAL_BOSS)
	{
		u2 = new UFO(*this);
	}

	if (!u3 && status == FINAL_BOSS)
	{
		u3 = new UFO(*this);
	}

}

void Game::checkUFO()
{
	if (ufo && !ufo->isActive() && status != FINAL_BOSS)
	{
		delete ufo;
		ufo = nullptr;
	}

	if (status == FINAL_BOSS)
	{
		if (u1 && !u1->isActive())
		{
			delete u1;
			u1 = nullptr;
		}

		if (u2 && !u2->isActive())
		{
			delete u2;
			u2 = nullptr;
		}

		if (u3 && !u3->isActive())
		{
			delete u3;
			u3 = nullptr;
		}
	}
}

bool Game::checkCollision()
{
	float dx, dy;

	if (status == PLAYING)
	{
		if (!player)
			return false;

		if (!ufo)
			return false;

		Disk d1 = player->getCollisionHull();
		Disk d4 = ufo->getCollisionHull();

		//player - ufo
		float dx1 = d1.cx - d4.cx;
		float dy1 = d1.cy - d4.cy;

		if (sqrt(dx1 * dx1 + dy1 * dy1) < d1.radius + d4.radius)
		{
			graphics::playSound(std::string(ASSET_PATH) + "ufokilled.ogg", 0.1);

			if (ufo->getType() == 1)
				points += 1;
			else if (ufo->getType() == 2)
				points += 3;
			else if (ufo->getType() == 3)
				points += 5;
			else if (ufo->getType() == 4)
				points += 7;
			else if (ufo->getType() == 5)
				points += 10;

			return true;
		}

		if (d4.cy > CANVAS_HEIGHT - d4.radius)
		{
			takingDamage = true;
			player->damage();
		}

		if (points >= 100)
			status = FINAL_BOSS;

		if (!bullet)
			return false;

		//Disk d3 = bullet->getCollisionHull();

		//for (int i= 0; i < bullets.size(); i++)
		//{
		//	//bullet - ufo
		//	dx = d3.cx - d4.cx;
		//	dy = d3.cy - d4.cy;

		//	if (sqrt(dx * dx + dy * dy) < d3.radius + d4.radius)
		//	{
		//		//std::cout << "YEP" << std::endl;
		//		return true;
		//	}
		//}

		//bullet - ufo
		//dx = d3.cx - d4.cx;
		//dy = d3.cy - d4.cy;

		/*if (sqrt(dx * dx + dy * dy) < d3.radius + d4.radius)
		{
			return true;
		}*/

	}
	else if (status == FINAL_BOSS)
	{

		if (!player)
			return false;

		Disk d1 = player->getCollisionHull();

		if (alien->isReady())
		{

			//player - minions
			if (!u1)
				return false;

			if (!u2)
				return false;

			if (!u3)
				return false;

			Disk du1 = u1->getCollisionHull();
			Disk du2 = u2->getCollisionHull();
			Disk du3 = u3->getCollisionHull();

			float dxu1 = d1.cx - du1.cx;
			float dyu1 = d1.cy - du1.cy;

			float dxu2 = d1.cx - du2.cx;
			float dyu2 = d1.cy - du2.cy;

			float dxu3 = d1.cx - du3.cx;
			float dyu3 = d1.cy - du3.cy;

			if (sqrt(dxu1 * dxu1 + dyu1 * dyu1) < d1.radius + du1.radius)
			{
				col = 1;
				alien->damage();
				graphics::playSound(std::string(ASSET_PATH) + "ufokilled.ogg", 0.1);
				return true;
			}

			if (sqrt(dxu2 * dxu2 + dyu2 * dyu2) < d1.radius + du2.radius)
			{
				col = 2;
				alien->damage();
				graphics::playSound(std::string(ASSET_PATH) + "ufokilled.ogg", 0.1);
				return true;
			}

			if (sqrt(dxu3 * dxu3 + dyu3 * dyu3) < d1.radius + du3.radius)
			{
				col = 3;
				alien->damage();
				graphics::playSound(std::string(ASSET_PATH) + "ufokilled.ogg", 0.1);
				return true;
			}

			if (du1.cy > CANVAS_HEIGHT - du1.radius)
			{
				takingDamage = true;
				player->damage();
			}

			if (du2.cy > CANVAS_HEIGHT - du2.radius)
			{
				takingDamage = true;
				player->damage();
			}

			if (du3.cy > CANVAS_HEIGHT - du3.radius)
			{
				takingDamage = true;
				player->damage();
			}

		}

	}
}

void Game::updateStartScreen()
{
	//click or enter
	graphics::MouseState ms;
	graphics::getMouseState(ms);
	if (ms.button_left_pressed || graphics::getKeyState(graphics::SCANCODE_RETURN))
	{
		status = PLAYING;
		t1 = graphics::getGlobalTime();
	}

}

void Game::updatePlayingScreen()
{
	if ((!player_created && graphics::getGlobalTime() >= 1000 + t1))
	{
		player = new Player(*this);
		player_created = true;
	}

	if (player)
		player->update();

	spawnInvader();

	spawnUFO();
	checkUFO();

	if (ufo)
		ufo->update();

	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i].update();

		if (bullets[i].bullet_y < 0)
			bullets.erase(bullets.begin() + i);

	}

	if (checkCollision())
	{
		delete ufo;
		ufo = nullptr;
	}

	if (player && player->getRemainingLife() <= 0.0f)
	{
		startOver = true;
		delete player;
		player = nullptr;
		status = DEATH;
	}
}

void Game::updateFinalBossScreen()
{
	if (!player_created)
	{
		player = new Player(*this);
		player_created = true;
	}

	if (player)
		player->update();

	spawnInvader();

	if (alien)
		alien->update();

	spawnUFO();
	checkUFO();

	if (u1)
		u1->update();

	if (u2)
		u2->update();

	if (u3)
		u3->update();

	for (int i = 0; i < bullets.size(); i++)
	{

		bullets[i].update();

		if (bullets[i].bullet_y < 0)
		{
			bullets.erase(bullets.begin() +i);
			//bullets.clear();
		}

	}

	if (checkCollision())
	{
		if (col == 1)
		{
			delete u1;
			u1 = nullptr;
			col = 4;
		}
		else if (col == 2)
		{
			delete u2;
			u2 = nullptr;
			col = 4;
		}
		else if (col == 3)
		{
			delete u3;
			u3 = nullptr;
			col = 4;
		}

		if (player && player->getRemainingLife() <= 0.0f)
		{
			delete player;
			player = nullptr;
			status = DEATH;
		}
	}

	if (alien && alien->getRemainingLife() <= 0.0f)
	{
		delete alien;
		alien = nullptr;
		status = WIN;
	}
}

void Game::updateDeathScreen()
{
	graphics::stopMusic();
	if (stopSound)
	{
		graphics::playSound(std::string(ASSET_PATH) + "fail.ogg", 0.5f, false);
		stopSound = false;
	}

	if (graphics::getKeyState(graphics::SCANCODE_Y))
	{
		if (startOver)
		{
			startOver = false;
			player_created = false;
			player = new Player(*this);
			points = 0;
			status = START;
		}
		else
		{
			player_created = false;
			status = FINAL_BOSS;
		}
	}
	else if (graphics::getKeyState(graphics::SCANCODE_N))
	{
		status = CREDITS;
	}
}

void Game::updateCreditsScreen()
{
	graphics::stopMusic();
	graphics::playSound(std::string(ASSET_PATH) + "end_credits.mp3", 0.3f);
}

void Game::updateWinScreen()
{
	graphics::stopMusic();
	graphics::playSound(std::string(ASSET_PATH) + "cheering.mp3", 0.01f);

	graphics::MouseState ms;
	graphics::getMouseState(ms);
	if (ms.button_left_pressed)
	{
		status = CREDITS;
	}
}

void Game::drawStartScreen()
{
	graphics::Brush br;
	br.fill_color[0] = 0.0f;
	br.fill_color[1] = 1.0f;
	br.fill_color[2] = 0.0f;

	char i1[30];
	sprintf_s(i1, "IT WAS THE LAST DAY OF 2020.");
	graphics::drawText(30, 50, 50, i1, br);
	char i2[50];
	sprintf_s(i2, "PEOPLE WERE CELEBRATING THIS YEAR WAS ALMOST");
	graphics::drawText(30, 100, 50, i2, br);

	char i3[40];
	sprintf_s(i3, "OVER BUT... IN THE LAST FEW MINUTES");
	graphics::drawText(30, 150, 50, i3, br);

	char i4[50];
	sprintf_s(i4, "ALIENS START INVADING AND ATTACKING");
	graphics::drawText(30, 200, 50, i4, br);

	char i5[50];
	sprintf_s(i5, "THE EARTH. THIS WAS THE LAST CHALLENGE OF 2020.");
	graphics::drawText(30, 250, 50, i5, br);

	char i6[30];
	sprintf_s(i6, "CAN YOU SURVIVE ???");
	graphics::drawText(30, 300, 50, i6, br);

	char i7[70];
	sprintf_s(i7, "(HINT:YOU WILL NEED 100 POINTS IN ORDER TO");
	graphics::drawText(30, 400, 50, i7, br);

	char i8[25];
	sprintf_s(i8, "FACE THE FINAL BOSS)");
		graphics::drawText(30, 450, 50, i8, br);
}

void Game::drawPlayingScreen()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "globe.png";
	br.outline_opacity = 0.0f;

	bool isShooting = false;

	//draw background
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_WIDTH, br);

	if (player)
	{
		player->draw();

		if (ufo)
			ufo->draw();
	}

	if (shootTimer < 100)
	{
		shootTimer++;
	}

	if (graphics::getKeyState(graphics::SCANCODE_SPACE) && shootTimer >= 100)
	{
		isShooting = true;
		shootTimer = 0;
	}

	if (isShooting)
	{

		if (player)
		{
			bullet = new Bullet(*this);
			bullet->setPos(player->getposX(), player->getposY());
			bullets.push_back(*bullet);
			isShooting = false;
		}
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		graphics::playSound(std::string(ASSET_PATH) + "shoot.ogg", 0.01);
		bullets[i].draw();
	}

	char info[20];
	sprintf_s(info, "Points: %d ", points);
	graphics::drawText(10, CANVAS_HEIGHT-10, 30, info, br);

	float player_life = player ? player->getRemainingLife() : 0.0f;

	//br.texture = "";
	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.2f;
	br.fill_color[2] = 0.2f;
	br.texture = "";
	br.fill_secondary_color[0] = 1.0 * (1.0f - player_life) + player_life * 0.2f;
	br.fill_secondary_color[1] = 0.2f;
	br.fill_secondary_color[2] = 0.2 * (1.0f - player_life) + player_life * 1.0f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100 - ((1.0f - player_life) * 120 / 2), 30, player_life * 120, 20, br);

	br.outline_opacity = 1.0f;
	br.gradient = false;
	br.fill_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100, 30, 120, 20, br);

	graphics::Brush br0;
	br0.texture = std::string(ASSET_PATH) + "explosion.png";
	br0.outline_opacity = 0.0f;
	if (player && takingDamage)
	{
		graphics::playSound(std::string(ASSET_PATH) + "explosion.ogg", 0.1);
		graphics::drawRect(player->getposX(), player->getposY(), 100, 100, br0);
		takingDamage = false;
	}
}

void Game::drawFinalBossScreen()
{
	bool isShooting = false;

	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "globe.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_WIDTH, br);

	float player_life = player ? player->getRemainingLife() : 0.0f;

	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.2f;
	br.fill_color[2] = 0.2f;
	br.texture = "";
	br.fill_secondary_color[0] = 1.0 * (1.0f - player_life) + player_life * 0.2f;
	br.fill_secondary_color[1] = 0.2f;
	br.fill_secondary_color[2] = 0.2 * (1.0f - player_life) + player_life * 1.0f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;
	graphics::drawRect(70 - ((1.0f - player_life) * 120 / 2), CANVAS_HEIGHT - 15, player_life * 120, 20, br);

	br.outline_opacity = 1.0f;
	br.gradient = false;
	br.fill_opacity = 0.0f;
	graphics::drawRect(70, CANVAS_HEIGHT - 15, 120, 20, br);

	graphics::Brush br0;
	br0.texture = std::string(ASSET_PATH) + "explosion.png";
	br0.outline_opacity = 0.0f;
	if (player && takingDamage)
	{
		graphics::playSound(std::string(ASSET_PATH) + "explosion.ogg", 0.1);
		graphics::drawRect(player->getposX(), player->getposY(), 100, 100, br0);
		takingDamage = false;
	}

	if (player)
		player->draw();

	if (alien)
		alien->draw();;

	if (shootTimer < 100)
	{
		shootTimer++;
	}

	if (graphics::getKeyState(graphics::SCANCODE_SPACE) && shootTimer >= 100)
	{
		isShooting = true;
		shootTimer = 0;
	}

	if (isShooting)
	{
		if (player)
		{
			bullet = new Bullet(*this);
			bullet->setPos(player->getposX(), player->getposY());
			bullets.push_back(*bullet);
			isShooting = false;
		}
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		graphics::playSound(std::string(ASSET_PATH) + "shoot.ogg", 0.01);
		bullets[i].draw();
	}

	if (alien && alien->isReady())
	{
		float invader_life = alien ? alien->getRemainingLife() : 0.0f;

		graphics::Brush bri;
		bri.outline_opacity = 0.0f;
		bri.fill_color[0] = 1.0f;
		bri.fill_color[1] = 1.0f;
		bri.fill_color[2] = 0.2f;
		bri.texture = "";
		bri.fill_secondary_color[0] = 1.0f * (1.0f - invader_life) + invader_life * 0.2f;
		bri.fill_secondary_color[1] = 1.0f;
		bri.fill_secondary_color[2] = 0.2 * (1.0f - invader_life) + invader_life * 1.0f;
		bri.gradient = true;
		bri.gradient_dir_u = 1.0f;
		bri.gradient_dir_v = 0.0f;
		graphics::drawRect(500 - ((1.0f - invader_life) * CANVAS_WIDTH / 4), 20, invader_life * CANVAS_WIDTH / 2, 20, bri);

		bri.outline_opacity = 1.0f;
		bri.gradient = false;
		bri.fill_opacity = 0.0f;
		graphics::drawRect(500, 20, CANVAS_WIDTH / 2, 20, bri);

		if (u1)
			u1->draw();

		if (u2)
			u2->draw();

		if (u3)
			u3->draw();
	}
}

void Game::drawDeathScreen()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "globe.png";
	br.outline_opacity = 0.0f;

	//draw background
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_WIDTH, br);

	char deth[20];
	sprintf_s(deth, "You died...");
	graphics::drawText(CANVAS_WIDTH / 2 - 150, 100, 100, deth, br);

	char deth2[20];
	sprintf_s(deth2, "TRY AGAIN ?");
	graphics::drawText(CANVAS_WIDTH / 2 - 200, CANVAS_HEIGHT / 2, 100, deth2, br);

	char deth3[10];
	sprintf_s(deth3, "Y / N");
	graphics::drawText(CANVAS_WIDTH / 2 - 100, CANVAS_HEIGHT * 3 / 4, 100, deth3, br);
}

void Game::drawCreditsScreen()
{
	/*float speed = 0.1f * graphics::getGlobalTime() / 10.f;
	float x = CANVAS_WIDTH;
	float y = CANVAS_HEIGHT + speed;

	graphics::Brush br;

	br.fill_color[0] = 0.2f;
	br.fill_color[1] = 0.3f;
	br.fill_color[2] = 0.6f;
	graphics::setWindowBackground(br);

	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 1.0f;
	br.fill_color[2] = 1.0f;

	char c1[15];
	sprintf_s(c1, "CREATED BY :");
	char c2[30];
	sprintf_s(c2, "KAMPOURIDIS PRODROMOS");
	char c3[30];
	sprintf_s(c3, "VOITSIDIS NIKOLAOS");
	char c4[15];
	sprintf_s(c4, "Playtesters");
	char c5[30];
	sprintf_s(c5, "Narrative Directors");
	char c6[20];
	sprintf_s(c6, "Art Directors");
	char c7[30];
	sprintf_s(c7, "Enviroment Directors");
	char c8[30];
	sprintf_s(c8, "Gameplay Directors");
	char c9[30];
	sprintf_s(c9, "Audio Directors");
	char c10[30];
	sprintf_s(c10, "Programming Directors");

	setWindowDimensions(x, y);
	graphics::drawText(CANVAS_WIDTH / 2 - 50, 50 + speed, 25, c1, br);
	graphics::drawText(200, 100 + speed, 25, c2, br);
	graphics::drawText(550, 100 + speed, 25, c3, br);

	graphics::drawText(30, 200 + speed, 25, c4, br);
	graphics::drawText(200, 200 + speed, 25, c2, br);
	graphics::drawText(500, 200 + speed, 25, c3, br);

	graphics::drawText(30, 300, 25, c5, br);
	graphics::drawText(300, 300, 25, c2, br);
	graphics::drawText(600, 300, 25, c3, br);

	graphics::drawText(30, 400, 25, c6, br);
	graphics::drawText(200, 400, 25, c2, br);
	graphics::drawText(500, 400, 25, c3, br);

	graphics::drawText(30, 500, 25, c7, br);
	graphics::drawText(300, 500, 25, c2, br);
	graphics::drawText(600, 500, 25, c3, br);

	graphics::drawText(30, 600, 25, c8, br);
	graphics::drawText(300, 600, 25, c2, br);
	graphics::drawText(600, 600, 25, c3, br);

	graphics::drawText(30, 700, 25, c9, br);
	graphics::drawText(300, 700, 25, c2, br);
	graphics::drawText(600, 700, 25, c3, br);

	graphics::drawText(30, 800, 25, c5, br);
	graphics::drawText(300, 800, 25, c2, br);
	graphics::drawText(600, 800, 25, c3, br);*/

	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "credits.png";
	br.outline_opacity = 0.0f;

	float offset = CANVAS_HEIGHT * sinf(graphics::getGlobalTime() / 8000.0f);

	graphics::drawRect(CANVAS_WIDTH / 2 + 30, CANVAS_HEIGHT / 2 - offset, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	char ty[30];
	sprintf_s(ty, "- THANK YOU FOR PLAYING -");

	float y = CANVAS_HEIGHT + 200 - offset;
	if (y <= CANVAS_HEIGHT / 2)
	{
		y = CANVAS_HEIGHT / 2;
		graphics::drawText(CANVAS_WIDTH / 5, y, 55, ty, br);
	}
	else
	{
		graphics::drawText(CANVAS_WIDTH / 5, y, 55, ty, br);
	}
	
}

void Game::drawWinScreen()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "globe.png";
	br.outline_opacity = 0.0f;

	//draw background
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_WIDTH, br);

	if (player)
		player->draw();

	br.fill_color[0] = 0.2f;
	br.fill_color[1] = 0.3f;
	br.fill_color[2] = 0.6f;

	char win[20];
	sprintf_s(win, "YOU WON ! ! !");
	graphics::drawText(CANVAS_WIDTH / 2 - 200, 100, 100, win, br);

	char win1[20];
	sprintf_s(win1, "CONGRATULATIONS");
	graphics::drawText(CANVAS_WIDTH / 2 - 350, CANVAS_HEIGHT / 2, 100, win1, br);

	char win2[20];
	sprintf_s(win2, "CLICK TO CONTINUE");
	graphics::drawText(CANVAS_WIDTH / 2 - 400, CANVAS_HEIGHT * 3 / 4, 100, win2, br);
}

void Game::update()
{
	if (status == START)
	{
		updateStartScreen();
	}
	else if (status == PLAYING)
	{
		updatePlayingScreen();
	}
	else if (status == FINAL_BOSS)
	{
		updateFinalBossScreen();
	}
	else if (status == DEATH)
	{
		updateDeathScreen();
	}
	else if (status == CREDITS)
	{
		updateCreditsScreen();
	}
	else if (status == WIN)
	{
		updateWinScreen();
	}
}

void Game::draw()
{
	if (status == START)
	{
		drawStartScreen();
	}
	else if (status == PLAYING)
	{
		drawPlayingScreen();
	}
	else if (status == FINAL_BOSS)
	{
		drawFinalBossScreen();
	}
	else if (status == DEATH)
	{
		drawDeathScreen();
	}
	else if (status == CREDITS)
	{
		drawCreditsScreen();
	}
	else if (status == WIN)
	{
		drawWinScreen();
	}
}

void Game::init()
{
	graphics::setFont(std::string(ASSET_PATH) + "youmurdererbb_reg.ttf");
	graphics::playMusic(std::string(ASSET_PATH) + "invasion.mp3", 0.05f, true, 4000);

	this->points = 0;
}

float Game::window2canvasX(float x)
{
	return x * CANVAS_WIDTH / (float)window_width;
}

float Game::window2canvasY(float y)
{
	return y * CANVAS_HEIGHT / (float)window_height;
}

Game::Game()
{
}

Game::~Game()
{
	if (player)
		delete player;
}
