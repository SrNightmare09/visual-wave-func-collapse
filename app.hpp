#pragma once

#include "entity.hpp"

#include <vector>
#include <SDL2/SDL.h>

class App {
public:
    void init(int width, int height);
    int getRefreshRate();
    int close();
    void clear();
    void render(Entity& entity);
    void display();
    void initTiles(int tileSize);
    void showTiles();
    void mapTiles();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::vector<Entity*> tiles;
    std::vector<std::vector<int>> points;
    std::vector<std::vector<int>> map;

    SDL_Texture* loadTexture(const char* filepath);
    void collapse(int row, int col);
    void initMap();
    int getWindowWidth();
    int getWindowHeight();
};
