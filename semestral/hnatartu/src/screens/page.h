#ifndef __PAGE_H__
#define __PAGE_H__

#include <memory>
#include <vector>

#include "../questions.h"
#include "header.h"

class CPage : public CSreenHeader {
   private:
    vector<shared_ptr<CQuestionBase>> m_Questions;

   public:
    CPage(vector<shared_ptr<CQuestionBase>> questions);

    void Render(SDL_Color bgColor) override;

    void Previous();
    void Next();
};

#endif  // __PAGE_H__
