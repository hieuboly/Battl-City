#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>

enum BulletDirection {
    BULLET_UP,
    BULLET_DOWN,
    BULLET_LEFT,
    BULLET_RIGHT
};

// Forward declaration for Map
class Map;

class Bullet {
public:
    int x, y;          // Vị trí
    int speed;       // Tốc độ di chuyển
    BulletDirection direction; // Hướng di chuyển
    bool isAlive;      // Đạn còn tồn tại hay không

    SDL_Texture* texture; // Texture của đạn

    Bullet(int startX, int startY, BulletDirection dir, SDL_Texture* tex);
    void move();
    void render(SDL_Renderer* renderer);
    bool checkCollision(Map& map);
    void handleCollision(Map& map);

private:
    static const int BULLET_SIZE = 16; // Kích thước đạn (16x16)
};

#endif // BULLET_H
