#include "window.h"

#include <SDL2/SDL.h>

CSDLWindow::CSDLWindow() {
    SDL_Init(SDL_INIT_EVERYTHING);

    m_Window = SDL_CreateWindow("Quizzz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);
}

CSDLWindow::~CSDLWindow() {
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

void CSDLWindow::run() {
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

        SDL_RenderClear(m_Renderer);
        SDL_SetRenderDrawColor(m_Renderer, i, 256 - i, i, i);

        SDL_RenderPresent(m_Renderer);
        i += 1;
    }
}
