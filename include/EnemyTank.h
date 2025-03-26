#ifndef ENEMYTANK_H
#define ENEMYTANK_H

#include <SDL2/SDL.h>
//#include "Map.h" // Xóa dòng này nếu có
#include "Bullet.h" // Thêm dòng này để có BulletDirection
#include <vector>
#include <cstdlib> // for rand()

class Map; // Forward declaration cho Map (nếu cần)

enum EnemyType {
    BASIC,
    FAST
};

class EnemyTank {
public:
    int x, y;
    int lives;
    SDL_Texture* texture;
    SDL_Texture* bulletTexture; // Texture đạn riêng - **THÊM DÒNG NÀY**
    bool isAlive;
    BulletDirection currentDirection; // Sử dụng BulletDirection - **ĐẢM BẢO ĐÃ THÊM "Bullet.h"**
    EnemyType type;
    int moveTimer; // Timer to control movement frequency
    int fireTimer;  // Timer to control firing frequency
    int stuckTimer; // Timer to detect and handle getting stuck

   EnemyTank(int startX, int startY, SDL_Texture* tex, SDL_Texture* bulletTex, EnemyType t);
    bool move(Map& map); // Simplified move function
    void render(SDL_Renderer* renderer);
    void fire();
    void takeDamage();

    std::vector<Bullet*> bullets;
    ~EnemyTank();

private:
    bool canMoveTo(int newX, int newY, Map& map);
    void changeDirection();
};

#endif
