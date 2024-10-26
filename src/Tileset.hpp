#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>

#include "GameObject.hpp"

class Tileset {
    public:
        Tileset(SDL_Renderer* renderer, SDL_Texture* tilesetTexture, int tileSize);

        void loadTiles(SDL_Renderer* renderer, const std::string& tilemapPath, std::vector<GameObject*>& gameobjects, std::string tileName, int tileSize, bool isCollider);

    private:
        std::vector<SDL_Texture*> tileset;
};

#endif