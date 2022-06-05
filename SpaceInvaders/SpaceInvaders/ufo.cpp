#include "ufo.h"
#include "config.h"
#include <random>

void UFO::update()
{
    pos_y += speed * graphics::getDeltaTime();

    Disk hull = getCollisionHull();
    if (pos_y > CANVAS_HEIGHT- hull.radius)
        active = false;

}

void UFO::draw()
{
    br.outline_opacity = 0.0f;

    switch (type)
    {
    case 1:
        size = 150;
        speed = 0.15f;
        br.texture = std::string(ASSET_PATH) + png + ".png";
        graphics::drawRect(pos_x, pos_y, size, size, br);
        break;
    case 2:
        size = 100;
        speed = 0.2f;
        br.texture = std::string(ASSET_PATH) + png + ".png";
        graphics::drawRect(pos_x, pos_y, size, size, br);
        break;
    case 3:
        size = 75;
        speed = 0.3f;
        br.texture = std::string(ASSET_PATH) + png + ".png";
        graphics::drawRect(pos_x, pos_y, size, size, br);
        break;
    case 4:
        size = 50;
        speed = 0.35f;
        br.texture = std::string(ASSET_PATH) + png + ".png";
        graphics::drawRect(pos_x, pos_y, size, size, br);
        break;
    case 5:
        size = 30;
        speed = 0.4f;
        br.texture = std::string(ASSET_PATH) + png + ".png";
        graphics::drawRect(pos_x, pos_y, size, size, br);
        break;
    }
}

void UFO::init()
{
    pos_x = rand() % (CANVAS_WIDTH - 50) + 50;
    pos_y = -100;
}

UFO::UFO(const Game& mygame)
    : GameObject(mygame)
{
    init();
}

UFO::~UFO()
{
}

Disk UFO::getCollisionHull() const
{
    Disk disk;
    disk.cx = pos_x;
    disk.cy = pos_y;
    disk.radius = size * 0.3f;
    return disk;
}