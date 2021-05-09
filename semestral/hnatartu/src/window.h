#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <memory>

class CWindow {
   public:
    /** @brief Initial window width */
    static int INIT_WINDOW_WIDTH;
    /** @brief Initial window height */
    static int INIT_WINDOW_HEIGHT;

    /** @brief Destroys a CWindow object. */
    virtual ~CWindow() = default;

    /** @brief Shows the main menu */
    virtual void mainMenu() = 0;
};

class CSDLWindow : public CWindow {
   private:
    /** @brief Black color */
    static SDL_Color m_Black;
    /** @brief White color */
    static SDL_Color m_White;

    /** @brief SDL Window pointer */
    SDL_Window *m_Window;
    /** @brief SDL Renderer pointer */
    SDL_Renderer *m_Renderer;

    /** @brief Roboto font pointer used as text */
    TTF_Font *m_Roboto;

   public:
    /** @brief Constructs a new CSDLWindow object*/
    CSDLWindow();
    /** @brief Destroys the CSDLWindow object. */
    ~CSDLWindow();

    /** @brief Shows the main menu */
    void mainMenu() override;
};

#endif  // __WINDOW_H__
