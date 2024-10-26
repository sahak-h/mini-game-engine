#include "GameObject.hpp"

GameObject::GameObject(SDL_Renderer* renderer, SDL_Texture* texture, std::string name, bool isStatic, bool isCollider, int x, int y, int w, int h) {
    this->texture = texture;
    this->name = name;
    this->isStatic = isStatic;
    this->isCollider = isCollider;
    rect = { x, y, w, h };
}

void GameObject::render(SDL_Renderer* renderer, SDL_Rect camera) {
    if (!SDL_HasIntersection(&rect, &camera)) {
        return;
    }
    
    SDL_Rect renderRect = rect;
    renderRect.x -= camera.x;
    renderRect.y -= camera.y;

    SDL_RenderCopy(renderer, texture, NULL, &renderRect);
}

void GameObject::setTexture(SDL_Texture* newTexture)
{
    texture = newTexture;
}

SDL_Rect GameObject::getRect() {
    return rect;
}

void GameObject::setRect(int x, int y, int w, int h) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

void GameObject::update(float deltaTime, std::vector<GameObject*> gameobjects)
{
    if (isStatic) {
        return;
    }
    
    deltaTime *= 10;

    velocityY += 9.81 * deltaTime;

    rect.x += static_cast<int>(velocityX * deltaTime);
    rect.y += static_cast<int>(velocityY * deltaTime);
    
    velocityX *= 0.95;
    if (std::abs(velocityX) < 0.1) {
        velocityX = 0;
    }

    isGrounded = false;

    for (GameObject* otherGameobject : gameobjects) {
        if (!isCollider)
        {
            break;
        }
        else if (!otherGameobject->isCollider || otherGameobject == this)
        {
            continue;
        }

        SDL_Rect otherRect = otherGameobject->getRect();

        if (SDL_HasIntersection(&rect, &otherRect)) {
            SDL_Rect intersection;
            SDL_IntersectRect(&rect, &otherRect, &intersection);

            int overlapX = intersection.w;
            int overlapY = intersection.h;

            if (overlapX < overlapY) {
                if (rect.x < otherRect.x) {
                    rect.x -= overlapX;
                } 
                else {
                    rect.x += overlapX;
                }
                velocityX = 0;
            } 
            else {
                if (rect.y < otherRect.y) {
                    rect.y -= overlapY;
                    isGrounded = true;
                } 
                else {
                    rect.y += overlapY;
                }
                velocityY = 0;
            }
        }
    }
}

void GameObject::addForce(float x, float y)
{
    velocityX += x;
    velocityY += y;
}