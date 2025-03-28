#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;
const int TILE_SIZE = 32;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

const int WALL = 2;
const int TREE = -2;
const int WATER = -1;
const int EMPTY = 0;


class Map
{
    public:
        vector <vector<int>> charMap;
        Map();
        int getTileType(int x, int y);
        void generateMapLayout();
        void render(SDL_Renderer* renderer, SDL_Texture* wallTexture, SDL_Texture* treeTexture,SDL_Texture* waterTexture, SDL_Texture* emptyTexture); // Thêm baseTexture
        void renderTrees(SDL_Renderer* renderer, SDL_Texture* treeTexture,SDL_Texture* waterTexture);
        void damageWall(int x, int y);

};

#endif // MAP_H
