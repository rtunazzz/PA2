#ifndef __QUESTIONS_H__
#define __QUESTIONS_H__

#include <string>

#include "answers.h"

using namespace std;
class CQuestionBase {
   private:
    unique_ptr<CAnswerBase> m_CorrectAnswer;
    string m_Question;
    string m_Type;

   public:
    CQuestionBase();
    CQuestionBase(const string& question, const string& type);
    CQuestionBase(const string& question, const CAnswerBase& answer);
    /**
     * @brief Construct a new CQuestionBase object from its string representation
     * @param stringRepr String (text) representation of the answer
     */
    CQuestionBase(const string& stringRepr);

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
    virtual bool isCorrect(const CAnswerBase& answer);
    /**
     * @brief Converts this question to a string representation.
     * This string representation can be then used for import and export.
     * @return string String (text) representation
     */
    virtual string toString();
};

class CTextInput : CQuestionBase {};
class CSingleChoice : CQuestionBase {};
class CMultiChoice : CQuestionBase {};

#endif  // __QUESTIONS_H__
