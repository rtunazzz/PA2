#include "quiz.h"

#include <iostream>

template <class T>
CQuiz<T>::CQuiz() {
    T tmp;
    m_Window = tmp.Clone();
}

template <class T>
void CQuiz<T>::Start() {
    cout << "Starting a quiz!" << endl;
    m_Window->run();
}

/** This is required for every template we want to use because otherwise it won't compile */
template class CQuiz<CSDLWindow>;
