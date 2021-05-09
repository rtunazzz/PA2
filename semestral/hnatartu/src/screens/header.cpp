#include "header.h"

CSreenHeader::CSreenHeader(SDL_Window* window, SDL_Renderer* renderer) : m_Window(window), m_Renderer(renderer) {}

void CSreenHeader::InitHeader(const string& title, TTF_Font* font, SDL_Color textColor) {
    // Get the window size
    int wWidth;
    int wHeight;
    SDL_GetWindowSize(m_Window, &wWidth, &wHeight);

    // Initialize the surface and the texture for the main menu title
    SDL_Surface* headerSurface = TTF_RenderText_Solid(font, title.c_str(), textColor);
    m_HeaderTexture = SDL_CreateTextureFromSurface(m_Renderer, headerSurface);

    // scale it so it's 2 times bigger than the usual text
    m_HeaderRect.w = 2 * headerSurface->w;
    m_HeaderRect.h = 2 * headerSurface->h;

    // center it horizontally
    m_HeaderRect.x = (int)(wWidth / 2) - (int)(m_HeaderRect.w / 2);
    // align it to the top portion of the screen vertically
    m_HeaderRect.y = (int)(wHeight / 18) - (int)(m_HeaderRect.h / 2);

    // free the surface since it's no longer needed (we only need the texture)
    SDL_FreeSurface(headerSurface);
}

void CSreenHeader::Render(SDL_Color bgColor) {
    // Clear the screen and set a white background
    SDL_SetRenderDrawColor(m_Renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(m_Renderer);
    // Copy the rectangle to our render
    SDL_RenderCopy(m_Renderer, m_HeaderTexture, NULL, &m_HeaderRect);
}

void CSreenHeader::ShowRender() {
    // Show the render
    SDL_RenderPresent(m_Renderer);
}

CSreenHeader::~CSreenHeader() {
    SDL_DestroyTexture(m_HeaderTexture);
}
