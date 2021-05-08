#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>

#include <memory>

class CWindow {
   public:
    virtual ~CWindow() = default;

    virtual std::unique_ptr<CWindow> Clone();

    virtual void run(){};
};

class CSDLWindow : public CWindow {
   private:
    SDL_Window *m_Window;
    SDL_Renderer *m_Renderer;

   public:
    CSDLWindow();
    ~CSDLWindow();

    std::unique_ptr<CWindow> Clone() override;

    void run() override;
};

#endif  // __WINDOW_H__