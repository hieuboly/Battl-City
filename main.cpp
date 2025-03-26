#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Map.h"
#include "Tank.h"
#include "EnemyTank.h"

//Xay dung ban do


using namespace std;

// Lớp Game (quản lý toàn bộ game)
class Game {
public:
    SDL_Window* window = nullptr;       // Cửa sổ game
    SDL_Renderer* renderer = nullptr;   // Renderer để vẽ lên cửa sổ
    Map map;                            // Bản đồ
    Tank* tank = nullptr;
    SDL_Texture* wallTexture = nullptr;   // Texture của tường
    SDL_Texture* treeTexture = nullptr;   // Texture của cây
    SDL_Texture* waterTexture = nullptr;  // Texture của nước
    SDL_Texture* emptyTexture = nullptr;  // Texture của ô trống
    SDL_Texture* tankTexture = nullptr;
    SDL_Texture* enemyTankTexture = nullptr;
    SDL_Texture* enemyBulletTexture = nullptr;
    SDL_Texture* bulletTexture = nullptr;
    SDL_Texture* wallDamagedTexture = nullptr;
    vector<EnemyTank*> enemies;
     bool gameOver = false;

    Game() {} // Hàm khởi tạo

    bool init() {
        // Khởi tạo SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            cout << "SDL không thể khởi tạo! Lỗi SDL: " << SDL_GetError() << endl;
            return false;
        }

