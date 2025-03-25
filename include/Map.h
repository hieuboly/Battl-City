#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
using namespace std;
const int SCREEN_WIDTH = 640; //chieu rong map
const int SCREEN_HEIGHT = 640;   //chiru cao man hinh
const int TILE_SIZE = 32;       // khich thuoc cua o
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;  // so o 'chieu rong cus ban do'
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE; // chieu cao cua ban do

const int WALL = 2; // 2 lần bắn để phá hủy
const int TREE = -2; // Không thể phá hủy
const int WATER = -1; // Không thể phá hủy
const int EMPTY = 0;

class Map
{
    public:
        vector <vector<int>> charMap;// khoi tao ma tran ban do
        Map();
        int getTileType(int x, int y);// dia hinh tai x, y
        void generateMapLayout();// bo cuc ban do
        void render(SDL_Renderer* renderer, SDL_Texture* wallTexture, SDL_Texture* treeTexture,SDL_Texture* waterTexture, SDL_Texture* emptyTexture);
        void renderTrees(SDL_Renderer* renderer, SDL_Texture* treeTexture); // ve lop phu cay
        void damageWall(int x, int y);
};
#endif // MAP_H
