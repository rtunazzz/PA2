#include "quiz.h"

#include <iostream>

template <class T>
CQuiz<T>::CQuiz() {}

template <class T>
void CQuiz<T>::Start() {
    cout << "Starting a quiz!" << endl;

    // Show main menu
    m_Window.mainMenu();
}

/** This is required for every template we want to use because otherwise it won't compile */
template class CQuiz<CSDLWindow>;
