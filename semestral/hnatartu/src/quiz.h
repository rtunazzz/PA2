#ifndef __QUIZ_H__
#define __QUIZ_H__

#include <vector>

#include "questions.h"
#include "window.h"

using namespace std;

template <class T>
class CQuiz {
   private:
    unique_ptr<CWindow> m_Window;
    vector<unique_ptr<CQuestionBase>> m_Questions;

   public:
    CQuiz();
    ~CQuiz() = default;

    void Start();
};

#endif  // __QUIZ_H__
