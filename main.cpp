#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Map.h"

using namespace std;

// Lớp Game (quản lý toàn bộ game)
class Game {
public:
    SDL_Window* window = nullptr;       // Cửa sổ game
    SDL_Renderer* renderer = nullptr;   // Renderer để vẽ lên cửa sổ
    Map map;                            // Bản đồ

    SDL_Texture* wallTexture = nullptr;   // Texture của tường
    SDL_Texture* treeTexture = nullptr;   // Texture của cây
    SDL_Texture* waterTexture = nullptr;  // Texture của nước
    SDL_Texture* emptyTexture = nullptr;  // Texture của ô trống

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

        if (!wallTexture || !treeTexture || !waterTexture || !emptyTexture) {
            cout << "Không thể load textures!" << endl;
            return false;
        }

        return true;
    }

    void close() {
        // Giải phóng textures
        SDL_DestroyTexture(wallTexture);
        SDL_DestroyTexture(treeTexture);
        SDL_DestroyTexture(waterTexture);
        SDL_DestroyTexture(emptyTexture);

        // Giải phóng renderer và cửa sổ
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;

        SDL_DestroyWindow(window);
        window = nullptr;

        // Tắt SDL và SDL_image
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

        while (!quit) {
            // Xử lý sự kiện
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    // Nếu sự kiện là đóng cửa sổ, thoát
                    quit = true;
                }
            }

            // Xóa màn hình
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Đặt màu đen
            SDL_RenderClear(renderer); // Xóa màn hình

            // Vẽ bản đồ
            map.render(renderer, wallTexture, treeTexture, waterTexture, emptyTexture);
            map.renderTrees(renderer, treeTexture);

            // Cập nhật màn hình
            SDL_RenderPresent(renderer);
        }
    }
};

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
