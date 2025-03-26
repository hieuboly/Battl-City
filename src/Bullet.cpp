#include "Bullet.h"
#include "Map.h"
#include "Tank.h"
#include <SDL2/SDL.h>

Bullet::Bullet(int startX, int startY, BulletDirection dir, SDL_Texture* tex, bool enemy) :
    x(startX), y(startY), speed(1), direction(dir), isAlive(true), texture(tex), isEnemyBullet(enemy) {}

void Bullet::move() {
    if (!isAlive) return;

    switch (direction) {
    case BULLET_UP:    y -= speed; break;
    case BULLET_DOWN:  y += speed; break;
    case BULLET_LEFT:  x -= speed; break;
    case BULLET_RIGHT: x += speed; break;
    }
}

void Bullet::render(SDL_Renderer* renderer) {
    if (!isAlive) return;

    SDL_Rect bulletRect;
    bulletRect.x = x;
    bulletRect.y = y;
    bulletRect.w = BULLET_SIZE;
    bulletRect.h = BULLET_SIZE;

    SDL_RenderCopy(renderer, texture, nullptr, &bulletRect);
}

bool Bullet::checkCollision(Map& map,Tank* playerTank) {
    if (!isAlive) return false;

    int gridX = x / TILE_SIZE;
    int gridY = y / TILE_SIZE;

    if (gridX < 0 || gridX >= MAP_WIDTH || gridY < 0 || gridY >= MAP_HEIGHT) {
        isAlive = false; // Ra khỏi bản đồ
        return true;
    }

    int terrain = map.getTileType(gridX, gridY);



    if (terrain > 0) {
        return true; //Va chạm tường
    }

    return false;
}
void Bullet::handleCollision(Map& map, Tank* playerTank) {
    if (!isAlive) return;

    int gridX = (int)(x / TILE_SIZE);
    int gridY = (int)(y / TILE_SIZE);

    if (gridX < 0 || gridX >= MAP_WIDTH || gridY < 0 || gridY >= MAP_HEIGHT) {
        isAlive = false; // Ra khỏi bản đồ
        return;
    }

    int terrain = map.getTileType(gridX, gridY);

    if (terrain > 0) {
        //Làm vỡ tường
        map.damageWall(gridX, gridY); //Gọi hàm damageWall của Map
        isAlive = false;
    }

}
