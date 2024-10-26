#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <SDL2/SDL.h>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <string>

class GameObject {
public:
    GameObject(SDL_Renderer* renderer, SDL_Texture* texture, std::string name, bool isStatic, bool isCollider, int x, int y, int w, int h);

    void render(SDL_Renderer* renderer, SDL_Rect camera);

    void setTexture(SDL_Texture* newTexture);

    SDL_Rect getRect();

    void setRect(int x, int y, int w, int h);

    void update(float deltaTime, std::vector<GameObject*> gameobjects);

    void addForce(float x, float y);
    
    std::string name;
    bool isGrounded = false;

private:
    SDL_Texture* texture;
    SDL_Rect rect;
    bool isStatic;
    bool isCollider;
    float velocityX;
    float velocityY;
};

#endif