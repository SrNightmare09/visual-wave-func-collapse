#include "app.hpp"
#include "entity.hpp"
#include "util.hpp"

#include <iostream>
#include <SDL2/SDL_image.h>

using namespace util;

enum {
    BLANK,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

void App::init(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Could not initialize SDL video! Err: " << SDL_GetError() << std::endl;
    }

    this->window = SDL_CreateWindow("Wave Function Collapse [ Press Enter to generate! ]", 200, 200, width, height, SDL_WINDOW_SHOWN);
    if (this->window == NULL) {
        std::cout << "Could not load window" << SDL_GetError() << std::endl;
    }

    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (this->renderer == NULL) {
        std::cout << "Could not load renderer" << SDL_GetError() << std::endl;
    }
}

int App::close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int App::getRefreshRate() {
    int displayIndex = SDL_GetWindowDisplayIndex(window);
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(displayIndex, 0, &mode);
    return mode.refresh_rate;
}

SDL_Window* App::getWindow() {
    return this->window;
}

SDL_Renderer* App::getRenderer() {
    return this->renderer;
}

void App::clear() {
    SDL_RenderClear(this->renderer);
}

void App::render(Entity& entity) {

    SDL_Rect source;

    source.x = 0;
    source.y = 0;

    int textureWidth;
    int textureHeight;
    SDL_QueryTexture(entity.getTexture(), NULL, NULL, &textureWidth, &textureHeight);

    source.w = textureWidth;
    source.h = textureHeight;

    SDL_Rect dest;
    dest.x = entity.getX();
    dest.y = entity.getY();
    dest.w = 40;
    dest.h = 40;

    SDL_RenderCopy(this->renderer, entity.getTexture(), &source, &dest);

}

void App::display() {
    SDL_RenderPresent(renderer);
}

SDL_Texture* App::loadTexture(const char* filepath) {
    SDL_Texture* texture;
    texture = IMG_LoadTexture(this->renderer, filepath);
    if (texture == NULL) {
        std::cout << "Could not load texture! Err: " << SDL_GetError() << std::endl;
    }
    return texture;
}

void App::initTiles() {
    // init textures

    SDL_Texture* blankTexture = this->loadTexture("assets/blank.png");
    SDL_Texture* upTexture = this->loadTexture("assets/up.png");
    SDL_Texture* rightTexture = this->loadTexture("assets/right.png");
    SDL_Texture* downTexture = this->loadTexture("assets/down.png");
    SDL_Texture* leftTexture = this->loadTexture("assets/left.png");

    Entity* blank = new Entity(0.0f, 0.0f, blankTexture);
    Entity* up = new Entity(0.0f, 0.0f, upTexture);
    Entity* right = new Entity(0.0f, 0.0f, rightTexture);
    Entity* down = new Entity(0.0f, 0.0f, downTexture);
    Entity* left = new Entity(0.0f, 0.0f, leftTexture);

    tiles.resize(5);

    tiles[BLANK] = blank;
    tiles[UP] = up;
    tiles[RIGHT] = right;
    tiles[DOWN] = down;
    tiles[LEFT] = left;

    // initialize respective point maps
    // sequence -> up right down left. 1 -> connection, 0 -> empty

    points.resize(5);

    points[BLANK] = { 0, 0, 0, 0 };
    points[UP] = { 1, 1, 0, 1 };
    points[RIGHT] = { 1, 1, 1, 0 };
    points[DOWN] = { 0, 1, 1, 1 };
    points[LEFT] = { 1, 0, 1 , 1 };

}

void App::initVec() {
    int winWidth;
    int winHeight;
    int tileSize = 40;

    SDL_GetWindowSize(window, &winWidth, &winHeight);

    map.resize(winHeight / tileSize);
    for (int i = 0; i < winHeight / tileSize; i++) {
        map[i].resize(winWidth / tileSize);
    }

    for (int i = 0; i < winHeight / tileSize; i++) {
        for (int j = 0; j < winWidth / tileSize; j++) {
            map[i][j] = 0;
        }
    }
}

void App::mapTiles() {

    int winWidth;
    int winHeight;
    int tileSize = 40;

    initVec();

    SDL_GetWindowSize(window, &winWidth, &winHeight);

    for (int row = 0; row < winHeight / tileSize; row++) {
        for (int col = 0; col < winWidth / tileSize; col++) {

            // tile at (0, 0) is selected at random as seed
            if (row == 0 && col == 0) {
                int randIndex = util::getRandomNumber(BLANK, LEFT);
                map[row][col] = randIndex;
            }

            else {
                // all other tiles based off of tile at 0, 0 are collapsed in collapse function
                collapse(row, col);
            }
        }
    }
}

