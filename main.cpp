#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "app.hpp"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

int main(int argc, char* argv[]) {

    App app;
    app.init(WINDOW_WIDTH, WINDOW_HEIGHT);
    app.initTiles();

    bool running = true;
    SDL_Event e;

    const float deltaTime = 0.01f;
    float accumulator = 0.0f;
    float currentTime = SDL_GetTicks() * 0.001f;

    while (running) {

        int startTicks = SDL_GetTicks();
        float newTime = SDL_GetTicks() * 0.001f;
        float frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= deltaTime) {
            while (SDL_PollEvent(&e)) {
                switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_RETURN) {
                        app.clear();
                        app.mapTiles();
                        app.showTiles();
                    }
                }
            }
            accumulator -= deltaTime;
        }
        const float alpha = accumulator / deltaTime;

        app.display();

        int frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < (1000 / app.getRefreshRate())) {
            SDL_Delay(1000 / (app.getRefreshRate() - frameTicks));
        }
    }

    return app.close();
}
