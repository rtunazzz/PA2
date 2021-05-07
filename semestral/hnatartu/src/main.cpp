#include <SDL2/SDL.h>

#include <iostream>

int main(int argv, char **args) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    bool isRunning = true;
    SDL_Event event;

    int i = 0;
    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        isRunning = false;
                    }
            }
        }

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, i, 256 - i, i, i);

        SDL_RenderPresent(renderer);
        i += 1;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
