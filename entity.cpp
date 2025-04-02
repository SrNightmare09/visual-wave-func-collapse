#include "entity.hpp"

int Entity::tileSize = 0;

Entity::Entity(float x, float y, SDL_Texture* texture) {
    this->x = x;
    this->y = y;

    this->texture = texture;

    this->rect.x = x;
    this->rect.y = y;
    this->rect.w = tileSize;
    this->rect.h = tileSize;
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

int Entity::getTileSize() {
    return tileSize;
}

void Entity::setTileSize(int size) {
    tileSize = size;
}
