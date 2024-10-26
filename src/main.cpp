#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>

#include "GameObject.hpp"
#include "Tileset.hpp"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cerr << "Error: " << IMG_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() != 0) {
        std::cerr << "Error: " << TTF_GetError() << std::endl;
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    if (window == nullptr) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Surface* iconSurface = IMG_Load("C:/Users/hakop/Coding/small projects/mini game engine/assets/icon.png");
    SDL_SetWindowIcon(window, iconSurface);
    SDL_FreeSurface(iconSurface);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
     
        return 1;
    }
    
    SDL_Texture* characterTexture = IMG_LoadTexture(renderer, "C:/Users/hakop/Coding/small projects/mini game engine/assets/knight.png");
    SDL_Texture* coinTexture = IMG_LoadTexture(renderer, "C:/Users/hakop/Coding/small projects/mini game engine/assets/coin.png");
    SDL_Texture* tilesetTexture = IMG_LoadTexture(renderer, "C:/Users/hakop/Coding/small projects/mini game engine/assets/tileset.png");

    TTF_Font* font = TTF_OpenFont("C:/Users/hakop/Coding/small projects/mini game engine/assets/PixelOperator8.ttf", 24);
    SDL_Color textColor = {255, 255, 255};

    Mix_Music* backgroundMusic = Mix_LoadMUS("C:/Users/hakop/Coding/small projects/mini game engine/assets/background music.mp3");
    Mix_PlayMusic(backgroundMusic, -1);

    SDL_Rect camera = {0, 0, 1280, 720};

    std::vector<GameObject*> gameobjects = {};

    GameObject character(renderer, characterTexture, "character", false, true, 0, 903, 39, 57);
    GameObject startBarrier (renderer, NULL, "barrier", true, true, -64, 0, 64, 1440);
    GameObject endBarrier (renderer, NULL, "barrier", true, true, 7680, 0, 64, 1440);

    Tileset tileset(renderer, tilesetTexture, 16);
    Tileset coin(renderer, coinTexture, 16);
    tileset.loadTiles(renderer, "C:/Users/hakop/Coding/small projects/mini game engine/assets/sky tilemap.csv", gameobjects, "sky", 64, false);
    tileset.loadTiles(renderer, "C:/Users/hakop/Coding/small projects/mini game engine/assets/main tilemap.csv", gameobjects, "tile", 64, false);
    coin.loadTiles(renderer, "C:/Users/hakop/Coding/small projects/mini game engine/assets/coin tilemap.csv", gameobjects, "coin", 64, false);
    tileset.loadTiles(renderer, "C:/Users/hakop/Coding/small projects/mini game engine/assets/collider tilemap.csv", gameobjects, "tile", 64, true);

    gameobjects.push_back(&character);
    gameobjects.push_back(&startBarrier);
    gameobjects.push_back(&endBarrier);

    bool isJumping = false;
    float jumpForce = 0;

    float currentTime = 0;
    float bestTime = INFINITY;
    int coins = 0;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    const double frameDelay = 1.0 / 60.0;
    
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = static_cast<double>(NOW - LAST) / SDL_GetPerformanceFrequency();

        currentTime += deltaTime;

        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
            character.addForce(-2, 0);
        }
        if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
            character.addForce(2, 0);
        }
        if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) {
            if (character.isGrounded) {
                character.addForce(0, -40);
                isJumping = true;
            }
            else if (isJumping && jumpForce > -20) {
                jumpForce += -2;
                character.addForce(0, -2);
            }
        }
        else {
            isJumping = false;
            jumpForce = 0;
        }

        for (int i = 0; i < gameobjects.size(); i++) {
            if (gameobjects[i]->name == "coin") {
                GameObject* coin = gameobjects[i];
                SDL_Rect characterRect = character.getRect();
                SDL_Rect coinRect = coin->getRect();
                if (SDL_HasIntersection(&characterRect, &coinRect))
                {
                    gameobjects.erase(gameobjects.begin() + i);
                    coins++;
                    continue;
                }
            }
        }
        
        if (character.getRect().x >= 7641 && coins == 32) {
            character.setRect(0, 903, 39, 57);

            if (currentTime < bestTime)
            {
                bestTime = currentTime;
            }
            currentTime = 0;
            coins = 0;

            for (int i = 0; i < gameobjects.size(); i++) {
                if (gameobjects[i]->name == "coin") {
                    gameobjects.erase(gameobjects.begin() + i);
                }
            }
            coin.loadTiles(renderer, "C:/Users/hakop/Coding/small projects/mini game engine/assets/coin tilemap.csv", gameobjects, "coin", 64, false);
        }
        if (character.getRect().y >= 1440) {
            character.setRect(0, 903, 39, 57);

            currentTime = 0;
            coins = 0;

            for (int i = 0; i < gameobjects.size(); i++) {
                if (gameobjects[i]->name == "coin") {
                    gameobjects.erase(gameobjects.begin() + i);
                }
            }
            coin.loadTiles(renderer, "C:/Users/hakop/Coding/small projects/mini game engine/assets/coin tilemap.csv", gameobjects, "coin", 64, false);
        }

        camera.y = character.getRect().y + character.getRect().h / 2 - camera.h / 2;
        if (camera.y > 720) {
            camera.y = 720;
        }
        camera.x = character.getRect().x + character.getRect().w / 2 - camera.w / 2;
        if (camera.x < 0) {
            camera.x = 0;
        }
        else if (camera.x >= 6400) {
            camera.x = 6400;
        }

        SDL_RenderClear(renderer);

        for (GameObject* gameobject : gameobjects) {
            gameobject->update(deltaTime, gameobjects);
        }

        for (GameObject* gameobject : gameobjects) {
            gameobject->render(renderer, camera);
        }

        std::ostringstream oss;

        oss << std::fixed << std::setprecision(2) << std::floor(currentTime * 100.0) / 100.0;
        std::string currentTimeText = "Time: " + oss.str();
        int currentTimeTextWidth;
        SDL_Surface* currentTimeTextSurface = TTF_RenderText_Solid(font, currentTimeText.c_str(), textColor);
        SDL_Rect currentTimeTextRect = { 10, 10, currentTimeTextSurface->w, currentTimeTextSurface->h };
        SDL_Texture* currentTimeTextTexture = SDL_CreateTextureFromSurface(renderer, currentTimeTextSurface);
        SDL_FreeSurface(currentTimeTextSurface);
        SDL_RenderCopy(renderer, currentTimeTextTexture, nullptr, &currentTimeTextRect);
        SDL_DestroyTexture(currentTimeTextTexture);

        oss.str("");
        oss.clear();

        std::string bestTimeText = "Best Time: -";
        if (bestTime < INFINITY) {
            oss << std::fixed << std::setprecision(2) << std::floor(bestTime * 100.0) / 100.0;
            bestTimeText = "Best Time: " + oss.str();
        }
        int bestTimeTextWidth;
        SDL_Surface* bestTimeTextSurface = TTF_RenderText_Solid(font, bestTimeText.c_str(), textColor);
        SDL_Texture* bestTimeTextTexture = SDL_CreateTextureFromSurface(renderer, bestTimeTextSurface);
        SDL_Rect bestTimeTextRect = { 10, 50, bestTimeTextSurface->w, bestTimeTextSurface->h };
        SDL_FreeSurface(bestTimeTextSurface);
        SDL_RenderCopy(renderer, bestTimeTextTexture, nullptr, &bestTimeTextRect);
        SDL_DestroyTexture(bestTimeTextTexture);

        std::string coinsText = "Coins: " + std::to_string(coins);
        SDL_Surface* coinsTextSurface = TTF_RenderText_Solid(font, coinsText.c_str(), textColor);
        SDL_Texture* coinsTextTexture = SDL_CreateTextureFromSurface(renderer, coinsTextSurface);
        SDL_Rect coinsTextRect = { 1100, 10, coinsTextSurface->w, coinsTextSurface->h };
        SDL_FreeSurface(coinsTextSurface);
        SDL_RenderCopy(renderer, coinsTextTexture, nullptr, &coinsTextRect);
        SDL_DestroyTexture(coinsTextTexture);

        SDL_RenderPresent(renderer);

        Uint64 frameTime = SDL_GetPerformanceCounter() - NOW;
        double frameDuration = static_cast<double>(frameTime) / SDL_GetPerformanceFrequency();
        if (frameDuration < frameDelay) {
            SDL_Delay((Uint32)((frameDelay - frameDuration) * 1000));
        }
    }

    for (GameObject* gameobject : gameobjects) {
        delete gameobject;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}