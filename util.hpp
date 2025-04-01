#include "app.hpp"

#include <random>
#include <iostream>
#include <SDL2/SDL_image.h>

namespace util {
    inline int getRandomNumber(int lower, int upper) {
        // R -> [lower, upper]

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(lower, upper);

        int rand = dist(gen);
        return rand;
    }
}
