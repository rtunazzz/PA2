#ifndef __QUESTIONS_H__
#define __QUESTIONS_H__

#include <string>

#include "answers.h"

using namespace std;
class CQuestionBase {
   private:
    unique_ptr<CAnswerBase> m_Answer;
    string m_Question;
    string m_Type;

   public:
    CQuestionBase();
    CQuestionBase(const string& question, const string& type);
    CQuestionBase(const string& question, const CAnswerBase& answer);
    virtual ~CQuestionBase();

    const string& getType();
    void setType(const string& type);

    const string& getQuestion();
    void setQuestion(const string& question);

    const CAnswerBase& getAnswer();
    void setAnswer(const CAnswerBase& answer);
    /**
     * @brief Checks whether or not the answer provided is correct.
     * @param answer Answer to check
     * @return Whether or not is the answer provided correct
     */
    bool isCorrect(const CAnswerBase& answer);
};

class CTextInput {};
class CSingleChoice {};
class CMultiChoice {};

#endif  // __QUESTIONS_H__
