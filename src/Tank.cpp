#include "Tank.h"
#include "Map.h"
#include <iostream>

Tank::Tank( SDL_Texture *tex):
   x(10), y(18), lives(7), texture(tex), isAlive(true), currentDirection(UP) {}
bool Tank ::move(int dx, int dy, Map& map){
    int newX=x+dx;
    int newY=y+dy;
    if(newX<0|| newX>=MAP_WIDTH||newY<0||newY>=MAP_HEIGHT){
        return false;// dung im
    }

  int terrain = map.getTileType(newX, newY);

 switch (terrain){
 case 2:
 case 1:
      return false;// neu vao tupng khong di chuyen duoc
 case -1 :
      takeDamage();// -1 mang neu di vao nuoc
 case 0:
    currentDirection=(dx==0&&dy==-1) ? UP : (dx==0 && dy==1) ? DOWN :(dx==-1 && dy==0) ? LEFT:RIGHT;
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
for (Bullet* bullet : bullets) {
        bullet->render(renderer);
    }
}

void Tank::fire(SDL_Texture* bulletTexture) {
    if (!isAlive) return;

    int bulletStartX = x * TILE_SIZE + TILE_SIZE / 2 - 8; // 8 = 16/2 (giữa xe tăng)
    int bulletStartY = y * TILE_SIZE + TILE_SIZE / 2 - 8;

    BulletDirection bulletDir;
    switch (currentDirection) {
    case UP:    bulletDir = BULLET_UP;    break;
    case DOWN:  bulletDir = BULLET_DOWN;  break;
    case LEFT:  bulletDir = BULLET_LEFT;  break;
    case RIGHT: bulletDir = BULLET_RIGHT; break;
    }

Bullet* newBullet = new Bullet(bulletStartX, bulletStartY, bulletDir, bulletTexture,this); // Tạo con trỏ Bullet*
    bullets.push_back(newBullet); //
}
Tank::~Tank() {
    // Giải phóng bộ nhớ của các viên đạn
    for (Bullet* bullet : bullets) {
        delete bullet;
    }
    bullets.clear(); // Xóa tất cả các con trỏ khỏi vector
}