void App::showTiles() {
    int winWidth;
    int winHeight;
    int tileSize = 40;
    SDL_GetWindowSize(window, &winWidth, &winHeight);

    for (int yPos = 0; yPos < winHeight; yPos += tileSize) {
        for (int xPos = 0; xPos < winWidth; xPos += tileSize) {

            int row = yPos / tileSize;
            int col = xPos / tileSize;

            Entity& tile = *tiles[map[row][col]];
            tile.setX(xPos);
            tile.setY(yPos);

            this->render(tile);
        }
    }
}

void App::collapse(int row, int col) {

    constexpr int tileSize = 40;
    int winWidth;
    int winHeight;
    SDL_GetWindowSize(this->window, &winWidth, &winHeight);

    if (row == 0) { // top row logic

        // need to check for connection on the right

        // index of right connection = 1

        // this is the tile which we need to connect to
        int connectTile = map[row][col - 1];

        // this is the tile's connection on the right (index 1). 0 means empty and 1 means line
        int connection = points[connectTile][1]; // 0 or 1

        /*
        to find possible options we will:
        1. generate a random number
        2. check the tiles array at index = random number
        3. since the current tile will connect from its left to the prev tile, we check the tiles left index (index 3)
        4. if the left index of the current tile = connection, it is a valid option. we add it to the map array
        5. if not, we continue this loop till a valid option is found
        */

        bool isValid = false;
        while (!isValid) {
            int randNum = util::getRandomNumber(BLANK, LEFT);

            int choice = points[randNum][3]; // 0 or 1

            if (connection == choice) {
                isValid = true;
                // add the choice to the map

                map[row][col] = randNum;
            }

        }

    }

    else if (col == 0) { // left most column logic

        // need to check for connection to the bottom

        // index of bottom connection = 2

        // this is the tile we want to connect to
        int connectTile = map[row - 1][col];

        // this is the tile's connection at the bottom (index 2). if there is a line the value will be 1, else 0
        int connection = points[connectTile][2];

        /*
        to find possible options we will:
        1. generate a random number
        2. check the tiles array at index = random number
        3. since the current tile will connect from its top to the prev tile, we check the tiles top index (index 0)
        4. if the top index of the current tile = connection, it is a valid option. we add it to the map array
        5. if not, we continue this loop till a valid option is found
        */

        bool isValid = false;
        while (!isValid) {
            int randNum = util::getRandomNumber(BLANK, LEFT);

            int choice = points[randNum][0]; // 0 or 1

            if (connection == choice) {
                isValid = true;
                // add the choice to the map

                map[row][col] = randNum;
            }

        }

    }

    else { // everything else

        // need to check for connection to the bottom AND to the right

        // index of buttom connection = 2 and index of right connection = 1

        // this is the first tile we want to connect to (tile above)
        int connectTileTop = map[row - 1][col];

        // this is the second tile we want to connect to (tile to left)
        int connectTileLeft = map[row][col - 1];

        // this is the top tile's connection at the bottom (index 2). if there is a line the value will be 1, else 0
        int connectionTop = points[connectTileTop][2];

        // this is the left tile's connection at its right (index 1). if there is a line the value will be 1, else 0
        int connectionLeft = points[connectTileLeft][1];

        /*
        to find possible options we will:
        1. generate a random number
        2. check the tiles array at index = random number
        3. since the current tile will connect from its top to the above tile, we check the tiles top index (index 0)
        4. the current tile will also connect from its left to the tile on its left, we will also check the tiles left index (index 3)
        4. if the top and left index of the current tile = respective connections, it is a valid option. we add it to the map array
        5. if not, we continue this loop till a valid option is found
        */

        bool isValid = false;
        while (!isValid) {
            int randNum = util::getRandomNumber(BLANK, LEFT);

            int choiceTop = points[randNum][0]; // 0 or 1
            int choiceLeft = points[randNum][3];

            if (connectionLeft == choiceLeft && connectionTop == choiceTop) {
                isValid = true;
                // add the choice to the map

                map[row][col] = randNum;
            }
        }

    }

}
