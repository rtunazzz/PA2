#ifndef __ANSWERS_H__
#define __ANSWERS_H__

#include <string>

using namespace std;
class CAnswerBase {
   private:
    string m_Type;

   public:
    CAnswerBase();
    CAnswerBase(const string& type);
    virtual ~CAnswerBase();

    const string& getType();
    void setType(const string& type);
};

class CExactMatch {};
class CFromSet {};
class CPattern {};

#endif  // __ANSWERS_H__
