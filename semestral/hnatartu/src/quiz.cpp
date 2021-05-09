#include "quiz.h"

#include <iostream>

template <class T>
CQuiz<T>::CQuiz() {}

template <class T>
void CQuiz<T>::Start() {
    cout << "Starting a quiz!" << endl;

    // Show main menu
    m_Window.mainMenu();

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

/** This is required for every template we want to use because otherwise it won't compile */
template class CQuiz<CSDLWindow>;
