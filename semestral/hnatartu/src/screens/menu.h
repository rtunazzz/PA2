#ifndef __MENU_H__
#define __MENU_H__

#include <vector>

#include "header.h"

class CMenu : public CSreenHeader {
   private:
    vector<string> m_Options;
    void renderOptions();

   public:
    CMenu() = delete;
    CMenu(vector<string> options);
    void Render(SDL_Color bgColor) override;
};

#endif  // __MENU_H__
