#ifndef TANK_H
#define TANK_H

#include <SDL2/SDL.h>
#include "Map.h"
#include "Bullet.h"
#include <vector>
enum Direction{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
class Tank
{
    public:
       int x, y; // vi tri
       int lives; // so mang
       SDL_Texture* texture;
       bool isAlive; // da che chua
       Direction currentDirection; // huong di chuyen hien tai ;
       Tank ( SDL_Texture* tex); // khoi tao
       bool move(int dx,int dy, Map& map);// ham di chuyen
       void takeDamage();// nhan sat thuong
       void render( SDL_Renderer* renderer); // ve xe tang
       void fire(SDL_Texture* bulletTexture);
       std::vector<Bullet*> bullets;


       ~Tank();
};

#endif // TANK_H
