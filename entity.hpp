#pragma once

#include <SDL2/SDL.h>

class Entity {
public:
    Entity(float x, float y, SDL_Texture* texture);
    float getX();
    float getY();
    void setX(float x);
    void setY(float y);
    SDL_Rect getRect();
    SDL_Texture* getTexture();
private:
    float x;
    float y;
    SDL_Texture* texture;
    SDL_Rect rect;
};
