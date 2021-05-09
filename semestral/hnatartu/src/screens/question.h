#ifndef __QUESTION_H__
#define __QUESTION_H__

#include <memory>

#include "../questions.h"
#include "header.h"

// TODO perhaps create a CQuestion class and have these two inherit from it?

class CQuestionView : public CSreenHeader {
   private:
    shared_ptr<CQuestionBase> m_Question;

    void renderQuestion();

   public:
    CQuestionView() = delete;
    CQuestionView(shared_ptr<CQuestionBase> question);

    void Render(SDL_Color bgColor) override;

    bool checkAnswer();
    string checkAnswerStr();
};

class CQuestionCreate : public CSreenHeader {
   private:
    shared_ptr<CQuestionBase> m_Question;

    void renderQuestion();
    void renderTypeSelectQ();
    void renderTypeSelectA();

   public:
    CQuestionCreate();

    void Render(SDL_Color bgColor) override;

    shared_ptr<CQuestionBase> GetQuestion();
};

#endif  // __QUESTION_H__
