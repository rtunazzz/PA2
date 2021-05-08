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

    /**
     * @brief Construct a new CAnswerBase object from its string representation
     * @param stringRepr String (text) representation of the answer
     */
    // CAnswerBase(const string& stringRepr);

    virtual ~CAnswerBase();

    const string& getType();
    void setType(const string& type);

    /**
     * @brief Checks whether or not the answer provided is correct.
     * @param answer Answer to check
     * @return Whether or not is the answer provided correct
     */
    virtual bool isCorrect(const string& answer);

    /**
     * @brief Converts this answer to a string representation.
     * This string representation can be then used for import and export.
     * @return string String (text) representation
     */
    virtual string toString();
};

class CExactMatch : CAnswerBase {};
class CPattern : CAnswerBase {};
class CFromSet : CAnswerBase {};

#endif  // __ANSWERS_H__
