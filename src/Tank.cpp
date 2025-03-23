#include "Tank.h"
#include "Map.h"
#include <iostream>

Tank::Tank(int startX, int startY, SDL_Texture *tex):
   x(startX), y(startY), lives(5), texture(tex), isAlive(true), currentDirection(UP) {}
bool Tank ::move(int dx, int dy, Map& map){
    int newX=x+dx;
    int newY=y+dy;
    if(newX<0|| newX>=MAP_WIDTH||newY<0||newY>=MAP_HEIGHT){
        return false;// dung im
    }

  char terrain = map.getTileType(newX, newY);

 switch (terrain){
 case WALL:
      return false;// neu vao tupng khong di chuyen duoc
 case WATER :
      takeDamage();// -1 mang neu di vao nuoc
 case EMPTY:
    currentDirection=(dx==0&&dy==-1) ? UP : (dx==0 && dy==1) ? DOWN :(dx==-1 && dy==0) ? LEFT:RIGHT;
    break;
 case TREE:
    currentDirection = (dx==0 && dy == -1) ? UP : (dx==0 && dy== 1) ? DOWN : (dx==-1 && dy == 0)? LEFT: RIGHT;
    break;
 default:
    break;
 }

        x=newX;
        y=newY;

 return true;
 }
void Tank::takeDamage (){
lives--;
if(lives<=0){
    isAlive =false;
    std::cout << "Tank destroyed!"<< std::endl;
}
else {
    std::cout << "Tank took damage! Lives remaining: " << lives << std::endl;
}
}
void Tank::render(SDL_Renderer* renderer){
if (!isAlive) return; // neu chet khong ve them
SDL_Rect tankRect;
    tankRect.x = x * TILE_SIZE;
    tankRect.y = y * TILE_SIZE;
    tankRect.w = TILE_SIZE;
    tankRect.h = TILE_SIZE;
    double angle = 0.0;
switch (currentDirection) {
        case UP:    angle = 0.0; break;
        case LEFT:  angle = 270.0; break;
        case DOWN:  angle = 180.0; break;
        case RIGHT: angle = 90.0; break;
    }

SDL_RenderCopyEx(renderer, texture, nullptr, &tankRect, angle, nullptr, SDL_FLIP_NONE);
}
