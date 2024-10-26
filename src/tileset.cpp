#include "Tileset.hpp"


Tileset::Tileset(SDL_Renderer* renderer, SDL_Texture* tilesetTexture, int tileSize)
{
    int textureWidth, textureHeight;
    SDL_QueryTexture(tilesetTexture, NULL, NULL, &textureWidth, &textureHeight);

    int numTilesX = textureWidth / tileSize;
    int numTilesY = textureHeight / tileSize;

    for (int y = 0; y < numTilesY; y++) {
        for (int x = 0; x < numTilesX; x++) {
            SDL_Rect srcRect = { x * tileSize, y * tileSize, tileSize, tileSize};

            SDL_Texture* tileTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tileSize, tileSize);

            SDL_SetRenderTarget(renderer, tileTexture);

            SDL_SetTextureBlendMode(tileTexture, SDL_BLENDMODE_BLEND);

            SDL_RenderCopy(renderer, tilesetTexture, &srcRect, NULL);

            tileset.push_back(tileTexture);
        }
    }

    SDL_SetRenderTarget(renderer, NULL);
}

void Tileset::loadTiles(SDL_Renderer* renderer, const std::string& tilemapPath, std::vector<GameObject*>& gameobjects, std::string tileName, int tileSize, bool isCollider)
{
    std::ifstream csvFile(tilemapPath);
    std::string line;
    int row = 0;

    while (std::getline(csvFile, line)) {
        std::stringstream ss(line);
        std::string tileIndexStr;
        int col = 0;

        while (std::getline(ss, tileIndexStr, ',')) {
            int tileIndex = std::stoi(tileIndexStr);

            if (tileIndex >= 0 && tileIndex < tileset.size()) {
                SDL_Texture* tileTexture = tileset[tileIndex];

                GameObject* tile = new GameObject(renderer, tileTexture, tileName, true, isCollider, col * tileSize, row * tileSize, tileSize, tileSize);

                gameobjects.push_back(tile);
            }

            col++;
        }

        row++;
    }
}