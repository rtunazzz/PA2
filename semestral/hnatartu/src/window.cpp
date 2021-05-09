#include "window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

#include "screens/header.h"

int CWindow::INIT_WINDOW_WIDTH = 800;
int CWindow::INIT_WINDOW_HEIGHT = 600;

SDL_Color CSDLWindow::m_Black = {0, 0, 0};
SDL_Color CSDLWindow::m_White = {255, 255, 255};

CSDLWindow::CSDLWindow() {
    SDL_Init(SDL_INIT_EVERYTHING);
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error:" << TTF_GetError() << std::endl;
    } else {
        m_Roboto = TTF_OpenFont("assets/fonts/Roboto-Regular.ttf", 24);
    }

    m_Window = SDL_CreateWindow("Quizzz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CWindow::INIT_WINDOW_WIDTH, CWindow::INIT_WINDOW_HEIGHT, 0);
    m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);

    std::cout << "Initialized a window." << std::endl;
}

CSDLWindow::~CSDLWindow() {
    // Free all TTF objects
    TTF_CloseFont(m_Roboto);
    TTF_Quit();

    // Free all SDL objects
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

void CSDLWindow::mainMenu() {
    std::cout << "Showing menu." << std::endl;

    // Initialize a screen header with a title "Main Menu"
    CSreenHeader header = CSreenHeader(m_Window, m_Renderer);
    header.initHeader("Main Menu", m_Roboto, m_Black);
    // Render it & make it visible
    header.Render(m_White);
    header.ShowRender();

    /* 
    TODO
    .
    .
    .
    .
    */
    bool isRunning = true;
    SDL_Event event;
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
    }
}
