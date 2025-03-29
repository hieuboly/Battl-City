#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Map.h"
#include "Tank.h"
#include "EnemyTank.h"
#include <SDL_mixer.h>
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
    SDL_Texture* titleTexture = nullptr;
    SDL_Texture* gameOverTexture = nullptr;
    vector<EnemyTank*> enemies;
    bool gameOver = false;
    bool gameWin = false;
    Mix_Music* backgroundMusic = nullptr;
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

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    cout << "SDL_mixer không thể khởi tạo! Lỗi SDL_mixer: " << Mix_GetError() << endl;
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
        titleTexture = loadTexture("man hinh ban dau.png");
        gameOverTexture = loadTexture("game_over.png");
    if (!gameOverTexture) {
        cout << "Không thể load texture Game Over!" << endl;
        return false;
    }
                if (!wallTexture || !treeTexture || !waterTexture || !emptyTexture || !tankTexture || !enemyTankTexture || !bulletTexture || !enemyBulletTexture||!titleTexture) {
            cout << "Không thể load textures!" << endl;
            return false;
        }

        tank = new Tank(tankTexture);

        // Create enemy tanks
        enemies.push_back(new EnemyTank(5, 2, enemyTankTexture, enemyBulletTexture, BASIC)); //Sử dụng enemyTankTexture
        enemies.push_back(new EnemyTank(10, 2, enemyTankTexture, enemyBulletTexture, FAST));//Sử dụng enemyTankTexture
        enemies.push_back(new EnemyTank(15, 2, enemyTankTexture, enemyBulletTexture, BASIC));//Sử dụng enemyTankTexture

          backgroundMusic = Mix_LoadMUS("play-time-fun-upbeat-gaming-birthday-music-259703.mp3");
        if (!backgroundMusic) {
            cout << "Không thể load nhạc nền! Lỗi SDL_mixer: " << Mix_GetError() << endl;
            return false;
        }


        Mix_PlayMusic(backgroundMusic, -1);

        return true;
    }

    void close() {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
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
        SDL_DestroyTexture(titleTexture);
        SDL_DestroyTexture(gameOverTexture);
        gameOverTexture = nullptr;

        // Giải phóng renderer và cửa sổ
        SDL_DestroyRenderer(renderer);

        renderer = nullptr;

        SDL_DestroyWindow(window);
        window = nullptr;
         if (gameOverTexture) {
        SDL_DestroyTexture(gameOverTexture);
        gameOverTexture = nullptr;
    }

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
        Mix_Quit();
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
    void renderTitleScreen() {
    SDL_Rect titleRect;
    titleRect.x = 0;  // Căn trái
    titleRect.y = 0;  // Căn trên
    titleRect.w = 700; // Chiều rộng
    titleRect.h = 800; // Chiều cao

    SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
    SDL_RenderPresent(renderer);
}

    void run() {
        // Vòng lặp chính của game
        bool quit = false; // Biến để kiểm soát việc thoát khỏi vòng lặp
        SDL_Event e;       // Biến để lưu trữ các sự kiện
        srand(time(nullptr));
        renderTitleScreen();
        SDL_Delay(3000);
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
            render();
        } else {
            showGameOverScreen();
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
            Mix_HaltMusic();

        }
        if(!tank->isAlive){
            gameOver=true;
            Mix_HaltMusic();

        }

    }
void updateBullets() {
    // Player bullets
    for (auto it = tank->bullets.begin(); it != tank->bullets.end();) {
        Bullet* bullet = *it;
        bullet->move();

        bool collision = false;
        // Check collision with enemies
        for (int i = enemies.size() - 1; i >= 0; --i) {
            EnemyTank* enemy = enemies[i];
            if (enemy->isAlive) {
                SDL_Rect bulletRect = {bullet->x, bullet->y, 16, 16};
                SDL_Rect enemyRect = {enemy->x * TILE_SIZE, enemy->y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                    enemy->takeDamage();
                    cout << "Enemy lives remaining: " << enemy->lives << endl;
                    if (!enemy->isAlive) {
                        delete enemies[i];
                        enemies.erase(enemies.begin() + i);
                    }
                    delete bullet;
                    it = tank->bullets.erase(it);
                    collision = true;
                    break;
                }
            }
        }


        if (collision) {
            continue;
        }

        if (bullet->checkCollision(map, tank)) {
            bullet->handleCollision(map, tank);
            delete bullet;
            it = tank->bullets.erase(it);
        } else {
            ++it;
        }
    }


    for (EnemyTank* enemy : enemies) {
        for (auto it = enemy->bullets.begin(); it != enemy->bullets.end();) {
            Bullet* bullet = *it;
            bullet->move();
            if (tank->isAlive) { // Thêm điều kiện kiểm tra isAlive
                SDL_Rect bulletRect = {bullet->x, bullet->y, 16, 16};
                SDL_Rect tankRect = {tank->x * TILE_SIZE, tank->y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                if (SDL_HasIntersection(&bulletRect, &tankRect)) {
                    cout << "Tank hit by enemy bullet!" << endl; // Thêm dòng này
                    tank->takeDamage();
                    cout << "Tank lives remaining: " << tank->lives << endl; // Thêm dòng này
                    delete bullet;
                    it = enemy->bullets.erase(it);
                    continue; // Bỏ qua phần còn lại của vòng lặp
                }
            }
            if (bullet->checkCollision(map, tank)) {
                bullet->handleCollision(map, tank);
                delete bullet;
                it = enemy->bullets.erase(it);
            } else {
                ++it;
            }
        }
    }
}
 void showGameOverScreen() {

    SDL_RenderClear(renderer);
    SDL_Rect destRect = {0, 0, 640, 640};
    SDL_RenderCopy(renderer, gameOverTexture, NULL, &destRect);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
}
    void render() {
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Render map
        map.render(renderer, wallTexture, treeTexture, waterTexture, emptyTexture);

        // Render tank
        tank->render(renderer);

         //Render enemy tanks
        for (EnemyTank* enemy : enemies) {
            if (enemy->isAlive) {
                enemy->render(renderer);
            }
        }
          map.renderTrees(renderer, treeTexture, waterTexture);
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
    Game game;

    if (!game.init()) {

        return 1;
    }

    game.run();
    game.close();

    return 0;
}
