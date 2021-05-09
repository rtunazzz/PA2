#ifndef __HEADER_H__
#define __HEADER_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

using namespace std;

class CSreenHeader {
   private:
    /** @brief SDL Window pointer */
    SDL_Window* m_Window;
    /** @brief SDL Renderer pointer */
    SDL_Renderer* m_Renderer;

    /** @brief SDL header texture */
    SDL_Texture* m_HeaderTexture;
    /** @brief SDL Rectange containing the header */
    SDL_Rect m_HeaderRect;

   public:
    CSreenHeader() = delete;
    /**
     * @brief Constructs a new CSreenHeader object
     * @param window SDL Window you plan on rendering this header on
     * @param renderer SDL Renderer that will render the header
     */
    CSreenHeader(SDL_Window* window, SDL_Renderer* renderer);

    /** @brief Destroys the CSreenHeader object */
    ~CSreenHeader();

    /**
     * @brief Initializes a header.
     * @param title Title of the header
     * @param font Font of the header
     * @param textColor Text color of the header
     */
    void initHeader(const string& title, TTF_Font* font, SDL_Color textColor);

    /**
     * @brief Renders the header with the background color.
     * @param bgColor Background color
     */
    virtual void Render(SDL_Color bgColor);
    /**
     * @brief Shows the render.
     */
    void ShowRender();
};

#endif  // __HEADER_H__
