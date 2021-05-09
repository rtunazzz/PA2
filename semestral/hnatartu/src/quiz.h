#ifndef __QUIZ_H__
#define __QUIZ_H__

#include <memory>
#include <vector>

#include "questions.h"
#include "window.h"

using namespace std;

template <class T>
class CQuiz {
   private:
    /** @brief Quiz window */
    T m_Window;

    /** @brief Vector of questions */
    vector<shared_ptr<CQuestionBase>> m_Questions;

    bool import(const string& filename);
    bool export(const string& filename);

   public:
    CQuiz();
    CQuiz(const CQuiz& old) = delete;
    ~CQuiz() = default;

    /** @brief Starts the quiz */
    void Start();
};

#endif  // __QUIZ_H__
