#include "EnemyTank.h"
#include "Map.h"
#include "Bullet.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

EnemyTank::EnemyTank(int startX, int startY, SDL_Texture* tex, SDL_Texture* bulletTex, EnemyType t) :
    x(startX), y(startY), texture(tex), bulletTexture(bulletTex),lives(2), isAlive(true),
    currentDirection(static_cast<BulletDirection>(rand() % 4)), // Ép kiểu sang BulletDirection
    type(t), moveTimer(0), fireTimer(0), stuckTimer(0) {
    srand(time(nullptr));
}

bool EnemyTank::move(Map& map) {
    if (moveTimer > 0) {
        moveTimer--;
        return false; // Don't move this frame
    }

    int dx = 0, dy = 0;
    switch (currentDirection) {
        case BULLET_UP:    dy = -1; break;
        case BULLET_DOWN:  dy = 1; break;
        case BULLET_LEFT:  dx = -1; break;
        case BULLET_RIGHT: dx = 1; break;
    }

    int newX = x + dx;
    int newY = y + dy;

    if (!canMoveTo(newX, newY, map)) {
        stuckTimer++;
        if (stuckTimer > 30) { // If stuck for a while, change direction
            changeDirection();
            stuckTimer = 0;
        }
        return false;
    }
    int terrain = map.getTileType(newX, newY);
    if (terrain == WATER) {
        takeDamage(); // Mất mạng nếu đi vào nước
    }
    stuckTimer = 0;
    x = newX;
    y = newY;
    moveTimer = (type == BASIC) ? 300 : 150;

    // Adjust move frequency based on type
    return true;
}

bool EnemyTank::canMoveTo(int newX, int newY, Map& map) {
    if (newX < 0 || newX >= MAP_WIDTH || newY < 0 || newY >= MAP_HEIGHT) {
        return false;
    }

    int terrain = map.getTileType(newX, newY);
    //Cho phep di chuyen qua cay
      if(terrain == TREE) return true;
    return (terrain == EMPTY || terrain == WATER); // Can move to empty or water tiles
}

void EnemyTank::changeDirection() {
    BulletDirection newDir; // Sử dụng BulletDirection
    do {
        newDir = static_cast<BulletDirection>(rand() % 4);
    } while (newDir == currentDirection);
    currentDirection = newDir;
}

void EnemyTank::render(SDL_Renderer* renderer) {
    if (!isAlive) return;

    SDL_Rect tankRect;
    tankRect.x = x * TILE_SIZE;
    tankRect.y = y * TILE_SIZE;
    tankRect.w = TILE_SIZE;
    tankRect.h = TILE_SIZE;

    double angle = 0.0;
    switch (currentDirection) {
    case BULLET_UP:    angle = 0.0; break;
    case BULLET_LEFT:  angle = 270.0; break;
    case BULLET_DOWN:  angle = 180.0; break;
    case BULLET_RIGHT: angle = 90.0; break;
    }

    SDL_RenderCopyEx(renderer, texture, nullptr, &tankRect, angle, nullptr, SDL_FLIP_NONE);

    for (Bullet* bullet : bullets) {
        bullet->render(renderer);
    }
}

void EnemyTank::fire() {
    if (!isAlive || fireTimer > 0) {
        fireTimer--;
        return;
    }

    int bulletStartX = x * TILE_SIZE + TILE_SIZE / 2 - 8;
    int bulletStartY = y * TILE_SIZE + TILE_SIZE / 2 - 8;

    BulletDirection bulletDir;
    switch (currentDirection) {
    case BULLET_UP:    bulletDir = BULLET_UP;    break;
    case BULLET_DOWN:  bulletDir = BULLET_DOWN;  break;
    case BULLET_LEFT:  bulletDir = BULLET_LEFT;  break;
    case BULLET_RIGHT: bulletDir = BULLET_RIGHT; break;
    }

    Bullet* newBullet = new Bullet(bulletStartX, bulletStartY, bulletDir, bulletTexture,nullptr,  true); // Sử dụng bulletTexture của xe tăng
    bullets.push_back(newBullet);

    fireTimer = 10 + rand() % 10;
}
void EnemyTank::takeDamage() {
    lives--;
    if (lives <= 0) {
        isAlive = false;
        std::cout << "Enemy Tank destroyed!" << std::endl;
    }
}
EnemyTank::~EnemyTank() {
    for (Bullet* bullet : bullets) {
        delete bullet;
    }
    bullets.clear();
}