        // Khởi tạo SDL_image
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            cout << "SDL_image không thể khởi tạo! Lỗi SDL_image: " << IMG_GetError() << endl;
            return false;
        }

        // Tạo cửa sổ
        window = SDL_CreateWindow("Battle City Map", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window) {
            cout << "Không thể tạo cửa sổ! Lỗi SDL: " << SDL_GetError() << endl;
            return false;
        }

        // Tạo renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            cout << "Không thể tạo renderer! Lỗi SDL: " << SDL_GetError() << endl;
            return false;
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Đặt màu vẽ mặc định

        // Load textures
        wallTexture = loadTexture("tuong.png");
        treeTexture = loadTexture("cay.png");
        waterTexture = loadTexture("nuoc.png");
        emptyTexture = loadTexture("nen.png");
        enemyTankTexture = loadTexture("tank_enemy.png");
        enemyBulletTexture = loadTexture("dan_dich.png");
        tankTexture = loadTexture("tank.png");
        bulletTexture = loadTexture("dan.png");
        wallDamagedTexture = loadTexture("nen.png");
         if (!wallTexture || !treeTexture || !waterTexture || !emptyTexture || !tankTexture || !enemyTankTexture || !bulletTexture || !enemyBulletTexture) { //Đã xóa enemyTankTexture2
            cout << "Không thể load textures!" << endl;
            return false;
        }

        tank = new Tank(tankTexture);

        // Create enemy tanks
        enemies.push_back(new EnemyTank(5, 2, enemyTankTexture, enemyBulletTexture, BASIC)); //Sử dụng enemyTankTexture
        enemies.push_back(new EnemyTank(10, 2, enemyTankTexture, enemyBulletTexture, FAST));//Sử dụng enemyTankTexture
        enemies.push_back(new EnemyTank(15, 2, enemyTankTexture, enemyBulletTexture, BASIC));//Sử dụng enemyTankTexture

        return true;

        return true;
    }

    void close() {
        // Giải phóng textures
        SDL_DestroyTexture(wallTexture);
        SDL_DestroyTexture(treeTexture);
        SDL_DestroyTexture(waterTexture);
        SDL_DestroyTexture(emptyTexture);
        SDL_DestroyTexture(tankTexture);
        SDL_DestroyTexture(enemyTankTexture);
        SDL_DestroyTexture(enemyBulletTexture);
        SDL_DestroyTexture(bulletTexture);
        SDL_DestroyTexture(wallDamagedTexture);
        // Giải phóng renderer và cửa sổ
        SDL_DestroyRenderer(renderer);

        renderer = nullptr;

        SDL_DestroyWindow(window);
        window = nullptr;

        // Tắt SDL và SDL_image
        for (EnemyTank* enemy : enemies) {
            delete enemy;
        }
        enemies.clear();
         if (tank != nullptr) {
            delete tank;
            tank = nullptr;
        }
        IMG_Quit();
        SDL_Quit();
    }

    SDL_Texture* loadTexture(const string& filePath) {
        // Load một texture từ file ảnh
        SDL_Surface* surface = IMG_Load(filePath.c_str()); // Load ảnh vào surface
        if (!surface) {
            cout << "Không thể load ảnh: " << filePath << " Lỗi SDL: " << IMG_GetError() << endl;
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); // Tạo texture từ surface
        SDL_FreeSurface(surface); // Giải phóng surface (không cần nữa)

        if (!texture) {
            cout << "Không thể tạo texture: " << SDL_GetError() << endl;
            return nullptr;
        }

        return texture; // Trả về texture
    }

    void run() {
        // Vòng lặp chính của game
        bool quit = false; // Biến để kiểm soát việc thoát khỏi vòng lặp
        SDL_Event e;       // Biến để lưu trữ các sự kiện
        srand(time(nullptr));
        while (!quit) {
            // Xử lý sự kiện
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    // Nếu sự kiện là đóng cửa sổ, thoát
                    quit = true;
                }
                if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            tank->move(0, -1, map); // Di chuyển lên
                            break;
                        case SDLK_DOWN:
                            tank->move(0, 1, map);  // Di chuyển xuống
                            break;
                        case SDLK_LEFT:
                            tank->move(-1, 0, map); // Di chuyển trái
                            break;
                        case SDLK_RIGHT:
                            tank->move(1, 0, map);  // Di chuyển phải
                            break;
                        case SDLK_SPACE: // Bắn đạn khi nhấn Space
                            tank->fire(bulletTexture);
                            break;
                    }
                }
            }
             if (!gameOver) {
                update();
            }

            // Render the scene
            render();

            if(gameOver)
            {
                 cout << "Game Over!" << endl;
                SDL_Delay(3000);
                quit = true;
            }
        }
    }

        void update() {
    // Enemy Tank AI
    for (EnemyTank* enemy : enemies) {
        if (enemy->isAlive) {
            enemy->move(map); // Move the enemy
            if (rand() % 100 < 2) { // 2% chance to fire each frame
                enemy->fire(); // Enemy fires
            }
        }
    }

     updateBullets();

        // Check win condition (all enemies dead)
        bool allEnemiesDead = true;
        for (EnemyTank* enemy : enemies) {
            if (enemy->isAlive) {
                allEnemiesDead = false;
                break;
            }
        }

        if (allEnemiesDead) {
            gameOver = true;
            cout << "You Win!" << endl;
        }
        if(!tank->isAlive){
            gameOver=true;
        }

    }
    void updateBullets() {
    // Player bullets
    for (auto it = tank->bullets.begin(); it != tank->bullets.end();) {
        Bullet* bullet = *it;
        bullet->move();
        if (bullet->checkCollision(map, tank)) { // Check collision with map and player
            bullet->handleCollision(map, tank);  // Handle collision with map and player
            delete bullet;
            it = tank->bullets.erase(it);
        } else {
            bullet->render(renderer);
            ++it;
        }
    }

    // Enemy bullets
    for (EnemyTank* enemy : enemies) {
        for (auto it = enemy->bullets.begin(); it != enemy->bullets.end();) {
            Bullet* bullet = *it;
            bullet->move();
            if (bullet->checkCollision(map, tank)) { // Check collision with map and player
                bullet->handleCollision(map, tank);  // Handle collision with map and player
                delete bullet;
                it = enemy->bullets.erase(it);
            } else {
                bullet->render(renderer);
                ++it;
            }
        }
    }
}
    void render() {
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Render map
        map.render(renderer, wallTexture, treeTexture, waterTexture, emptyTexture);
        map.renderTrees(renderer, treeTexture);
        // Render tank
        tank->render(renderer);

         //Render enemy tanks
        for (EnemyTank* enemy : enemies) {
            if (enemy->isAlive) {
                enemy->render(renderer);
            }
        }
         //Render player bullets
        for (Bullet* bullet : tank->bullets) {
            bullet->render(renderer);
        }

        // Render enemy bullets
        for (EnemyTank* enemy : enemies) {
            for (Bullet* bullet : enemy->bullets) {
                bullet->render(renderer);
            }
        }
        SDL_RenderPresent(renderer);
    }
};       // Xóa màn hình


int main(int argc, char* args[]) {
    Game game; // Tạo đối tượng game

    if (!game.init()) {
        // Nếu khởi tạo thất bại, thoát
        return 1;
    }

    game.run(); // Chạy game
    game.close(); // Giải phóng tài nguyên

    return 0; // Kết thúc chương trình
}
