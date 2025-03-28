#include "Map.h"

Map::Map() : charMap(MAP_HEIGHT, std::vector<int>(MAP_WIDTH, EMPTY)) {

    // Khởi tạo bản đồ với kích thước đã định và lấp đầy bằng ô trống
    generateMapLayout(); // Tạo bố cục bản đồ
}

int Map::getTileType(int x, int y) {
    // Kiểm tra xem có nằm ngoài bản đồ không
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return WALL; // Coi như là tường nếu ra ngoài bản đồ
    }
    return charMap[y][x]; // Trả về loại địa hình tại vị trí (x, y)
}

void Map::generateMapLayout() {
    // Mảng định nghĩa bố cục bản đồ (có thể thay đổi)
    std::string mapLayout[] = {
        "####################",
        "#                  #",
        "#    ##^####^##    #",
        "#  ^            ^  #",
        "#   ##^##  ##^##   #",
        "#   ~#^#~  ~#^#~   #",
        "#   ^^^^^  ^^^^^   #",
        "#    ^~^    ^~^    #",
        "#                  #",
        "#     ########     #",
        "#   #   ^   ^  #   #",
        "#   #   ## ##  #   #",
        "#   #          #   #",
        "#     ^  # #  ^    #",
        "#    ^#^     ^#^   #",
        "#     ^       ^    #",
        "#   ############   #",
        "##  ##        ##  ##",
        "##  ##        ##  ##",
        "####################"
    };

    // Tính toán offset để căn giữa bản đồ (tùy chọn)
    int offsetY = (MAP_HEIGHT - sizeof(mapLayout) / sizeof(mapLayout[0])) / 2;
    int offsetX = (MAP_WIDTH - mapLayout[0].length()) / 2;

    // Sao chép bố cục từ mảng vào charMap
    for (int i = 0; i < sizeof(mapLayout) / sizeof(mapLayout[0]); ++i) {
        for (int j = 0; j < mapLayout[i].length(); ++j) {
            if (i + offsetY >= 0 && i + offsetY < MAP_HEIGHT && j + offsetX >= 0 && j + offsetX < MAP_WIDTH)
            {
                switch (mapLayout[i][j]) {
                    case '#':
                        charMap[i + offsetY][j + offsetX] = WALL;
                        break;
                    case '^':
                        charMap[i + offsetY][j + offsetX] = TREE;
                        break;
                    case '~':
                        charMap[i + offsetY][j + offsetX] = WATER;
                        break;

                    default:
                        charMap[i + offsetY][j + offsetX] = EMPTY;
                        break;
                }
            }
        }
    }

    for (int i = 0; i < MAP_HEIGHT; i++) {
        if (i + offsetY < 0 || i + offsetY >= MAP_HEIGHT) continue;
        charMap[i][0] = WALL; // Cạnh trái
    }
    for (int i = 0; i < MAP_HEIGHT; i++) {
        if (i + offsetY < 0 || i + offsetY >= MAP_HEIGHT) continue;
        charMap[i][MAP_WIDTH - 1] = WALL; // Cạnh phải
    }
}
void Map::render(SDL_Renderer* renderer, SDL_Texture* wallTexture, SDL_Texture* treeTexture, SDL_Texture* waterTexture, SDL_Texture* emptyTexture) {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            SDL_Rect tileRect;
            tileRect.x = j * TILE_SIZE;
            tileRect.y = i * TILE_SIZE;
            tileRect.w = TILE_SIZE;
            tileRect.h = TILE_SIZE;

            // Vẽ Nền (ô trống)
            SDL_RenderCopy(renderer, emptyTexture, nullptr, &tileRect);

            SDL_Texture* currentTexture = nullptr;
            switch (charMap[i][j]) {
                case WALL:
                case 1:
                    currentTexture = wallTexture;
                    break;
                case WATER:
                    currentTexture = waterTexture;
                    break;

                default:
                    currentTexture = nullptr;
                    break;
            }

             if (currentTexture != nullptr ) {
                SDL_RenderCopy(renderer, currentTexture, nullptr, &tileRect);
            }

        }
    }
}

void Map::renderTrees(SDL_Renderer* renderer, SDL_Texture* treeTexture,SDL_Texture* waterTexture) {
    // Duyệt qua từng ô của bản đồ
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            if (charMap[i][j] == TREE) { // Nếu ô hiện tại là cây
                SDL_Rect tileRect; // Hình chữ nhật đại diện cho ô cây
                tileRect.x = j * TILE_SIZE; // Vị trí x của ô
                tileRect.y = i * TILE_SIZE; // Vị trí y của ô
                tileRect.w = TILE_SIZE;     // Chiều rộng của ô
                tileRect.h = TILE_SIZE;     // Chiều cao của ô
            if (charMap[i][j] == TREE) {
                SDL_RenderCopy(renderer, treeTexture, nullptr, &tileRect);
            }

            // Ve nuoc neu co
            if (charMap[i][j] == WATER) {
                SDL_RenderCopy(renderer, waterTexture, nullptr, &tileRect);
            }
        }
    }
}
}
void Map::damageWall(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return;

    // Kiểm tra xem ô đó có phải là tường không
    if (charMap[y][x] > 0) {
        // Thay đổi ký tự đại diện cho ô đó thành ký tự đại diện cho tường bị phá hủy
         charMap[y][x]--; // Hoặc một ký tự khác nếu bạn muốn biểu diễn tường bị phá hủy một phần
         if (charMap[y][x] == 0) {
            // Tường đã bị phá hủy hoàn toàn
            charMap[y][x] = EMPTY;
        }
    }
}

