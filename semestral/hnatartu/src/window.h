#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>

class CWindow {
   public:
    virtual ~CWindow() = default;

    virtual void run(){};
};

class CSDLWindow : public CWindow {
   private:
    SDL_Window *m_Window;
    SDL_Renderer *m_Renderer;

   public:
    CSDLWindow();
    ~CSDLWindow();

    void run() override;
};

#endif  // __WINDOW_H__