#ifndef __QUESTIONS_H__
#define __QUESTIONS_H__

#include <string>

using namespace std;
class CQuestionBase {
   private:
    string m_Question;
    string m_Type;

   public:
    CQuestionBase();
    CQuestionBase(const string& question, const string& type);
    virtual ~CQuestionBase();

    const string& getType();
    void setType(const string& type);

    const string& getQuestion();
    void setQuestion(const string& question);
};

#endif  // __QUESTIONS_H__
