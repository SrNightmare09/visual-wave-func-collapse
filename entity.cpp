#include "entity.hpp"

Entity::Entity(float x, float y, SDL_Texture* texture) {
    this->x = x;
    this->y = y;

    this->texture = texture;

    this->rect.x = x;
    this->rect.y = y;
    this->rect.w = 40;
    this->rect.h = 40;
}

float Entity::getX() {
    return this->x;
}

float Entity::getY() {
    return this->y;
}

void Entity::setX(float x) {
    this->x = x;
}

void Entity::setY(float y) {
    this->y = y;
}

SDL_Rect Entity::getRect() {
    return this->rect;
}

SDL_Texture* Entity::getTexture() {
    return this->texture;
}
