#ifndef __QUIZ_H__
#define __QUIZ_H__

#include <map>
#include <memory>

#include "questions.h"
#include "window.h"

using namespace std;

template <class T>
class CQuiz {
   private:
    /** @brief Quiz window */
    T m_Window;

    /** @brief Map (index, quesition) of questions */
    map<int, shared_ptr<CQuestionBase>> m_Questions;

    bool importQuiz(const string& filename);
    bool exportQuiz(const string& filename);

    void addQuestion(shared_ptr<CQuestionBase> question);
    bool removeQuestion(shared_ptr<CQuestionBase> question);

    void listQuestions();

    void startQuiz();

   public:
    CQuiz();
    CQuiz(const CQuiz& old) = delete;
    ~CQuiz() = default;

    /** @brief Starts the quiz */
    void Start();
};

#endif  // __QUIZ_H__
